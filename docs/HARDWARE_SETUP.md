# Hardware Setup Guide

This guide details the physical connections between the ESP32 and the Bill Acceptor.

## Components Required
- **ESP32 DevKit V1**
- **ICT P70 / P77 Bill Acceptor**
- **MAX3232 Level Shifter** (Required for RS232)
- **12V DC 2A Power Supply**
- **I2C 16x2 LCD Display**

## Wiring Diagram

### Bill Acceptor (9-Pin AMP Connector)
| Pin | Color | Signal | ESP32 Connection |
| :--- | :--- | :--- | :--- |
| **1** | Yellow | Inhibit (+) | GPIO 5 |
| **2** | Green | Inhibit (-) | GND |
| **3** | Red | +12V DC | Power Supply (+) |
| **9** | Orange | GND | Common GND |

### Protocol Specifics
#### Pulse Mode (6-Pin TMT)
- **Blue Wire (Pin 5)** -> **GPIO 4** (Pulse Input)

#### RS232 Mode (8-Pin TMT)
- **Black Wire (Pin 7)** -> **RX2 (GPIO 17)** via MAX3232
- **Purple Wire (Pin 8)** -> **TX2 (GPIO 16)** via MAX3232

#### I2C LCD Display
- **SDA Pin** -> **GPIO 21**
- **SCL Pin** -> **GPIO 22**
- **VCC** -> **5V** (Most LCDs require 5V for backlight brightness)
- **GND** -> **Common GND**

> [!WARNING]
> Never connect the RS232 TX/RX lines directly to the ESP32. The 12V signals will damage the microcontroller. Use the MAX3232 level shifter.

## DIP Switch Configuration (India INR)
For the 1:5 Pulse ratio and RS232 support:
- **SW1-SW3**: OFF (1 Pulse / 5 INR)
- **SW5**: OFF (Inhibit Active High)
- **SW8**: ON (RS232 Enabled)


