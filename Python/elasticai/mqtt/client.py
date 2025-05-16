"""MQTT Client implementation."""

import logging
import re as regex
from typing import Any, Callable

import paho.mqtt.client as paho_client
import paho.mqtt.enums as paho_enums
from logdecorator import log_on_end, log_on_error, log_on_start

from elasticai.protocol.client_interface import IPubSubClient


class PublishError(Exception):
    """Publishing to topic failed!"""

    pass


class SubscribeError(Exception):
    """Subscribing to topic failed!"""

    pass


class UnsubscribeError(Exception):
    """Unsubscribing topic failed!"""

    pass


class MQTTClient(IPubSubClient):
    """Exclipse Paho backed MQTT client for the elastic-AI protocol."""

    def __init__(
        self,
        auto_reconnect: bool = True,
    ) -> None:
        """Initialize the client.

        IMPORTANT: call `connect()` to establish connection with a Broker!

        Args:
            auto_reconnect (bool):
                Whether to automatically reconnect after losing a connection.

        Returns:
            None
        """
        self.__subscriptions: dict[str, Callable[[str, str], None]] = {}
        self.__delivered: dict[int, bool] = {}
        self.__connected: bool = False

        self.__paho_client: paho_client.Client = paho_client.Client(
            callback_api_version=paho_enums.CallbackAPIVersion.VERSION2,
            clean_session=True,
            protocol=paho_client.MQTTv311,
            reconnect_on_failure=auto_reconnect,
        )
        self.__paho_client.on_connect = self.__on_connect
        self.__paho_client.on_connect_fail = self.__on_connect_fail
        self.__paho_client.on_message = self.__on_message
        self.__paho_client.on_disconnect = self.__on_disconnect
        self.__paho_client.on_publish = self.__on_publish

    def __del__(self) -> None:
        """Close MQTT connection on class destruction."""
        self.disconnect()

    def is_connected(self) -> bool:
        """If Client is currently connected with a broker."""
        return self.__connected

    @log_on_end(logging.INFO, "Set LWT for topic '{topic}' to '{payload}'")
    def set_lwt(self, topic: str, payload: str | bytes | bytearray | None) -> None:
        """Set LWT message for client.

        IMPORTANT: Has to be called before `connect` to take effect

        Args:
            topic (str): topic to publish lwt
            payload (str|bytes|bytearray|None): message to publish as lwt

        Returns:
            None
        """
        self.__paho_client.will_set(topic=topic, payload=payload, retain=True)

    @log_on_error(
        logging.ERROR,
        "Error connecting to {mqtt_broker_address}:{mqtt_broker_port}: {e!r}",
        on_exceptions=Exception,
        reraise=True,
    )
    @log_on_start(
        logging.DEBUG,
        "Try connecting to {mqtt_broker_address}:{mqtt_broker_port}",
    )
    @log_on_end(
        logging.INFO,
        "Connection established with {mqtt_broker_address}:{mqtt_broker_port}",
    )
    def connect(self, mqtt_broker_address: str, mqtt_broker_port: int) -> None:
        """Connect to a MQTT broker.

        IMPORTANT: Has to be called before any other action!

        Args:
            mqtt_broker_address (str): address of the MQTT broker
            mqtt_broker_port (int): port of the MQTT broker

        Returns:
            None
        """
        self.__connected = False
        self.__paho_client.connect(host=mqtt_broker_address, port=mqtt_broker_port)
        self.__paho_client.loop_start()
        while not self.__connected:
            pass

    @log_on_error(
        logging.ERROR,
        "Error disconnecting: {e!r}",
        on_exceptions=Exception,
        reraise=True,
    )
    @log_on_start(
        logging.DEBUG,
        "Try disconnecting from Broker",
    )
    @log_on_end(
        logging.INFO,
        "Successfully disconnected!",
    )
    def disconnect(self) -> None:
        """Disconnect from the MQTT broker."""
        self.__paho_client.disconnect()

    @log_on_start(logging.INFO, "Publish message to '{topic}'")
    def publish(
        self, topic: str, payload: str | bytes | bytearray | None, retain: bool = False
    ) -> None:
        """Publish Message.

        Args:
            topic (str): topic to publish to
            payload (str): message to publish. (max. 268435455 bytes)
            retain (bool): if messaged should be retained

        Raises:
            ValueError: if the Payload is too long, or topic is invalid
            PublishError: if publish failed

        Returns:
            None
        """
        status, mid = self.__paho_client.publish(
            topic=topic, payload=payload, retain=retain
        )
        self.__delivered[mid] = False
        if status != paho_client.MQTT_ERR_SUCCESS:
            raise PublishError(status)
        while not self.__delivered[mid]:
            pass
        self.__delivered.pop(mid)

    @log_on_start(logging.INFO, "Subscribing to '{topic}'")
    def subscribe(self, topic: str, handler: Callable[[str, str], None]) -> None:
        """Subscribe to topic.

        Args:
            topic (str): topic to subscribe
            handler (Callable): handler function for received messages

        Raises:
            ValueError: if the topic is invalid
            SubscribeError: if subscription failed

        Returns:
            None
        """
        status, _ = self.__paho_client.subscribe(topic=topic)
        if status == paho_client.MQTT_ERR_SUCCESS:
            self.__subscriptions[topic] = handler
        else:
            raise SubscribeError(status)

    @log_on_start(logging.INFO, "Unsubscribe from '{topic}'")
    def unsubscribe(self, topic: str) -> None:
        """Unsubscribe from topic.

        Args:
            topic (str): topic to unsubscribe

        Raises:
            ValueError: if the topic is invalid
            KeyError: if topic was not subscribed

        Returns:
            None
        """
        status, _ = self.__paho_client.unsubscribe(topic=topic)
        if status == paho_client.MQTT_ERR_SUCCESS:
            self.__subscriptions.pop(topic)
        else:
            raise UnsubscribeError(status)

    @staticmethod
    def __replace(level: str) -> str:
        WILDCARD_PLUS: str = "[^/]*"
        WILDCARD_HASH: str = ".*"
        if level == "+":
            return WILDCARD_PLUS
        elif level == "#":
            return WILDCARD_HASH
        else:
            return level

    @staticmethod
    def __check_topic_equal(topic1: str, topic2: str) -> bool:
        """Check if topic2 matches topic1.

        IMPORTANT: Only topic1 is allowed to contain wildcards

        Args:
            topic1 (str): subscribed topic
            topic2 (str): topic to evaluate

        Returns:
            match (bool): if topic1 matches topic2
        """
        pattern: str = (
            "^" + "/".join(list(map(MQTTClient.__replace, topic1.split("/")))) + "$"
        )
        compiled_pattern: regex.Pattern = regex.compile(pattern)
        return True if compiled_pattern.search(topic2) is not None else False

    def __on_connect(
        self,
        client: paho_client.Client,
        userdata: Any,
        connect_flags: paho_client.ConnectFlags,
        reason_code: paho_client.ReasonCode,
        properties: paho_client.Properties | None,
    ) -> None:
        """Callback for connect."""
        if reason_code == paho_client.MQTT_ERR_SUCCESS:
            self.__connected = True

    def __on_connect_fail(self, client: paho_client.Client, userdata: Any) -> None:
        """Callback for auto reconnect failed."""
        self.__connected = False

    def __on_disconnect(
        self,
        client: paho_client.Client,
        userdata: Any,
        diconnect_flags: paho_client.DisconnectFlags,
        reason_code: paho_client.ReasonCode,
        properties: paho_client.Properties | None,
    ) -> None:
        self.__connected = False

    def __on_message(
        self, client: paho_client.Client, userdata: Any, msg: paho_client.MQTTMessage
    ) -> None:
        for topic, handler in self.__subscriptions.items():
            if MQTTClient.__check_topic_equal(topic, msg.topic):
                handler(msg.topic, msg.payload.decode("utf-8"))

    def __on_publish(
        self,
        client: paho_client.Client,
        userdata: Any,
        mid: int,
        reason_code: paho_client.ReasonCode,
        properties: paho_client.Properties,
    ) -> None:
        if mid in self.__delivered:
            self.__delivered[mid] = True

