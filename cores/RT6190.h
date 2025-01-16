#include <stdbool.h>
#include <stdint.h>

#ifndef PD_ADDR
#define PD_ADDR                             (0u)
#endif

#ifndef PD_VALUE
#define PD_VALUE                            (1u)
#endif

/* PD controller I2C address */
extern const uint8_t rt6190DeviceAddress[2];

#define RT6190_SLAVE_ADDRESS0				(0x2D)
#define RT6190_SLAVE_ADDRESS1				(0x2C)

#define RT6190_REG_MID						(0x00)
#define RT6190_REG_OUTPUT_VOLTAGE_L			(0x01)
#define RT6190_REG_OUTPUT_VOLTAGE_H			(0x02)
#define RT6190_REG_OUTPUT_CURRENT_L			(0x03)
#define RT6190_REG_OUTPUT_CURRENT_H			(0x04)

#define RT6190_REG_VREF_SC      			(0x08)

#define RT6190_REG_VREF_POCP				(0x0A)
#define RT6190_REG_OVP						(0x0B)
#define RT6190_REG_UVP						(0x0C)

#define RT6190_REG_SETTING1					(0x0D)
#define RT6190_REG_SETTING2					(0x0E)
#define RT6190_REG_SETTING3					(0x0F)
#define RT6190_REG_SETTING4					(0x10)

#define RT6190_REG_RATIO					(0x11)

#define RT6190_REG_OUTPUT_VOLTAGE			(0x12)
#define RT6190_REG_OUTPUT_CURRENT			(0x14)

#define RT6190_REG_ALERT1					(0x1E)
#define RT6910_REG_ALERT2					(0x1F)

#define RT6190_REG_MASK1					(0x20)

#define RT6190_REG_OCP1_SETTING				(0x22)
#define RT6190_REG_OCP2_SETTING				(0x23)
#define RT6190_REG_OCP3_SETTING				(0x24)
#define RT6190_REG_OCP4_SETTING				(0x25)

#define RT6190_REG_OCP1_DELAY_TIME			(0x26)
#define RT6190_REG_OCP2_DELAY_TIME			(0x27)
#define RT6190_REG_OCP_ENABLE				(0x28)

#define RT6190_REG_SETTING5					(0x29)

#define RT6190_REG_OPTION2					(0x2B)

#define RT6190_REG_ARALM_HI					(0x2C)
#define RT6190_REG_ALARM_LO					(0x2E)

#define RT6190_REG_WATCHGOD					(0x30)

#define RT6190_REG_STATUS3					(0x37)

#define RT6190_REG_ALERT3					(0x38)
#define RT6190_REG_MASK3					(0x39)

#define RT6190_REG_FUNCTION                 (0xA0)

/* RT6190_REG_OUTPUT_CURRENT_L			(0x03) */
#define OUTPUT_CURRENT_L_12A                (0xFF)

/* RT6190_REG_OUTPUT_CURRENT_H			(0x04) */
#define OUTPUT_CURRENT_H_12A                (0x01)
    
/* RT6190_REG_VREF_POCP (0x0A) */
#define REG_VREF_POCP_24A                   (0x3Fu)

/* RT6190_REG_SETTING1 (0x0D) */
#define SETTING1_REG_FSW_615KHZ             (0x04u)
#define SETTING1_REG_FSW_730KHZ             (0x05u)
#define SETTING1_REG_FSW_845KHZ             (0x06u)
#define SETTING1_REG_FSW_960KHZ             (0x07u)
#define SETTING1_REG_SLEWRATE_F_32US        (0x18u)
#define SETTING1_REG_SLEWRATE_R_32US        (0x60u)
#define SETTING1_REG_FCCM_EN                (0x80u)

/* RT6190_REG_SETTING2 (0x0E) */
#define SETTING2_REG_EN_PWM					(1u<<7)
#define SETTING2_REG_DIS_INCC				(1u<<6)
#define SETTING2_REG_DIS_INCV				(1u<<5)
#define SETTING2_REG_EN_DISCHARGE			(1u<<4)
#define SETTING2_REG_IR_COMP				(1u<<0)

