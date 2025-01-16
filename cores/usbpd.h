
#ifdef __cplusplus
extern "C" {
#endif

#include "cy_usbpd_common.h"
#include "cy_pdstack_common.h"
#include "cy_usbpd_typec.h"
#include "cy_pdstack_dpm.h"
#include "cy_usbpd_defines.h"
#include "cy_usbpd_vbus_ctrl.h"
#include "cy_usbpd_phy.h"
#include "cy_pdutils_sw_timer.h"
#include "user_timer_id.h"

extern cy_stc_pdstack_context_t *get_pdstack_context(uint8_t portIdx);
extern void updatePeerSrcPdo(uint8_t port, const cy_stc_pdstack_pd_packet_t* srcCap);
extern bool enterVBUSSafe5V(uint8_t port);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

typedef union
{
    uint32_t val;
    /** @brief Structure representing a Fixed Supply PDO - Source. */
    struct FIXED_SRC_DO
    {
        uint32_t maxCurrent               : 10;     /**< Maximum current in 100mA units. */
        uint32_t voltage                  : 10;     /**< Voltage in 50mV units. */
        uint32_t pkCurrent                : 2;      /**< Peak current. */
#if CY_PD_EPR_ENABLE
        uint32_t rsrvd                    : 1;      /**< Reserved field. */
        uint32_t eprModeCapable           : 1;      /**< EPR mode capable. */
#else
        uint32_t rsrvd                    : 2;      /**< Reserved field. */
#endif /* CY_PD_EPR_ENABLE */
        uint32_t unchunkSup               : 1;      /**< Unchunked extended messages supported. */
        uint32_t drSwap                   : 1;      /**< Data Role Swap supported. */
        uint32_t usbCommCap               : 1;      /**< USB communication capability. */
        uint32_t extPowered               : 1;      /**< Externally powered. */
        uint32_t usbSuspendSup            : 1;      /**< USB suspend supported. */
        uint32_t dualRolePower            : 1;      /**< Dual role power support. */
        uint32_t supplyType               : 2;      /**< Supply type - should be 'b00. */
    } fixed_src_do_t;                                    /**< DO interpreted as a Fixed Supply PDO - Source. */

    /** @brief Structure representing a Variable Supply PDO - Source. */
    struct VAR_SRC_DO
    {
        uint32_t maxCurrent                : 10;   /**< Maximum current in 10mA units. */
        uint32_t minVoltage                : 10;   /**< Minimum voltage in 50mV units. */
        uint32_t maxVoltage                : 10;   /**< Maximum voltage in 50mV units. */
        uint32_t supplyType                : 2;    /**< Supply type - should be 'b10. */
    } var_src_do_t;                                      /**< DO interpreted as a Variable Supply PDO - Source. */

    /** @brief Structure representing a Battery Supply PDO - Source. */
    struct BAT_SRC_DO
    {
        uint32_t maxPower                  : 10;   /**< Maximum power in 250mW units. */
        uint32_t minVoltage                : 10;   /**< Minimum voltage in 50mV units. */
        uint32_t maxVoltage                : 10;   /**< Maximum voltage in 50mV units. */
        uint32_t supplyType                : 2;    /**< Supply type - should be 'b01. */
    } bat_src_do_t;                                      /**< DO interpreted as a Battery Supply PDO - Source. */

    /** @brief Structure representing a generic source PDO. */
    struct SRC_GEN_DO
    {
        uint32_t maxCurPower               : 10;   /**< Maximum current in 10 mA or power in 250 mW units. */
        uint32_t minVoltage                : 10;   /**< Minimum voltage in 50mV units. */
        uint32_t maxVoltage                : 10;   /**< Maximum voltage in 50mV units. */
        uint32_t supplyType                : 2;    /**< Supply type. */
    } src_gen_do_t;                                      /**< DO interpreted as a generic PDO - Source. */
} src_cap_t;

typedef union 
{
    uint32_t val;
    struct FIXED_SNK_DO
    {
        uint32_t opCurrent                  : 10;   /**< Operational current in 10mA units. */
        uint32_t voltage                    : 10;   /**< Voltage in 50mV units. */
        uint32_t rsrvd                      : 3;    /**< Reserved field. */
        uint32_t frSwap                     : 2;    /**< FR swap support. */
        uint32_t drSwap                     : 1;    /**< Data Role Swap supported. */
        uint32_t usbCommCap                 : 1;    /**< USB communication capability. */
        uint32_t extPowered                 : 1;    /**< Externally powered. */
        uint32_t highCap                    : 1;    /**< Higher capability possible. */
        uint32_t dualRolePower              : 1;    /**< Dual role power support. */
        uint32_t supplyType                 : 2;    /**< Supply type - should be 'b00. */
    } fixed_snk_do_t;                                    /**< DO interpreted as a Fixed Supply PDO - Sink. */

    /** @brief Structure representing a Variable Supply PDO - Sink. */
    struct VAR_SNK_DO
    {
        uint32_t opCurrent                  : 10;   /**< Operational current in 10mA units. */
        uint32_t minVoltage                 : 10;   /**< Minimum voltage in 50mV units. */
        uint32_t maxVoltage                 : 10;   /**< Maximum voltage in 50mV units. */
        uint32_t supplyType                 : 2;    /**< Supply type - should be 'b10. */
    } var_snk_do_t;                                      /**< DO interpreted as a Variable Supply PDO - Sink. */

    /** @brief Structure representing a Battery Supply PDO - Sink. */
    struct BAT_SNK_DO
    {
        uint32_t opPower                    : 10;   /**< Maximum power in 250mW units. */
        uint32_t minVoltage                 : 10;   /**< Minimum voltage in 50mV units. */
        uint32_t maxVoltage                 : 10;   /**< Maximum voltage in 50mV units. */
        uint32_t supplyType                 : 2;    /**< Supply type - should be 'b01. */
    } bat_snk_do_t;                                      /**< DO interpreted as a Battery Supply PDO - Sink. */
} snk_cap_t;

#ifndef CY_PD_SRCCAP_FLAGS_1_DUAL_ROLE_POWER
#define CY_PD_SRCCAP_FLAGS_1_DUAL_ROLE_POWER    (1U << 1U)
#endif

#ifndef CY_PD_SRCCAP_FLAGS_1_USB_SUSP_SUPP
#define CY_PD_SRCCAP_FLAGS_1_USB_SUSP_SUPP      (1U << 0U)
#endif

#ifndef CY_PD_SRCCAP_FLAGS_0_UNCONSTRAINED_POW
#define CY_PD_SRCCAP_FLAGS_0_UNCONSTRAINED_POW  (1U << 7U)
#endif

#ifndef CY_PD_SRCCAP_FLAGS_0_USB_COMM
#define CY_PD_SRCCAP_FLAGS_0_USB_COMM           (1U << 6U)
#endif

#ifndef CY_PD_SRCCAP_FLAGS_0_DUAL_ROLE_DATA
#define CY_PD_SRCCAP_FLAGS_0_DUAL_ROLE_DATA     (1U << 5U)
#endif

#ifndef CY_PD_SRCCAP_FLAGS_0_UNCHUNKED_SUPP
#define CY_PD_SRCCAP_FLAGS_0_UNCHUNKED_SUPP     (1U << 4U)
#endif

#ifndef CY_PD_SRCCAP_FLAGS_0_EPR_MODE_CAP
#define CY_PD_SRCCAP_FLAGS_0_EPR_MODE_CAP       (1U << 3U)
#endif

#ifndef CY_PD_SNKCAP_FLAGS_1_DUAL_ROLE_POWER
#define CY_PD_SNKCAP_FLAGS_1_DUAL_ROLE_POWER    (1U << 1U)
#endif

#ifndef CY_PD_SNKCAP_FLAGS_1_HIGH_CAP
#define CY_PD_SNKCAP_FLAGS_1_HIGH_CAP           (1U << 0U)
#endif 

#ifndef CY_PD_SNKCAP_FLAGS_0_UNCONSTRAINED_POW
#define CY_PD_SNKCAP_FLAGS_0_UNCONSTRAINED_POW  (1U << 7U)
#endif

#ifndef CY_PD_SNKCAP_FLAGS_0_USB_COMM
#define CY_PD_SNKCAP_FLAGS_0_USB_COMM           (1U << 6U)
#endif

#ifndef CY_PD_SNKCAP_FLAGS_0_DUAL_ROLE_DATA
#define CY_PD_SNKCAP_FLAGS_0_DUAL_ROLE_DATA     (1U << 5U)
#endif

class USBPD
{
private:
    uint8_t                     portIdx;
    //cy_stc_pdstack_context_t    *ctx;
    snk_cap_t                   iSprSnkPdo[7] = {0};
    uint8_t                     iSprSnkPdoCnt = 0x01;
    uint8_t                     iSprSnkMask = 0x01;
    cy_stc_pdstack_context_t    *ctx;
    bool                        portInitated[2] = {false, false};


#if PMGDUINO_BOARD 
    src_cap_t                   iSprSrcPdo[7] = {0};
    uint8_t                     iSprSrcPdoCnt = 0x01;
    uint8_t                     iSprSrcMask = 0x01;

    bool setSrcPdo(uint8_t pdoNum, src_cap_t srcCap);
#endif
    bool setSnkPdo(uint8_t pdoNum, snk_cap_t snkCap);
    
public:    
    bool        attached;
    uint8_t     polarity;
    uint8_t     attachedDeviceType;
    bool        contractExisted;
    uint8_t     powerRole;

    bool        usbPdCmdComplete = false;       

    bool        iPartnerSrcPdoUpdated = false;
    src_cap_t   iPartnerSrcPdo[13] = {0};
    uint8_t     iPartnerSrcPdoCnt = 0x00;    

    bool        iPartnerSnkPdoUpdated = false;
    snk_cap_t   iPartnerSnkPdo[13] = {0};
    uint8_t     iPartnerSnkPdoCnt = 0x00; 

    USBPD() {}
    USBPD(uint8_t port) {
        portIdx = port;
        ctx = get_pdstack_context(port);
    }

    void begin(void);
    void end(void);

    void updateStatus(void);
    uint8_t getCurrentSrcRdo(void);
    uint8_t getCurrentSnkPdo(void);

#if PMGDUINO_BOARD
    void setSrcUSBCommFlag(bool enable);
    void setSrcDualDataRoleFlag(bool enable);
    void setSrcDualPowerModeFlag(bool enable);
    void setSrcUnconstrainedPowerFlag(bool enable);
#if CY_PD_EPR_ENABLE
    void enableSrcEprPdo(bool enable);
    void setSrcEPRFlag(bool enable);
#endif
    void setUnchunkedExtMsgFlag(bool enable);
    void setSrcUSBSuspendCapFlag(bool enable);
    void setSrcPeakCurrent(uint8_t peakCurrent);

    bool addFixedSrcPdo(uint8_t pdoNum, uint32_t voltage, uint32_t maxCurrent, uint8_t peakCurrent);
    bool addVarSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxCurrent);
    bool addBatSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxPower);
    bool addAugmentedSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxCurrent);

    bool removeSrcPdo(uint8_t pdoNum);
    uint8_t getSrcPdoNum(void);
