/******************************************************************************
* File Name:   debug.c
*
* Description: This file implements uart based debugging functionality.
*
* Related Document: See README.md
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/

#include "Arduino.h"

#define RINGBUFFER_RX_INDEX 0
#define RINGBUFFER_TX_INDEX 1

static cy_stc_scb_uart_context_t duinoUartContext;

static ringbuffer_t uartRxRingBuff;
static ringbuffer_t uartTxRingBuff;

uint8_t rxData = 0;
uint8_t txData[16] = {0};

void dbgPrintStr(char *str)
{
    if (Serial.getUartInitiated())
        Serial.print(str);
}

void dbgPrintVal(unsigned int val)
{
    if (Serial.getUartInitiated())
        Serial.print(val);
}

void initRingBuffer(ringbuffer_t *buff)
{
    memset((void *)buff->buffer, 0, sizeof(uint8_t) * UART_BUFFER_SIZE);
    buff->head = 0;
    buff->tail = 0;
}

uint8_t getRingBufferSpace(ringbuffer_t *buff)
{
    uint8_t size;

    if (buff->head >= buff->tail)
        size = UART_BUFFER_SIZE - 1 - buff->head + buff->tail;
    else
        size = buff->tail - buff->head - 1;

    return size;
}

uint8_t getRingBufferOccupied(ringbuffer_t *buff)
{
    uint8_t size;

    if (buff->head >= buff->tail)
        size = buff->head - buff->tail; 
    else
        size = UART_BUFFER_SIZE - buff->tail + buff->head;

    return size;
}

uint8_t peekRingBuffer(ringbuffer_t *buff)
{
    uint8_t data;

    if (buff->head == buff->tail)
        data = 0xFF;
    else 
        data = buff->buffer[buff->tail];

    return data;
}

void queueRingBuffer(ringbuffer_t *buff, uint8_t data)
{
    uint8_t pos;

    pos = buff->head + 1;
    if (pos >= UART_BUFFER_SIZE)
        pos = 0;
    
    if (pos != buff->tail)
    {
        buff->buffer[buff->head] = data;
        buff->head = pos;
    }
}

uint8_t dequeueRingBuffer(ringbuffer_t *buff)
{
    uint8_t data;

    if (buff->head == buff->tail)
        data = 0xFF;
    
    data = buff->buffer[buff->tail];
    buff->tail ++;
    if (buff->tail >= UART_BUFFER_SIZE)
        buff->tail = 0;

    return data;
}

uint8_t queueArrayRingBuffer(ringbuffer_t *buff, uint8_t *data, uint8_t size)
{
    uint8_t space;
    
    space = getRingBufferSpace(buff);
    if (space >= size) 
        space = size;

    for (uint8_t cnt=0; cnt<space; cnt++)
    {
        queueRingBuffer(buff, data[cnt]);
    }    

    return space;
}

bool isRingBufferEmpty(ringbuffer_t *buff)
{
    bool empty;

    empty = ((buff->head == buff->tail) ? true : false);

    return empty;
}

void checkUartTxFifoWrite(void)
{
    uint8_t fifoSizeAvailable = Cy_SCB_GetFifoSize(UART_HW) - Cy_SCB_GetNumInTxFifo(UART_HW);
    uint8_t queueAvailable = getRingBufferOccupied(&uartTxRingBuff);
    uint8_t numCopySize = (fifoSizeAvailable >= queueAvailable) ? queueAvailable : fifoSizeAvailable;
    uint8_t i;

    if ((numCopySize > 0) && Cy_SCB_UART_IsTxComplete(UART_HW))
    {
        memset((void *)txData, 0, 16);
        for (i=0; i<numCopySize; i++)
        {
            txData[i] = dequeueRingBuffer(&uartTxRingBuff);
        }
        Cy_SCB_UART_Transmit(UART_HW, txData, numCopySize, &duinoUartContext);
    }
}

void checkUartRxFifoRead(void)
{
}

void UART_Isr(void)
{
    Cy_SCB_UART_Interrupt(UART_HW, &duinoUartContext);
}

void handleUartEvent(uint32_t event)
{
    if (event & CY_SCB_UART_RECEIVE_DONE_EVENT)
    {
        queueRingBuffer(&uartRxRingBuff, rxData);
        Cy_SCB_UART_Receive(UART_HW, &rxData, 1, &duinoUartContext);
    }
    
    if (event & CY_SCB_UART_TRANSMIT_DONE_EVENT)
    {
        checkUartTxFifoWrite(); 
    }   
}

bool uart_init(uint8_t cfg, uint32_t bitrate)
{
    cy_stc_scb_uart_config_t uart_cfg;
    uint8_t i;
    uint32_t divNum = 0;

    /* Configure and enable the UART peripheral. */
    memcpy((void *)&uart_cfg, (const void *)&serial_config_default, sizeof(cy_stc_scb_uart_config_t));

    Cy_GPIO_SetHSIOM(UART_RX_PORT, UART_RX_PIN_NUM, UART_RX_HSIOM);
    Cy_GPIO_SetHSIOM(UART_TX_PORT, UART_TX_PIN_NUM, UART_TX_HSIOM);

    Cy_GPIO_SetDrivemode(UART_RX_PORT, UART_RX_PIN_NUM, CY_GPIO_DM_HIGHZ);
    Cy_GPIO_SetDrivemode(UART_TX_PORT, UART_TX_PIN_NUM, CY_GPIO_DM_STRONG);

    Cy_SysClk_PeriphAssignDivider(PCLK_UART_CLOCK, CY_SYSCLK_DIV_16_BIT, CLK_UART_NUM);

    for (i=0; i<UART_BIT_RATE_MAX_NUM; i++)
    {
        if(brMapping[i].bitRate == bitrate)
        {
            divNum = brMapping[i].divNum;
            break;
        }
    }

    if (divNum == 0)
    {
        Cy_GPIO_SetDrivemode(UART_RX_PORT, UART_RX_PIN_NUM, CY_GPIO_DM_HIGHZ);
        Cy_GPIO_SetDrivemode(UART_TX_PORT, UART_TX_PIN_NUM, CY_GPIO_DM_HIGHZ);
        return false;
    }

    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, CLK_UART_NUM, divNum);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, CLK_UART_NUM);

    uart_cfg.dataWidth = uartCfgs[cfg].dataBit;
    uart_cfg.parity = uartCfgs[cfg].parity;
    uart_cfg.stopBits = uartCfgs[cfg].stopBit;

    Cy_SCB_UART_Init(UART_HW, &uart_cfg, &duinoUartContext);

    Cy_SCB_SetTxInterruptMask(UART_HW, CY_SCB_UART_TX_DONE);
    Cy_SCB_SetRxInterruptMask(UART_HW, CY_SCB_RX_INTR_NOT_EMPTY);
    
    /* Hook interrupt service routine and enable interrupt */
    if (Cy_SysInt_Init(&uartIntrConfig, &UART_Isr) != CY_SYSINT_SUCCESS)
    {
        Cy_SCB_UART_DeInit(UART_HW);
        Cy_GPIO_SetDrivemode(UART_RX_PORT, UART_RX_PIN_NUM, CY_GPIO_DM_HIGHZ);
        Cy_GPIO_SetDrivemode(UART_TX_PORT, UART_TX_PIN_NUM, CY_GPIO_DM_HIGHZ);
        Cy_SCB_ClearTxInterrupt(UART_HW, CY_SCB_UART_TX_DONE);
        Cy_SCB_ClearRxInterrupt(UART_HW, CY_SCB_RX_INTR_NOT_EMPTY);

        return false;
    }

    NVIC_EnableIRQ(UART_IRQ);
    // setup the callback function point in the context
    Cy_SCB_UART_RegisterCallback(UART_HW, handleUartEvent, &duinoUartContext);

	Cy_SCB_UART_Enable(UART_HW);

    // start to receive
    Cy_SCB_UART_Receive(UART_HW, &rxData, 1, &duinoUartContext);

    return true;
}

