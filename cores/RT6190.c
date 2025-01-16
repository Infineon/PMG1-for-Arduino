#include "cy_scb_i2c.h"
#include "cy_sysint.h"
#include "cybsp.h"
#include "config.h"
#include "rt6190.h"

#if PMGDUINO_BOARD
#define I2CM_DATA_RATE_HZ           (100000U)
#define I2CM_TIMER_PERIOD           (10U)
#define I2CM_RETRY_COUNT			(3u) 
#define I2CM_RETRY_DELAY_MS			(5u)

#define RICKTEK_PD_CFG_COLS         (2u)
#define RICKTEK_PD_ADDR_H           (2u)
#define RICKTEK_PD_ADDR_L           (1u)
#define RICKTEK_PD_VALUE            (1u)

#define PMIC_BUFFER_SIZE            (8UL)

static cy_stc_scb_i2c_context_t  pmicI2cContext;

/* Allocate buffers for I2C slave operation */

uint8_t pmicI2cReadBuffer[PMIC_BUFFER_SIZE];
uint8_t pmicI2cWriteBuffer[PMIC_BUFFER_SIZE];

/* Populate configuration structure */
const cy_stc_sysint_t pmicI2cIntrConfig =
{
    .intrSrc      = PMIC_I2C_IRQ,
    .intrPriority = 3,
};

const uint8_t rt6190DeviceAddress[2] = {RT6190_SLAVE_ADDRESS1, RT6190_SLAVE_ADDRESS0};

void PMIC_I2C_Isr(void)
{
    Cy_SCB_I2C_Interrupt(PMIC_I2C_HW, &pmicI2cContext);
}
/* Callback implementation */
void PMIC_I2C_SlaveEventHandler(uint32_t events)
{
    /* Slave sent data to master */
    if (0UL != (events & CY_SCB_I2C_SLAVE_RD_CMPLT_EVENT))
    {
        if (0UL == (events & CY_SCB_I2C_SLAVE_ERR_EVENT))
        {
            /* Read complete without errors: update buffer content */
        }
        /* Setup read buffer for the next read transaction */
        Cy_SCB_I2C_SlaveConfigReadBuf(PMIC_I2C_HW, pmicI2cReadBuffer,  PMIC_BUFFER_SIZE, &pmicI2cContext);
    }
    /* Slave received data from master */
    if (0UL != (events & CY_SCB_I2C_SLAVE_WR_CMPLT_EVENT))
    {
        if (0UL == (events & CY_SCB_I2C_SLAVE_ERR_EVENT))
        {
            /* Write complete without errors: process received data */
        }
        /* Setup buffer for the next write transaction */
        Cy_SCB_I2C_SlaveConfigWriteBuf(PMIC_I2C_HW, pmicI2cWriteBuffer, PMIC_BUFFER_SIZE, &pmicI2cContext);
    }
    /* Ignore all other events */
}

/* Configure the I2C interface. */
void pmicI2cMInit(void)
{
    uint32_t dataRate = 0U;

    /* Connect SCB0 I2C function to pins */
    Cy_GPIO_SetHSIOM(PMIC_I2C_SCL_PORT, PMIC_I2C_SCL_NUM, P6_1_SCB6_I2C_SCL);
    Cy_GPIO_SetHSIOM(PMIC_I2C_SDA_PORT, PMIC_I2C_SDA_NUM, P6_0_SCB6_I2C_SDA);
    /* Configure pins for I2C operation */
    Cy_GPIO_SetDrivemode(PMIC_I2C_SCL_PORT, PMIC_I2C_SCL_NUM, PMIC_I2C_SCL_DRIVEMODE);
    Cy_GPIO_SetDrivemode(PMIC_I2C_SDA_PORT, PMIC_I2C_SDA_NUM, PMIC_I2C_SDA_DRIVEMODE);
    
    /* Connect assigned divider to be a clock source for I2C */
    Cy_SysClk_PeriphAssignDivider(PCLK_SCB6_CLOCK, CLK_PMIC_HW, CLK_PMIC_NUM);

    Cy_SysClk_PeriphSetDivider   (CLK_PMIC_HW, CLK_PMIC_NUM, 14u);
    Cy_SysClk_PeriphEnableDivider(CLK_PMIC_HW, CLK_PMIC_NUM);
    
    Cy_SCB_I2C_Init (PMIC_I2C_HW, &PMIC_I2C_config, &pmicI2cContext);

    /* Note. This function will have to be changed if the clock divider for the SCB is changed. */
    dataRate = Cy_SCB_I2C_SetDataRate(PMIC_I2C_HW, I2CM_DATA_RATE_HZ, Cy_SysClk_PeriphGetFrequency(CLK_PMIC_HW, CLK_PMIC_NUM));
    if ((dataRate > I2CM_DATA_RATE_HZ) || (dataRate == 0U))
    {
        return;
    }

    Cy_SCB_I2C_SlaveConfigReadBuf (PMIC_I2C_HW, pmicI2cReadBuffer,  PMIC_BUFFER_SIZE, &pmicI2cContext);
    Cy_SCB_I2C_SlaveConfigWriteBuf(PMIC_I2C_HW, pmicI2cWriteBuffer, PMIC_BUFFER_SIZE, &pmicI2cContext);
    
    /* Hook interrupt service routine and enable interrupt */
    (void) Cy_SysInt_Init(&pmicI2cIntrConfig, &PMIC_I2C_Isr);

    Cy_SCB_I2C_RegisterEventCallback(PMIC_I2C_HW, PMIC_I2C_SlaveEventHandler, &pmicI2cContext);

    Cy_SCB_I2C_Enable(PMIC_I2C_HW, &pmicI2cContext);
}


