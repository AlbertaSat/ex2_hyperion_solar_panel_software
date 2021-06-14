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
 * @file    adc_handler.c
 * @author  Vasu Gupta
 * @date    2020-06-15
 */

#include "adc_handler.h"

/**
 * @brief
 * 		Send value over SPI
 * @details
 * 		This function uses Halcogen generated SPI drivers to send data over SPI.
 *      The CS lines are set accordingly using demux_handler functions.
 * @param handl
 * 		Pointer to an ADC_Handler struct
 * @param data
 * 		Pointer to data buffer
 * @param len
 * 		Size of data buffer
 * @return
 * 		None
 */
void write_spi(ADC_Handler *handl, uint16_t *data, uint32_t len) 
{
    spiTransmitData_ExpectAnyArgsAndReturn(0xFF);
    //demux_select_pin();
    spiTransmitData(SPI_BASE_ADDR, &(handl->spi_dat_conf), len, data);
    //demux_reset();
}

/**
 * @brief
 * 		Receive value over SPI
 * @details
 * 		This function uses Halcogen generated SPI drivers to receive data over SPI.
 *      The CS lines are set accordingly using demux_handler functions.
 * @param handl
 * 		Pointer to an ADC_Handler struct
 * @param data
 * 		Pointer to data buffer
 * @param len
 * 		Size of data buffer
 * @return
 * 		None
 */
void read_spi(ADC_Handler *handl, uint16_t *data, uint32_t len) 
{
    spiReceiveData_ExpectAnyArgsAndReturn(0xFF);
    spiReceiveData_ReturnArrayThruPtr_destbuff(&Mock_Buffer.value, Mock_Buffer.size);
    //demux_select_pin();
    spiReceiveData(SPI_BASE_ADDR, &(handl->spi_dat_conf), len, data);
    //demux_reset();
}

/**
 * @brief
 * 		Initialize ADC_Handler struct
 * @details
 * 		This function sets the default control register value, 
 *      and the SPI register configuration
 * @param handl
 * 		Pointer to an ADC_Handler struct
 * @return
 * 		None
 */
void adc_init(ADC_Handler *handl, enum panel_t panel) {
    //Initialize ADC defaults and SPI here
    handl->control_reg_val  = 0;
    
    //Initialize SPI register configuration (Refer to TMS570LS1224 datasheet)
    handl->spi_dat_conf.CS_HOLD = TRUE;
    handl->spi_dat_conf.WDEL    = TRUE;
    handl->spi_dat_conf.DFSEL   = SPI_FMT_0;
    handl->spi_dat_conf.CSNR    = 0xFF;

    handl->panel = panel;
}

/**
 * @brief
 * 		Set and send control register bits to the ADC
 * @details
 * 		Refer to page 16 of AD7298 datasheet for more details
 * @param handl
 * 		Pointer to ADC_Handler struct
 * @param repeat
 * 		1 = Enable
 *      0 = Disable
 * @param channel
 * 		Select which channel to enable (0-7)
 *      Accepted values: 0-7
 * @param ext_ref
 *      Enable/Disable the use of external reference.
 * 		1 = Enable
 *      0 = Disable
 * @param tsense
 *      Enable/Disable (internal) temperature conversion
 * 		1 = Enable
 *      0 = Disable
 * @param tsense_avg
 *      Enable/Disable temperature sensor averaging function
 * 		1 = Enable
 *      0 = Disable
 * @return
 * 		None
 */
void adc_set_control_reg(ADC_Handler *handl, uint8_t repeat,
                                             uint8_t channel,
                                             uint8_t ext_ref,
                                             uint8_t tsense,
                                             uint8_t tsense_avg) 
{
    uint16_t control_reg_value = 0;

    control_reg_value = AD7298_WRITE 
                      | (repeat     * AD7298_REPEAT) 
                      |  AD7298_CH(channel)
                      | (ext_ref    * AD7298_EXT_REF)
                      | (tsense     * AD7298_TSENSE)
                      | (tsense_avg * AD7298_TSENSE_AVG);

    //send buffer to SPI
    write_spi(handl, &control_reg_value, 1);    

    handl->control_reg_val = control_reg_value;
    
}

/**
 * @brief
 * 		Returns the conversion results from ADC
 * @details
 * 		The ADC returns a 16-bit value, in which the 4 MSB are channel address bits,
 *      and the remaining 12 is the raw ADC value
 * @attention
 *      This function MUST be called twice (second time gives the correct value).
 *      According to the AD7298 datasheet, first read from DOUT will always be invalid.
 * @param handl
 * 		Pointer to ADC_Handler struct
 * @param data
 * 		Buffer for storing ADC raw value
 * @param ch
 * 		Buffer for storing which channel the value was received from.
 * @return
 * 		None
 */
