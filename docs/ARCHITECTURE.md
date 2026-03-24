# System Architecture

The ESP32 Vending Controller is designed with a modular, interface-driven architecture to support multiple bill acceptor protocols simultaneously.

## Component Overview

```mermaid
graph TD
    Main[main.cpp] --> |Uses| Acceptor[IBillAcceptor Interface]
    Main --> |Updates| Cash[CashStorage]
    
    Acceptor --> |Impl| RS232[RS232BillAcceptor]
    Acceptor --> |Impl| Pulse[PulseBillAcceptor]
    Acceptor --> |Wrapper| Universal[UniversalBillAcceptor]
    
    Universal --> |Polls| RS232
    Universal --> |Polls| Pulse
```

### Core Libraries

#### 1. BillAcceptor (`lib/BillAcceptor`)
Handles communication with the hardware.
- **`IBillAcceptor`**: Abstract base class defining `begin()`, `read()`, `enable()`, and `disable()`.
- **`RS232BillAcceptor`**: High-security serial driver (9600 baud, 8E1).
- **`PulseBillAcceptor`**: High-speed interrupt-driven pulse counter.
- **`UniversalBillAcceptor`**: A protocol-agnostic wrapper that allows both modes to run in parallel.

#### 2. VendingSystem (`lib/VendingSystem`)
Manages high-level state.
- **`CashStorage`**: Persistent tracking of total money accepted via NVRAM.

## Operational Flow

The following diagram illustrates the main application logic:

```mermaid
flowchart TD
    Start([Power On]) --> Init[Initialize NVS & Serial]
    Init --> Load[Load Total Cash from NVS]
    Load --> SetupBA[Initialize Bill Acceptor]
    SetupBA --> Loop{Main Loop}
    
    Loop --> Poll[Call acceptor->read]
    Poll --> NewCode{New Valid Code?}
    
    NewCode -- No --> Loop
    NewCode -- Yes --> Map[Map Code to Currency Value]
    
    Map --> UpdateTotal[Update Running Total]
    UpdateTotal --> Store[Save New Total to NVS]
    Store --> Log[Print to Serial Monitor]
    Log --> Delay[Wait 2s - Status Clear]
    Delay --> Loop
```

### Communication Flow
1. **Pulse**: Interrupts increment a volatile counter; `read()` checks for a 500ms inactivity timeout before returning a value.
2. **RS232**: Periodic polling for hex bytes via HardwareSerial (Serial2).
3. **Storage**: Values are stored in NVS/EEPROM to survive power failure.
