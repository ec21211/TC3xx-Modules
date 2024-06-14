/**
 * \file IfxSdmmc.c
 * \brief SDMMC  basic functionality
 *
 * \version iLLD_1_0_1_12_0_1
 * \copyright Copyright (c) 2019 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees. If and as long as no such terms
 * of use are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxSdmmc.h"
#include "Scu/Std/IfxScuCcu.h"

/******************************************************************************/
/*-----------------------Exported Variables/Constants-------------------------*/
/******************************************************************************/

Ifx_SDMMC_CMD_Bits IfxSdmmc_CMD[IFXSDMMC_NUM_COMMANDS] = {                                                                                                                                                                                                                          /* SD card commands */
    {IfxSdmmc_ResponseLength_none,            IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_disable, IfxSdmmc_CommandIndexCheck_disable, IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD0_GO_IDLE_STATE,            IFXSDMMC_CMD_REG_BIT_RES}, /* 0 */
    {IfxSdmmc_ResponseLength_136,             IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_disable, IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD2_ALL_SEND_CID,             IFXSDMMC_CMD_REG_BIT_RES}, /* 1 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD3_SEND_RELATIVE_ADDR,       IFXSDMMC_CMD_REG_BIT_RES}, /* 2 */
    {IfxSdmmc_ResponseLength_none,            IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_disable, IfxSdmmc_CommandIndexCheck_disable, IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD4_SET_DSR,                  IFXSDMMC_CMD_REG_BIT_RES}, /* 3 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_CMD6_SWITCH_FUNC,              IFXSDMMC_CMD_REG_BIT_RES}, /* 4 */
    {IfxSdmmc_ResponseLength_48WithBusyCheck, IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD7_SELECT_DESELECT_CARD,     IFXSDMMC_CMD_REG_BIT_RES}, /* 5 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD8_SEND_IF_COND,             IFXSDMMC_CMD_REG_BIT_RES}, /* 6 */
    {IfxSdmmc_ResponseLength_136,             IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_disable, IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD9_SEND_CSD,                 IFXSDMMC_CMD_REG_BIT_RES}, /* 7 */
    {IfxSdmmc_ResponseLength_136,             IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_disable, IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD10_SEND_CID,                IFXSDMMC_CMD_REG_BIT_RES}, /* 8 */
    {IfxSdmmc_ResponseLength_48WithBusyCheck, IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD12_STOP_TRANSMISSION,       IFXSDMMC_CMD_REG_BIT_RES}, /* 9 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD13_SEND_STATUS,             IFXSDMMC_CMD_REG_BIT_RES}, /* 10 */
    {IfxSdmmc_ResponseLength_none,            IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_disable, IfxSdmmc_CommandIndexCheck_disable, IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD15_GO_INACTIVE_STATE,       IFXSDMMC_CMD_REG_BIT_RES}, /* 11 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD16_SET_BLOCKLEN,            IFXSDMMC_CMD_REG_BIT_RES}, /* 12 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_CMD17_READ_SINGLE_BLOCK,       IFXSDMMC_CMD_REG_BIT_RES}, /* 13 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_CMD18_READ_MULTIPLE_BLOCK,     IFXSDMMC_CMD_REG_BIT_RES}, /* 14 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_CMD24_WRITE_BLOCK,             IFXSDMMC_CMD_REG_BIT_RES}, /* 15 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_CMD25_WRITE_MULTIPLE_BLOCK,    IFXSDMMC_CMD_REG_BIT_RES}, /* 16 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_CMD27_PROGRAM_CSD,             IFXSDMMC_CMD_REG_BIT_RES}, /* 17 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD28_SET_WRITE_PROT,          IFXSDMMC_CMD_REG_BIT_RES}, /* 18 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD29_CLR_WRITE_PROT,          IFXSDMMC_CMD_REG_BIT_RES}, /* 19 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_CMD30_SEND_WRITE_PROT,         IFXSDMMC_CMD_REG_BIT_RES}, /* 20 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD32_ERASE_WR_BLK_START,      IFXSDMMC_CMD_REG_BIT_RES}, /* 21 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD33_ERASE_WR_BLK_END,        IFXSDMMC_CMD_REG_BIT_RES}, /* 22 */
    {IfxSdmmc_ResponseLength_48WithBusyCheck, IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD38_ERASE,                   IFXSDMMC_CMD_REG_BIT_RES}, /* 23 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_CMD42_LOCK_UNLOCK,             IFXSDMMC_CMD_REG_BIT_RES}, /* 24 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD55_APP_CMD,                 IFXSDMMC_CMD_REG_BIT_RES}, /* 25 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_CMD56_GEN_CMD,                 IFXSDMMC_CMD_REG_BIT_RES}, /* 26 */
    /* application specific commands */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_ACMD6_SET_BUS_WIDTH,           IFXSDMMC_CMD_REG_BIT_RES}, /* 27 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_ACMD13_SD_STATUS,              IFXSDMMC_CMD_REG_BIT_RES}, /* 28 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_ACMD22_SEND_NUM_WR_BLOCKS,     IFXSDMMC_CMD_REG_BIT_RES}, /* 29 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_ACMD23_SET_WR_BLK_ERASE_COUNT, IFXSDMMC_CMD_REG_BIT_RES}, /* 30 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_disable, IfxSdmmc_CommandIndexCheck_disable, IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_ACMD41_SD_SEND_OP_COND,        IFXSDMMC_CMD_REG_BIT_RES}, /* 31 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_ACMD42_SET_CLR_CARD_DETECT,    IFXSDMMC_CMD_REG_BIT_RES}, /* 32 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_ACMD51_SEND_SCR,               IFXSDMMC_CMD_REG_BIT_RES}, /* 33 */
    /* MMC Card specific commands */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_disable, IfxSdmmc_CommandIndexCheck_disable, IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD1_MMC_SEND_OP_COND,         IFXSDMMC_CMD_REG_BIT_RES}, /* 34 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD3_MMC_SET_RELATIVE_ADDR,    IFXSDMMC_CMD_REG_BIT_RES}, /* 35 */
    {IfxSdmmc_ResponseLength_48WithBusyCheck, IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD5_MMC_SLEEP_AWAKE,          IFXSDMMC_CMD_REG_BIT_RES}, /* 36 */
    {IfxSdmmc_ResponseLength_48WithBusyCheck, IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD6_MMC_SWITCH_FUNC,          IFXSDMMC_CMD_REG_BIT_RES}, /* 37 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_CMD8_MMC_SEND_EXT_CSD,         IFXSDMMC_CMD_REG_BIT_RES}, /* 38 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD35_MMC_ERASE_GROUP_START,   IFXSDMMC_CMD_REG_BIT_RES}, /* 39 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandIndexCheck_enable,  IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD36_MMC_ERASE_GROUP_END,     IFXSDMMC_CMD_REG_BIT_RES}, /* 40 */
    /* SDIO Card specific commands */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_disable, IfxSdmmc_CommandCrcCheck_disable,   IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD5_IO_SEND_OP_COND,          IFXSDMMC_CMD_REG_BIT_RES}, /* 41 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandCrcCheck_enable,    IfxSdmmc_DataPresentSelect_noData,   IfxSdmmc_CommandType_normal, IFXSDMMC_CMD52_IO_RW_DIRECT,            IFXSDMMC_CMD_REG_BIT_RES}, /* 42 */
    {IfxSdmmc_ResponseLength_48,              IfxSdmmc_CommandFlag_main, IfxSdmmc_CommandCrcCheck_enable,  IfxSdmmc_CommandCrcCheck_enable,    IfxSdmmc_DataPresentSelect_withData, IfxSdmmc_CommandType_normal, IFXSDMMC_CMD53_IO_RW_EXTENDED,          IFXSDMMC_CMD_REG_BIT_RES}, /* 43 */
};

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxSdmmc_configureClock(Ifx_SDMMC *sdmmcSFR, uint32 frequency)
{
    uint16 divider       = 0;
    uint16 dividerSetVal = 0;
    uint32 sdmmcFreq     = (uint32)IfxScuCcu_getSpbFrequency();
    uint32 maxClockFreq  = sdmmcFreq;

    if (sdmmcSFR->CAPABILITIES2.B.CLK_MUL != 0)
    {
        sdmmcSFR->CLK_CTRL.B.CLK_GEN_SELECT = 1;
    }

    if (sdmmcSFR->CAPABILITIES1.B.BASE_CLK_FREQ != 0)
    {
        /* update maxClockFreq to maximum allowed */
        maxClockFreq = sdmmcSFR->CAPABILITIES1.B.BASE_CLK_FREQ * 1000000;
    }

    if ((frequency != 0) && (frequency <= maxClockFreq))
    {
        divider = (uint8)(sdmmcFreq / (2 * frequency));

        /* write the divider into register now */
        if (divider > (uint8)0)
        {
            dividerSetVal                       = divider - (uint16)1;
            sdmmcSFR->CLK_CTRL.B.FREQ_SEL       = (uint32)((uint8)dividerSetVal);
            sdmmcSFR->CLK_CTRL.B.UPPER_FREQ_SEL = (uint32)(dividerSetVal >> 8);
        }
        else
        {
            sdmmcSFR->CLK_CTRL.B.FREQ_SEL       = (uint32)0;
            sdmmcSFR->CLK_CTRL.B.UPPER_FREQ_SEL = (uint32)0;
        }
    }
    else
    {
        /* Frequency value is either 0 or out of bounds for host */
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }
}


