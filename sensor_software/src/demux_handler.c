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
 * @file    demux.c
 * @author  Noah Rozario
 * @date    2020-06-17
 */


#include "gio.h"
#include "demux_handler.h"

void demux_enable(gioPORT_t *port)
{
    gioSetBit(port, DEMUX_E1_PIN, LOW);
    gioSetBit(port, DEMUX_E2_PIN, LOW);
    gioSetBit(port, DEMUX_E3_PIN, HIGH);
}

/**
 * @brief
 *      Selects the given output pin.
 * @param port
 *      GIO port register
 * @param pin
 *      Select which output pin to use. (OUT_Y0...OUT_Y7)
 */
void demux_select_pin(gioPORT_t *port, enum demux_out_pins pin)
{
    //gioInit(); //initialising gio ports
    switch(pin)
    {
        case OUT_Y0:
            gioSetBit(port, DEMUX_A0_PIN, LOW);
            gioSetBit(port, DEMUX_A1_PIN, LOW);
            gioSetBit(port, DEMUX_A2_PIN, LOW);
            break;

        case OUT_Y1:
            gioSetBit(port, DEMUX_A0_PIN, HIGH);
            gioSetBit(port, DEMUX_A1_PIN, LOW);
            gioSetBit(port, DEMUX_A2_PIN, LOW);
            break;

        case OUT_Y2:
            gioSetBit(port, DEMUX_A0_PIN, LOW);
            gioSetBit(port, DEMUX_A1_PIN, HIGH);
            gioSetBit(port, DEMUX_A2_PIN, LOW);
            break;

        case OUT_Y3:
            gioSetBit(port, DEMUX_A0_PIN, HIGH);
            gioSetBit(port, DEMUX_A1_PIN, HIGH);
            gioSetBit(port, DEMUX_A2_PIN, LOW);
            break;

        case OUT_Y4:
            gioSetBit(port, DEMUX_A0_PIN, LOW);
            gioSetBit(port, DEMUX_A1_PIN, LOW);
            gioSetBit(port, DEMUX_A2_PIN, HIGH);
            break;

        case OUT_Y5:
            gioSetBit(port, DEMUX_A0_PIN, HIGH);
            gioSetBit(port, DEMUX_A1_PIN, LOW);
            gioSetBit(port, DEMUX_A2_PIN, HIGH);
            break;

        case OUT_Y6:
            gioSetBit(port, DEMUX_A0_PIN, LOW);
            gioSetBit(port, DEMUX_A1_PIN, HIGH);
            gioSetBit(port, DEMUX_A2_PIN, HIGH);
            break;

        case OUT_Y7:
            gioSetBit(port, DEMUX_A0_PIN, HIGH);
            gioSetBit(port, DEMUX_A1_PIN, HIGH);
            gioSetBit(port, DEMUX_A2_PIN, HIGH);
            break;

        default:
            //OUT_Y0 default 
            gioSetBit(port, DEMUX_A0_PIN, LOW);
            gioSetBit(port, DEMUX_A1_PIN, LOW);
            gioSetBit(port, DEMUX_A2_PIN, LOW);
            break;
    }
}


