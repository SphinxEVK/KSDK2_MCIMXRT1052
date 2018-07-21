#include "ALEX_I2C&&SCCB.h"
#include "ALEX_MT9V034.h"

void MT9V034_WriteReg(uint8_t SlaveAddr, uint8_t RegAddr, uint16_t RegVal)
{
    //SCCB_UnlockI2C();
    MT9V034_writereg_start:
    SCCB_Start();
    SCCB_WriteByte_soft(SlaveAddr, RegAddr, RegVal);
    SCCB_Stop();
    MT9V034_DelayMs(2);

    if((RegAddr !=  MT9V034_RESET) && (RegVal != MT9V034_ReadReg(SlaveAddr, RegAddr)))
    {
        goto MT9V034_writereg_start;
    }

    MT9V034_DelayMs(2);
    return;
}

uint16_t MT9V034_ReadReg(uint8_t SlaveAddr, uint8_t RegAddr)
{
    uint16_t val;
    val = SCCB_ReadByte_soft(SlaveAddr, RegAddr);
    SCCB_Stop();
    MT9V034_DelayMs(2);
    return val;
}

///////////////////////////////////////////////////////////////////////////////
status_t MT9V034_CheckID(uint8_t MT9V034_I2C_ADDR)
{
	uint16_t id = MT9V034_ReadReg(MT9V034_I2C_ADDR, 0x00);
    if(MT9V034_DEFAULT_VERSION != id)
    {
        return false;
    }

	return true;
}

status_t MT9V034_Reset(uint8_t MT9V034_I2C_ADDR)
{
    //Reset MT9V034, but register config will not change.
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_RESET, 0x0001);
    MT9V034_DelayMs(10);

    //Unlock MT9V034, allowing user to initiate register settings and readout
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_CHIP_CONTROL, 0x0188);

    //Reset Again.
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_RESET, 0x0001);
    MT9V034_DelayMs(10);

	return true;
}

status_t MT9V034_SetReservedReg(uint8_t MT9V034_I2C_ADDR)
{
    //Here we write some reserved registers as recommendations from Rev.G datasheet, Table.8
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x13, 0x2D2E);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x20, 0x03C7);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x24, 0x001B);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x2B, 0x0003);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x2F, 0x0003);

	return true;
}

status_t MT9V034_SetFrameRate(uint8_t MT9V034_I2C_ADDR, uint8_t frameRate)
{
    return true;
}

status_t MT9V034_SetFrameResolution(uint8_t MT9V034_I2C_ADDR, uint16_t width, uint16_t height)
{
    uint16_t data = 0;

    if((width*4)<=MT9V034_MAX_WIDTH && (height*4)<=MT9V034_MAX_HEIGHT)
    {
        width *= 4;
        height *= 4;
        data |= MT9V034_READ_MODE_ROW_BIN_4;
        data |= MT9V034_READ_MODE_COL_BIN_4;
    }
    else if((width*2)<=MT9V034_MAX_WIDTH && (height*2)<=MT9V034_MAX_HEIGHT)
    {
        width *= 2;
        height *= 2;
        data |= MT9V034_READ_MODE_ROW_BIN_2;
        data |= MT9V034_READ_MODE_COL_BIN_2;
    }

    //data |= (MT9V034_READ_MODE_ROW_FLIP|MT9V034_READ_MODE_COL_FLIP);       //LQ-MT9V034 needs vertical mirror to capture correct image

    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_READ_MODE, data);

    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_WINDOW_WIDTH,  width);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_WINDOW_HEIGHT, height);

    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_COL_START, (MT9V034_MAX_WIDTH-width)/2+MT9V034_COL_START_MIN);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_ROW_START, (MT9V034_MAX_HEIGHT-height)/2+MT9V034_ROW_START_MIN);

	return true;
}

status_t MT9V034_SetAutoExposure(uint8_t MT9V034_I2C_ADDR, bool enable)
{
    uint16_t reg = MT9V034_ReadReg(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE);
    if(true == enable)
    {
        MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg|MT9V034_AEC_ENABLE);
    }
    else
    {
        MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg&~MT9V034_AEC_ENABLE);
    }

	return true;
}

status_t MT9V034_SetAutoGainCtrl(uint8_t MT9V034_I2C_ADDR, bool enable)
{
    uint16_t reg = MT9V034_ReadReg(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE);
    if(true == enable)
    {
        MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg|MT9V034_AGC_ENABLE);
    }
    else
    {
        MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg&~MT9V034_AGC_ENABLE);
    }

	return true;
}

status_t MT9V034_SetHDRMode(uint8_t MT9V034_I2C_ADDR, MT9V034_HDRMode_e mode)
{
	switch(mode)
	{
		case MT9V034_HDR_OFF:
			MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x08, 0x01BB);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x09, 0x01D9);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0A, 0x0164);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0B, 0x01E0);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0F, 0x0100);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x35, 0x0010);
			break;

		case MT9V034_HDR_80dB:
			MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x08, 0x03CA);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x09, 0x03DE);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0A, 0x0064);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0B, 0x03E8);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0F, 0x0103);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x35, 0x8010);
			break;

		case MT9V034_HDR_100dB:
			MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x08, 0x03D4);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x09, 0x03E7);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0A, 0x0064);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0B, 0x03E8);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0F, 0x0103);
    		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x35, 0x8010);
			break;

		default:
			break;
	}

	return true;
}

status_t MT9V034_SetTestPattern(uint8_t MT9V034_I2C_ADDR, bool enable)
{
	if(true == enable)
	{
		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x7F, 0x3000);               // Horizonal Test Pattern enabled.
	}
	else
	{
		MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x7F, 0x0000);               // Test Pattern disabled.
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////

status_t MT9V034_InitReg(uint8_t MT9V034_I2C_ADDR)
{
    uint16_t id = MT9V034_ReadReg(MT9V034_I2C_ADDR, 0x00);
    if(MT9V034_DEFAULT_VERSION != id)
    {
        return 0;
    }

    MT9V034_Reset(MT9V034_I2C_ADDR);

    MT9V034_SetReservedReg(MT9V034_I2C_ADDR);

    MT9V034_SetFrameResolution(MT9V034_I2C_ADDR, MT9V034_W, MT9V034_H);

    MT9V034_SetAutoExposure(MT9V034_I2C_ADDR, true);

    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0xAC, 0x0001);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0xAD, 0x01E0);

    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x2C, 0x0004);

    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x7F, 0x3000);           // test pattern

    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0F, 0x0000);

    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x08, 0x01BB);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x09, 0x01D9);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0xA5, 0x003A);

    //100dB HDR:
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x08, 0x03D4);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x09, 0x03E7);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0A, 0x0064);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0B, 0x03E8);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0F, 0x0103);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x35, 0x8010);

    //80dB HDR
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x08, 0x03CA);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x09, 0x03DE);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0A, 0x0064);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0B, 0x03E8);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0F, 0x0103);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x35, 0x8010);

    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0xA5, 0x0030);

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
uint8_t regTable[][3] = {
    {{0x00},{0x00}, {0x00}},
};