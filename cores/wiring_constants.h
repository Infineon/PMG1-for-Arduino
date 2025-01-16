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

#ifndef _WIRING_CONSTANTS_
#define _WIRING_CONSTANTS_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//****************************************************************************
// @Defines
//****************************************************************************
    /* Arduino wiring macros and bit defines  */
#define HIGH                1
#define LOW                 0
#define ENABLED             1
#define DISABLED            0

#define INPUT               CY_GPIO_DM_HIGHZ    // 1
#define OUTPUT              CY_GPIO_DM_STRONG   // 6
#define INPUT_PULLUP        CY_GPIO_DM_PULLUP   // 2
#define INPUT_PULLDOWN      CY_GPIO_DM_PULLUP_DOWN
#define OUTPUT_OPENDRAIN    CY_GPIO_DM_OD_DRIVESHIGH

#define RISING              CY_GPIO_INTR_RISING     // 1
#define FALLING             CY_GPIO_INTR_FALLING    // 2
#define CHANGE              CY_GPIO_INTR_BOTH       // 3
#ifndef CY_GPIO_INTR_LOW
#define CY_GPIO_INTR_LOW    4
#endif

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

enum BitOrder {
    LSBFIRST = 0,
    MSBFIRST = 1
};


#define DEFAULT 0
#define EXTERNAL 1
#define INTERNAL 2

#define NOT_A_PIN   -1
#define NOT_A_PORT  -1
#define NOT_AN_INTERRUPT -1

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif // abs

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif // min

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif // max

#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define map( x, in_min, in_max, out_min, out_max)   ( ( (x - in_min) * (out_max - out_min) ) / (in_max - in_min) + out_min)

#define interrupts() __enable_irq()
#define noInterrupts() __disable_irq()

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bit(b) (1UL << (b))

#define AR_DEFAULT      CY_SAR_VREF_SEL_BGR
#define AR_INTERNAL     CY_SAR_VREF_SEL_BGR
#define AR_VDD          CY_SAR_VREF_SEL_VDDA_DIV_2
//****************************************************************************
// @Typedefs
//****************************************************************************
typedef bool boolean ;
typedef uint8_t byte;
typedef uint8_t word;

void        pinMode(uint8_t pin, uint8_t mode);
void        digitalWrite(uint8_t pin, uint8_t value);
uint8_t     digitalRead(uint8_t pin);

uint8_t     shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
void        shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);

void        attachInterrupt(uint8_t pin, void (*userFunc)(void), int mode);
void        detachInterrupt(uint8_t pin);

#if (0) // William
void        interrupts(void);
void        noInterrupts(void);
#endif

void        delay(unsigned int ms);
void        delayMicroseconds(unsigned int us);

void        tone(uint8_t pin, unsigned int grequency);
void        noTone(uint8_t pin);

uint16_t    analogRead(uint8_t pin);
void        analogReference(uint8_t type);
void        analogWrite(uint8_t pin, uint8_t value);

uint32_t    micros();
uint32_t    millis();

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* _WIRING_CONSTANTS_ */
