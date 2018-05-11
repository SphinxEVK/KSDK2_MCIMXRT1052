#ifndef _DRV_CAMERA_DEVICE_H_
#define _DRV_CAMERA_DEVICE_H_

#define CAMERA_DEVICE_WIDTH				(CAMERA_WIDTH)
#define CAMERA_DEVICE_HEIGHT			(CAMERA_HEIGHT)

extern const camera_device_operations_t MT9V034_ops;

static status_t MT9V034_Init(camera_device_handle_t *handle, const camera_config_t *config);
static status_t MT9V034_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig);
static status_t MT9V034_Start(camera_device_handle_t *handle);
static status_t MT9V034_Stop(camera_device_handle_t *handle);

#endif