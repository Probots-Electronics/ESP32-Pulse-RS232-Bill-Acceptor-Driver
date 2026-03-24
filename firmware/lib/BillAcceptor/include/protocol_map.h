#ifndef PROTOCOL_MAP_H
#define PROTOCOL_MAP_H

/**
 * @file protocol_map.h
 * @brief Defines the hex codes for the ICT Bill Acceptor protocol.
 *
 * This file contains strictly typed macros for all known command and status
 * bytes sent by the bill acceptor.
 *
 */

// --- Status Codes ---
#define STATUS_IDLING 0x80
#define STATUS_ACCEPTING 0x81
#define STATUS_ESCROWED 0x82
#define STATUS_STACKING 0x83
#define STATUS_VEND_VALID 0x84
#define STATUS_STACKED 0x10 // Updated to 0x10 based on TP77 logs
#define STATUS_REJECTING 0x86
#define STATUS_RETURNING 0x87
#define STATUS_JAMMED 0x8A
#define STATUS_STACKER_FULL 0x8B
#define STATUS_CASSETTE_REMOVED 0x8C

// --- Bill Value Codes (sent on successful vend) ---
#define CODE_BILL_5  0x40  // 5 currency units
#define CODE_BILL_10 0x41  // 10 currency units
#define CODE_BILL_20 0x42  // 20 currency units
#define CODE_BILL_50 0x43  // 50 currency units
#define CODE_BILL_100 0x44 // 100 currency units
#define CODE_BILL_200 0x45 // 200 currency units
#define CODE_BILL_500 0x46 // 500 currency units

// --- Commands ---
#define COMMAND_ENABLE_ALL  0x3E  // Enable all bill types
#define COMMAND_DISABLE_ALL 0x5E  // Disable all bill types
#define COMMAND_ACK         0x02  // Mandatory Acknowledge

// --- Error/Unknown Code ---
#define CODE_UNKNOWN 0xFF // For handling unrecognized bytes

#endif // PROTOCOL_MAP_H
