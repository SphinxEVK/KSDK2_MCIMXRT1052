#ifndef _DRV_CAMERA_H_
#define _DRV_CAMERA_H_

#define CAMERA_WIDTH				(752)
#define CAMERA_HEIGHT				(480)

#define CAMERA_BPP					(1)		// Bytes Per Pixel
#define CAMERA_CONTROL_FLAGS 		(kCAMERA_VsyncActiveHigh|kCAMERA_HrefActiveHigh|kCAMERA_DataLatchOnFallingEdge)
#define CAMERA_FRAME_BUFFER_COUNT	(4)

volatile static 
csi_frameBuffer[CAMERA_FRAME_BUFFER_COUNT][CAMERA_HEIGHT][CAMERA_WIDTH];

extern void Camera_Start(void);
	
#endif