/* Transfer data to slave */
// devAddr: the I2C device address
// buffer: the transmission data including register address
// count: among of data for transmission in byte
// retry_cnt: how many time to retry the I2C access if the transmission fails
bool pmicI2cMWrite(uint8_t devAddr, uint8_t *buffer, uint8_t count, uint8_t retry_cnt)
{
    cy_en_scb_i2c_status_t status;
    uint32_t timeout = I2CM_TIMER_PERIOD;

    /* Send Start condition, address and receive ACK/NACK response from slave */
    status = Cy_SCB_I2C_MasterSendStart(PMIC_I2C_HW, devAddr, CY_SCB_I2C_WRITE_XFER, timeout, &pmicI2cContext);
    if (status == CY_SCB_I2C_SUCCESS)
    {
        uint32_t cnt = 0UL;
        /* Write data into the slave from the buffer */
        do
        {
            /* Write byte and receive ACK/NACK response */
            status = Cy_SCB_I2C_MasterWriteByte(PMIC_I2C_HW, buffer[cnt], timeout, &pmicI2cContext);
            ++cnt;
        }
        while((status == CY_SCB_I2C_SUCCESS) && (cnt < count));
    }
    /* Check status of transaction */
    if ((status == CY_SCB_I2C_SUCCESS)           ||
            (status == CY_SCB_I2C_MASTER_MANUAL_NAK) ||
            (status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK))
    {
        /* Send Stop condition on the bus */
        Cy_SCB_I2C_MasterSendStop(PMIC_I2C_HW, timeout, &pmicI2cContext);
    }

    if((status != CY_SCB_I2C_SUCCESS) && (retry_cnt > 0))
    {
    	Cy_SysLib_Delay(I2CM_RETRY_DELAY_MS);
        pmicI2cMWrite(devAddr, buffer, count, retry_cnt - 1);
    }

    return (status == CY_SCB_I2C_SUCCESS) ? true : false;
}

/* Receive data from slave */
// devAddr: the I2C device address
// buffer: the transmission data including register address
// count: among of data for transmission in byte
// retry_cnt: how many time to retry the I2C access if the transmission fails
bool pmicI2cMRead(uint8_t devAddr, uint8_t *buffer, uint8_t count, uint8_t retry_cnt)
{
    cy_en_scb_i2c_status_t status;
    uint32_t timeout = I2CM_TIMER_PERIOD;

    /* Send Start condition, address and receive ACK/NACK response from slave */
    status = Cy_SCB_I2C_MasterSendStart(PMIC_I2C_HW, devAddr, CY_SCB_I2C_READ_XFER, timeout, &pmicI2cContext);
    if (status == CY_SCB_I2C_SUCCESS)
    {
        uint32_t cnt = 0UL;
        /* Write data into the slave from the buffer */
        do
        {
            /* Read byte and send ACK/NACK response */
            status = Cy_SCB_I2C_MasterReadByte(PMIC_I2C_HW, ((cnt+1 == count) ? CY_SCB_I2C_NAK : CY_SCB_I2C_ACK), &buffer[cnt], timeout, &pmicI2cContext);
            ++cnt;
        }
        while((status == CY_SCB_I2C_SUCCESS) && (cnt < count));
    }
    /* Check status of transaction */
    if ((status == CY_SCB_I2C_SUCCESS)           ||
            (status == CY_SCB_I2C_MASTER_MANUAL_NAK) ||
            (status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK))
    {
        /* Send Stop condition on the bus */
        Cy_SCB_I2C_MasterSendStop(PMIC_I2C_HW, timeout, &pmicI2cContext);
    }

    if((status != CY_SCB_I2C_SUCCESS) && (retry_cnt > 0))
    {
        retry_cnt--;
        pmicI2cMRead(devAddr, buffer, count, retry_cnt);
    }

    return (status == CY_SCB_I2C_SUCCESS) ? true : false;
}

