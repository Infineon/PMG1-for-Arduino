#if CY7113_BOARD

/*
#ifndef APP_SWITCH_PORT
#define APP_SWITCH_PORT GPIO_PRT3
#endif

#ifndef APP_SWITCH_PIN
#define APP_SWITCH_PIN  3
#endif

#ifndef APP_SWITCH_NUM
#define APP_SWITCH_NUM  3
#endif
*/

#define D0    0
#define D1    1
#define D2    2
#define D3    3
#define D4    4
#define D5    5
#define D6    6
#define D7    7
#define D8    8
#define D9    9
#define D10   10
#define D11   11
#define D12   12
#define D13   13
#define D14   14
#define D15   15
#define D16   16
#define D17   17
#define D18   18
#define D19   19
#define D20   20
#define D21   21
#define D22   22

#define A0    5
#define A1    6

#ifndef IFX_DUINO_MAX_NUM_GPIO  
#define IFX_DUINO_MAX_NUM_GPIO 23
#endif

#ifndef IFX_DUINO_MAX_NUM_PWM
#define IFX_DUINO_MAX_NUM_PWM 6
#endif

#ifndef IFX_DUINO_MAX_NUM_ADC
#define IFX_DUINO_MAX_NUM_ADC 2
#endif

#ifndef LED_BUILTIN_PORT
#define LED_BUILTIN_PORT  GPIO_PRT5
#endif

#ifndef LED_BUILTIN_PIN_NUM
#define LED_BUILTIN_PIN_NUM   5
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 23
#endif

#ifndef HSIOM_SEL_PWM
#define HSIOM_SEL_PWM 8
#endif

#ifndef PWM_MAX_FREQ
#define PWM_MAX_FREQ  (8000000u)
#endif

#ifndef UART_HW
#define UART_HW SCB4
#endif

#ifndef UART_IRQ
#define UART_IRQ ((IRQn_Type)scb_4_interrupt_IRQn)
#endif

#ifndef UART_RX_PORT
#define UART_RX_PORT GPIO_PRT3
#endif

#ifndef UART_RX_PORT_NUM
#define UART_RX_PORT_NUM 3U
#endif

#ifndef UART_RX_PIN
#define UART_RX_PIN 5U
#endif

#ifndef UART_RX_PIN_NUM
#define UART_RX_PIN_NUM 5U
#endif

#ifndef UART_RX_DRIVEMODE
#define UART_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#endif

#ifndef UART_RX_INIT_DRIVESTATE
#define UART_RX_INIT_DRIVESTATE 1
#endif

#ifndef ioss_0_port_3_pin_5_HSIOM
    #define ioss_0_port_3_pin_5_HSIOM P3_5_SCB4_UART_RX
#endif

#define UART_RX_HSIOM ioss_0_port_3_pin_5_HSIOM   

#if defined (CY_USING_HAL)
    #define UART_RX_HAL_PORT_PIN P3_5
    #define UART_RX P3_5
    #define UART_RX_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define UART_RX_HAL_DIR CYHAL_GPIO_DIR_INPUT 
    #define UART_RX_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif //defined (CY_USING_HAL)

#ifndef UART_TX_PORT
#define UART_TX_PORT GPIO_PRT3
#endif

#ifndef UART_TX_PORT_NUM
#define UART_TX_PORT_NUM 3U
#endif

#ifndef UART_TX_PIN
#define UART_TX_PIN 6U
#endif

#ifndef UART_TX_PIN_NUM
#define UART_TX_PIN_NUM 6U
#endif

#ifndef UART_TX_DRIVEMODE
#define UART_TX_DRIVEMODE CY_GPIO_DM_STRONG
#endif

#ifndef UART_TX_INIT_DRIVESTATE
#define UART_TX_INIT_DRIVESTATE 1
#endif

#ifndef ioss_0_port_3_pin_6_HSIOM
    #define ioss_0_port_3_pin_6_HSIOM P3_6_SCB4_UART_TX
#endif

#define UART_TX_HSIOM ioss_0_port_3_pin_6_HSIOM

#if defined (CY_USING_HAL)
    #define UART_TX_HAL_PORT_PIN P3_6
    #define UART_TX P3_6
    #define UART_TX_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define UART_TX_HAL_DIR CYHAL_GPIO_DIR_INPUT 
    #define UART_TX_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif //defined (CY_USING_HAL)

#ifndef PCLK_UART_CLOCK
#define PCLK_UART_CLOCK PCLK_SCB4_CLOCK
#endif

extern void (*pin_intr_handler[IFX_DUINO_MAX_NUM_GPIO])(void);

