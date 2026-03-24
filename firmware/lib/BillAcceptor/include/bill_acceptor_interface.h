/**
 * @file bill_acceptor_interface.h
 * @brief Interface for Bill Acceptor communication modules.
 */

#ifndef BILL_ACCEPTOR_INTERFACE_H
#define BILL_ACCEPTOR_INTERFACE_H

#include <Arduino.h>

/**
 * @class IBillAcceptor
 * @brief Abstract base class for bill acceptor communication.
 */
class IBillAcceptor {
public:
    virtual ~IBillAcceptor() {}

    /**
     * @brief Initializes the communication method.
     */
    virtual void begin() = 0;

    /**
     * @brief Enables the bill acceptor to start accepting cash.
     */
    virtual void enable() = 0;

    /**
     * @brief Disables the bill acceptor.
     */
    virtual void disable() = 0;

    /**
     * @brief Reads the latest status or bill code from the acceptor.
     * @return uint8_t The parsed code (status or bill value code).
     */
    virtual uint8_t read() = 0;
};

#endif // BILL_ACCEPTOR_INTERFACE_H
