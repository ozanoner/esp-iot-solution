/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_log.h"
#include "esp_check.h"
#include "driver/gpio.h"
#include "i2c_bus.h"
#include "ina219.h"
#include "ina_219_reg.h"

static const char *TAG = "INA219";
#define ESP_INTR_FLAG_DEFAULT 0

typedef struct {
    uint8_t i2c_addr;
    i2c_bus_device_handle_t i2c_dev;
    ina219_reg_t reg;
} ina219_t;

static esp_err_t ina219_read_reg(ina219_t *ina219, uint8_t reg, uint16_t *data)
{
    uint8_t ina219_data[2] = { 0 };
    esp_err_t ret = i2c_bus_read_bytes(ina219->i2c_dev, reg, 2, &ina219_data[0]);
    *data = ((ina219_data[0] << 8 | ina219_data[1]));
    return ret;
}

static esp_err_t ina219_write_reg(ina219_t *ina219, uint8_t reg, uint16_t *data)
{
    esp_err_t ret = i2c_bus_write_bytes(ina219->i2c_dev, reg, 2, (uint8_t *)&data);
    return ret;
}

static esp_err_t ina219_reg_init(ina219_t *ina219)
{
    uint16_t calibration = 10;
    ina219_write_reg(ina219, INA219_REG_CALIBRATION, &calibration);
    ina219_reg_cfg_t cfg = {
        .bit.adcrange = 0,
        .bit.avg = 0,
        .bit.mode = 0x07,
        .bit.rst = 0,
        .bit.vbusct = 0x02,
        .bit.vshct = 0x02,
    };
    ina219_write_reg(ina219, INA219_REG_CFG, (uint16_t *)&cfg);
    return ESP_OK;
}
static void ina219_read_all_reg(ina219_t *ina219)
{
    ina219_read_reg(ina219, INA219_REG_CFG, (uint16_t *)&ina219->reg.cfg);
    ina219_read_reg(ina219, INA219_REG_VSHUNT, (uint16_t *)&ina219->reg.vshunt);
    ina219_read_reg(ina219, INA219_REG_VBUS, (uint16_t *)&ina219->reg.vbus);
    ina219_read_reg(ina219, INA219_REG_POWER, (uint16_t *)&ina219->reg.power);
    ina219_read_reg(ina219, INA219_REG_CURRENT, (uint16_t *)&ina219->reg.current);
    ina219_read_reg(ina219, INA219_REG_CALIBRATION, (uint16_t *)&ina219->reg.calibration);
}

esp_err_t ina219_create(ina219_handle_t *handle, ina219_config_t *config)
{
    esp_err_t err = ESP_OK;
    ESP_RETURN_ON_FALSE(handle, ESP_ERR_INVALID_ARG, TAG, "handle is NULL");
    ESP_RETURN_ON_FALSE(config, ESP_ERR_INVALID_ARG, TAG, "config is NULL");
    ina219_t *ina219 = (ina219_t *)calloc(1, sizeof(ina219_t));
    ESP_RETURN_ON_FALSE(ina219, ESP_ERR_NO_MEM, TAG, "Failed to allocate memory for ina219");

    ina219->i2c_addr = config->dev_addr;
    ina219->i2c_dev = i2c_bus_device_create(config->bus, ina219->i2c_addr, i2c_bus_get_current_clk_speed(config->bus));
    if (ina219->i2c_dev == NULL) {
        free(ina219);
        return ESP_FAIL;
    }
    ina219_reg_init(ina219);
    ina219_read_all_reg(ina219);
   
    *handle = (ina219_handle_t)ina219;
    return err;
}

esp_err_t ina219_delete(ina219_handle_t handle)
{
    ina219_t *ina219 = (ina219_t *)handle;
    i2c_bus_device_delete(&ina219->i2c_dev);
    free(ina219);
    return ESP_OK;
}

esp_err_t ina219_get_voltage(ina219_handle_t handle, float *volt)
{
    ina219_t *ina219 = (ina219_t *)handle;
    uint16_t buffer = 0;
    ina219_read_reg(ina219, INA219_REG_VBUS, &buffer);
    *volt = buffer * 0.0016f;
    return ESP_OK;
}

esp_err_t ina219_get_current(ina219_handle_t handle, float *curr)
{
    uint16_t buffer = 0;
    ina219_t *ina219 = (ina219_t *)handle;
    ina219_read_reg(ina219, INA219_REG_VSHUNT, &buffer);
    if ((buffer & 0x8000) >> 15) {
        *curr = 0;
    } else {
        *curr = buffer / 3970.0f;
    }
    return ESP_OK;
}
