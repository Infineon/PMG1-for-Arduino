/******************************************************************************
* File Name: Serial.h
*
* Description: This file defines UART based debug APIs.
*
* Related Document: See README.md
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/
#ifndef __Serial_h__
#define __Serial_h__

//****************************************************************************
// @External Prototypes
//****************************************************************************


#include "cy_pdl.h"
#include "config.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern void dbgPrintStr(char *str);
extern void dbgPrintVal(unsigned int val);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#include "Stream.h"

#ifndef UART_BIT_RATE_MAX_NUM
#define UART_BIT_RATE_MAX_NUM 13
#endif

#ifndef UART_BUFFER_SIZE
#define UART_BUFFER_SIZE    32
#endif

typedef enum
{
    SERIAL_5N1 = 0,
    SERIAL_6N1,
    SERIAL_7N1,
    SERIAL_8N1,
    SERIAL_5N2,
    SERIAL_6N2,
    SERIAL_7N2,
    SERIAL_8N2,
    SERIAL_5E1,
    SERIAL_6E1,
    SERIAL_7E1,
    SERIAL_8E1,
    SERIAL_5E2,
    SERIAL_6E2,
    SERIAL_7E2,
    SERIAL_8E2,
    SERIAL_5O1,
    SERIAL_6O1,
    SERIAL_7O1,
    SERIAL_8O1,
    SERIAL_5O2,
    SERIAL_6O2,
    SERIAL_7O2,
    SERIAL_8O2,    
} uart_cfg_idx_t;

typedef struct duinoUartCfg
{
    uint32_t dataBit;
    cy_en_scb_uart_parity_t parity;
    cy_en_scb_uart_stop_bits_t stopBit;
} duinoUartCfg_t;

const duinoUartCfg_t uartCfgs[] =
{
    {5, CY_SCB_UART_PARITY_NONE, CY_SCB_UART_STOP_BITS_1}, // SERIAL_5N1
    {6, CY_SCB_UART_PARITY_NONE, CY_SCB_UART_STOP_BITS_1}, // SERIAL_6N1
    {7, CY_SCB_UART_PARITY_NONE, CY_SCB_UART_STOP_BITS_1}, // SERIAL_7N1
    {8, CY_SCB_UART_PARITY_NONE, CY_SCB_UART_STOP_BITS_1}, // SERIAL_8N1
    {5, CY_SCB_UART_PARITY_NONE, CY_SCB_UART_STOP_BITS_2}, // SERIAL_5N2
    {6, CY_SCB_UART_PARITY_NONE, CY_SCB_UART_STOP_BITS_2}, // SERIAL_6N2
    {7, CY_SCB_UART_PARITY_NONE, CY_SCB_UART_STOP_BITS_2}, // SERIAL_7N2
    {8, CY_SCB_UART_PARITY_NONE, CY_SCB_UART_STOP_BITS_2}, // SERIAL_8N2

    {5, CY_SCB_UART_PARITY_EVEN, CY_SCB_UART_STOP_BITS_1}, // SERIAL_5E1
    {6, CY_SCB_UART_PARITY_EVEN, CY_SCB_UART_STOP_BITS_1}, // SERIAL_6E1
    {7, CY_SCB_UART_PARITY_EVEN, CY_SCB_UART_STOP_BITS_1}, // SERIAL_7E1
    {8, CY_SCB_UART_PARITY_EVEN, CY_SCB_UART_STOP_BITS_1}, // SERIAL_8E1
    {5, CY_SCB_UART_PARITY_EVEN, CY_SCB_UART_STOP_BITS_2}, // SERIAL_5E2
    {6, CY_SCB_UART_PARITY_EVEN, CY_SCB_UART_STOP_BITS_2}, // SERIAL_6E2
    {7, CY_SCB_UART_PARITY_EVEN, CY_SCB_UART_STOP_BITS_2}, // SERIAL_7E2
    {8, CY_SCB_UART_PARITY_EVEN, CY_SCB_UART_STOP_BITS_2}, // SERIAL_8E2

    {5, CY_SCB_UART_PARITY_ODD, CY_SCB_UART_STOP_BITS_1}, // SERIAL_5O1
    {6, CY_SCB_UART_PARITY_ODD, CY_SCB_UART_STOP_BITS_1}, // SERIAL_6O1
    {7, CY_SCB_UART_PARITY_ODD, CY_SCB_UART_STOP_BITS_1}, // SERIAL_7O1
    {8, CY_SCB_UART_PARITY_ODD, CY_SCB_UART_STOP_BITS_1}, // SERIAL_8O1
    {5, CY_SCB_UART_PARITY_ODD, CY_SCB_UART_STOP_BITS_2}, // SERIAL_5O2
    {6, CY_SCB_UART_PARITY_ODD, CY_SCB_UART_STOP_BITS_2}, // SERIAL_6O2
    {7, CY_SCB_UART_PARITY_ODD, CY_SCB_UART_STOP_BITS_2}, // SERIAL_7O2
    {8, CY_SCB_UART_PARITY_ODD, CY_SCB_UART_STOP_BITS_2}, // SERIAL_8O2
};

typedef struct bitRateMapping
{
    uint32_t bitRate;
    uint32_t divNum;
}bitRateMapping_t;

const bitRateMapping_t brMapping[UART_BIT_RATE_MAX_NUM] =
{
    {   300, 9999},
    {   600, 4999},
    {  1200, 2499},
    {  1800, 1666},
    {  2400, 1249},
    {  4800,  624},
    {  7200,  416},
    {  9600,  312},
    { 14400,  207},
    { 19200,  155},
    { 38400,   77},
    { 57600,   51},
    {115200,   25}
};

//****************************************************************************
// ring buffer definition
//****************************************************************************
typedef struct ringbuffer
{
    uint8_t buffer[UART_BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
} ringbuffer_t;

void initRingBuffer(ringbuffer_t *buff);
uint8_t peekRingBuffer(ringbuffer_t *buff);
void queueRingBuffer(ringbuffer_t *buff, uint8_t data);
uint8_t dequeueRingBuffer(ringbuffer_t *buff);
uint8_t getRingBufferSpace(ringbuffer_t *buff);
uint8_t getRingBufferOccupied(ringbuffer_t *buff);

void checkUartTxFifoWrite(void);
void checkUartRxFifoRead(void);

//****************************************************************************
// @Class Definitions
//****************************************************************************
class HardwareSerial: public Stream
{
private:
    bool uartInitiated;
public:
    HardwareSerial();
	
    bool getUartInitiated(void);

    void begin(uint32_t speed);
    void begin(uint32_t speed, uart_cfg_idx_t config);
    void end(void);
    int available(void);
    int availableForWrite(void);
    int peek(void);
    void flush(void);
    int read(void);
    
    
    size_t write(const uint8_t);
    inline size_t write(unsigned long n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(long n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(unsigned int n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(int n)
    {
        return write((uint8_t)n);
    }

    using Print::write; 
    operator bool()
    {
        return true;
    } 
}; 

extern HardwareSerial Serial;

#endif  /* cplusplus */

bool uart_init(uint8_t cfg, uint32_t bitrate);

#endif /*__Serial_h__ */