void IfxSdmmc_enableModule(Ifx_SDMMC *sdmmcSFR)
{
    uint16 psw = IfxScuWdt_getCpuWatchdogPassword();

    if (IfxSdmmc_isModuleEnabled(sdmmcSFR) != 1) /* if module is not enabled already */
    {
        IfxScuWdt_clearCpuEndinit(psw);          /* clears the endinit protection*/
        sdmmcSFR->CLC.B.DISR = 0;                /* set the enable request */
        IfxScuWdt_setCpuEndinit(psw);            /* sets the endinit protection back on*/
    }

    IfxSdmmc_isModuleEnabled(sdmmcSFR);          /* read back to ensure proper enabling */
}


IfxSdmmc_Status IfxSdmmc_readResponse(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_Command command, IfxSdmmc_ResponseType responseType, IfxSdmmc_Response *response)
{
    IfxSdmmc_Status status = IfxSdmmc_Status_success;

    /* Check for no-response type commands */
    if ((command == IfxSdmmc_Command_goIdleState) || (command == IfxSdmmc_Command_goInactiveState))
    {
        status = IfxSdmmc_Status_success;
    }
    else
    {
        /* clear the structure for new response */
        response->resp01       = 0;
        response->resp23       = 0;
        response->resp45       = 0;
        response->resp67       = 0;
        response->cardStatus.U = 0;

        /* read first 32 bits of the response, excluding end bit and crc7 */
        response->resp01 = IfxSdmmc_readResponseReg01(sdmmcSFR);

        /* Check response type */
        switch (responseType)
        {
        /* Response R1 */
        case IfxSdmmc_ResponseType_r1:
            response->cardStatus.U = response->resp01;
            /* check the error bits in the response */
            status                 = IfxSdmmc_checkErrorInResponseR1(response->cardStatus.U);
            break;

        /* Response R1b */
        case IfxSdmmc_ResponseType_r1b:
            response->cardStatus.U = response->resp01;
            /* check the error bits in the response */
            status                 = IfxSdmmc_checkErrorInResponseR1(response->cardStatus.U);

            if (status != IfxSdmmc_Status_success)
            {
                /* Wait for transfer complete interrupt */
                //status = IfxSdmmc_transferDelay(sd->sdmmcSFR);
            }

            break;

        /* Response R2: response register R0-R7 */
        case IfxSdmmc_ResponseType_r2:
            response->resp01 = IfxSdmmc_readResponseReg01(sdmmcSFR);
            response->resp23 = IfxSdmmc_readResponseReg23(sdmmcSFR);
            response->resp45 = IfxSdmmc_readResponseReg45(sdmmcSFR);
            response->resp67 = IfxSdmmc_readResponseReg67(sdmmcSFR);
            break;

        /* Responses R3 and R7 */
        case IfxSdmmc_ResponseType_r3:
        case IfxSdmmc_ResponseType_r7:

            break;

        /* IO response: response r4,r5 : SDIO */
        case IfxSdmmc_ResponseType_r4:
            break;

        case IfxSdmmc_ResponseType_r5:
            status = IfxSdmmc_checkerrorInReponseR5(response->resp01);
            break;

        /* Response R6 */
        case IfxSdmmc_ResponseType_r6:
            response->cardStatus.U = response->resp01;
            /* check the error bits in the response */
            status                 = IfxSdmmc_checkErrorInResponseR6(response->cardStatus.U);
            break;

        /* No response */
        case IfxSdmmc_ResponseType_none:
            status = IfxSdmmc_Status_success;
            break;

        default:
            status = IfxSdmmc_Status_success;
            break;
        }
    }

    return status;
}