void adc_get_raw(ADC_Handler *handl, uint16_t *data, uint8_t *ch) 
{  
    uint16_t buffer = 0;

    //SPI read into value
    read_spi(handl, &buffer, 1);
    
    *data = buffer & 0x0FFF;
    
    //get current data channel as well.
    *ch   = (buffer >> 12);
}

/**
 * @brief
 * 		Converts the given raw ADC value to voltage (mV), relative to reference voltage.
 * @param value
 * 		The raw ADC value
 *      This value can be retrieved using adc_get_raw(..)
 * @param vref
 * 		The value of reference voltage (in mV) provided to the ADC module
 *      Refer to AD7298 datasheet.
 * @return
 * 		Value in mV.
 */
float adc_calculate_vin(uint16_t value, float vref) {
    float volts = 0;

    // from AD7298 datasheet
    volts = ((float)value * vref) / AD7298_RES;

    return volts;
}

/**
 * @brief
 * 		Calculates temperature in celsius from temperature sensor output voltage.
 * @param value
 * 		The raw ADC value
 *      This value can be retrieved using adc_get_raw(..)
 * @param vref
 * 		The value of reference voltage (in mV) provided to the ADC module
 *      Refer to AD7298 datasheet.
 * @return
 * 		Value in celsius.
 */
float adc_calculate_sensor_temp(uint16_t value, float vref) {
    float temp_voltage = adc_calculate_vin(value, vref);

    float celsius = 0;

    celsius = temp_voltage - TEMP_VOLT_MIN;
    celsius = (celsius * (TEMP_VAL_MAX - TEMP_VAL_MIN)) 
                / (TEMP_VOLT_MAX - TEMP_VOLT_MIN);
    celsius = TEMP_VAL_MAX - celsius;
    
    return celsius;
}

/**
 * @brief
 * 		Converts the given raw ADC value to voltage (mV), relative to reference voltage.
 * @param value
 * 		The raw ADC value
 *      This value can be retrieved using adc_get_raw(..)
 * @param vref
 * 		The value of reference voltage (in mV) provided to the ADC module
 *      Refer to AD7298 datasheet.
 * @return
 * 		Value in mV.
 */
float adc_calculate_sensor_voltage(uint16_t value, float vref) {
    float val = adc_calculate_vin(value, vref);

    val = val * ((VOLT_MAX - VOLT_MIN) / (ADC_VOLT_MAX - ADC_VOLT_MIN)); 

    return val;
}

/**
 * @brief
 * 		Converts the given raw ADC value to current (mA), relative to reference voltage.
 * @param value
 * 		The raw ADC value
 *      This value can be retrieved using adc_get_raw(..)
 * @param vref
 * 		The value of reference voltage (in mV) provided to the ADC module
 *      Refer to AD7298 datasheet.
 * @return
 * 		Value in mA.
 */
float adc_calculate_sensor_current(uint16_t value, float vref)
{
    float val = adc_calculate_vin(value, vref);

    val = val * ((CURR_MAX - CURR_MIN) / (ADC_VOLT_MAX - ADC_VOLT_MIN)); 

    return val;
}

/**
 * @brief
 * 		Calculates temperature in celsius from temperature sensor output voltage.
 * @param value
 * 		The raw ADC value
 *      This value can be retrieved using adc_get_raw(..)
 * @param vref
 * 		The value of reference voltage (in mV) provided to the ADC module
 *      Refer to AD7298 datasheet.
 * @return
 * 		Value in celsius.
 */
float adc_calculate_sensor_pd(uint16_t value, float vref)
{
    float val = adc_calculate_vin(value, vref);

    return val;
}

/**
 * @brief
 * 		Converts given raw ADC temperature value (from internal temp sensor) to celsius 
 *      (relative to reference voltage)
 * @details
 * 		The raw ADC value is retrieved from the TSENSE channel on the ADC
 *      Refer to AD7298 datasheet.
 * @attention
 * 		This function is to be used only if 'TSENSE' on the ADC control reg. is enabled.
 * @param handl
 * 		Pointer to ADC_Handler struct
 * @param value
 * 		The raw ADC temperature value
 *      This value can be retrieved usig adc_get_raw(..) and when 'TSENSE' is enabled
 *      by setting the control register
 * @param vref
 * 		The value of reference voltage (in mV) provided to the ADC module
 *      Refer to AD7298 datasheet.
 * @return
 * 		Temperature value in celsius
 */
float adc_get_tsense_temp(uint16_t value, float vref) {
    float temp_celsius = 0;

    if(value >= 0x800) {
        value  = AD7298_RES - value;
        value *= -1;
    }

    // from AD7298 datasheet
    temp_celsius = vref * (((float)value / 10) + 109.3) - 273.15; 

    return temp_celsius;
}

