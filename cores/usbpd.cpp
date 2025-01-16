#include <usbpd.h>

void (*usbpd_ebent_handler[APP_TOTAL_EVENTS])(uint8_t portIndex) = {NULL};

void srcCapChangeRetryCbk(cy_timer_id_t id, void *ptrContext)
{
    cy_stc_pdstack_context_t *ptrPdStackContext = (cy_stc_pdstack_context_t*) ptrContext;

    Cy_PdUtils_SwTimer_Stop(ptrPdStackContext->ptrTimerContext, GET_USER_TIMER_ID(ptrPdStackContext, SRC_CAP_CHANGE_RETRY_TIMER));

    if(Cy_PdStack_Dpm_SendPdCommand(ptrPdStackContext, CY_PDSTACK_DPM_CMD_SRC_CAP_CHNG, NULL, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
    {
    	Cy_PdUtils_SwTimer_Start(ptrPdStackContext->ptrTimerContext, 
                                 ptrPdStackContext, 
                                 GET_USER_TIMER_ID(ptrPdStackContext, SRC_CAP_CHANGE_RETRY_TIMER), 
                                 SRC_CAP_CHANGE_RETRY_TIMER_PERIOD, 
                                 srcCapChangeRetryCbk);
    }
}

void snkCapChangeRetryCbk(cy_timer_id_t id, void *ptrContext)
{
    cy_stc_pdstack_context_t *ptrPdStackContext = (cy_stc_pdstack_context_t*) ptrContext;

    Cy_PdUtils_SwTimer_Stop(ptrPdStackContext->ptrTimerContext, GET_USER_TIMER_ID(ptrPdStackContext, SNK_CAP_CHANGE_RETRY_TIMER));

    if(Cy_PdStack_Dpm_SendPdCommand(ptrPdStackContext, CY_PDSTACK_DPM_CMD_SNK_CAP_CHNG, NULL, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
    {
    	Cy_PdUtils_SwTimer_Start(ptrPdStackContext->ptrTimerContext, 
                                 ptrPdStackContext, 
                                 GET_USER_TIMER_ID(ptrPdStackContext, SNK_CAP_CHANGE_RETRY_TIMER), 
                                 SNK_CAP_CHANGE_RETRY_TIMER_PERIOD, 
                                 snkCapChangeRetryCbk);
    }
}


void updatePeerSrcPdo(uint8_t port, const cy_stc_pdstack_pd_packet_t* srcCap)
{

    uint8_t numPdo;
    uint8_t i;
    
    if ((numPdo = srcCap->len) != 0)
    {
        for (i=0; i<numPdo; i++)
        {
            if (port == 0)
            {
                usbpd0.iPartnerSrcPdo[i].val = srcCap->dat[i].val;
            }
#if PMGDUINO_BOARD
            else
            {
                usbpd1.iPartnerSrcPdo[i].val = srcCap->dat[i].val;
            }
#endif
        }
    }
    
    if (port == 0)
    {
        usbpd0.iPartnerSrcPdoCnt = numPdo;
    }
#if PMGDUINO_BOARD
    else 
    {
        usbpd1.iPartnerSrcPdoCnt = numPdo;
    }
#endif
}

bool enterVBUSSafe5V(uint8_t port)
{
    uint8_t i;
    uint8_t cnt;

    if (port == 0)
    {
        usbpd0.updateStatus();

        if (usbpd0.powerRole == CY_PD_PRT_ROLE_SINK)  
            cnt = usbpd0.getSnkPdoNum();
#if PMGDUINO_BOARD
        else
            cnt = usbpd0.getSrcPdoNum();
#endif

        if ( usbpd0.contractExisted && 
             (
                (usbpd0.powerRole == CY_PD_PRT_ROLE_SINK && usbpd0.getCurrentSnkPdo() != 0) 
#if PMGDUINO_BOARD
             || (usbpd0.powerRole == CY_PD_PRT_ROLE_SOURCE && usbpd0.getCurrentSrcRdo() != 0)
#endif
             )
           )
        {
            if (usbpd0.powerRole == CY_PD_PRT_ROLE_SINK)
            {
                for (i=1; i<cnt; i++)
                    usbpd0.removeSnkPdo(i);
                usbpd0.updateSnkPdo();
                return true;
            }
#if PMGDUINO_BOARD
            else
            {
                for (i=1; i<cnt; i++)
                    usbpd0.removeSrcPdo(i);
                usbpd0.updateSrcPdo();
                return true;
            }
#endif
        }
    }
#if PMGDUINO_BOARD
    else
    {
        usbpd1.updateStatus();

        if (usbpd1.powerRole == CY_PD_PRT_ROLE_SINK)
            cnt = usbpd1.getSnkPdoNum();
        else
            cnt = usbpd1.getSrcPdoNum();

        if ( usbpd1.contractExisted && 
             (
             (usbpd1.powerRole == CY_PD_PRT_ROLE_SINK && usbpd1.getCurrentSnkPdo() != 0) ||
             (usbpd1.powerRole == CY_PD_PRT_ROLE_SOURCE && usbpd1.getCurrentSrcRdo() != 0)
             )
           )
        {
            if (usbpd1.powerRole == CY_PD_PRT_ROLE_SINK)
            {
                for (i=1; i<cnt; i++)
                    usbpd1.removeSnkPdo(i);
                usbpd1.updateSnkPdo();
                return true;
            }
            else
            {
                for (i=1; i<cnt; i++)
                    usbpd1.removeSrcPdo(i);
                usbpd1.updateSrcPdo();
                return true;
            }
        }
    }
#endif
    return false;
}

static void usbPdCmdCbk(cy_stc_pdstack_context_t *ptrPdStackContext,
					  cy_en_pdstack_resp_status_t resp,
					  const cy_stc_pdstack_pd_packet_t* pkt_ptr)
{
	uint8_t port = ((cy_stc_pdstack_context_t *)ptrPdStackContext)->port;
}

void USBPD::begin(void)
{
    memcpy((void *)iSprSnkPdo, (const void *)ctx->dpmStat.snkPdo, sizeof(cy_pd_pd_do_t)*7);
    iSprSnkMask = ctx->dpmStat.snkPdoMask;
    iSprSnkPdoCnt = ctx->dpmStat.snkPdoCount;
#if PMGDUINO_BOARD
    memcpy((void *)iSprSrcPdo, (const void *)ctx->dpmStat.srcPdo, sizeof(cy_pd_pd_do_t)*7);
    iSprSrcMask = ctx->dpmStat.srcPdoMask;
    iSprSrcPdoCnt = ctx->dpmStat.srcPdoCount;
#endif
    portInitated[portIdx] = true;
}

void USBPD::end(void)
{
    // do nothing
}

void USBPD::updateStatus(void)
{
    attached = ctx->dpmConfig.attach;
    polarity = ctx->dpmConfig.polarity;
    contractExisted = ctx->dpmConfig.connect;
    powerRole = ctx->dpmConfig.curPortRole;
    attachedDeviceType = (uint8_t)ctx->dpmConfig.attachedDev;
}

uint8_t USBPD::getCurrentSrcRdo(void)
{
    uint8_t rdoNum = 0xFF;
    uint8_t pdoType;
    bool    pdoMatched = false;

    pdoType = (ctx->dpmStat.srcSelPdo.val >> 30) & 0x03;

    if ((ctx->dpmConfig.connect) && (ctx->dpmConfig.curPortRole == CY_PD_PRT_ROLE_SOURCE))
    {
        for (uint8_t i=0; i<13; i++)
        {
            switch(pdoType)
            {
                case 0: // fixed pdo
                    if ((ctx->dpmStat.srcSelPdo.val & 0xFFC00) == (ctx->dpmStat.curSrcPdo[i].val & 0xFFC00))
                    {
                        rdoNum = i;
                        pdoMatched = true;
                    }
                    break;
                case 1: // battery
                case 2: // variable
                    if ((ctx->dpmStat.srcSelPdo.val & 0x3FFFFC00) == (ctx->dpmStat.curSrcPdo[i].val & 0x3FFFFC00))
                    {
                        rdoNum = i;
                        pdoMatched = true;
                    }
                    break;
                case 3: // Augmented
                    switch (((ctx->dpmStat.srcSelPdo.val >> 28) & 0x03))
                    {
                        case 0: // SPR programmable
                            if ((ctx->dpmStat.srcSelPdo.val & 0x1FEFF00) == (ctx->dpmStat.curSrcPdo[i].val & 0x1FEFF00))
                            {
                                rdoNum = i;
                                pdoMatched = true;
                            }
                            break;                                            
                        case 1: // EPR AVS
                            if ((ctx->dpmStat.srcSelPdo.val & 0x3FEFF00) == (ctx->dpmStat.curSrcPdo[i].val & 0x3FEFF00))
                            {
                                rdoNum = i;
                                pdoMatched = true;
                            }
                            break;                                            

                        case 2: // SPR AVS
                            if ((ctx->dpmStat.srcSelPdo.val & 0xFFFFF) == (ctx->dpmStat.curSrcPdo[i].val & 0xFFFFF))
                            {
                                rdoNum = i;
                                pdoMatched = true;
                            }
                            break;      
                        default:
                            pdoMatched = true;
                            break;
                    }
                    break;
                default:
                    pdoMatched = true;
                    break;

            }
            
            if (pdoMatched)
                break;
        }
    }

    return rdoNum;
}

uint8_t USBPD::getCurrentSnkPdo(void)
{
    uint8_t pdoNum = 0xFF;
    uint8_t pdoType;
    bool    pdoMatched = false;

    pdoType = (ctx->dpmStat.snkSelPdo.val >> 30) & 0x03;

    if ((ctx->dpmConfig.connect) && (ctx->dpmConfig.curPortRole == CY_PD_PRT_ROLE_SINK))
    {
        for (uint8_t i=0; i<13; i++)
        {
            switch(pdoType)
            {
                case 0: // fixed pdo
                    if ((ctx->dpmStat.snkSelPdo.val & 0xFFC00) == (ctx->dpmStat.curSnkPdo[i].val & 0xFFC00))
                    {
                        pdoNum = i;
                        pdoMatched = true;
                    }
                    break;
                case 1: // battery
                case 2: // variable
                    if ((ctx->dpmStat.snkSelPdo.val & 0x3FFFFC00) == (ctx->dpmStat.curSnkPdo[i].val & 0x3FFFFC00))
                    {
                        pdoNum = i;
                        pdoMatched = true;
                    }
                    break;
                case 3: // Augmented
                    switch (((ctx->dpmStat.snkSelPdo.val >> 28) & 0x03))
                    {
                        case 0: // SPR programmable
                            if ((ctx->dpmStat.snkSelPdo.val & 0x1FEFF00) == (ctx->dpmStat.curSnkPdo[i].val & 0x1FEFF00))
                            {
                                pdoNum = i;
                                pdoMatched = true;
                            }
                            break;                                            
                        case 1: // EPR AVS
                            if ((ctx->dpmStat.snkSelPdo.val & 0x3FEFF00) == (ctx->dpmStat.curSnkPdo[i].val & 0x3FEFF00))
                            {
                                pdoNum = i;
                                pdoMatched = true;
                            }
                            break;                                            

                        case 2: // SPR AVS
                            if ((ctx->dpmStat.snkSelPdo.val & 0xFFFFF) == (ctx->dpmStat.curSnkPdo[i].val & 0xFFFFF))
                            {
                                pdoNum = i;
                                pdoMatched = true;
                            }
                            break;      
                        default:
                            pdoMatched = true;
                            break;
                    }
                    break;
                default:
                    pdoMatched = true;
                    break;

            }

            if (pdoMatched)
                break;
        }
    }

    return pdoNum;
}

#if PMGDUINO_BOARD  
bool USBPD::setSrcPdo(uint8_t pdoNum, src_cap_t srcCap)
{  
    if (pdoNum == 0 || pdoNum > 7)
        return false;

    uint8_t i;
    uint8_t pdoCnt = 0;

    if (pdoNum > 7)
        return false;
    
    memcpy((void *)&iSprSrcPdo[pdoNum], (const void *)&srcCap, sizeof(uint32_t));
    
    iSprSrcMask |= (1 << pdoNum);

    for (i=0; i<7; i++)
    {
        if (iSprSrcMask & (1 << i))
            pdoCnt ++;
    }

    iSprSrcPdoCnt = pdoCnt;

    return true;
}
#endif

bool USBPD::setSnkPdo(uint8_t pdoNum, snk_cap_t snkCap)
{
    uint8_t i;
    uint8_t pdoCnt = 0;

    if (pdoNum == 0 || pdoNum > 7)
        return false;
    
    memcpy((void *)&iSprSnkPdo[pdoNum], (const void*)&snkCap, sizeof(uint32_t));
    
    iSprSnkMask |= (1 << pdoNum);

    for (i=0; i<7; i++)
    {
        if (iSprSnkMask & (1 << i))
            pdoCnt ++;
    }

    iSprSnkPdoCnt = pdoCnt;

    if (iSprSnkPdoCnt > 1)
        iSprSnkPdo[0].fixed_snk_do_t.highCap = 1;

    return true;
}

#if PMGDUINO_BOARD
bool USBPD::addFixedSrcPdo(uint8_t pdoNum, uint32_t voltage, uint32_t maxCurrent, uint8_t peakCurrent)
{
    src_cap_t srcCap;

    srcCap.val = 0;
    srcCap.fixed_src_do_t.voltage = voltage / 50;
    srcCap.fixed_src_do_t.maxCurrent = maxCurrent / 10;
    srcCap.fixed_src_do_t.pkCurrent = peakCurrent;
    srcCap.fixed_src_do_t.supplyType = 0;

    return (setSrcPdo(pdoNum, srcCap));
}

void USBPD::setSrcUSBCommFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.usbCommCap = enable ? 1 : 0;
}

void USBPD::setSrcDualDataRoleFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.drSwap = enable ? 1 : 0;
}

void USBPD::setSrcDualPowerModeFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.dualRolePower = enable ? 1 : 0;
}

void USBPD::setSrcUnconstrainedPowerFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.extPowered = enable ? 1 : 0;
    iSprSrcMask &= 0x7F;
    iSprSrcMask |= enable ? 0x80 : 0x00;
}

