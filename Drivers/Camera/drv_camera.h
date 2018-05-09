#ifndef _DRV_CAMERA_H_
#define _DRV_CAMERA_H_

#define CAMERA_WIDTH			(752)
#define CAMERA_HEIGHT			(480)

#define CAMERA_BPP			(1)		// Bytes Per Pixel
#define CAMERA_CONTROL_FLAGS 		(kCAMERA_VsyncActiveHigh|kCAMERA_HrefActiveHigh|kCAMERA_DataLatchOnFallingEdge)
#define CAMERA_FRAME_BUFFER_COUNT	(4)

<<<<<<< HEAD
volatile static uint8_t csi_frameBuffer[CAMERA_FRAME_BUFFER_COUNT][CAMERA_HEIGHT][CAMERA_WIDTH];
=======
extern uint32_t activeFrameAddr;
extern uint32_t inactiveFrameAddr;

volatile static uint16_t csi_frameBuffer[CAMERA_FRAME_BUFFER_COUNT][CAMERA_HEIGHT*CAMERA_WIDTH*CAMERA_BPP/sizeof(uint16_t)];
>>>>>>> 28af8751a004a5338b14d64f71ce516305afdf74
extern void CSI_DriverIRQHandler(void);
extern void Camera_Start(void);
	
#endif