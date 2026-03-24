# Documentation Roadmap

Welcome to the ESP32 Vending Controller documentation. This directory contains all technical guides required to build, configure, and maintain the system.

## 🧭 Documentation Map

| Document | Purpose | Audience |
| :--- | :--- | :--- |
| 🚀 **[Quick Start](../README.md)** | Project overview and fast-build instructions. | Beginners |
| 🔌 **[Hardware Setup](HARDWARE_SETUP.md)** | Wiring diagrams, DIP switches, and level shifter specs. | Hardware Engineers |
| 🏛️ **[System Architecture](ARCHITECTURE.md)** | Modular design, class relations, and operational flow. | Architects / Developers |
| 📗 **[API Reference](API_REFERENCE.md)** | Detailed library method signatures and class details. | Developers |
| 📦 **[Deployment Guide](DEPLOYMENT.md)** | PlatformIO environment setup and upload steps. | Operators |
| 📜 **[Protocol Specs](protocol_specs.md)** | Hex/Pulse specs and Multi-Currency setup guide. | Integration Specialists |

---

## 📅 Roadmap for Future Documentation

The following guides are planned for future releases to reach full industrial grade compliance:

- [ ] **Error Handling & Fault Recovery**: Comprehensive guide on resolving jams and NVS corruption.
- [ ] **Security Hardening Guide**: Best practices for protecting physical serial lines and Inhibit logic.
- [ ] **Power Management Specs**: Detailed analysis of current draw at peak acceptance.
- [ ] **I2C Expansion Guide**: Adding additional peripherals like coin acceptors or keypads.

---

## 🛠️ Tools Used
- **Doxygen**: Used for source code level documentation.
- **Mermaid.js**: Used for architecture and flow diagrams within these Markdown files.
- **PlatformIO**: Used as the primary build and test management system.