#if CY_PD_EPR_ENABLE

#if 0
void exitSrcEprRetryCbk(cy_timer_id_t id, void *ptrContext)
{
    cy_stc_pdstack_context_t *ptrPdStackContext = (cy_stc_pdstack_context_t*) ptrContext;
    cy_stc_pdstack_dpm_pd_cmd_buf_t cmdBuf;
    cy_pd_pd_do_t cmdDo;

    Cy_PdUtils_SwTimer_Stop(ptrPdStackContext->ptrTimerContext, GET_USER_TIMER_ID(ptrPdStackContext, EPR_EXIT_MODE_RETRY_TIMER));
                
    memset((void *)&cmdBuf, 0, sizeof(cy_stc_pdstack_dpm_pd_cmd_buf_t));
    memset((void *)&cmdDo, 0, sizeof(cy_pd_pd_do_t));
    cmdDo.val = ((0x05) << 24);
    cmdBuf.cmdSop = CY_PD_SOP;
    cmdBuf.noOfCmdDo = 1;
    cmdBuf.cmdDo[0] = cmdDo;
    if (Cy_PdStack_Dpm_SendPdCommand(ptrPdStackContext, CY_PDSTACK_DPM_CMD_SEND_EPR_MODE, &cmdBuf, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
    {
    	Cy_PdUtils_SwTimer_Start(ptrPdStackContext->ptrTimerContext, ptrPdStackContext,
    							 GET_USER_TIMER_ID(ptrPdStackContext, EPR_EXIT_MODE_RETRY_TIMER), 
                                 EPR_EXIT_MODE_RETRY_TIMER_PERIOD, 
                                 exitSrcEprRetryCbk);                       
    }
    
}
#endif

void USBPD::enableSrcEprPdo(bool enable)
{
    if (enable)
    {
        ctx->dpmExtStat.epr.srcPdoMask = 0x03;
    }
    else
    {
        ctx->dpmExtStat.epr.srcPdoMask = 0x00;
    }
}

void USBPD::setSrcEPRFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.eprModeCapable = enable ? 1 : 0;

    if (enable)
    {
        ctx->dpmExtStat.epr.srcPdoMask = 0x03;
    }
    else
    {
        ctx->dpmExtStat.epr.srcPdoMask = 0x00;
#if 0
        if ((true == ctx->dpmExtStat.eprActive))
        {
            cy_stc_pdstack_dpm_pd_cmd_buf_t cmdBuf;
            cy_pd_pd_do_t cmdDo;
                
            memset((void *)&cmdBuf, 0, sizeof(cy_stc_pdstack_dpm_pd_cmd_buf_t));
            memset((void *)&cmdDo, 0, sizeof(cy_pd_pd_do_t));
            cmdDo.val = ((0x05) << 24);
            cmdBuf.cmdSop = CY_PD_SOP;
            cmdBuf.noOfCmdDo = 1;
            cmdBuf.cmdDo[0] = cmdDo;
            if (Cy_PdStack_Dpm_SendPdCommand(ctx, CY_PDSTACK_DPM_CMD_SEND_EPR_MODE, &cmdBuf, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
            {
    			Cy_PdUtils_SwTimer_Start(ctx->ptrTimerContext, ctx,
    									 GET_USER_TIMER_ID(ctx, EPR_EXIT_MODE_RETRY_TIMER), 
                                         EPR_EXIT_MODE_RETRY_TIMER_PERIOD, 
                                         exitSrcEprRetryCbk);                       
            }
        }
#endif
    }
}
#endif

void USBPD::setUnchunkedExtMsgFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.unchunkSup = enable ? 1 : 0;
}

void USBPD::setSrcUSBSuspendCapFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.usbSuspendSup = enable ? 1 : 0;
}

void USBPD::setSrcPeakCurrent(uint8_t peakCurrent)
{
    if (peakCurrent > 3)
        return;
    iSprSrcPdo[0].fixed_src_do_t.pkCurrent = peakCurrent;
}

bool USBPD::addVarSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxCurrent)
{
    src_cap_t srcCap;

    srcCap.val = 0;
    srcCap.var_src_do_t.maxCurrent = maxCurrent / 10;
    srcCap.var_src_do_t.maxVoltage = maxVoltage / 50;
    srcCap.var_src_do_t.minVoltage = minVoltage / 50;
    srcCap.var_src_do_t.supplyType = 2;

    return (setSrcPdo(pdoNum, srcCap));
}

