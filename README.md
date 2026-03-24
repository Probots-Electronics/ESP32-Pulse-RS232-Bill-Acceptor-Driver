# ESP32 Vending Controller (Bill Acceptor)

A robust, industry-standard firmware for interfacing ICT/Probots Bill Acceptors with ESP32 microcontrollers. This project features a modular, test-driven architecture supporting both **RS232** and **Pulse** protocols simultaneously.

![Architecture Overview](https://img.shields.io/badge/Architecture-Modular-blue)
![Testing](https://img.shields.io/badge/Tests-Unity_Native-green)
![Hardware](https://img.shields.io/badge/Hardware-ESP32-red)

## 🚀 Key Features

- **Universal Mode**: Automatically detects and processes both RS232 and Pulse signals in parallel.
- **Modular Library Structure**: Decoupled drivers for Pulse, RS232, and Universal modes.
- **Multi-Currency Support**: Currently optimized for **INR**, but easily adaptable for USD, EUR, etc.
- **India-Specific Support**: Pre-configured for the Indian INR8 dataset (1:5 Pulse ratio).
- **Persistent Ledger**: Total cash counts are saved to NVS (Non-Volatile Storage).

## 🔌 Hardware Setup

Connect your Bill Acceptor according to the [Hardware Setup Guide](docs/HARDWARE_SETUP.md).

| Signal | ESP32 Pin | Logic Level |
| :--- | :--- | :--- |
| **RS232 RXD** | GPIO 16 (RX2) | 3.3V TTL (Direct Connect) |
| **RS232 TXD** | GPIO 17 (TX2) | 3.3V TTL (Direct Connect) |
| **Pulse Output**| GPIO 27 | 3.3V (Requires 10k Pull-up) |
| **Inhibit Pin** | GPIO 5 | 3.3V |

## 💻 Quick Start: Using the Library
To use the bill acceptor in your own custom ESP32 project, simply include the library and pass the Serial interface:

```cpp
#include "protocol_map.h"
#include "bill_acceptor.h"

// 1. Initialize the RS232 object on Pins 16 & 17
RS232BillAcceptor acceptor(Serial2, 9600, 16, 17);

void setup() {
    acceptor.begin();
    acceptor.enable(); // Sends 0x3E to wake up the motors
}

void loop() {
    // 2. Simply call read() in your main loop!
    // Handshaking (0x02 ACK) is handled automatically behind the scenes.
    uint8_t code = acceptor.read();

    if (code == CODE_BILL_50) {
        Serial.println("Accepted ₹50!");
    } else if (code == STATUS_STACKED) {
        Serial.println("Money securely dropped in box!");
    }
}
```

## 📖 Documentation Roadmap

A comprehensive **[Documentation Roadmap](docs/README.md)** is available, covering every aspect of the project:

- 🏛️ **[System Architecture](docs/ARCHITECTURE.md)**: Design patterns and library organization.
- 🔌 **[Hardware Setup](docs/HARDWARE_SETUP.md)**: Pinouts, wiring diagrams, and DIP switch settings.
- 📘 **[API Reference](docs/API_REFERENCE.md)**: Class headers and method documentation.
- 📦 **[Deployment Guide](docs/DEPLOYMENT.md)**: Build, upload, and hardware verification instructions.
- 📜 **[Protocol Specs](docs/protocol_specs.md)**: Hex codes and Pulse mappings for Indian Rupees.

## 🛠️ Installation & Build

1. **Install PlatformIO**: Ensure the PlatformIO Core or VS Code extension is installed.
2. **Clone & Open**: Open the `firmware/` directory.
3. **Build & Upload**:
   ```bash
   # Upload to ESP32 (Universal Mode)
   pio run -t upload
   ```

## 🛡️ License
This project is licensed under the MIT License.
