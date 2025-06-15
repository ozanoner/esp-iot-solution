/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

 #include "stdint.h"

 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /**
  * INA219 register addresses (per TI datasheet, Table 2)
  */
 enum ina219_reg_addr {
     INA219_REG_CFG         = 0x00,
     INA219_REG_VSHUNT      = 0x01,
     INA219_REG_VBUS        = 0x02,
     INA219_REG_POWER       = 0x03,
     INA219_REG_CURRENT     = 0x04,
     INA219_REG_CALIBRATION = 0x05,
 };
 
 /**
  * Configuration register (16-bit)
  */
 typedef union {
     struct __attribute__((packed)) {
         uint16_t mode       : 3; // Operating mode
         uint16_t vshct      : 3; // Shunt conversion time
         uint16_t vbusct     : 3; // Bus conversion time
         uint16_t avg        : 3; // Averaging mode
         uint16_t adcrange   : 1; // ADC range
         uint16_t reserved   : 2;
         uint16_t rst        : 1; // Reset bit
     } bit;
     uint16_t all;
 } ina219_reg_cfg_t;
 
 /**
  * Shunt voltage register (16-bit)
  */
 typedef union {
     uint16_t vshunt;
 } ina219_reg_vshunt_t;
 
 /**
  * Bus voltage register (16-bit)
  */
 typedef union {
     struct __attribute__((packed)) {
         uint16_t ovf       : 1;  // [0]  Math overflow flag
         uint16_t cnvr      : 1;  // [1]  Conversion ready flag
         uint16_t reserved  : 1;  // [2]  Reserved
         uint16_t vbus      : 13; // [15:3] Bus voltage data (LSB = 4 mV), must be right-shifted by 3 bits
     } bit;
     uint16_t all;
 } ina219_reg_vbus_t;
 
 /**
  * Power register (16-bit)
  */
 typedef union {
     uint16_t power;
 } ina219_reg_power_t;
 
 /**
  * Current register (16-bit)
  */
 typedef union {
     uint16_t current;
 } ina219_reg_current_t;
 
 /**
  * Calibration register (16-bit)
  */
 typedef union {
     struct __attribute__((packed)) {
         uint16_t shunt_cal : 15;
         uint16_t reserved  : 1;
     } bit;
     uint16_t all;
 } ina219_reg_calibration_t;
 
 /**
  * Combined register structure for INA219
  */
 typedef struct {
     ina219_reg_cfg_t           cfg;
     ina219_reg_vshunt_t        vshunt;
     ina219_reg_vbus_t          vbus;
     ina219_reg_power_t         power;
     ina219_reg_current_t       current;
     ina219_reg_calibration_t   calibration;
 } ina219_reg_t;
 
 #ifdef __cplusplus
 }
 #endif
 