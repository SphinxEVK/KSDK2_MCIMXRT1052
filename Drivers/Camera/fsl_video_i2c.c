/*
 * The Clear BSD License
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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

#include "fsl_common.h"
#include "fsl_video_i2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t VIDEO_I2C_WriteReg(video_i2c_t i2c,
                            uint8_t i2cAddr,
                            video_reg_addr_t addrType,
                            uint32_t reg,
                            video_reg_width_t regWidth,
                            uint32_t value)
{
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    uint8_t data[6];
    uint8_t size = 0;
    status_t status;

    if (kVIDEO_RegAddr16Bit == addrType)
    {
        data[size++] = (reg >> 8U) & 0xFFU;
    }

    data[size++] = (uint8_t)reg;

    while(regWidth--)
    {
        data[(uint32_t)addrType + (uint32_t)regWidth] = (uint8_t)value;
        value >>= 8;
        size++;
    }

    while (1)
    {
        status = LPI2C_MasterStart(i2c, i2cAddr, kLPI2C_Write);

        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    LPI2C_MasterSend(i2c, data, size);

    return LPI2C_MasterStop(i2c);

#else

    uint8_t buff[4];
    i2c_master_transfer_t xfer;

    xfer.slaveAddress = i2cAddr;
    xfer.direction = kI2C_Write;
    xfer.subaddress = reg;

    xfer.subaddressSize = (uint32_t)addrType;
    xfer.dataSize = (uint32_t)regWidth;
    xfer.data = buff;

    while (regWidth--)
    {
        buff[regWidth] = (uint8_t)value;
        value >>= 8;
    }

    return I2C_MasterTransferBlocking(handle->base, &handle->xfer);
#endif
}

status_t VIDEO_I2C_ReadReg(
    video_i2c_t i2c, uint8_t i2cAddr, video_reg_addr_t addrType, uint32_t reg, video_reg_width_t regWidth, void *value)
{
    status_t status;
    uint8_t buff[4];
    uint8_t i=0;

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    uint8_t size = 0;

    if (kVIDEO_RegAddr16Bit == addrType)
    {
        buff[size++] = (reg >> 8U) & 0xFFU;
    }

    buff[size++] = (uint8_t)reg;

    while (1)
    {
        status = LPI2C_MasterStart(i2c, i2cAddr, kLPI2C_Write);

        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    LPI2C_MasterSend(i2c, buff, size);

    LPI2C_MasterStart(i2c, i2cAddr, kLPI2C_Read);

    LPI2C_MasterReceive(i2c, buff, (uint32_t)regWidth);

    status = LPI2C_MasterStop(i2c);

#else

    i2c_master_transfer_t xfer;

    xfer.slaveAddress = i2cAddr;
    xfer.direction = kI2C_Read;
    xfer.subaddress = reg;
    xfer.subaddressSize = (uint32_t)addrType;
    xfer.dataSize = (uint32_t)regWidth;
    xfer.data = buff;

    status = I2C_MasterTransferBlocking(i2c, base, &xfer);

#endif

    while (regWidth--)
    {
        ((uint8_t*)value)[i++] = buff[regWidth];
    }

    return status;
}

status_t VIDEO_I2C_ModifyReg(video_i2c_t i2c,
                             uint8_t i2cAddr,
                             video_reg_addr_t addrType,
                             uint32_t reg,
                             video_reg_width_t regWidth,
                             uint32_t clrMask,
                             uint32_t value)
{
    status_t status;
    uint32_t regVal;

    status = VIDEO_I2C_ReadReg(i2c, i2cAddr, addrType, reg, regWidth, &regVal);

    if (kStatus_Success != status)
    {
        return status;
    }

    regVal = (regVal & ~(clrMask)) | (value & clrMask);

    return VIDEO_I2C_WriteReg(i2c, i2cAddr, addrType, reg, regWidth, regVal);
}
