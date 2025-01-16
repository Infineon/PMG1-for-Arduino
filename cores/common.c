#include "Arduino.h"

void (*pin_intr_handler[IFX_DUINO_MAX_NUM_GPIO])(void) = {NULL};

void pinMode(uint8_t pin, uint8_t mode)
{
    if (pin > IFX_DUINO_MAX_NUM_GPIO || pin == LED_BUILTIN)
        return;

    Cy_GPIO_SetHSIOM(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, HSIOM_SEL_GPIO);
    Cy_GPIO_SetVtrip(gpio_pin_mapping[pin].port, CY_GPIO_VTRIP_CMOS);
    Cy_GPIO_SetSlewRate(gpio_pin_mapping[pin].port, CY_GPIO_SLEW_FAST);
    Cy_GPIO_Write(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, 1);

    switch(mode)
    {
        case INPUT:
        case INPUT_PULLUP:
        case OUTPUT:
            Cy_GPIO_SetDrivemode(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, mode);
        default:
            break;
    }
}

uint8_t digitalRead(uint8_t pin)
{
    if (pin < IFX_DUINO_MAX_NUM_GPIO)
        return ((uint8_t) Cy_GPIO_Read(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin));
    else
        return 0;
}

void digitalWrite(uint8_t pin, uint8_t value)
{
    if (pin == LED_BUILTIN)
    {
        Cy_GPIO_Write(LED_BUILTIN_PORT, LED_BUILTIN_PIN_NUM, value);    
    }
#if PMGDUINO_BOARD
    else if (pin == LED1)
    {
        Cy_GPIO_Write(LED1_PORT, LED1_PIN_NUM, value);    
    }
#endif
    else if (pin < IFX_DUINO_MAX_NUM_GPIO)
    {
        Cy_GPIO_Write(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, value);
    }
    else
    {
        // do nothing
    }
}

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) 
{
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i) {
        digitalWrite(clockPin, HIGH);
        if (bitOrder == LSBFIRST)
            value |= digitalRead(dataPin) << i;
        else
            value |= digitalRead(dataPin) << (7 - i);
        digitalWrite(clockPin, LOW);
    }
    return value;
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
        uint8_t i;

        digitalWrite(clockPin, LOW);
        digitalWrite(dataPin, LOW);

        for (i = 0; i < 8; i++)  {
                if (bitOrder == LSBFIRST) {
                        digitalWrite(dataPin, val & 0x01);
                        val >>= 1;
                } else {
                        digitalWrite(dataPin, (val & 0x80) != 0);
                        val <<= 1;
                }

                digitalWrite(clockPin, HIGH);
                digitalWrite(clockPin, LOW);
        }
}

void attachInterrupt(uint8_t pin, void (*userFunc)(void), int mode)
{	
	Cy_GPIO_SetDrivemode(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, CY_GPIO_DM_HIGHZ);
	Cy_GPIO_SetHSIOM(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, HSIOM_SEL_GPIO);
	Cy_GPIO_ClearInterrupt(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin);
	
    pin_intr_handler[pin] = userFunc;

    if (mode == LOW)    //LOW
    {
       uint8_t value;
       value=Cy_GPIO_Read(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin);
       if (value == LOW)
       {
    	   userFunc();
       }
       else
       {
    	    Cy_GPIO_SetInterruptEdge(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, CY_GPIO_INTR_FALLING);
       }
    }
    else if (mode == CHANGE)
    {
    	Cy_GPIO_SetInterruptEdge(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, CY_GPIO_INTR_BOTH);
    }
    else if (mode == FALLING)
	{
    	Cy_GPIO_SetInterruptEdge(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, CY_GPIO_INTR_FALLING);
	}
    else if (mode == RISING)
	{
    	Cy_GPIO_SetInterruptEdge(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, CY_GPIO_INTR_RISING);
	}
    else
    {
    	Cy_GPIO_SetInterruptEdge(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, CY_GPIO_INTR_DISABLE);
        pin_intr_handler[pin] = NULL;
    }
}

void detachInterrupt(uint8_t pin)
{
    Cy_GPIO_ClearInterrupt(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin);
    Cy_GPIO_SetInterruptEdge(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, CY_GPIO_INTR_DISABLE);
    pin_intr_handler[pin] = NULL;
}

#if (0) // William
uint32_t gl_intr_state = 0;

void interrupts(void)
{
	Cy_SysLib_ExitCriticalSection(gl_intr_state);
}

void noInterrupts(void)
{
	gl_intr_state=Cy_SysLib_EnterCriticalSection();
}
#endif

void delay(unsigned int ms)
{
    Cy_SysLib_Delay(ms);
}

void delayMicroseconds(unsigned int us)
{
    Cy_SysLib_DelayUs(us);    
}

uint8_t pinToPwmNum(uint8_t pin)
{
    uint8_t i;
    uint8_t pwmNum = 0xFF;

    for (i=0; i<IFX_DUINO_MAX_NUM_PWM; i++)
    {
        if (pin == pwm_pin_mapping[i].pin)
        {
            pwmNum = pwm_pin_mapping[i].pwm_num;
            break;
        }
    }

    return pwmNum;
}