#endif

    void setSnkDualDataRoleFlag(bool enable);
    void setSnkUnconstrainedPowerFlag(bool enable);
    void setSnkUSBCommFlag(bool enable);
    void setSnkDualPowerModeFlag(bool enable);

    bool addFixedSnkPdo(uint8_t pdoNum, uint32_t voltage, uint32_t opCurrent);
    bool addVarSnkPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t opCurrent);
    bool addBatSnkPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t opPower);

    bool removeSnkPdo(uint8_t pdoNum);
    uint8_t getSnkPdoNum(void);

    bool updateSrcPdo(void);
    bool updateSnkPdo(void);
#if CY_PD_EPR_ENABLE
    bool getEprModeActive(void);
#endif    
#if CY_PD_EPR_ENABLE
    void enableSnkEprPdo(bool enable);
#endif

#if 0
    void enableSnkEprMode(bool enable);
#endif
    void getPartnerPortSrcPdo(void);
    void getPartnerPortSnkPdo(void);

    void doPwrRoleSwap(void);

    void registerEvent(uint8_t event, void (*userFunc)(uint8_t portIndex));
    void unRegisterEvent(uint8_t event);
};

extern USBPD usbpd0;
#if PMGDUINO_BOARD
extern USBPD usbpd1;
#endif

extern void (*usbpd_ebent_handler[APP_TOTAL_EVENTS])(uint8_t portIndex);

#endif