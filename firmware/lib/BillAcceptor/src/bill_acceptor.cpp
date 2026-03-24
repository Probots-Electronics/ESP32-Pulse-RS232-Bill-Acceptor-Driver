/**
 * @file bill_acceptor.cpp
 * @brief Implementation of the RS232BillAcceptor class.
 */

#include "bill_acceptor.h"

/**
 * @brief Construct a new RS232 Bill Acceptor object.
 */
RS232BillAcceptor::RS232BillAcceptor(HardwareSerial& serial, long baudRate, int8_t rxPin, int8_t txPin) 
    : _serial(serial), _baudRate(baudRate), _rxPin(rxPin), _txPin(txPin) {
}

/**
 * @brief Initializes the RS232 serial communication.
 */
void RS232BillAcceptor::begin() {
    // ICT/Probots RS232 protocol typically uses 8 data bits, Even parity, and 1 stop bit (8E1).
    _serial.begin(_baudRate, SERIAL_8E1, _rxPin, _txPin);
}

/**
 * @brief Sends the ENABLE command to the bill acceptor.
 */
void RS232BillAcceptor::enable() {
    _serial.write(COMMAND_ENABLE_ALL);
    _serial.flush();
}

/**
 * @brief Sends the DISABLE command to the bill acceptor.
 */
void RS232BillAcceptor::disable() {
    _serial.write(COMMAND_DISABLE_ALL);
    _serial.flush();
}

/**
 * @brief Reads and parses a byte from the serial port.
 */
uint8_t RS232BillAcceptor::read() {
    if (_serial.available() > 0) {
        uint8_t byte = _serial.read();

        // 🚨 CRITICAL FIX: The ICT protocol requires an ACK (0x02) response
        // to every important byte. If we don't ACK, the acceptor will timeout
        // and spit the bill back out to the customer.
        if (byte != STATUS_IDLING) {
            _serial.write(COMMAND_ACK);
        }

        switch (byte) {
            // Status Codes
            case STATUS_IDLING:
            case STATUS_ACCEPTING:
            case STATUS_ESCROWED:
            case STATUS_STACKING:
            case STATUS_VEND_VALID:
            case STATUS_STACKED:
            case STATUS_REJECTING:
            case STATUS_RETURNING:
            case STATUS_JAMMED:
            case STATUS_STACKER_FULL:
            case STATUS_CASSETTE_REMOVED:
                return byte;

            // Bill Value Codes
            case CODE_BILL_5:
            case CODE_BILL_10:
            case CODE_BILL_20:
            case CODE_BILL_50:
            case CODE_BILL_100:
            case CODE_BILL_200:
            case CODE_BILL_500:
                return byte;

            default:
                return CODE_UNKNOWN;
        }
    }
    return STATUS_IDLING; 
}
