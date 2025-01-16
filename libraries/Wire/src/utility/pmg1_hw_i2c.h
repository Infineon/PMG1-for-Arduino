#include "cy_scb_i2c.h"
#include "cy_gpio.h"
#include "config.h"

#if CY7113_BOARD
#define I2C_HW SCB0
#define I2C_IRQ scb_0_interrupt_IRQn

#define I2C_CLOCK_BLOCK PCLK_SCB0_CLOCK

#define I2C_SDA_PORT GPIO_PRT4
#define I2C_SDA_PIN 1U
#define I2C_SDA_HSIOM P4_1_SCB0_I2C_SDA
#define I2C_SCL_PORT GPIO_PRT4
#define I2C_SCL_PIN 0U
#define I2C_SCL_HSIOM P4_0_SCB0_I2C_SCL
#endif

#if PMGDUINO_BOARD
#define I2C_HW SCB1
#define I2C_IRQ scb_1_interrupt_IRQn

#define I2C_CLOCK_BLOCK PCLK_SCB1_CLOCK

#define I2C_SDA_PORT GPIO_PRT1
#define I2C_SDA_PIN 5U
#define I2C_SDA_HSIOM P1_5_SCB1_I2C_SDA
#define I2C_SCL_PORT GPIO_PRT1
#define I2C_SCL_PIN 6U
#define I2C_SCL_HSIOM P1_6_SCB1_I2C_SCL
#endif

#define I2C_MAX_BUFFER_SIZE 32

cy_stc_scb_i2c_config_t I2C_config =
{
    .i2cMode = CY_SCB_I2C_SLAVE,
    .useRxFifo = true,
    .useTxFifo = true,
    .slaveAddress = 16,
    .slaveAddressMask = 254,
    .acceptAddrInFifo = false,
    .ackGeneralAddr = false,
    .enableWakeFromSleep = false,
    .enableDigitalFilter = false,
    .lowPhaseDutyCycle = 0,
    .highPhaseDutyCycle = 0,
    .delayInFifoAddress = 0,
};

cy_stc_gpio_pin_config_t I2C_SDA_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = I2C_SDA_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

const cy_stc_gpio_pin_config_t I2C_SCL_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = I2C_SCL_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

cy_stc_sysint_t I2C_SCB_IRQ_cfg =
{
    .intrSrc = I2C_IRQ,
    .intrPriority = 0u
};

//****************************************************************************
// @Global Variables
//****************************************************************************
extern cy_stc_scb_i2c_context_t I2C_context;
extern bool timeoutFlag;
extern uint8_t i2cRxBufIdx;
extern uint8_t i2cTxBufIdx;

bool i2cInit(uint8_t address);
void i2cDeInit(void);
void setPeripheralClock(uint32_t rate);

cy_en_scb_i2c_status_t i2cSendStart(uint8_t address, uint32_t timeoutMs);
cy_en_scb_i2c_status_t i2cSendRestart(uint8_t address, uint32_t timeoutMs);
cy_en_scb_i2c_status_t i2cSendStop(uint32_t timeoutMs);
cy_en_scb_i2c_status_t i2cSendByte(uint8_t data, uint32_t timeoutMS);
cy_en_scb_i2c_status_t i2cSendRestBytes(uint32_t timeoutMS);
cy_en_scb_i2c_status_t i2cReadByte(bool respACK, uint32_t timeoutMS);
int i2cGetByte(void);
int i2cPeekByte(void);
void i2cResetTxBuffer(void);
void i2cResetRxBuffer(void);

size_t i2cFillData(uint8_t data);
int getI2CRxDataNum(void);