void IfxSdmmc_resetModule(Ifx_SDMMC *sdmmcSFR)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(passwd);

    sdmmcSFR->KRST0.B.RST = 1;          /* Only if both Kernel reset bits are set a reset is executed */
    sdmmcSFR->KRST1.B.RST = 1;
    IfxScuWdt_setCpuEndinit(passwd);

    while (0 == sdmmcSFR->KRST0.B.RSTSTAT) /* Wait until reset is executed */
    {}

    IfxScuWdt_clearCpuEndinit(passwd);
    sdmmcSFR->KRSTCLR.B.CLR = 1;            /* Clear Kernel reset status bit */

    IfxScuWdt_setCpuEndinit(passwd);
}


IfxSdmmc_Status IfxSdmmc_sendApplicationCommand(Ifx_SDMMC *sdmmcSFR, uint16 rca)
{
    IfxSdmmc_Status   status   = IfxSdmmc_Status_success;
    IfxSdmmc_Response response;
    uint32            argument = 0;

    argument |= (uint32)(rca << 16);

    status    = IfxSdmmc_sendCommand(sdmmcSFR, IfxSdmmc_Command_appCommand, argument, IfxSdmmc_ResponseType_r1, &response);

    return status;
}


IfxSdmmc_Status IfxSdmmc_sendCommand(Ifx_SDMMC *sdmmcSFR, IfxSdmmc_Command command, uint32 argument, IfxSdmmc_ResponseType responseType, IfxSdmmc_Response *response)
{
    IfxSdmmc_Status status  = IfxSdmmc_Status_success;
    uint32          timeout = 0;
    Ifx_SDMMC_CMD   cmd;

    /* check if command and data lines are free */
    timeout = 1000000;

    while ((IfxSdmmc_isDataLineBusy(sdmmcSFR) || (IfxSdmmc_isCommandLineBusy(sdmmcSFR))) && (timeout > 0))
    {
        timeout--;
    }

    if (timeout == 0)
    {
        status = IfxSdmmc_Status_timeout;
    }

    if (status == IfxSdmmc_Status_success)
    {
        IfxSdmmc_setArgument(sdmmcSFR, argument);

        cmd.B   = IfxSdmmc_CMD[command];
        IfxSdmmc_setCommand(sdmmcSFR, cmd.U);
        timeout = 1000000;

        /* wait until command complete flag is set */
        while ((IfxSdmmc_isNormalInterruptOccured(sdmmcSFR, IfxSdmmc_NormalInterrupt_commandComplete) == 0) && (timeout > 0))
        {
            timeout--;
        }

        if (timeout == 0)
        {
            status = IfxSdmmc_Status_commandError;
        }
        else
        {
            IfxSdmmc_clearNormalInterrupt(sdmmcSFR, IfxSdmmc_NormalInterrupt_commandComplete);
            /* read response */
            status = IfxSdmmc_readResponse(sdmmcSFR, command, responseType, response);
        }
    }

    return status;
}


