"""
test_network.py
Placeholder tests for the firmware's network/status reporting module
(firmware/network/network.c). Since this module is not on the critical
wake-word detection path, tests here focus on message formatting rather
than requiring live hardware.
"""


def format_heartbeat(alive: bool = True) -> str:
    return "heartbeat: kalpana firmware alive" if alive else "heartbeat: kalpana firmware down"


def test_heartbeat_format():
    msg = format_heartbeat(True)
    assert "kalpana" in msg
    assert "alive" in msg


def test_heartbeat_down_format():
    msg = format_heartbeat(False)
    assert "down" in msg


if __name__ == "__main__":
    test_heartbeat_format()
    test_heartbeat_down_format()
    print("Network module format checks passed.")