/* RT6190_REG_SETTING3 (0x0F) */
#define SETTING3_REG_DT_SEL_30NS			(0u<<6)
#define SETTING3_REG_DT_SEL_50NS			(1u<<6)
#define SETTING3_REG_DT_SEL_70NS			(2u<<6)
#define SETTING3_REG_DT_SEL_90NS			(3u<<6)
#define SETTING3_REG_GM_EA_275UA			(0u<<4)
#define SETTING3_REG_GM_EA_550UA			(1u<<4)
#define SETTING3_REG_GM_EA_825UA			(2u<<4)
#define SETTING3_REG_GM_EA_1100UA			(3u<<4)
#define SETTING3_REG_GAIN_ICS_10X			(0u<<4)
#define SETTING3_REG_GAIN_ICS_20X			(1u<<4)
#define SETTING3_REG_GAIN_ICS_30X			(2u<<4)
#define SETTING3_REG_GAIN_ICS_40X			(3u<<4)
#define SETTING3_REG_GAIN_OCS_10X			(0u<<0)
#define SETTING3_REG_GAIN_OCS_20X			(1u<<0)
#define SETTING3_REG_GAIN_OCS_30X			(2u<<0)
#define SETTING3_REG_GAIN_OCS_40X			(3u<<0)

/* RT6190_REG_SETTING4 (0x10) */
#define SETTING4_REG_ADC_AVG_8_TIMES        (0x80u)
#define SETTING4_REG_OCP4_TIME_X10_EN       (0x10u)
#define SETTING4_REG_ADC_EN                 (0x02u)

/* RT6190_REG_SETTING4 (0x11) */
#define RATIO_REG_SSP_EN                    (1<<7)
#define RATIO_REG_VIN_RATIO_0_0_8VV         (0<<6)
#define RATIO_REG_VIN_RATIO_0_0_5VV         (1<<6)
#define RATIO_REG_VOUT_RATIO_0_0_8VV        (0<<5)
#define RATIO_REG_VOUT_RATIO_0_0_5VV        (1<<5)

/* RT6910_REG_ALERT2 (0x1F) */
#define RT6190_REG_ALERT2_PG                (0x40u)
#define RT6190_REG_ALERT2_OCP1              (0x01u) 
#define RT6190_REG_ALERT2_OCP2              (0x02u) 
#define RT6190_REG_ALERT2_OCP3              (0x04u) 
#define RT6190_REG_ALERT2_OCP4              (0x08u) 

/* RT6190_REG_OCP1_DELAY_TIME (0x26) */
#define OCP1_DELAY_TIME_8MS                 (0x01u)

/* RT6190_REG_FUNCTION (0xA0) */
#define MASK_INTERNAL_FUNCTION_FOR_VOLTAGE  (0xFDu)

#define NO_DELAY                            (0u)
#define RT6190_56MS_OCP2_TIMING             (0x07u)
#define RT6190_4S_OCP2_TIMING               (0xFFu)
#define RT6190_50MS_OCP4_TIMING             (0x00u)
#define RT6190_400MS_OCP4_TIMING            (0x0Cu)

#define RT6190_OCP_ENABLE_OCP1_EN           (0x10u)
#define RT6190_OCP_ENABLE_OCP2_EN           (0x20u)
#define RT6190_OCP_ENABLE_OCP3_EN           (0x40u)
#define RT6190_OCP_ENABLE_OCP4_EN           (0x80u)

#define RT6190_9A_OCP1_SETTING              (0x6Eu)
#define RT6190_11A_OCP1_SETTING             (0x86u)
#define RT6190_13A_OCP1_SETTING             (0xA4u)
 
#define RT6190_4225MA_OCP2_SETTING          (0x34u)
#define RT6190_6240MA_OCP2_SETTING          (0x4Du)
#define RT6190_5200MA_OCP2_SETTING          (0x44u) 
    
#define RT6190_4225MA_OCP4_SETTING          (0x34u)
#define RT6190_6240MA_OCP4_SETTING          (0x4Du)

#define OCP_VALUE_4_8A                 		(0x0F)

void pmicI2cMInit(void);
bool pmicI2cMWrite(uint8_t devAddr, uint8_t *buffer, uint8_t count, uint8_t retry_cnt);
bool pmicI2cMRead(uint8_t devAddr, uint8_t *buffer, uint8_t count, uint8_t retry_cnt);
bool pmicI2cMRegRead(uint8_t devAddr, uint8_t *buffer, uint8_t count, uint8_t regAddr, uint8_t regSize);
bool pmicI2cMRegWrite(uint8_t devAddr, uint8_t buffer, uint8_t count, uint8_t regAddr, uint8_t regSize);
bool pd_ctrl_power_rt6190(uint8_t port, uint16_t volt, uint8_t state);
bool set_pd_ctrl_voltage_rt6190(uint8_t port, uint16_t volt);
bool set_pd_ocp_rt6190(uint8_t port, uint16_t volt);
bool pd_ctrl_ocp_rt6190(uint8_t port, uint8_t state);
uint8_t pd_check_pg_rt6190(uint8_t port);

/* [] END OF FILE */