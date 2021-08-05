/*
 * Copyright (C) 2015  University of Alberta
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/**
 * @file    hyperion.h
 * @author  Trung Tran
 * @date    2021-06-04
 */

#ifndef EX2_HAL_EX2_HYPERION_SOLAR_PANEL_SOFTWARE_HARDWARE_INTERFACE_INCLUDE_HYPERION_H_
#define EX2_HAL_EX2_HYPERION_SOLAR_PANEL_SOFTWARE_HARDWARE_INTERFACE_INCLUDE_HYPERION_H_

#include <ex2_hal/ex2_hyperion_solar_panel_software/equipment_handler/include/adc_handler.h>

typedef struct __attribute__((packed)) {
    float Nadir_Temp1;
    float Port_Temp1;
    float Port_Temp2;
    float Port_Temp3;
    float Port_Temp_Adc;
    float Port_Dep_Temp1;
    float Port_Dep_Temp2;
    float Port_Dep_Temp3;
    float Port_Dep_Temp_Adc;
    float Star_Temp1;
    float Star_Temp2;
    float Star_Temp3;
    float Star_Temp_Adc;
    float Star_Dep_Temp1;
    float Star_Dep_Temp2;
    float Star_Dep_Temp3;
    float Star_Dep_Temp_Adc;
    float Zenith_Temp1;
    float Zenith_Temp2;
    float Zenith_Temp3;
    float Zenith_Temp_Adc;
    float Nadir_Pd1;
    float Port_Pd1;
    float Port_Pd2;
    float Port_Pd3;
    float Port_Dep_Pd1;
    float Port_Dep_Pd2;
    float Port_Dep_Pd3;
    float Star_Pd1;
    float Star_Pd2;
    float Star_Pd3;
    float Star_Dep_Pd1;
    float Star_Dep_Pd2;
    float Star_Dep_Pd3;
    float Zenith_Pd1;
    float Zenith_Pd2;
    float Zenith_Pd3;
    float Port_Voltage;
    float Port_Dep_Voltage;
    float Star_Voltage;
    float Star_Dep_Voltage;
    float Zenith_Voltage;
    float Port_Current;
    float Port_Dep_Current;
    float Star_Current;
    float Star_Dep_Current;
    float Zenith_Current;
} Hyperion_HouseKeeping;

void hyperion_config_1_value(enum config_1_panel_t panel, enum config_1_channel_type_t channel, float* param);
void hyperion_config_2_value(config_2_panel_t panel, config_2_channel_type_t channel, float* param);
void hyperion_config_3_value(config_3_panel_t panel, config_3_channel_type_t channel, float* param);
#endif /* EX2_HAL_EX2_HYPERION_SOLAR_PANEL_SOFTWARE_HARDWARE_INTERFACE_INCLUDE_HYPERION_H_ */