const ifx_gpio_port_pin_t gpio_pin_mapping[IFX_DUINO_MAX_NUM_GPIO] =
{
    {GPIO_PRT3, 5},     // D0
    {GPIO_PRT3, 6},     // D1
    {GPIO_PRT3, 0},     // D2
    {GPIO_PRT2, 3},     // D3
    {GPIO_PRT2, 4},     // D4
    {GPIO_PRT2, 2},     // D5, A0
    {GPIO_PRT2, 1},     // D6, A1
    {GPIO_PRT5, 2},     // D7
    {GPIO_PRT5, 3},     // D8
    {GPIO_PRT5, 0},     // D9
    {GPIO_PRT1, 1},     // D10
    {GPIO_PRT1, 2},     // D11
    {GPIO_PRT6, 0},     // D12
    {GPIO_PRT6, 1},     // D13
    {GPIO_PRT0, 0},     // D14
    {GPIO_PRT6, 2},     // D15
    {GPIO_PRT6, 3},     // D16
    {GPIO_PRT1, 3},     // D17
    {GPIO_PRT1, 4},     // D18
    {GPIO_PRT4, 0},     // D19
    {GPIO_PRT4, 1},     // D20
    {GPIO_PRT1, 6},     // D21
    {GPIO_PRT1, 5}      // D22
};

const ifx_pwm_port_pin_t pwm_pin_mapping[IFX_DUINO_MAX_NUM_PWM] = 
{
  { 2, 6},   // D2 , PWM6
  { 4, 3},   // D4 , PWM3
  { 6, 2},   // D6 , PWM2
  {15, 0},   // D15, PWM0
  {17, 4},   // D17, PWM4
  {18, 5}    // D18, PWM5
};

#ifndef pwm_INPUT_DISABLED
#define pwm_INPUT_DISABLED (0x07u)
#endif

const cy_stc_tcpwm_pwm_config_t pwm_config_default = 
{
  .pwmMode = CY_TCPWM_PWM_MODE_PWM,
  .clockPrescaler = CY_TCPWM_PWM_PRESCALER_DIVBY_1,
  .pwmAlignment = CY_TCPWM_PWM_LEFT_ALIGN,
  .deadTimeClocks = 0,
  .runMode = CY_TCPWM_PWM_CONTINUOUS,
  .period0 = 32768,
  .period1 = 32768,
  .enablePeriodSwap = false,
  .compare0 = 16384,
  .compare1 = 16384,
  .enableCompareSwap = false,
  .interruptSources = CY_TCPWM_INT_NONE,
  .invertPWMOut = CY_TCPWM_PWM_INVERT_DISABLE,
  .invertPWMOutN = CY_TCPWM_PWM_INVERT_DISABLE,
  .killMode = CY_TCPWM_PWM_STOP_ON_KILL,
  .swapInputMode = pwm_INPUT_DISABLED & 0x3U,
  .swapInput = CY_TCPWM_INPUT_0,
  .reloadInputMode = pwm_INPUT_DISABLED & 0x3U,
  .reloadInput = CY_TCPWM_INPUT_0,
  .startInputMode = pwm_INPUT_DISABLED & 0x3U,
  .startInput = CY_TCPWM_INPUT_0,
  .killInputMode = pwm_INPUT_DISABLED & 0x3U,
  .killInput = CY_TCPWM_INPUT_0,
  .countInputMode = pwm_INPUT_DISABLED & 0x3U,
  .countInput = CY_TCPWM_INPUT_1,
};

const uint32_t adc_routing_mask[7] = {1, 4, 8, 16, 32, 64, 128};

const ifx_adc_vplus_t adc_VPLUS_mapping[IFX_DUINO_MAX_NUM_ADC] = 
{
//{VPLUS_PORT, VPLUS_PIN}    
  {0, 2},
  {0, 1}
};

const cy_stc_sysint_t uartIntrConfig =
{
    .intrSrc      = UART_IRQ,
    .intrPriority = 3,
};

const cy_stc_scb_uart_config_t serial_config_default = 
{
  .uartMode = CY_SCB_UART_STANDARD,
  .oversample = 16,  
  .dataWidth = 8UL,
  .enableMsbFirst = false,
  .stopBits = CY_SCB_UART_STOP_BITS_1,
  .parity = CY_SCB_UART_PARITY_NONE,
  .enableInputFilter = false,
  .dropOnParityError = false,
  .dropOnFrameError = false,
  .enableMutliProcessorMode = false,
  .receiverAddress = 0x0UL,
  .receiverAddressMask = 0x0UL,
  .acceptAddrInFifo = false,
  .irdaInvertRx = false,
  .irdaEnableLowPowerReceiver = false,
  .smartCardRetryOnNack = false,
  .enableLinMode = false,
  .enableCts = false,
  .ctsPolarity = CY_SCB_UART_ACTIVE_LOW,
  .rtsRxFifoLevel = 0UL,
  .rtsPolarity = CY_SCB_UART_ACTIVE_LOW,
  .breakWidth = 11UL,
  .breakLevel = false,
  .rxFifoTriggerLevel = 7UL,
  .rxFifoIntEnableMask = 12UL,
  .txFifoTriggerLevel = 7UL,
  .txFifoIntEnableMask = 2UL
};

#define SPI_SYSTEM_RATE         48000000
#define SPI_OVERSAMPLE_COUNT    16