void tone(uint8_t pin, unsigned int frequency)
{
    cy_stc_tcpwm_pwm_config_t pwm_config;
    cy_en_tcpwm_status_t pwm_result;
    uint32_t new_period = 32768;
    uint32_t new_prescaler = 7;
    uint8_t i = 0;

    uint8_t pwmNum = pinToPwmNum(pin);

    // check if the pin has PWM capability
    if (pwmNum == 0xFF)
        return;

    // if the setting frequency excess the maximum value, then return
    if (frequency > PWM_MAX_FREQ)
        return;
    
    // configure pin
    Cy_GPIO_Pin_Init(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, &ioss_pwm_pin_config);

    // configure clocks
    Cy_SysClk_PeriphAssignDivider((PCLK_TCPWM_CLOCKS0 + pwmNum), CY_SYSCLK_DIV_8_BIT, 4U);

    memcpy((void *)&pwm_config, (const void *)&pwm_config_default, sizeof(cy_stc_tcpwm_pwm_config_t));
    
    // calculate the pre-scaler of the pwm
    for (i=0; i<=7; i++)
    {
        new_period = (48000000 >> (i))/frequency;
        if ((new_period >= 2) && (new_period <= 32768))
        {
            new_prescaler = (uint32_t)i;
            break;
        }
    }

    new_period = (48000000 >> (new_prescaler))/frequency;

    pwm_config.period0 = new_period;
    pwm_config.compare0 = (new_period >> 1);
    pwm_config.clockPrescaler = new_prescaler;

    // configure PWM
    pwm_result = Cy_TCPWM_PWM_Init(TCPWM, pwmNum, &pwm_config);

    if (pwm_result != CY_TCPWM_SUCCESS)
    {
        // set the pin to High-Z for safety
        pinMode(pin, INPUT);
        return;
    }

	/* Enable the TCPWM as PWM */
	Cy_TCPWM_PWM_Enable(TCPWM, pwmNum);

	/* Start the PWM */
	Cy_TCPWM_TriggerReloadOrIndex(TCPWM, (1UL << pwmNum));
}

void noTone(uint8_t pin)
{
    uint8_t pwmNum = pinToPwmNum(pin);

    // check if the pin has PWM capability
    if (pwmNum == 0xFF)
        return;

    Cy_TCPWM_PWM_Disable(TCPWM, pwmNum);

    Cy_GPIO_SetDrivemode(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, CY_GPIO_DM_HIGHZ);	
}

uint16_t analogRead(uint8_t pin)
{
    uint8_t adcPin = 0;
    uint16_t val;

    if (pin < A0 || pin > (A0 + IFX_DUINO_MAX_NUM_ADC))
        return 0;
    
    adcPin = pin - A0;
    
    // configure pin
    Cy_GPIO_Pin_Init(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, &ioss_adc_pin_config);

    // enable channel
    SAR0->CHAN_EN |= (1UL << adcPin); 
    
    Cy_SAR_StartConvert(SAR0, CY_SAR_START_CONVERT_SINGLE_SHOT);

	// wait for the conversion complete
	while (Cy_SAR_IsEndConversion(SAR0, CY_SAR_RETURN_STATUS) != CY_SAR_SUCCESS) {}

	// read converted value
	val = Cy_SAR_GetResult16(SAR0, adcPin);

    SAR0->CHAN_EN &= ~(1UL << adcPin); 

    // configure pin
	Cy_GPIO_SetDrivemode(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, CY_GPIO_DM_HIGHZ);					
    return val;
}
void analogReference(uint8_t type)
{
    if ((type != AR_DEFAULT) && (type != AR_INTERNAL) && (type != AR_VDD))
        return;
    
    SAR0->CTRL = ((SAR0->CTRL & 0xFFFFFF8F) | (type << 4)); 
}

void analogWrite(uint8_t pin, uint8_t value)
{
    cy_stc_tcpwm_pwm_config_t pwm_config;
	cy_en_tcpwm_status_t pwm_result;
	uint32_t new_period,new_duty;
	new_period = (48000000 / 128 / 490);
    new_duty=(value * 3); //(256-value)*8;

    uint8_t pwmNum = pinToPwmNum(pin);

    // check if the pin has PWM capability
    if (pwmNum == 0xFF)
        return;

    // configure pin
    Cy_GPIO_Pin_Init(gpio_pin_mapping[pin].port, gpio_pin_mapping[pin].pin, &ioss_pwm_pin_config);

    // configure clocks
    Cy_SysClk_PeriphAssignDivider((PCLK_TCPWM_CLOCKS0 + pwmNum), CY_SYSCLK_DIV_8_BIT, 4U);

    memcpy((void *)&pwm_config, (const void *)&pwm_config_default, sizeof(cy_stc_tcpwm_pwm_config_t));

    pwm_config.clockPrescaler = 7;
    pwm_config.period0 = new_period;
    pwm_config.compare0 = new_duty;

    /* Initialize PWM using the configuration structure generated using device configurator */
    pwm_result =Cy_TCPWM_PWM_Init(TCPWM, pwmNum, &pwm_config);

    if (pwm_result != CY_TCPWM_SUCCESS)
    {
        // set the pin to High-Z for safety
        pinMode(pin, INPUT);
        return;
    }

	Cy_TCPWM_PWM_Enable(TCPWM, pwmNum);
	Cy_TCPWM_TriggerReloadOrIndex(TCPWM, (1UL << pwmNum));
}

uint32_t millSecondCnt = 0;

uint32_t millis()
{
    return millSecondCnt;
}

uint32_t micros()
{
    return (millSecondCnt * 1000) + Cy_TCPWM_Counter_GetCounter(TCPWM, 6);
}
