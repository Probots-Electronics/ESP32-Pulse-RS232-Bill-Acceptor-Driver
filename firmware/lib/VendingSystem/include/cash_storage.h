/**
 * @file cash_storage.h
 * @brief Header for the CashStorage class managing persistent cash totals.
 */

#ifndef CASH_STORAGE_H
#define CASH_STORAGE_H

#include <Preferences.h>

/**
 * @class CashStorage
 * @brief Manages persistent storage of cash totals using NVS.
 *
 * This class is a wrapper for the ESP32 Preferences library to make
 * saving and loading the total cash amount straightforward.
 */
class CashStorage {
public:
    /**
     * @brief Construct a new Cash Storage object.
     * @param storageNamespace The namespace for the Preferences data.
     * @param storageKey The key for the cash value.
     */
    CashStorage(const char* storageNamespace, const char* storageKey);

    /**
     * @brief Initializes the persistent storage.
     */
    void begin();

    /**
     * @brief Saves the total cash amount to NVS.
     * @param totalCash The amount to save.
     */
    void saveTotal(float totalCash);

    /**
     * @brief Loads the total cash amount from NVS.
     * @return float The loaded amount, or 0.0 if not found.
     */
    float loadTotal();

    /**
     * @brief Closes the connection to the Preferences storage.
     */
    void end();

private:
    Preferences _preferences;
    const char* _namespace;
    const char* _key;
};

#endif // CASH_STORAGE_H
