#include <cy_gpio.h>
#include <cy_scb_spi.h>
#include <cy_sysint.h>
#include <cy_sysclk.h>
#include <Arduino.h>

cy_stc_scb_spi_context_t spiContext;

void spiDeInitPin()
{
    Cy_GPIO_Pin_Init(SPI_CK_PORT, SPI_CK_PIN, &DEFAULT_GPIO_HZ_CONFIG);
    Cy_GPIO_Pin_Init(SPI_COPI_PORT, SPI_COPI_PIN, &DEFAULT_GPIO_HZ_CONFIG);
    Cy_GPIO_Pin_Init(SPI_CIPO_PORT, SPI_CIPO_PIN, &DEFAULT_GPIO_HZ_CONFIG);
    Cy_GPIO_Pin_Init(SPI_CS_PORT, SPI_CS_PIN, &DEFAULT_GPIO_HZ_CONFIG);   
}

void SPI_Isr(void)
{
    Cy_SCB_SPI_Interrupt(SPI_HW, &spiContext);
}

void spiPinInit(void)
{
    Cy_GPIO_Pin_Init(SPI_CK_PORT, SPI_CK_PIN, &SPI_CK_config);
    Cy_GPIO_Pin_Init(SPI_COPI_PORT, SPI_COPI_PIN, &SPI_COPI_config);
    Cy_GPIO_Pin_Init(SPI_CIPO_PORT, SPI_CIPO_PIN, &SPI_CIPO_config);
    Cy_GPIO_Pin_Init(SPI_CS_PORT, SPI_CS_PIN, &SPI_CS_config);    
}

bool spiInit(uint8_t div, bool msbFirst, uint8_t mode)
{
    cy_stc_scb_spi_config_t SPI_config;
    // maximum rate is 48MHx / 2 (DIV) / 16 (Oversample) = 1.5MHz
    // Minimum rate is 48MHx / 128 (DIV) / 16 (Oversample) = 23.4375kHz

    if ((div > 128) || (div < 2))
        return false;

    if (mode > 3)
        return false;

    spiPinInit();

    // SPI clock source configuration
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 6U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 6U, div-1);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 6U);
    Cy_SysClk_PeriphAssignDivider(SPI_CLOCK_BLOCK, CY_SYSCLK_DIV_8_BIT, 6U);

    memcpy((void *)&SPI_config, (const void *)&default_SPI_config, sizeof(cy_stc_scb_spi_config_t));
    // configure mode and bit-order
    SPI_config.sclkMode = (cy_en_scb_spi_sclk_mode_t)mode;
    SPI_config.enableMsbFirst = (msbFirst) ? true : false;

    /* Configure SPI to operate */
    if (CY_SCB_SPI_SUCCESS != Cy_SCB_SPI_Init(SPI_HW, (cy_stc_scb_spi_config_t const *)&SPI_config, &spiContext))
    {
        spiDeInitPin();
        return false;
    }

    /* Hook interrupt service routine and enable interrupt */
    if (Cy_SysInt_Init(&SPI_SCB_IRQ_cfg, &SPI_Isr) != CY_SYSINT_SUCCESS)
    {
        spiDeInit();
        return false;
    }

    NVIC_EnableIRQ(SPI_SCB_IRQ_cfg.intrSrc);

    /* Enable SPI to operate */
    Cy_SCB_SPI_Enable(SPI_HW);

    return true;
}

void spiDeInit(void)
{
    Cy_SCB_SPI_Disable(SPI_HW, &spiContext);
    Cy_SCB_SPI_DeInit(SPI_HW);
    spiDeInitPin();     
}

void spiEnable(void)
{
   Cy_SCB_SPI_Enable(SPI_HW); 
}

void spiDisable(void)
{
    Cy_SCB_SPI_Disable(SPI_HW, &spiContext);
}

void spiSetPeripheralClock(uint8_t div)
{
    if (div != SPI_CLOCK_DIV2  || 
        div != SPI_CLOCK_DIV4  ||
        div != SPI_CLOCK_DIV8  ||
        div != SPI_CLOCK_DIV16 ||
        div != SPI_CLOCK_DIV64 ||
        div != SPI_CLOCK_DIV128
       )
       return;

    Cy_SysClk_PeriphAssignDivider(SPI_CLOCK_BLOCK, CY_SYSCLK_DIV_8_BIT, 6U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 6U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 6U, div-1);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 6U);    
}

void spiSendData(uint8_t *txData, uint8_t *rxData, uint8_t size)
{
    (void)Cy_SCB_SPI_Transfer(SPI_HW, txData, rxData, size, &spiContext);
    /* Blocking wait for transfer completion */
    while (0UL != (CY_SCB_SPI_TRANSFER_ACTIVE & Cy_SCB_SPI_GetTransferStatus(SPI_HW, &spiContext)))
    {
    }
}