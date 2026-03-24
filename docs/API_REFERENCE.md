# API Reference

Detailed class and method documentation for the Bill Acceptor libraries.

## lib/BillAcceptor

### `IBillAcceptor` (Interface)
The abstract base class for all acceptor drivers.

- `virtual void begin()`: Initialize pins and interrupts.
- `virtual void enable()`: Allow the acceptor to take money.
- `virtual void disable()`: Stop the acceptor from taking money.
- `virtual uint8_t read()`: Poll for new bill data. Returns `STATUS_IDLING` if no change, or a bill code (e.g., `CODE_BILL_10`).

---

### `PulseBillAcceptor`
Implements interrupt-driven pulse counting.

- **Constructor**: `PulseBillAcceptor(int8_t pulsePin, int8_t inhibitPin)`

---

### `UniversalBillAcceptor`
The high-level wrapper used in the main application.

- **Constructor**: `UniversalBillAcceptor(RS232BillAcceptor& rs232, PulseBillAcceptor& pulse)`
- **`read()`**: Returns the first valid signal from either RS232 or Pulse.

---

## lib/VendingSystem

### `CashStorage`
Handles persistence of currency values.

- `void add(int amount)`: Increments the total balance.
- `int getTotal()`: Retrieves current balance from memory.
- `void reset()`: Clears the balance.
