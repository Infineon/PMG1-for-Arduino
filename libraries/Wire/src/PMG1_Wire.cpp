const uint8_t i2cTimeout = 25;

// Constructors ////////////////////////////////////////////////////////////////
TwoWire::TwoWire()
{
    twoWireInitiazed = false;
}

void TwoWire::begin(void)
{
    if (i2cInit(0xFF))
        twoWireInitiazed = true;
}

void TwoWire::begin(uint8_t address)
{
    if (i2cInit(address))
        twoWireInitiazed = true;
}

void TwoWire::begin(int address)
{
    if (i2cInit((uint8_t)address))
        twoWireInitiazed = true;
}

void TwoWire::end(void)
{
    if (twoWireInitiazed)
    {
        i2cDeInit();
        twoWireInitiazed = false;
    }
}

void TwoWire::setClock(uint32_t rate)
{
    if (rate != 400000 || rate !=100000)
        return;

    if (rate == 400000)
        setPeripheralClock(4U);
    else // 100000Hz
        setPeripheralClock(14U);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
    uint8_t i = 0;

    if (twoWireInitiazed == false)
        return 0;

    i2cResetRxBuffer();
    if (isize > 0)
    {
        // send internal address; this mode allows sending a repeated start to access
        // some devices' internal registers. This function is executed by the hardware
        // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)
        beginTransmission(address);

        // the maximum size of internal address is 3 bytes
        if (isize > 3){
            isize = 3;
        }

        // write internal register address - most significant byte first
        while (isize-- > 0)
        {
            //Cy_SCB_I2C_MasterWriteByte(I2C_HW, (uint8_t)(iaddress >> (isize*8)), 10, &I2C_context);
            i2cSendByte((uint8_t)(iaddress >> (isize*8)), i2cTimeout);
        }

        //Cy_SCB_I2C_MasterSendReStart(I2C_HW, address, CY_SCB_I2C_READ_XFER, 10, &I2C_context);
        i2cSendRestart(address, i2cTimeout);
    }
    else
    {
        //Cy_SCB_I2C_MasterSendStart(I2C_HW, address, CY_SCB_I2C_READ_XFER, 10, &I2C_context);
        i2cSendStart(address, i2cTimeout);
    }

    // clamp to buffer length
    if(quantity > I2C_MAX_BUFFER_SIZE)
    {
        quantity = I2C_MAX_BUFFER_SIZE;
    }

    // perform blocking read into buffer
    while(i < (quantity - 1))
    {
        //Cy_SCB_I2C_MasterReadByte(I2C_HW, CY_SCB_I2C_ACK , &rxBuffer[i], 10, &I2C_context);
        i2cReadByte(true, i2cTimeout);
        i++;
    }

    //Cy_SCB_I2C_MasterReadByte(I2C_HW, CY_SCB_I2C_NAK , &rxBuffer[i], 10, &I2C_context);
    i2cReadByte(false, i2cTimeout);

    if(sendStop)
    {
        //Cy_SCB_I2C_MasterSendStop(I2C_HW, 10, &I2C_context);
        i2cSendStop(i2cTimeout);
    }

    // set rx buffer iterator vars
    //rxBufferIndex = 0;
    //rxBufferLength = quantity;

    return quantity;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

void TwoWire::beginTransmission(uint8_t address)
{
    if (twoWireInitiazed == false)
        return;

    i2cResetTxBuffer();
    i2cSendStart(address, i2cTimeout);
}

void TwoWire::beginTransmission(int address)
{
    if (twoWireInitiazed == false)
        return;

    beginTransmission((uint8_t)address);
}

//
//  Originally, 'endTransmission' was an f(void) function.
//  It has been modified to take one parameter indicating
//  whether or not a STOP should be performed on the bus.
//  Calling endTransmission(false) allows a sketch to
//  perform a repeated start.
//
//  WARNING: Nothing in the library keeps track of whether
//  the bus tenure has been properly ended with a STOP. It
//  is very possible to leave the bus in a hung state if
//  no call to endTransmission(true) is made. Some I2C
//  devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    uint8_t i = 0;

    if (twoWireInitiazed == false)
        return 0;

    i2cSendRestBytes(i2cTimeout);

    if(sendStop)
    {
        i2cSendStop(i2cTimeout);
    }

    return 0;
}

//  This provides backwards compatibility with the original
//  definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(uint8_t data)
{
    return i2cFillData(data);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(const uint8_t* data, size_t quantity)
{
    uint8_t i = 0;

    for (i = 0; i < quantity; ++i)
    {
        if (write(data[i]) == 0)
            break;
    }

    return (size_t) i;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void)
{
    //return rxBufferLength - rxBufferIndex;
    return getI2CRxDataNum();
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void)
{
    int value;

    value = i2cGetByte();

    return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::peek(void)
{
    int value;
    
    value = i2cPeekByte();

    return value;
}

void TwoWire::flush(void)
{
    if (twoWireInitiazed == false)
        return;
        
    i2cSendRestBytes(i2cTimeout);
    i2cSendStop(i2cTimeout);
}

void (*onReceiveHandler)(int) = NULL;
void (*onRequestHandler)(void) = NULL;

// sets function called on slave write
void TwoWire::onReceive(void (*function)(int))
{
    onReceiveHandler = function;
}

// sets function called on slave read
void TwoWire::onRequest(void (*function)(void))
{
    onRequestHandler = function;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire = TwoWire();

//****************************************************************************
//                                 END OF FILE
//****************************************************************************