// devAddr: device address
// buffer: data for the transmission
// count: among of data for transmission in byte
// regAddr: register address
// regSize: register address length in byte
bool pmicI2cMRegRead(uint8_t devAddr, uint8_t *buffer, uint8_t count,
                                uint8_t regAddr, uint8_t regSize)
{
    (void) regSize;

    bool status;
    uint8_t wr_buffer[1] = {0};

    wr_buffer[0] = regAddr;

    status = pmicI2cMWrite(devAddr, wr_buffer, regSize, I2CM_RETRY_COUNT);
    
    if(status == true)
    {
        status = pmicI2cMRead(devAddr, buffer, count, 0);
    }

    return status;
}

// devAddr: device address
// buffer: data for the transmission
// count: among of data for transmission in byte
// regAddr: register address
// regSize: register address length in byte
bool pmicI2cMRegWrite(uint8_t devAddr, uint8_t buffer, uint8_t count,
                                 uint8_t regAddr, uint8_t regSize)
{
    (void) count;
    (void) regSize;

    bool status;
    uint8_t wr_buffer[2] = {0};

    wr_buffer[0] = regAddr;
    wr_buffer[1] = buffer;

    status = pmicI2cMWrite(devAddr, wr_buffer, 2, I2CM_RETRY_COUNT);

    return status;
}

static bool vinVoutRatio0d05[2] = {false, false};

bool pd_ctrl_power_rt6190(uint8_t port, uint16_t volt, uint8_t state)
{
    uint8_t devAddr = rt6190DeviceAddress[port];
    uint8_t regAddr;
    uint8_t regData;
    uint8_t regReadData;
    uint16_t mv = volt + 200; // compensation, optional
    bool return_value = true;

    regAddr = RT6190_REG_VREF_SC;
    regData = 0x3F;
    if (pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1) == false)
    {
        return_value = false;
    }
    
    regAddr = RT6190_REG_SETTING1;
    regData = SETTING1_REG_FCCM_EN | SETTING1_REG_SLEWRATE_R_32US | SETTING1_REG_FSW_615KHZ; //0xE4;
    if (pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1) == false)
    {
        return_value = false;
    }
    
    regAddr = RT6190_REG_VREF_POCP;
    regData = REG_VREF_POCP_24A;
    if (pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1) == false)
    {
        return_value = false;
    }
    
    regAddr = RT6190_REG_SETTING4;
	regData = SETTING4_REG_ADC_AVG_8_TIMES | SETTING4_REG_ADC_EN;
    if (pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1) == false)
    {
        return_value = false;
    }
 
    regAddr = RT6190_REG_RATIO;
    if (pmicI2cMRegRead(devAddr, &regData, 1, regAddr, 1) == true)
    {
        regReadData = regData;
        if (regData & (RATIO_REG_VOUT_RATIO_0_0_5VV))
        {
            vinVoutRatio0d05[port] = true;
        }
        else
        {
            regAddr = RT6190_REG_SETTING2;
            if (pmicI2cMRegRead(devAddr, &regData, 1, regAddr, 1) == true)
            {
                if ((regData & SETTING2_REG_EN_PWM) == 0)
                {
                    regAddr = RT6190_REG_RATIO;
                    regData = regReadData | RATIO_REG_VOUT_RATIO_0_0_5VV;
                    if (pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1) == true)
                        vinVoutRatio0d05[port] = true;
                }
            }
                
        }
    }

    regAddr = RT6190_REG_SETTING2;
    regData = SETTING2_REG_IR_COMP;
    if(state != 0)
	{
        regData |= SETTING2_REG_EN_PWM;
    }
    if (pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1) == false)
    {
        return_value = false;
    }

    regAddr = RT6190_REG_OUTPUT_VOLTAGE_L;
    if (vinVoutRatio0d05[port] == false)
        regData = ((mv * 10) / 125u) & 0xFF;
    else
        regData = ((mv * 10) / 200u) & 0xFF;
    pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1);
    
    regAddr = RT6190_REG_OUTPUT_VOLTAGE_H;
    if (vinVoutRatio0d05[port] == false)
        regData = ((mv * 10) / 125u) >> 8;
    else
        regData = ((mv * 10) / 200u) >> 8;
    pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1);    
   
    regAddr = RT6190_REG_FUNCTION;
    regData = MASK_INTERNAL_FUNCTION_FOR_VOLTAGE;
    if (pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1) == false)
    {
        return_value = false;
	}

    if (mv == 20000)
    {
        regAddr = RT6190_REG_SETTING3;//0x0F;
        regData = SETTING3_REG_GM_EA_1100UA;//0x30;
        pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1);
    }

    regAddr = RT6190_REG_OUTPUT_CURRENT_L;
    regData = OUTPUT_CURRENT_L_12A;
    pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1);
    
    regAddr = RT6190_REG_OUTPUT_CURRENT_H;
    regData = OUTPUT_CURRENT_H_12A;
    pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1);

    return return_value;
}

