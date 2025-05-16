"""Interface for the pub-sub-system used for the elastic-AI protocol."""

from abc import ABC, abstractmethod
from typing import Callable


class IPubSubClient(ABC):
    """Interface for the pub-sub-system used for the elastic-AI protocol."""

    @abstractmethod
    def set_lwt(self, topic: str, payload: str) -> None:
        """Set LWT message for client.

        Args:
            topic (str): topic to publish lwt
            payload (str|byte|bytearray|None): message to publish as lwt

        Returns:
            None
        """
        pass

    @abstractmethod
    def publish(self, topic: str, payload: str, retain: bool = False) -> None:
        """Publish a message to the defined topic.

        Args:
            topic(str): topic to publish to
            payload(str): message to publish
            retain (bool): if message is retained

        Returns:
            None
        """
        pass

    @abstractmethod
    def subscribe(self, topic: str, handler: Callable[[str, str], None]) -> None:
        """Subscribe a message to the defined topic.

        Args:
            topic(str): topic to subscribe to
            handler(Callable): handler function

        Returns:
            None
        """
        pass

    @abstractmethod
    def unsubscribe(self, topic: str) -> None:
        """Unsubscribe a message to the defined topic.

        Args:
            topic(str): topic to unsubscribe to

        Returns:
            None
        """
        pass
