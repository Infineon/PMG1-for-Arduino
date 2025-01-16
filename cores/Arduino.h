/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Copyright (c) 2018 Infineon Technologies AG
  This file has been modified for the XMC microcontroller series.
*/
#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#ifdef __cplusplus
extern "C" {
#endif

//****************************************************************************
// @Std Includes
//****************************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <types.h>
#include <math.h>
#include <pgmspace.h>
#include "cy_pdl.h"
#include "config.h"


typedef struct
{
  GPIO_PRT_Type *port;
  uint32_t  pin;
} ifx_gpio_port_pin_t;

typedef struct
{
  uint8_t pin;
  uint8_t pwm_num;

} ifx_pwm_port_pin_t;

typedef struct
{
  uint8_t vplus_port;
  uint8_t vplus_pin;
} ifx_adc_vplus_t;

const cy_stc_gpio_pin_config_t ioss_adc_pin_config = 
{
  .outVal = 1,
  .driveMode = CY_GPIO_DM_ANALOG,
  .hsiom = HSIOM_SEL_GPIO,
  .intEdge = CY_GPIO_INTR_DISABLE,
  .vtrip = CY_GPIO_VTRIP_CMOS,
  .slewRate = CY_GPIO_SLEW_FAST,
};

#ifndef HSIOM_SEL_PWM
#define HSIOM_SEL_PWM (en_hsiom_sel_t)8
#endif

const cy_stc_gpio_pin_config_t ioss_pwm_pin_config = 
{
  .outVal = 1,
  .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
  .hsiom = HSIOM_SEL_PWM,
  .intEdge = CY_GPIO_INTR_DISABLE,
  .vtrip = CY_GPIO_VTRIP_CMOS,
  .slewRate = CY_GPIO_SLEW_FAST,
};

//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************
void yield( void );
extern void setup(void);
extern void loop(void);

//****************************************************************************
// @Arduino Core Includes
//****************************************************************************
#include "wiring_constants.h"
#include "binary.h"
#include "itoa.h"
#include "dtostrf.h"
#include "WCharacter.h"

//****************************************************************************
// @Infineon Core Includes
//****************************************************************************
//#include "reset.h"

#ifdef __cplusplus
} // extern "C"
#include "WMath.h"
#endif	// __cplusplus

#include "Print.h"
#include "Serial.h"
#include "usbpd.h"


//****************************************************************************
// @Board Variant Includes
//****************************************************************************
#include "Arduino_pin_define.h"

#endif  /*_ARDUINO_H_ */