#ifndef _ALEX_MT9V034_H_
#define _ALEX_MT9V034_H_

#include "appinc.h"
#include "ALEX_MT9V034_REG.h"

#define MT9V034_I2C_ADDRESS		(0x5C)

#define MT9V034_H_MAX                   (480)
#define MT9V034_W_MAX                   (752)

#define MT9V034_H			(120)
#define MT9V034_W                       (188)
#define MT9V034_SIZE                    (MT9V034_H*MT9V034_W)

#define MT9V034_Delay(n)       		ALEX_CPU_Delay(n)
#define MT9V034_DelayMs(ms)    		ALEX_lptmr_DelayMs(ms)
///////////////////////////////////////////////////////////
typedef enum{
	MT9V034_HDR_OFF = 0,
	MT9V034_HDR_80dB  = 1,
	MT9V034_HDR_100dB = 2,
}MT9V034_HDRMode_e;
///////////////////////////////////////////////////////////
static void MT9V034_WriteReg(uint8_t SlaveAddr, uint8_t RegAddr, uint16_t RegVal);
static uint16_t MT9V034_ReadReg(uint8_t SlaveAddr, uint8_t RegAddr);
///////////////////////////////////////////////////////////
status_t MT9V034_CheckID(uint8_t MT9V034_I2C_ADDR);
status_t MT9V034_Reset(uint8_t MT9V034_I2C_ADDR);
status_t MT9V034_SetReservedReg(uint8_t MT9V034_I2C_ADDR);
status_t MT9V034_SetFrameRate(uint8_t MT9V034_I2C_ADDR, uint8_t frameRate);
status_t MT9V034_SetFrameResolution(uint8_t MT9V034_I2C_ADDR, uint16_t width, uint16_t height);
status_t MT9V034_SetAutoExposure(uint8_t MT9V034_I2C_ADDR, bool enable);
status_t MT9V034_SetAutoGainCtrl(uint8_t MT9V034_I2C_ADDR, bool enable);
status_t MT9V034_SetHDRMode(uint8_t MT9V034_I2C_ADDR, MT9V034_HDRMode_e mode);
status_t MT9V034_SetTestPattern(uint8_t MT9V034_I2C_ADDR, bool enable);
///////////////////////////////////////////////////////////
status_t MT9V034_InitReg(uint8_t MT9V034_I2C_ADDR);
#endif