# Hardware Setup Guide

This guide details the physical connections between the ESP32 and the ICT TP77 Bill Acceptor.

## ⚠️ Critical Warning
> [!WARNING]
> Both the ESP32 and the bill acceptor **must share a common ground**. 
> DO NOT connect the 12V power supply directly to the ESP32 GPIO pins.

## 1. Protocol Wiring Diagrams
Choose **one** of the following communication protocols depending on your `platformio.ini` environment.

### Option A: RS232 Mode (Recommended)
This uses the secondary **8-Pin TMT** harness. 

| Signal | Pin | Color | ESP32 / Power Connection |
| :--- | :--- | :--- | :--- |
| **Acceptor TX (TXD)** | Pin 8  | **GPIO 16 (RX2)** |
| **Acceptor RX (RXD)** | Pin 7  | **GPIO 17 (TX2)** |
| **VCC (+12V DC)** | Pin 6 |**3.3 to 5V DC Supply** |
| **GND** | Pin 1 | **Common GND** |

> [!CAUTION]
> The RS232 interface uses **5V TTL Logic**. Since the ESP32 is a 3.3V device, you **must use a Logic Level Shifter** or a voltage divider on the Purple (TXD) line to prevent damage to the ESP32.

### Option B: Pulse Mode (Legacy)
This uses the primary **9-Pin AMP** heavy power harness.

| Signal | Pin | Color | ESP32 / Power Connection |
| :--- | :--- | :--- | :--- |
| **Pulse (METER+)** | Pin 7 | Blue | **GPIO 27** (Pulse Input) |
| **Pulse (METER-)** | Pin 8 | Purple | **Common GND** |
| **Inhibit Pin (+)** | Pin 1 | Yellow | **GPIO 5** (Enable Output) |
| **Inhibit Pin (-)** | Pin 2 | Green | **Common GND** |
| **VCC (+12V)** | Pin 5 | Red | **+12V DC Supply** |
| **GND** | Pin 9 | Orange | **Common GND** |

> [!NOTE]
> A **10kΩ Pull-Up Resistor** is recommended between GPIO 27 and 3.3V for reliable pulse counting.

## 2. DIP Switch Configuration (India INR Firmware)
You must physically program the bill acceptor's behavior using the small DIP switches on its side. Always completely unplug the 12V power adapter for 5 seconds after changing any dip switch position.

### For RS232 Serial Protocol
*   **Bank 1 (10 Switches):** SW6,SW7, SW8, SW9, SW10 -> **ON**
*   **Bank 2 (4 Switches):** SW2 -> **OFF**
*   **Bank 2 (4 Switches):** SW3 -> **OFF** (Turn-Over / 4-way direction enabled)

### For Pulse Protocol
*   **Bank 2 (4 Switches):** SW2 -> **ON**
*   **Bank 2 (4 Switches):** SW1 -> **OFF** (Normal Low / Normally Closed)