bool USBPD::addBatSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxPower)
{
    src_cap_t srcCap;

    srcCap.val = 0;
    srcCap.bat_src_do_t.maxPower = maxPower / 250;
    srcCap.bat_src_do_t.maxVoltage = maxVoltage / 50;
    srcCap.bat_src_do_t.minVoltage = minVoltage / 50;
    srcCap.bat_src_do_t.supplyType = 1;

    return (setSrcPdo(pdoNum, srcCap));
}

bool USBPD::addAugmentedSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxCurrent)
{
    src_cap_t srcCap;

    srcCap.val = 0;
    srcCap.src_gen_do_t.maxCurPower = maxCurrent / 50;
    srcCap.src_gen_do_t.maxVoltage = maxVoltage / 100;
    srcCap.src_gen_do_t.minVoltage = minVoltage / 100;
    srcCap.src_gen_do_t.supplyType = 3;

    return (setSrcPdo(pdoNum, srcCap));
}

bool USBPD::removeSrcPdo(uint8_t pdoNum)
{
    if (pdoNum == 0 || pdoNum > 7)
        return false;

    iSprSrcMask &= ~(1 << pdoNum);

    if (iSprSrcPdoCnt > 1)
        iSprSrcPdoCnt --;

    iSprSrcPdo[pdoNum].val = 0;

    return true;
}