HardwareSerial::HardwareSerial()
{
    initRingBuffer(&uartRxRingBuff);
    initRingBuffer(&uartTxRingBuff);
    uartInitiated = false;
}

bool HardwareSerial::getUartInitiated(void)
{
    return uartInitiated;
}

void HardwareSerial::begin(uint32_t speed)
{
    if (uart_init((uint8_t)SERIAL_8N1, speed))
        uartInitiated = true;
}

void HardwareSerial::begin(uint32_t speed, uart_cfg_idx_t config)
{
    if (uart_init((uint8_t)config, speed))
        uartInitiated = true;
}

void HardwareSerial::end(void)
{
    Cy_GPIO_SetHSIOM(UART_RX_PORT, UART_RX_PIN_NUM, HSIOM_SEL_GPIO);
    Cy_GPIO_SetHSIOM(UART_TX_PORT, UART_TX_PIN_NUM, HSIOM_SEL_GPIO);
    Cy_GPIO_SetDrivemode(UART_RX_PORT, UART_RX_PIN_NUM, CY_GPIO_DM_HIGHZ);
    Cy_GPIO_SetDrivemode(UART_TX_PORT, UART_TX_PIN_NUM, CY_GPIO_DM_HIGHZ);
    Cy_SCB_UART_Disable(UART_HW, &duinoUartContext);
    Cy_SCB_UART_DeInit(UART_HW);
    uartInitiated = false;
}

int HardwareSerial::available(void)
{
    if (uartInitiated)
        return (int)getRingBufferOccupied(&uartRxRingBuff);
    else
        return 0;
}

int HardwareSerial::availableForWrite(void)
{
    if (uartInitiated)
        return (int)getRingBufferSpace(&uartTxRingBuff);
    else
        return 0;
}

int HardwareSerial::peek(void)
{
    if (uartInitiated)
        return (int)peekRingBuffer(&uartRxRingBuff);
    else
        return 0;
}

int HardwareSerial::read(void)
{
    if (uartInitiated)
        return (int)dequeueRingBuffer(&uartRxRingBuff);
    else
        return 0;
}

void HardwareSerial::flush(void)
{
    if (uartInitiated)
    {
        while (uartTxRingBuff.head == uartTxRingBuff.tail);
        while (!(CY_SCB_UART_TRANSMIT_ACTIVE & duinoUartContext.txStatus));
    }
}

size_t HardwareSerial::write(uint8_t val)
{
    if (uartInitiated)
    {
        while (getRingBufferSpace(&uartTxRingBuff) < 1) {}

        queueRingBuffer(&uartTxRingBuff, val);
        checkUartTxFifoWrite();
        return (size_t) 1;
    }
    else
        return 0;
}

HardwareSerial Serial = HardwareSerial();
