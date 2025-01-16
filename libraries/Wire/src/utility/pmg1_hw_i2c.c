cy_stc_scb_i2c_context_t I2C_context;

uint8_t i2cRxBufIdx = 0;
uint8_t i2cTxBufIdx = 0;
uint8_t i2cTxBufNum = 0;
uint8_t i2cRxBufNum = 0;
uint8_t i2cRxBuffer[I2C_MAX_BUFFER_SIZE] = {0};
uint8_t i2cTxBuffer[I2C_MAX_BUFFER_SIZE] = {0};
bool timeoutFlag = false;

extern void (*onReceiveHandler)(int);
extern void (*onRequestHandler)(void);

void I2C_Interrupt(void)
{
    Cy_SCB_I2C_SlaveInterrupt(I2C_HW, &I2C_context);
}

void i2cEventHandler(uint32_t events)
{
    /* Master request data */
    if (0UL != (events & CY_SCB_I2C_SLAVE_READ_EVENT))
    {
    	// The read operation is complete, a NAK or STOP condition received,
    	// do nothing here
        if (0UL == (events & CY_SCB_I2C_SLAVE_ERR_EVENT))
        {
            if (onRequestHandler != NULL)
                onRequestHandler();
        }
        /* Setup read buffer for the next write transaction to the I2C master */
        Cy_SCB_I2C_SlaveConfigReadBuf(I2C_HW, i2cTxBuffer, I2C_MAX_BUFFER_SIZE, &I2C_context);
    }

    /* Slave received data from master */
    if (0UL != (events & CY_SCB_I2C_SLAVE_WR_CMPLT_EVENT))
    {
        if (0UL == (events & CY_SCB_I2C_SLAVE_ERR_EVENT))
        {
            if (onReceiveHandler != NULL)
            {
                onReceiveHandler(Cy_SCB_I2C_SlaveGetWriteTransferCount(I2C_HW, &I2C_context));
            }
        }
        /* Setup buffer for the next write transaction from the I2C master */
        Cy_SCB_I2C_SlaveConfigWriteBuf(I2C_HW, i2cRxBuffer, I2C_MAX_BUFFER_SIZE, &I2C_context);
    }
    /* Ignore all other events */
}

void i2cDeInitPin(void)
{
    Cy_GPIO_Pin_Init(I2C_SDA_PORT, I2C_SDA_PIN, &DEFAULT_GPIO_HZ_CONFIG);
    Cy_GPIO_Pin_Init(I2C_SCL_PORT, I2C_SCL_PIN, &DEFAULT_GPIO_HZ_CONFIG);
}

void i2cDeInit(void)
{
    i2cDeInitPin();   
    Cy_SCB_I2C_Disable(I2C_HW, &I2C_context);
    Cy_SCB_I2C_DeInit(I2C_HW);
}

bool i2cInit(uint8_t address)
{
    cy_en_scb_i2c_status_t initStatus;
    cy_en_sysint_status_t sysStatus;

    Cy_GPIO_Pin_Init(I2C_SDA_PORT, I2C_SDA_PIN, &I2C_SDA_config);
    Cy_GPIO_Pin_Init(I2C_SCL_PORT, I2C_SCL_PIN, &I2C_SCL_config);

    //clock 400kbps
    Cy_SysClk_PeriphAssignDivider(I2C_CLOCK_BLOCK, CY_SYSCLK_DIV_8_BIT, 7U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 7U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 7U, 4U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 7U);

    if (address != 0xFF)
    {
        I2C_config.i2cMode = CY_SCB_I2C_SLAVE;
        I2C_config.slaveAddress = address;
    }
    else
    {
        I2C_config.i2cMode = CY_SCB_I2C_MASTER;
    }

    /* Initialize and enable the I2C in master mode */
    initStatus = Cy_SCB_I2C_Init(I2C_HW, &I2C_config, &I2C_context);
    if (initStatus != CY_SCB_I2C_SUCCESS)
    {
        i2cDeInitPin();
        return false;
    }

    /* Hook interrupt service routine */
    sysStatus = Cy_SysInt_Init(&I2C_SCB_IRQ_cfg, &I2C_Interrupt);
    if (sysStatus != CY_SYSINT_SUCCESS)
    {
        i2cDeInit();
        return false;
    }
    NVIC_EnableIRQ((IRQn_Type) I2C_SCB_IRQ_cfg.intrSrc);

    memset((void *)i2cRxBuffer, 0, I2C_MAX_BUFFER_SIZE);
    memset((void *)i2cTxBuffer, 0, I2C_MAX_BUFFER_SIZE);
    i2cRxBufIdx = 0;
    i2cTxBufIdx = 0;

    Cy_SCB_I2C_SlaveConfigWriteBuf(I2C_HW, i2cRxBuffer, I2C_MAX_BUFFER_SIZE, &I2C_context);
    Cy_SCB_I2C_SlaveConfigReadBuf(I2C_HW, i2cTxBuffer, I2C_MAX_BUFFER_SIZE, &I2C_context);        

    Cy_SCB_I2C_Enable(I2C_HW, &I2C_context);

    Cy_SCB_I2C_RegisterEventCallback(I2C_HW, i2cEventHandler, &I2C_context);

    return true;
}

