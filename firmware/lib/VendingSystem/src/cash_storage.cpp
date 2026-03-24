/**
 * @file cash_storage.cpp
 * @brief Implementation of the CashStorage class for NVS persistence.
 */

#include "cash_storage.h"

/**
 * @brief Construct a new Cash Storage object.
 * @param[in] storageNamespace NVS namespace to use.
 * @param[in] storageKey Key for the total cash value.
 */
CashStorage::CashStorage(const char* storageNamespace, const char* storageKey)
    : _namespace(storageNamespace), _key(storageKey) {
}

/**
 * @brief Initializes the storage connection in Read/Write mode.
 */
void CashStorage::begin() {
    _preferences.begin(_namespace, false); // false = R/W mode
}

/**
 * @brief Persists the current cash total to NVS.
 * @param[in] totalCash The value to save.
 */
void CashStorage::saveTotal(float totalCash) {
    _preferences.putFloat(_key, totalCash);
}

/**
 * @brief Loads the cached total from NVS.
 * @return float The total amount, defaults to 0.0 if not found.
 */
float CashStorage::loadTotal() {
    // Returns the stored value, or 0.0 if the key doesn't exist.
    return _preferences.getFloat(_key, 0.0f);
}

/**
 * @brief Closes the NVS storage connection.
 */
void CashStorage::end() {
    _preferences.end();
}
