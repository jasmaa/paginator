# Paginator

A gadget that flashes an LED when you get paged.

![Paginator circuit diagram](./docs/diagram.png)

## Getting started

Obtain the following:
- ESP32 (1x)
- 128x64 SSD1306 OLED display (1x)
- LED (1x)
- 200 ohm resistor (1x)
- Breadboard
- Jumper wires

Build circuit according to diagram.

Plug board into computer.

Install ESP32-ANCS-Notification:

```
cd /path/to/ardino/libraries
git clone https://github.com/jasmaa/ESP32-ANCS-Notifications.git
```

Open Arduino IDE.

Go to Library Manager and install:
- Adafruit SSD1306

Set board to "EP32-WROOM-DA".

Open `paginator.ino` and upload to ESP32.