void setPeripheralClock(uint32_t rate)
{
    if (rate != 400000 || rate != 100000)
        return;

    Cy_SysClk_PeriphAssignDivider(PCLK_SCB1_CLOCK, CY_SYSCLK_DIV_8_BIT, 7U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 7U);

    if (rate == 400000)
        Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 7U, 4U);
    else // 100000Hz
        Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 7U, 14U);

    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 7U);

    Cy_SCB_I2C_SetDataRate(I2C_HW, rate, Cy_SysClk_PeriphGetFrequency(CY_SYSCLK_DIV_8_BIT, 7));
}

cy_en_scb_i2c_status_t i2cSendStart(uint8_t address, uint32_t timeoutMs)
{
    cy_en_scb_i2c_status_t status;

    status = Cy_SCB_I2C_MasterSendStart(I2C_HW, 
                                        address, 
                                        CY_SCB_I2C_WRITE_XFER, 
                                        timeoutMs,
                                        &I2C_context);

    if (status == CY_SCB_I2C_MASTER_MANUAL_TIMEOUT)
        timeoutFlag = true;

    return status;  
}

cy_en_scb_i2c_status_t i2cSendRestart(uint8_t address, uint32_t timeoutMs)
{
    cy_en_scb_i2c_status_t status;

    status = Cy_SCB_I2C_MasterSendReStart(I2C_HW, 
                                          address, 
                                          CY_SCB_I2C_WRITE_XFER, 
                                          timeoutMs,
                                          &I2C_context);

    if (status == CY_SCB_I2C_MASTER_MANUAL_TIMEOUT)
        timeoutFlag = true;

    return status;     
}

cy_en_scb_i2c_status_t i2cSendStop(uint32_t timeoutMS)
{
    cy_en_scb_i2c_status_t status;

    status = Cy_SCB_I2C_MasterSendStop(I2C_HW, timeoutMS, &I2C_context);

    if (status == CY_SCB_I2C_MASTER_MANUAL_TIMEOUT)
        timeoutFlag = true;

    return status;     
}

cy_en_scb_i2c_status_t i2cSendByte(uint8_t data, uint32_t timeoutMS)
{
    cy_en_scb_i2c_status_t status;

    status = Cy_SCB_I2C_MasterWriteByte(I2C_HW, data, timeoutMS, &I2C_context);

    if (status == CY_SCB_I2C_MASTER_MANUAL_TIMEOUT)
        timeoutFlag = true;

    return status;     
}

cy_en_scb_i2c_status_t i2cSendRestBytes(uint32_t timeoutMS)
{
    cy_en_scb_i2c_status_t status;
    uint8_t i;

    for (i=i2cTxBufIdx; i<i2cTxBufNum; i++)
    {
        status = Cy_SCB_I2C_MasterWriteByte(I2C_HW, i2cTxBuffer[i], timeoutMS, &I2C_context);
    }

    i2cTxBufNum = 0;
    i2cTxBufIdx = 0;
    return status;
}

cy_en_scb_i2c_status_t i2cReadByte(bool respACK, uint32_t timeoutMS)
{
    cy_en_scb_i2c_status_t status;

    status = Cy_SCB_I2C_MasterReadByte(I2C_HW, 
                                       (respACK ? CY_SCB_I2C_ACK: CY_SCB_I2C_NAK), 
                                       &i2cRxBuffer[i2cRxBufNum], 
                                       timeoutMS, 
                                       &I2C_context);

    i2cRxBufNum ++;

    if (status == CY_SCB_I2C_MASTER_MANUAL_TIMEOUT)
        timeoutFlag = true;

    return status;     
}

int i2cGetByte(void)
{
    int val;

    if (i2cRxBufNum != 0)
    {
        i2cRxBufNum --;
        val = i2cRxBuffer[i2cRxBufIdx]; 
        if (i2cRxBufIdx)
            i2cRxBufIdx ++;
    }
    else
    {
        i2cRxBufIdx = 0;
        val = -1;
    }

    return val;
}

int i2cPeekByte(void)
{
    int val = -1;

    if (i2cRxBufNum != 0)
    {
        val = i2cRxBuffer[i2cRxBufIdx]; 
    }

    return val;    
}

size_t i2cFillData(uint8_t data)
{
    if ((i2cTxBufNum + 1) < I2C_MAX_BUFFER_SIZE)
    {
        i2cTxBuffer[i2cTxBufNum] = data;
        ++ i2cTxBufNum;
        return 1;
    }
    else
        return 0;
}

int getI2CRxDataNum(void)
{
    return i2cRxBufNum - i2cRxBufIdx;
}

void i2cResetTxBuffer(void)
{
    i2cTxBufIdx = 0;
    i2cTxBufNum = 0;
    memset((void *)i2cTxBuffer, 0, I2C_MAX_BUFFER_SIZE);
}

void i2cResetRxBuffer(void)
{
    i2cRxBufIdx = 0;
    i2cRxBufNum = 0;
    memset((void *)i2cRxBuffer, 0, I2C_MAX_BUFFER_SIZE);
}