/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "external_flash_config.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    #if defined(__CC_ARM) || defined(__GNUC__)
        __attribute__((section(".boot_hdr.conf")))
    #elif defined(__ICCARM__)
        #pragma location = ".boot_hdr.conf"
    #endif
        
        //QSPI Config
        const flexspi_nor_config_t xip_config = {
            .memConfig =
            {
                .tag = FLEXSPI_CFG_BLK_TAG,
                .version = FLEXSPI_CFG_BLK_VERSION,
                .readSampleClkSrc = kFlexSPIReadSampleClk_LoopbackFromDqsPad,
                .csHoldTime = 3u,
                .csSetupTime = 3u,
                
                .deviceModeCfgEnable = true,
                .deviceModeType = 1, //Quad enable command
                .deviceModeSeq.seqNum = 1,
                .deviceModeSeq.seqId = 4,
                .deviceModeArg = 0x000200,  //Set QE
                .deviceType = kFlexSpiDeviceType_SerialNOR,
                
                .sflashPadType = kSerialFlash_4Pads,
                .serialClkFreq = kFlexSpiSerialClk_166MHz,   // 80Mhz for winbond, 100Mhz for GD, 133Mhz for ISSI
                .sflashA1Size = 32u * 1024u * 1024u,
                .dataValidTime = {16u, 16u},
                
                .lookupTable =
                {
                    //Quad Input/Output read sequence
                    [0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xEB, RADDR_SDR, FLEXSPI_4PAD, 0x18),
                    [1] = FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_4PAD, 0x06, READ_SDR, FLEXSPI_4PAD, 0x04),
                    [2] = FLEXSPI_LUT_SEQ(0, 0, 0, 0, 0, 0),
                    
                    //Read Status
                    [1*4] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x05, READ_SDR, FLEXSPI_1PAD, 0x04),
                    
                    //Write Enable
                    [3*4] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x06, STOP, FLEXSPI_1PAD, 0),
                    
                    //Write Status
                    [4*4] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x01, WRITE_SDR, FLEXSPI_1PAD, 0x04),
                },
            },
            .pageSize = 256u,                   //Page: 256K
            .sectorSize = 4u * 1024u,           //Sector: 4K
            .blockSize = 64u * 1024u,           //Block: 64K
        };
        
        /*
        //HyperFlash Config
        const flexspi_nor_config_t xip_config =
        {
            .memConfig =
            {
                .tag = FLEXSPI_CFG_BLK_TAG,
                .version = FLEXSPI_CFG_BLK_VERSION,
                .readSampleClkSrc = kFlexSPIReadSampleClk_ExternalInputFromDqsPad,
                .csHoldTime = 3u,
                .csSetupTime = 3u,
                .columnAddressWidth = 3u,
                // Enable DDR mode, Wordaddassable, Safe configuration, Differential clock
                .controllerMiscOption = (1u << kFlexSpiMiscOffset_DdrModeEnable) |
                                        (1u << kFlexSpiMiscOffset_WordAddressableEnable) |
                                        (1u << kFlexSpiMiscOffset_SafeConfigFreqEnable) |
                                        (1u << kFlexSpiMiscOffset_DiffClkEnable),
                .sflashPadType = kSerialFlash_8Pads,
                .serialClkFreq = kFlexSpiSerialClk_133MHz,
                .sflashA1Size = 64u * 1024u * 1024u,
                .dataValidTime = {16u, 16u},
                .lookupTable =
                {
                    // Read LUTs
                    FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0xA0, RADDR_DDR, FLEXSPI_8PAD, 0x18),
                    FLEXSPI_LUT_SEQ(CADDR_DDR, FLEXSPI_8PAD, 0x10, DUMMY_DDR, FLEXSPI_8PAD, 0x06),
                    FLEXSPI_LUT_SEQ(READ_DDR, FLEXSPI_8PAD, 0x04, STOP, FLEXSPI_1PAD, 0x0),
                },
            },
            .pageSize = 512u,
            .sectorSize = 256u * 1024u,
            .blockSize = 256u * 1024u,
            .isUniformBlockSize = true,
        };*/
 
    #if defined(XIP_BOOT_HEADER_DCD_ENABLE) && (XIP_BOOT_HEADER_DCD_ENABLE == 1)
        const uint8_t dcd_data[] = {0x00, 0x00};
    #else
        // null
    #endif

#endif /* XIP_BOOT_HEADER_ENABLE */