IfxSdmmc_Status IfxSdmmc_setUpInternalClock(Ifx_SDMMC *sdmmcSFR)
{
    IfxSdmmc_Status status  = IfxSdmmc_Status_success;
    uint32          timeout = 0;

    /* enable internal clock */
    IfxSdmmc_enableInternalClock(sdmmcSFR);
    timeout = 1000000;

    while ((IfxSdmmc_isInternalClockStable(sdmmcSFR) == 0) && (timeout > 0))
    {
        timeout--;
    }

    if (timeout == 0)
    {
        status = IfxSdmmc_Status_failure;
    }

    /* enable PLL */
    IfxSdmmc_enablePll(sdmmcSFR);
    timeout = 1000000;

    while ((IfxSdmmc_isInternalClockStable(sdmmcSFR) == 0) && (timeout > 0))
    {
        timeout--;
    }

    if (timeout == 0)
    {
        status = IfxSdmmc_Status_success;
    }

    return status;
}


void IfxSdmmc_switchClockFrequency(Ifx_SDMMC *sdmmcSFR, uint32 frequency)
{
    IfxSdmmc_disableCardClock(sdmmcSFR);          //disable clock

    IfxSdmmc_disablePll(sdmmcSFR);                //disable PLL

    IfxSdmmc_configureClock(sdmmcSFR, frequency); //setup clock

    IfxSdmmc_setUpInternalClock(sdmmcSFR);        //setup PLL and internal clock
}


IfxSdmmc_Status IfxSdmmc_checkerrorInReponseR5(uint32 resp01)
{
    IfxSdmmc_SdIoResponseR5 r5;
    r5.r5 = resp01;
    uint8                   responseFlags = r5.bits.flags;

    return (IfxSdmmc_Status)(responseFlags & IFXSDMMC_IOCARD_R5RESPONSE_ERROR_CHECK_MSK);
}