uint8_t USBPD::getSrcPdoNum(void)
{
    return iSprSrcPdoCnt;
}
#endif

void USBPD::setSnkDualDataRoleFlag(bool enable)
{
    iSprSnkPdo[0].fixed_snk_do_t.drSwap = enable ? 1 : 0;
}

void USBPD::setSnkUnconstrainedPowerFlag(bool enable)
{
    iSprSnkPdo[0].fixed_snk_do_t.extPowered = enable ? 1 : 0;
}

void USBPD::setSnkUSBCommFlag(bool enable)
{
    iSprSnkPdo[0].fixed_snk_do_t.usbCommCap = enable ? 1 : 0;
}

void USBPD::setSnkDualPowerModeFlag(bool enable)
{
    iSprSnkPdo[0].fixed_snk_do_t.dualRolePower = enable ? 1 : 0;
}

bool USBPD::addFixedSnkPdo(uint8_t pdoNum, uint32_t voltage, uint32_t opCurrent)
{
    snk_cap_t snkCap;

    snkCap.val = 0;
    snkCap.fixed_snk_do_t.voltage = voltage / 50;
    snkCap.fixed_snk_do_t.opCurrent = opCurrent / 10;
    snkCap.fixed_snk_do_t.supplyType = 0;

    return (setSnkPdo(pdoNum, snkCap));
}

bool USBPD::addVarSnkPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t opCurrent)
{
    snk_cap_t snkCap;

    snkCap.val = 0;
    snkCap.var_snk_do_t.opCurrent = opCurrent / 10;
    snkCap.var_snk_do_t.maxVoltage = maxVoltage / 50;
    snkCap.var_snk_do_t.minVoltage = minVoltage / 50;
    snkCap.var_snk_do_t.supplyType = 2;

    return (setSnkPdo(pdoNum, snkCap));
}

bool USBPD::addBatSnkPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t opPower)
{
    snk_cap_t snkCap;

    snkCap.val = 0;
    snkCap.bat_snk_do_t.opPower = opPower / 250;
    snkCap.bat_snk_do_t.maxVoltage = maxVoltage / 50;
    snkCap.bat_snk_do_t.minVoltage = minVoltage / 50;
    snkCap.bat_snk_do_t.supplyType = 1;

    return (setSnkPdo(pdoNum, snkCap));
}