/* Setting power voltage */
bool set_pd_ctrl_voltage_rt6190(uint8_t port, uint16_t volt)
{
    uint8_t devAddr = rt6190DeviceAddress[port];
    uint8_t regAddr;
    uint8_t regData;
    uint16_t mV = volt + 200;

    if (volt == 20000)
    {
        regAddr = RT6190_REG_SETTING3;//0x0F;
        regData = SETTING3_REG_GM_EA_1100UA;//0x30;
        pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1);
    }
    
    regAddr = RT6190_REG_OUTPUT_CURRENT_L;
    regData = OUTPUT_CURRENT_L_12A;
    pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1);
    
    regAddr = RT6190_REG_OUTPUT_CURRENT_H;
    regData = OUTPUT_CURRENT_H_12A;
    pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1);
      
    regAddr = RT6190_REG_OUTPUT_VOLTAGE_L;
    if ( vinVoutRatio0d05[port] == false)
        regData = ((mV * 10) / 125u) & 0xFF;
    else
        regData = ((mV * 10) / 200u) & 0xFF;
    pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1);
    
    regAddr = RT6190_REG_OUTPUT_VOLTAGE_H;
    if ( vinVoutRatio0d05[port] == false)
        regData = ((mV * 10) / 125u) >> 8;
    else
        regData = ((mV * 10) / 200u) >> 8;
    pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1);    
    
    return true;
}

bool set_pd_ocp_rt6190(uint8_t port, uint16_t volt)
{
    uint8_t devAddr = rt6190DeviceAddress[port];
    uint8_t regAddr;
    uint8_t regData;
    bool return_value = true;
    
    regAddr = RT6190_REG_OCP2_SETTING;    

    if (volt != 5000)
        regData = RT6190_5200MA_OCP2_SETTING; // 5.57A
    else
        regData = 0xC8; // 16.384A

    if (pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1) == false)
    {
        return_value = false;
    }    

    return return_value;        
}

bool pd_ctrl_ocp_rt6190(uint8_t port, uint8_t state)
{
    uint8_t devAddr = rt6190DeviceAddress[port];
    uint8_t regAddr;
    uint8_t regData;

    /* Write data in index 1 to address in index 0. */
    bool return_value = true;
    regAddr = RT6190_REG_OCP1_DELAY_TIME;
    regData = OCP1_DELAY_TIME_8MS;
    if (pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1) == false)
    {
        return_value = false;
    }
    
    regAddr = RT6190_REG_OCP2_DELAY_TIME;
    regData = RT6190_56MS_OCP2_TIMING;
    if (pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1) == false)
    {
        return_value = false;
    }
    
    regAddr = RT6190_REG_OCP_ENABLE;

    if(state)
    {
        regData = RT6190_OCP_ENABLE_OCP2_EN;
    }
    else
    {
        regData = 0;
    }

    if (pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1) == false)
    {
        return_value = false;
    }

    return return_value;
}

uint8_t pd_check_pg_rt6190(uint8_t port)
{
    uint8_t devAddr = rt6190DeviceAddress[port];
    uint8_t regAddr;
    uint8_t regData;
    uint8_t alert_val; 
    
    regAddr = RT6910_REG_ALERT2;
    while(!pmicI2cMRegRead(devAddr, &regData, 1, regAddr, 1));
    
    alert_val = regData & ~((uint8_t)RT6190_REG_ALERT2_PG);

    /* check power good bit */
    if(regData & RT6190_REG_ALERT2_PG)
    {
        regAddr = RT6910_REG_ALERT2;
        regData = RT6190_REG_ALERT2_PG;
        pmicI2cMRegWrite(devAddr, regData, 1, regAddr, 1);
    }

    return alert_val;
}
#endif
/* [] END OF FILE */
