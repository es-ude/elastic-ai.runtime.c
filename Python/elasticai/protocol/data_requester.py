"""Class to simplify the process of requesting data from another participant."""

from collections import deque

from elasticai.protocol.base import DeviceState, DeviceType, Protocol
from elasticai.protocol.client_interface import IPubSubClient


class DeviceNotAvailableError(Exception):
    """Target device can't be reached."""

    pass


class DataRequester(Protocol):
    """Data Request tool."""

    __target_id: str
    __target_topic: str
    __target_online: bool

    __data_buffer: deque

    def __init__(
        self,
        target_device: str,
        target_data_id: str,
        client: IPubSubClient,
        client_device_id: str,
        client_type: DeviceType = DeviceType.APPLICATION,
        base_url: str = "eaip://uni-due.de",
        max_buffer_length: int = 100,
    ) -> None:
        """Initialize DataRequester.

        Args:
            target_device (str): id of the device to request data from
            target_data_id (str): id of the data field to request
            client (IPubSubClient): handler for pub-sub-system
            client_device_id (str): id of the client
            client_type (DeviceType): client type
            base_url (str): base URL for the communication
            max_buffer_length (int): max number of messages to buffer

        Returns:
            None
        """
        super().__init__(
            handler=client,
            device_id=client_device_id,
            device_type=client_type,
            base_url=base_url,
        )
        self.__target_id = target_device
        self.__target_topic = target_data_id
        self.__target_online = False

        self.__data_buffer: deque = deque(maxlen=max_buffer_length)

        super().subscribe_status(self.__target_id, self.__status_handler)
        super().subscribe_data(
            self.__target_id, self.__target_topic, self.__data_handler
        )

    def __status_handler(self, topic: str, message: str) -> None:
        status: dict[str, str] = super().parse_status(message)
        online: bool = status["STATE"] == DeviceState.ONLINE.value
        self.__target_online = online

    def __data_handler(self, topic: str, message: str) -> None:
        self.__data_buffer.appendleft(message)

    def start(self) -> None:
        """Start requesting data.

        Raises:
            DeviceNotAvailableError if target device is OFFLINE

        Returns:
            None
        """
        if not self.__target_online:
            raise DeviceNotAvailableError()
        super().publish_start(self.__target_id, self.__target_topic)

    def stop(self) -> None:
        """Stop requesting data.

        Raises:
            DeviceNotAvailableError if target device is OFFLINE

        Returns:
            None
        """
        if not self.__target_online:
            raise DeviceNotAvailableError()
        super().publish_stop(self.__target_id, self.__target_topic)

    def get_data(self) -> str | None:
        """Get data from buffer.

        Returns:
            str: if data is present
            None: else
        """
        latest_val: str | None
        try:
            latest_val = self.__data_buffer.pop()
        except IndexError:
            latest_val = None

        return latest_val
