/**
 * @file bill_acceptor.h
 * @brief Header for the BillAcceptor class managing RS232 communication.
 */

#ifndef RS232_BILL_ACCEPTOR_H
#define RS232_BILL_ACCEPTOR_H

#include <Arduino.h>
#include "protocol_map.h"
#include "bill_acceptor_interface.h"

/**
 * @class RS232BillAcceptor
 * @brief Manages communication with the RS232 bill acceptor.
 *
 * This class implements the IBillAcceptor interface for RS232 communication.
 */
class RS232BillAcceptor : public IBillAcceptor {
public:
    /**
     * @brief Construct a new RS232 Bill Acceptor object.
     * @param serial A reference to the hardware serial port (e.g., Serial2).
     * @param baudRate The communication speed.
     * @param rxPin The GPIO pin for receiving data.
     * @param txPin The GPIO pin for transmitting data.
     */
    RS232BillAcceptor(HardwareSerial& serial, long baudRate, int8_t rxPin, int8_t txPin);

    /**
     * @brief Initializes the RS232 serial communication.
     */
    void begin() override;

    /**
     * @brief Sends the ENABLE command to the bill acceptor.
     */
    void enable() override;

    /**
     * @brief Sends the DISABLE command to the bill acceptor.
     */
    void disable() override;

    /**
     * @brief Reads and parses a byte from the serial port.
     * @return uint8_t The parsed code.
     */
    uint8_t read() override;

private:
    HardwareSerial& _serial;
    long _baudRate;
    int8_t _rxPin;
    int8_t _txPin;
};

#endif // RS232_BILL_ACCEPTOR_H