#define SPI_CLOCK_DIV2      2
#define SPI_CLOCK_DIV4      4
#define SPI_CLOCK_DIV8      8
#define SPI_CLOCK_DIV16     16
#define SPI_CLOCK_DIV32     32
#define SPI_CLOCK_DIV64     64
#define SPI_CLOCK_DIV128    128

#define SPI_BUFFER_MAX_SIZE 32

#define SPI_HW SCB3
#define SPI_IRQ scb_3_interrupt_IRQn

#define SPI_CLOCK_BLOCK PCLK_SCB3_CLOCK

#define SPI_CK_PORT GPIO_PRT1
#define SPI_CK_PORT_NUM 1U
#define SPI_CK_PIN 1U
#define SPI_CK_NUM 1U
#define SPI_CK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPI_CK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_1_HSIOM
    #define ioss_0_port_1_pin_1_HSIOM P1_1_SCB3_SPI_CLK //HSIOM_SEL_GPIO
#endif
#define SPI_CK_HSIOM ioss_0_port_1_pin_1_HSIOM
#define SPI_CK_IRQ ioss_interrupt_gpio_IRQn

#define SPI_COPI_PORT GPIO_PRT1
#define SPI_COPI_PORT_NUM 1U
#define SPI_COPI_PIN 3U
#define SPI_COPI_NUM 3U
#define SPI_COPI_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPI_COPI_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_3_HSIOM
    #define ioss_0_port_1_pin_3_HSIOM P1_3_SCB3_SPI_MOSI //HSIOM_SEL_GPIO
#endif
#define SPI_COPI_HSIOM ioss_0_port_1_pin_3_HSIOM
#define SPI_COPI_IRQ ioss_interrupt_gpio_IRQn

#define SPI_CIPO_PORT GPIO_PRT1
#define SPI_CIPO_PORT_NUM 1U
#define SPI_CIPO_PIN 2U
#define SPI_CIPO_NUM 2U
#define SPI_CIPO_DRIVEMODE CY_GPIO_DM_HIGHZ
#define SPI_CIPO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_2_HSIOM
    #define ioss_0_port_1_pin_2_HSIOM P1_2_SCB3_SPI_MISO //HSIOM_SEL_GPIO
#endif
#define SPI_CIPO_HSIOM ioss_0_port_1_pin_2_HSIOM
#define SPI_CIPO_IRQ ioss_interrupt_gpio_IRQn

#define SPI_CS_PORT GPIO_PRT1
#define SPI_CS_PORT_NUM 1U
#define SPI_CS_PIN 4U
#define SPI_CS_NUM 4U
#define SPI_CS_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPI_CS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_4_HSIOM
    #define ioss_0_port_1_pin_4_HSIOM P1_4_SCB3_SPI_SELECT0//HSIOM_SEL_GPIO
#endif
#define SPI_CS_HSIOM ioss_0_port_1_pin_4_HSIOM
#define SPI_CS_IRQ ioss_interrupt_gpio_IRQn

const cy_stc_gpio_pin_config_t SPI_CK_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = SPI_CK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

const cy_stc_gpio_pin_config_t SPI_COPI_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = SPI_COPI_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

const cy_stc_gpio_pin_config_t SPI_CIPO_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = SPI_CIPO_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

const cy_stc_gpio_pin_config_t SPI_CS_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = SPI_CS_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

const cy_stc_scb_spi_config_t default_SPI_config = 
{
    .spiMode = CY_SCB_SPI_MASTER,
    .subMode = CY_SCB_SPI_MOTOROLA,
    .sclkMode = CY_SCB_SPI_CPHA0_CPOL0,
    .parity = CY_SCB_SPI_PARITY_NONE,
    .dropOnParityError = false,
    .oversample = SPI_OVERSAMPLE_COUNT,
    .rxDataWidth = 8UL,
    .txDataWidth = 8UL,
    .enableMsbFirst = true,
    .enableFreeRunSclk = false,
    .enableInputFilter = true,
    
    .enableMisoLateSample = true,
    .enableTransferSeperation = false,
    .ssPolarity = ((CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT0) | \
                   (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT1) | \
                   (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT2) | \
                   (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT3)),
    .ssSetupDelay = CY_SCB_SPI_SS_SETUP_DELAY_0_75_CYCLES,
    .ssHoldDelay = CY_SCB_SPI_SS_HOLD_DELAY_0_75_CYCLES,
    .ssInterDataframeDelay = CY_SCB_SPI_SS_INTERFRAME_DELAY_1_5_CYCLES,
    .enableWakeFromSleep = false,
    .mosiIdleHigh = true,
    .rxFifoTriggerLevel = 7UL,
    .rxFifoIntEnableMask = 0UL,
    .txFifoTriggerLevel = 0UL,
    .txFifoIntEnableMask = 0UL,
    .masterSlaveIntEnableMask = 0UL,
};

const cy_stc_sysint_t SPI_SCB_IRQ_cfg =
{
    .intrSrc      = SPI_IRQ,
    .intrPriority = 3
};

const cy_stc_gpio_pin_config_t DEFAULT_GPIO_HZ_CONFIG = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = HSIOM_SEL_GPIO,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};
#endif

