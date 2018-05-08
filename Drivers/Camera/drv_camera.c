#include "pin_mux.h"
#include "fsl_csi_camera_adapter.h"
#include "fsl_camera_receiver.h"
#include "fsl_camera_device.h"
#include "drv_camera_device.h"
#include "drv_camera.h"

#include "fsl_gpio.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(__CC_ARM) || defined(__GNUC__)
    AT_NONCACHEABLE_SECTION_ALIGN(volatile static uint8_t csi_frameBuffer[CAMERA_FRAME_BUFFER_COUNT][CAMERA_HEIGHT][CAMERA_WIDTH], FRAME_BUFFER_ALIGN);
#elif defined(__ICCARM__)
	#pragma location = ".csi_data"
	volatile static uint8_t csi_frameBuffer[CAMERA_FRAME_BUFFER_COUNT][CAMERA_HEIGHT][CAMERA_WIDTH];// @ 0x81E00000;
#endif

static csi_private_data_t csiPrivateData;


/* CSI接收属性配置 */
camera_receiver_handle_t cameraReceiver = {
	.resource = &(csi_resource_t){.csiBase = CSI},
	.ops = &csi_ops,
	.privateData = &csiPrivateData,
};

/* Camera设备操作配置 */
const camera_device_handle_t cameraDevice = {
	.ops = &MT9V034_ops,
};

/* Camera设备属性配置 */
const camera_config_t cameraConfig = {
    .pixelFormat = kVIDEO_PixelFormatRGB565,
    .bytesPerPixel = CAMERA_BPP,
    .resolution = FSL_VIDEO_RESOLUTION(CAMERA_WIDTH, CAMERA_HEIGHT),
    .frameBufferLinePitch_Bytes = CAMERA_WIDTH*CAMERA_BPP,
    .interface = kCAMERA_InterfaceGatedClock,
    .controlFlags = CAMERA_CONTROL_FLAGS,
    .framePerSec = 50,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
void CSI_IRQHandler(void)
{
    //CSI_DriverIRQHandler();
	GPIO_WritePinOutput(GPIO1, 15U, !GPIO_PinRead(GPIO1, 15U));
}

void Camera_Start(void)
{
	BOARD_InitCSIPins();
	
	//Clear Video Frame Buffer
	//memset(csi_frameBuffer, 0, sizeof(csi_frameBuffer));
	//volatile uint32_t * write_uint32 = (uint32_t *)csi_frameBuffer;	
	//for(uint32_t i=0; i <(sizeof(csi_frameBuffer)/sizeof(uint16_t)); i++)
	//{
	//	*write_uint32++ = 0x00000000;
	//}
	
    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);
    CAMERA_DEVICE_Init((camera_device_handle_t*)&cameraDevice, &cameraConfig);
    CAMERA_DEVICE_Start((camera_device_handle_t*)&cameraDevice);
	
	/* 将空帧缓冲区提交到缓冲区队列. */
    for (uint32_t i = 0; i < CAMERA_FRAME_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(csi_frameBuffer[i]));
    }
	
	//CSI_DisableInterrupts(CSI, 0xFFFFFFFF);
	//CSI_EnableInterrupts(CSI, CSI_CSICR1_EOF_INT_EN(true));
	
	CAMERA_RECEIVER_Start(&cameraReceiver);
}