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

#ifndef _FSL_VIDEO_I2C_H_
#define _FSL_VIDEO_I2C_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
typedef LPI2C_Type *video_i2c_t;
#include "fsl_lpi2c.h"
#else
typedef I2C_Type *video_i2c_t;
#include "fsl_i2c.h"
#endif

/*! @brief Video device register address type. */
typedef enum _video_reg_addr
{
    kVIDEO_RegAddr8Bit = 1U,  /*!< 8-bit register address.  */
    kVIDEO_RegAddr16Bit = 2U, /*!< 16-bit register address. */
} video_reg_addr_t;

/*! @brief Video device register width. */
typedef enum _video_reg_width
{
    kVIDEO_RegWidth8Bit = 1U,  /*!< 8-bit register width.  */
    kVIDEO_RegWidth16Bit = 2U, /*!< 16-bit register width. */
    kVIDEO_RegWidth32Bit = 4U, /*!< 32-bit register width. */
} video_reg_width_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Write value to the register.
 *
 * @param i2c I2C port.
 * @param i2cAddr I2C address.
 * @param addrType Register address type.
 * @param reg The register to write.
 * @param regWidth The width of the register.
 * @param value The value to write.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t VIDEO_I2C_WriteReg(video_i2c_t i2c,
                            uint8_t i2cAddr,
                            video_reg_addr_t addrType,
                            uint32_t reg,
                            video_reg_width_t regWidth,
                            uint32_t value);

/*!
 * @brief Read the register value.
 *
 * @param i2c I2C port.
 * @param i2cAddr I2C address.
 * @param addrType Register address type.
 * @param reg The register to read.
 * @param regWidth The width of the register.
 * @param value The value read out.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t VIDEO_I2C_ReadReg(
    video_i2c_t i2c, uint8_t i2cAddr, video_reg_addr_t addrType, uint32_t reg, video_reg_width_t regWidth, void *value);

/*!
 * @brief Modify the register value.
 *
 * This function modifies some bit fields of a register.
 * reg[clrMask] = value & clrMask
 *
 * @param i2c I2C port.
 * @param i2cAddr I2C address.
 * @param addrType Register address type.
 * @param reg The register to modify.
 * @param regWidth The width of the register.
 * @param clrMask The mask value to clear.
 * @param value The value to set.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t VIDEO_I2C_ModifyReg(video_i2c_t i2c,
                             uint8_t i2cAddr,
                             video_reg_addr_t addrType,
                             uint32_t reg,
                             video_reg_width_t regWidth,
                             uint32_t clrMask,
                             uint32_t value);

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_VIDEO_I2C_H_ */
