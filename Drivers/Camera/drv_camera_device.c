#include "fsl_camera_device.h"
#include "drv_camera_device.h"
#include "ALEX_MT9V034.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
const camera_device_operations_t MT9V034_ops = {
    .init = MT9V034_Init,
	.init_ext = MT9V034_InitExt,
    .start = MT9V034_Start,
    .stop  = MT9V034_Stop,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static status_t MT9V034_Init(camera_device_handle_t *handle, const camera_config_t *config)
{
    uint16_t width, height;

    if ((kCAMERA_InterfaceNonGatedClock != config->interface) && (kCAMERA_InterfaceGatedClock != config->interface) &&
        (kCAMERA_InterfaceCCIR656 != config->interface))
    {
        return kStatus_InvalidArgument;
    }

    width  = FSL_VIDEO_EXTRACT_WIDTH(config->resolution);
    height = FSL_VIDEO_EXTRACT_HEIGHT(config->resolution);

    if ((width > MT9V034_W_MAX) || (height > MT9V034_H_MAX))
    {
        return kStatus_InvalidArgument;
    }

	//Check Camera ID through I2C interface
    if(true != MT9V034_CheckID(MT9V034_I2C_ADDRESS))
    {
        return kStatus_Fail;
    }

	MT9V034_Reset(MT9V034_I2C_ADDRESS);

	MT9V034_SetTestPattern(MT9V034_I2C_ADDRESS, false);

	MT9V034_SetFrameResolution(MT9V034_I2C_ADDRESS, CAMERA_DEVICE_WIDTH, CAMERA_DEVICE_HEIGHT);

	MT9V034_SetAutoExposure(MT9V034_I2C_ADDRESS, true);

	MT9V034_SetHDRMode(MT9V034_I2C_ADDRESS, MT9V034_HDR_OFF);

	MT9V034_SetReservedReg(MT9V034_I2C_ADDRESS);

	return kStatus_Success;
}

static status_t MT9V034_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig)
{
	return kStatus_Success;
}

static status_t MT9V034_Start(camera_device_handle_t *handle)
{
	return kStatus_Success;
}

static status_t MT9V034_Stop(camera_device_handle_t *handle)
{
	return kStatus_Success;
}
