/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "i2c_bus.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INA219_I2C_ADDRESS_DEFAULT   (0x40)

/**
 * @brief INA219 handle
 *
 */
typedef struct ina219_t *ina219_handle_t;

/**
 * @brief ina219 alert callback function
 *
 */
typedef void (*ina219_alert_cb_t)(void *arg);

/**
 * @brief ina219 configuration structure
 *
 */
typedef struct {
    i2c_bus_handle_t bus;         /*!< I2C bus object */
    bool alert_en;                /*!< Enable alert callback*/
    uint8_t dev_addr;             /*!< I2C device address */
    uint8_t alert_pin;            /*!< Alert pin number */
    ina219_alert_cb_t alert_cb;   /*!< Alert callback function */
} ina219_config_t;

/**
 * @brief Create and init object and return a handle
 *
 * @param handle Pointer to handle
 * @param config Pointer to configuration
 *
 * @return
 *     - ESP_OK Success
 *     - Others Fail
 */
esp_err_t ina219_create(ina219_handle_t *handle, ina219_config_t *config);

/**
 * @brief Deinit object and free memory
 *
 * @param handle ina219 handle Handle
 *
 * @return
 *     - ESP_OK Success
 *     - Others Fail
 */
esp_err_t ina219_delete(ina219_handle_t handle);

/**
 * @brief Get the Voltage on the bus
 *
 * @param handle object handle of ina219
 * @param volt Voltage value in volts
 * @return
 *    - ESP_OK Success
 *    - Others Fail
 */
esp_err_t ina219_get_voltage(ina219_handle_t handle, float *volt);

/**
 * @brief Get the Current on the bus
 *
 *
 * @param handle object handle of ina219
 * @param curr Current value in A
 * @return
 *     - ESP_OK Success
 *     - Others Fail
 */
esp_err_t ina219_get_current(ina219_handle_t handle, float *curr);

#ifdef __cplusplus
}
#endif