bool USBPD::removeSnkPdo(uint8_t pdoNum)
{
    if (pdoNum == 0 || pdoNum > 7)
        return false;

    iSprSnkMask &= ~(1 << pdoNum);

    if (iSprSnkPdoCnt > 1)
        iSprSnkPdoCnt --;

    if (iSprSnkPdoCnt == 1)
         iSprSnkPdo[0].fixed_snk_do_t.highCap = 0;

    iSprSnkPdo[pdoNum].val = 0;

    return true;
}

uint8_t USBPD::getSnkPdoNum(void)
{
    return iSprSnkPdoCnt;
}

bool USBPD::updateSrcPdo(void)
{

#if PMGDUINO_BOARD
    cy_en_pdstack_status_t status;
    cy_pd_pd_do_t srcPdo[7];

    if (portInitated[portIdx] == false)
        return false;

    for (uint8_t i=0; i<iSprSrcPdoCnt; i++)
        srcPdo[i].val = iSprSrcPdo[i].val;

#if CY_PD_EPR_ENABLE
    if (iSprSrcPdo[0].fixed_src_do_t.eprModeCapable)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_0_EPR_MODE_CAP;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_0_EPR_MODE_CAP;
#endif
    if (iSprSrcPdo[0].fixed_src_do_t.unchunkSup)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_0_UNCHUNKED_SUPP;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_0_UNCHUNKED_SUPP;

    if (iSprSrcPdo[0].fixed_src_do_t.drSwap)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_0_DUAL_ROLE_DATA;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_0_DUAL_ROLE_DATA;

    if (iSprSrcPdo[0].fixed_src_do_t.usbCommCap)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_0_USB_COMM;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_0_USB_COMM;

    if (iSprSrcPdo[0].fixed_src_do_t.extPowered)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_0_UNCONSTRAINED_POW;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_0_UNCONSTRAINED_POW;

    if (iSprSrcPdo[0].fixed_src_do_t.usbSuspendSup)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_1_USB_SUSP_SUPP;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_1_USB_SUSP_SUPP;

    if (iSprSrcPdo[0].fixed_src_do_t.dualRolePower)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_1_DUAL_ROLE_POWER;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_1_DUAL_ROLE_POWER;

    ctx->dpmStat.srcPdoFlags[1] = (iSprSrcPdo[0].fixed_src_do_t.pkCurrent & 0x03);

    // optional, called if the PDO content changes
    status = Cy_PdStack_Dpm_UpdateSrcCap(ctx, iSprSrcPdoCnt, srcPdo);
    if (status == CY_PDSTACK_STAT_SUCCESS)
    {
        // optional, called if the PDO mask changes
        status = Cy_PdStack_Dpm_UpdateSrcCapMask(ctx, iSprSrcMask);
        if (status == CY_PDSTACK_STAT_SUCCESS)
        {
    		if ((ctx->dpmConfig.connect) && (ctx->dpmConfig.curPortRole == CY_PD_PRT_ROLE_SOURCE))
    		{
                // optional, set a timer for the callback to handle the command sending failure condition
    			if(Cy_PdStack_Dpm_SendPdCommand(ctx, CY_PDSTACK_DPM_CMD_SRC_CAP_CHNG, NULL, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
    			{
    				Cy_PdUtils_SwTimer_Start(ctx->ptrTimerContext, ctx,
    										GET_USER_TIMER_ID(ctx, SRC_CAP_CHANGE_RETRY_TIMER), 
                                            SRC_CAP_CHANGE_RETRY_TIMER_PERIOD, 
                                            srcCapChangeRetryCbk);
    			}
    		}
        }

    }
#endif

    return true;
}

bool USBPD::updateSnkPdo(void)
{
    cy_en_pdstack_status_t status;
    cy_pd_pd_do_t snkPdo[7];

    if (portInitated[portIdx] == false)
        return false;
        
    for (uint8_t i=0; i<iSprSnkPdoCnt; i++)
        snkPdo[i].val = iSprSnkPdo[i].val;

    if (iSprSnkPdo[0].fixed_snk_do_t.drSwap)
        ctx->dpmStat.snkPdoFlags[0] |= CY_PD_SNKCAP_FLAGS_0_DUAL_ROLE_DATA;
    else
        ctx->dpmStat.snkPdoFlags[0] &= ~CY_PD_SNKCAP_FLAGS_0_DUAL_ROLE_DATA;
    
    if (iSprSnkPdo[0].fixed_snk_do_t.usbCommCap)
        ctx->dpmStat.snkPdoFlags[0] |= CY_PD_SNKCAP_FLAGS_0_USB_COMM;
    else
        ctx->dpmStat.snkPdoFlags[0] &= ~CY_PD_SNKCAP_FLAGS_0_USB_COMM;

    if (iSprSnkPdo[0].fixed_snk_do_t.extPowered)
        ctx->dpmStat.snkPdoFlags[0] |= CY_PD_SNKCAP_FLAGS_0_UNCONSTRAINED_POW;
    else
        ctx->dpmStat.snkPdoFlags[0] &= ~CY_PD_SNKCAP_FLAGS_0_UNCONSTRAINED_POW;

    if (iSprSnkPdo[0].fixed_snk_do_t.highCap)
        ctx->dpmStat.snkPdoFlags[1] |= CY_PD_SNKCAP_FLAGS_1_HIGH_CAP;
    else
        ctx->dpmStat.snkPdoFlags[1] &= ~CY_PD_SNKCAP_FLAGS_1_HIGH_CAP;

    if (iSprSnkPdo[0].fixed_snk_do_t.dualRolePower)
        ctx->dpmStat.snkPdoFlags[1] |= CY_PD_SNKCAP_FLAGS_1_DUAL_ROLE_POWER;
    else
        ctx->dpmStat.snkPdoFlags[1] &= ~CY_PD_SNKCAP_FLAGS_1_DUAL_ROLE_POWER;

    status = Cy_PdStack_Dpm_UpdateSnkCap(ctx, iSprSnkPdoCnt, snkPdo);
    if (status == CY_PDSTACK_STAT_SUCCESS)
    {
        Cy_PdStack_Dpm_UpdateSnkCapMask(ctx, iSprSnkMask);
        if (status == CY_PDSTACK_STAT_SUCCESS)
        {
    		if ((ctx->dpmConfig.connect) && (ctx->dpmConfig.curPortRole == CY_PD_PRT_ROLE_SINK))
    		{
    			if(Cy_PdStack_Dpm_SendPdCommand(ctx, CY_PDSTACK_DPM_CMD_SNK_CAP_CHNG, NULL, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
    			{
    				Cy_PdUtils_SwTimer_Start(ctx->ptrTimerContext, ctx,
    										GET_USER_TIMER_ID(ctx, SNK_CAP_CHANGE_RETRY_TIMER), 
                                            SNK_CAP_CHANGE_RETRY_TIMER_PERIOD, 
                                            snkCapChangeRetryCbk);
    			}
    		}
        }
    }

    return true;
}

#if CY_PD_EPR_ENABLE
bool USBPD::getEprModeActive(void)
{
    return (ctx->dpmExtStat.eprActive);
}

void USBPD::enableSnkEprPdo(bool enable)
{
    if (enable)
        ctx->dpmExtStat.epr.snkPdoMask = 0x01;
    else
        ctx->dpmExtStat.epr.snkPdoMask = 0x00;
}

#if 0
void enterEprRetryCbk(cy_timer_id_t id, void *ptrContext)
{
    cy_stc_pdstack_context_t *ptrPdStackContext = (cy_stc_pdstack_context_t*) ptrContext;

    Cy_PdUtils_SwTimer_Stop(ptrPdStackContext->ptrTimerContext, GET_USER_TIMER_ID(ptrPdStackContext, EPR_ENTER_MODE_RETRY_TIMER));

    if(Cy_PdStack_Dpm_SendPdCommand(ptrPdStackContext, CY_PDSTACK_DPM_CMD_SNK_EPR_MODE_ENTRY, NULL, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
    {
    	Cy_PdUtils_SwTimer_Start(ptrPdStackContext->ptrTimerContext, 
                                 ptrPdStackContext, 
                                 GET_USER_TIMER_ID(ptrPdStackContext, EPR_ENTER_MODE_RETRY_TIMER), 
                                 EPR_ENTER_MODE_RETRY_TIMER_PERIOD, 
                                 enterEprRetryCbk);
    }
   
}

void exitEprRetryCbk(cy_timer_id_t id, void *ptrContext)
{
    cy_stc_pdstack_context_t *ptrPdStackContext = (cy_stc_pdstack_context_t*) ptrContext;
    cy_stc_pdstack_dpm_pd_cmd_buf_t cmdBuf;
    cy_pd_pd_do_t cmdDo;

    Cy_PdUtils_SwTimer_Stop(ptrPdStackContext->ptrTimerContext, GET_USER_TIMER_ID(ptrPdStackContext, EPR_EXIT_MODE_RETRY_TIMER));
                
    memset((void *)&cmdBuf, 0, sizeof(cy_stc_pdstack_dpm_pd_cmd_buf_t));
    memset((void *)&cmdDo, 0, sizeof(cy_pd_pd_do_t));
    cmdDo.val = ((0x05) << 24);
    cmdBuf.cmdSop = CY_PD_SOP;
    cmdBuf.noOfCmdDo = 1;
    cmdBuf.cmdDo[0] = cmdDo;
    if (Cy_PdStack_Dpm_SendPdCommand(ptrPdStackContext, CY_PDSTACK_DPM_CMD_SEND_EPR_MODE, &cmdBuf, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
    {
    	Cy_PdUtils_SwTimer_Start(ptrPdStackContext->ptrTimerContext, ptrPdStackContext,
    							 GET_USER_TIMER_ID(ptrPdStackContext, EPR_EXIT_MODE_RETRY_TIMER), 
                                 EPR_EXIT_MODE_RETRY_TIMER_PERIOD, 
                                 exitEprRetryCbk);                       
    }
    
}

void USBPD::enableSnkEprMode(bool enable)
{
    if (portInitated[portIdx] == false)
        return;

    ctx->dpmExtStat.epr.snkEnable = enable;

    if ((ctx->dpmConfig.curPortRole == CY_PD_PRT_ROLE_SINK) && ctx->dpmConfig.connect)
    {
        if (enable)
        {
            
            if ((false == ctx->dpmExtStat.eprActive) &&
                (usbpd0.iPartnerSrcPdo[0].fixed_src_do_t.eprModeCapable == true) &&
                (ctx->dpmExtStat.epr.snkEnable == true))
            {

                if (Cy_PdStack_Dpm_SendPdCommand(ctx, CY_PDSTACK_DPM_CMD_SNK_EPR_MODE_ENTRY, NULL, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
                {
    				Cy_PdUtils_SwTimer_Start(ctx->ptrTimerContext, ctx,
    										 GET_USER_TIMER_ID(ctx, EPR_ENTER_MODE_RETRY_TIMER), 
                                             EPR_ENTER_MODE_RETRY_TIMER_PERIOD, 
                                             enterEprRetryCbk);                    
                }
            }
        }
        else
        {
            if ((true == ctx->dpmExtStat.eprActive) &&
                (ctx->dpmExtStat.epr.snkEnable == false))
            {
                cy_stc_pdstack_dpm_pd_cmd_buf_t cmdBuf;
                cy_pd_pd_do_t cmdDo;
                
                memset((void *)&cmdBuf, 0, sizeof(cy_stc_pdstack_dpm_pd_cmd_buf_t));
                memset((void *)&cmdDo, 0, sizeof(cy_pd_pd_do_t));
                cmdDo.val = ((0x05) << 24);
                cmdBuf.cmdSop = CY_PD_SOP;
                cmdBuf.noOfCmdDo = 1;
                cmdBuf.cmdDo[0] = cmdDo;
                if (Cy_PdStack_Dpm_SendPdCommand(ctx, CY_PDSTACK_DPM_CMD_SEND_EPR_MODE, &cmdBuf, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
                {
    				Cy_PdUtils_SwTimer_Start(ctx->ptrTimerContext, ctx,
    										 GET_USER_TIMER_ID(ctx, EPR_EXIT_MODE_RETRY_TIMER), 
                                             EPR_EXIT_MODE_RETRY_TIMER_PERIOD, 
                                             exitEprRetryCbk);                       
                }
            }

        }
    }
}
#endif
#endif

void getPartnerPortSrcPdo_cb(cy_stc_pdstack_context* ptrPdStackContext, cy_en_pdstack_resp_status_t resp, const cy_stc_pdstack_pd_packet_t *respDat);

void getPartnerSrcPdoRetryCbk(cy_timer_id_t id, void *ptrContext)
{
    cy_stc_pdstack_context_t *ptrPdStackContext = (cy_stc_pdstack_context_t*) ptrContext;

    Cy_PdUtils_SwTimer_Stop(ptrPdStackContext->ptrTimerContext, GET_USER_TIMER_ID(ptrPdStackContext, GET_PARTNER_PORT_SRC_PDO_TIMER));

    if(Cy_PdStack_Dpm_SendPdCommand(ptrPdStackContext, CY_PDSTACK_DPM_CMD_GET_SRC_CAP, NULL, false, getPartnerPortSrcPdo_cb) != CY_PDSTACK_STAT_SUCCESS)
    {
    	Cy_PdUtils_SwTimer_Start(ptrPdStackContext->ptrTimerContext, 
                                 ptrPdStackContext, 
                                 GET_USER_TIMER_ID(ptrPdStackContext, GET_PARTNER_PORT_SRC_PDO_TIMER), 
                                 GET_PARTNER_PORT_SRC_PDO_TIMER_PERIOD, 
                                 getPartnerSrcPdoRetryCbk);
    }
}

void getPartnerPortSrcPdo_cb(cy_stc_pdstack_context* ptrPdStackContext, cy_en_pdstack_resp_status_t resp, const cy_stc_pdstack_pd_packet_t *respDat)
{
    if (resp == CY_PDSTACK_RES_RCVD)
    {
        if (ptrPdStackContext->port == 0)
        {
            usbpd0.iPartnerSrcPdoCnt = respDat->len;
            for (uint8_t i=0; i<respDat->len; i++)
                usbpd0.iPartnerSrcPdo[i].val = respDat->dat[i].val;
            usbpd0.iPartnerSrcPdoUpdated = true;
        }
#if PMGDUINO_BOARD        
        else
        {
            usbpd1.iPartnerSrcPdoCnt = respDat->len;
            for (uint8_t i=0; i<respDat->len; i++)
                usbpd1.iPartnerSrcPdo[i].val = respDat->dat[i].val;
            usbpd1.iPartnerSrcPdoUpdated = true;
        }
#endif
    }

    if ((resp == CY_PDSTACK_CMD_FAILED) || (resp == CY_PDSTACK_RES_TIMEOUT))
    {
        if(Cy_PdStack_Dpm_SendPdCommand(ptrPdStackContext, CY_PDSTACK_DPM_CMD_GET_SRC_CAP, NULL, false, getPartnerPortSrcPdo_cb) != CY_PDSTACK_STAT_SUCCESS)
        {
            Cy_PdUtils_SwTimer_Start(ptrPdStackContext->ptrTimerContext, 
                                    ptrPdStackContext, 
                                    GET_USER_TIMER_ID(ptrPdStackContext, GET_PARTNER_PORT_SRC_PDO_TIMER), 
                                    GET_PARTNER_PORT_SRC_PDO_TIMER_PERIOD, 
                                    getPartnerSrcPdoRetryCbk);
        }
    }
}

void USBPD::getPartnerPortSrcPdo(void)
{
    iPartnerSrcPdoUpdated = false;
    if ((ctx->dpmConfig.connect) && portInitated[portIdx])
    {
        if (Cy_PdStack_Dpm_SendPdCommand(ctx, CY_PDSTACK_DPM_CMD_GET_SRC_CAP, NULL, false, getPartnerPortSrcPdo_cb) != CY_PDSTACK_STAT_SUCCESS)
        {
    		Cy_PdUtils_SwTimer_Start(   ctx->ptrTimerContext, ctx,
    								    GET_USER_TIMER_ID(ctx, GET_PARTNER_PORT_SRC_PDO_TIMER), 
                                        GET_PARTNER_PORT_SRC_PDO_TIMER_PERIOD, 
                                        getPartnerSrcPdoRetryCbk);            
        }
    }
}

void getPartnerPortSnkPdo_cb(cy_stc_pdstack_context* ptrPdStackContext, cy_en_pdstack_resp_status_t resp, const cy_stc_pdstack_pd_packet_t *respDat);

void getPartnerSnkPdoRetryCbk(cy_timer_id_t id, void *ptrContext)
{
    cy_stc_pdstack_context_t *ptrPdStackContext = (cy_stc_pdstack_context_t*) ptrContext;

    Cy_PdUtils_SwTimer_Stop(ptrPdStackContext->ptrTimerContext, GET_USER_TIMER_ID(ptrPdStackContext, GET_PARTNER_PORT_SNK_PDO_TIMER));

    if(Cy_PdStack_Dpm_SendPdCommand(ptrPdStackContext, CY_PDSTACK_DPM_CMD_GET_SNK_CAP, NULL, false, getPartnerPortSnkPdo_cb) != CY_PDSTACK_STAT_SUCCESS)
    {
    	Cy_PdUtils_SwTimer_Start(ptrPdStackContext->ptrTimerContext, 
                                 ptrPdStackContext, 
                                 GET_USER_TIMER_ID(ptrPdStackContext, GET_PARTNER_PORT_SNK_PDO_TIMER), 
                                 GET_PARTNER_PORT_SNK_PDO_TIMER_PERIOD, 
                                 getPartnerSnkPdoRetryCbk);
    }
}

void getPartnerPortSnkPdo_cb(cy_stc_pdstack_context* ptrPdStackContext, cy_en_pdstack_resp_status_t resp, const cy_stc_pdstack_pd_packet_t *respDat)
{
    if (resp == CY_PDSTACK_RES_RCVD)
    {
        if (ptrPdStackContext->port == 0)
        {
            usbpd0.iPartnerSnkPdoCnt = respDat->len;
            for (uint8_t i=0; i<respDat->len; i++)
                usbpd0.iPartnerSnkPdo[i].val = respDat->dat[i].val;
            usbpd0.iPartnerSnkPdoUpdated = true;
        }
#if PMGDUINO_BOARD        
        else
        {
            usbpd1.iPartnerSnkPdoCnt = respDat->len;
            for (uint8_t i=0; i<respDat->len; i++)
                usbpd1.iPartnerSnkPdo[i].val = respDat->dat[i].val;
            usbpd1.iPartnerSnkPdoUpdated = true;
        }
#endif
    }
    
    if ((resp == CY_PDSTACK_CMD_FAILED) || (resp == CY_PDSTACK_RES_TIMEOUT))
    {
        if(Cy_PdStack_Dpm_SendPdCommand(ptrPdStackContext, CY_PDSTACK_DPM_CMD_GET_SNK_CAP, NULL, false, getPartnerPortSnkPdo_cb) != CY_PDSTACK_STAT_SUCCESS)
        {
            Cy_PdUtils_SwTimer_Start(ptrPdStackContext->ptrTimerContext, 
                                    ptrPdStackContext, 
                                    GET_USER_TIMER_ID(ptrPdStackContext, GET_PARTNER_PORT_SNK_PDO_TIMER), 
                                    GET_PARTNER_PORT_SNK_PDO_TIMER_PERIOD, 
                                    getPartnerSnkPdoRetryCbk);
        }
    }    
}


void USBPD::getPartnerPortSnkPdo(void)
{
    iPartnerSnkPdoUpdated = false;
    if ((ctx->dpmConfig.connect) && portInitated[portIdx])
    {
        if (Cy_PdStack_Dpm_SendPdCommand(ctx, CY_PDSTACK_DPM_CMD_GET_SNK_CAP, NULL, false, getPartnerPortSnkPdo_cb) != CY_PDSTACK_STAT_SUCCESS)
        {
            Cy_PdUtils_SwTimer_Start(ctx->ptrTimerContext, 
                                    ctx, 
                                    GET_USER_TIMER_ID(ctx, GET_PARTNER_PORT_SNK_PDO_TIMER), 
                                    GET_PARTNER_PORT_SNK_PDO_TIMER_PERIOD, 
                                    getPartnerSnkPdoRetryCbk);            
        }
    }
}

void doPwrRoleSwapRetryCbk(cy_timer_id_t id, void *ptrContext)
{
    cy_stc_pdstack_context_t *ptrPdStackContext = (cy_stc_pdstack_context_t*) ptrContext;

    Cy_PdUtils_SwTimer_Stop(ptrPdStackContext->ptrTimerContext, GET_USER_TIMER_ID(ptrPdStackContext, POWER_ROLE_SWAP_RETRY_TIMER));
    
    if(Cy_PdStack_Dpm_SendPdCommand(ptrPdStackContext, CY_PDSTACK_DPM_CMD_SEND_PR_SWAP, NULL, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
    {
    	Cy_PdUtils_SwTimer_Start(ptrPdStackContext->ptrTimerContext, 
                                 ptrPdStackContext, 
                                 GET_USER_TIMER_ID(ptrPdStackContext, POWER_ROLE_SWAP_RETRY_TIMER), 
                                 POWER_ROLE_SWAP_RETRY_TIMER_PERIOD, 
                                 doPwrRoleSwapRetryCbk);
    }
}


void USBPD::doPwrRoleSwap(void)
{
    if ((ctx->dpmConfig.connect) && portInitated[portIdx])
    {
        if (Cy_PdStack_Dpm_SendPdCommand(ctx, CY_PDSTACK_DPM_CMD_SEND_PR_SWAP, NULL, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
        {
            Cy_PdUtils_SwTimer_Start(ctx->ptrTimerContext, 
                                    ctx, 
                                    GET_USER_TIMER_ID(ctx, POWER_ROLE_SWAP_RETRY_TIMER), 
                                    POWER_ROLE_SWAP_RETRY_TIMER_PERIOD, 
                                    doPwrRoleSwapRetryCbk);
        }
    }
}

void USBPD::registerEvent(uint8_t event, void (*userFunc)(uint8_t portIndex))
{
    usbpd_ebent_handler[event] = userFunc;
}

void USBPD::unRegisterEvent(uint8_t event)
{
    usbpd_ebent_handler[event] = NULL;
}

USBPD usbpd0 = USBPD(0);
#if PMGDUINO_BOARD
USBPD usbpd1 = USBPD(1);
#endif
