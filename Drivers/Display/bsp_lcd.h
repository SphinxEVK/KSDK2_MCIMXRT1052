#ifndef __BSP_LCD_H
#define	__BSP_LCD_H

#include "fsl_common.h"
#include "./font/fonts.h"


#define APP_ELCDIF    LCDIF
#define APP_PXP       PXP

/* LCD 分辨率 */
#define LCD_PIXEL_WIDTH     480
#define LCD_PIXEL_HEIGHT    800
//兼容官方demo
#define APP_IMG_WIDTH   LCD_PIXEL_WIDTH
#define APP_IMG_HEIGHT  LCD_PIXEL_HEIGHT

/* LCD时序 */
#define APP_HSW 40		//min = 24		
#define APP_HFP 10		//min = 5
#define APP_HBP 10		//min = 5
#define APP_VSW 4			//min = 2
#define APP_VFP 4			//min = 2
#define APP_VBP 4			//min = 2
#define APP_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)


/* LCD数据线类型*/
#define APP_LCDIF_DATA_BUS    kELCDIF_DataBus16Bit


/* LCD背光引脚，高电平点亮 */
#define LCD_BL_GPIO 				      GPIO1
#define LCD_BL_GPIO_PIN 		      (15U)
#define LCD_BL_IOMUXC			        IOMUXC_GPIO_AD_B0_15_GPIO1_IO15

/* 缓冲区对齐配置, 为了提高性能, LCDIF缓冲区要64B对齐 */
#define FRAME_BUFFER_ALIGN    64


/* PS input buffer is square. */
#if APP_IMG_WIDTH > APP_IMG_HEIGHT
#define APP_PS_SIZE (APP_IMG_HEIGHT / 2U)
#else
#define APP_PS_SIZE (APP_IMG_WIDTH / 2U)
#endif

#define APP_PS_ULC_X 0U
#define APP_PS_ULC_Y 0U

#define APP_BPP 4U /* Use 24-bit RGB888 format. */

/* 触摸画板使用到的液晶像素格式宏 */
#define LCD_XRGB_8888             1
//#define LCD_RGB_888             1
//#define LCD_RGB_565             1

#define LCD_INTERRUPT_DISABLE   0
#define LCD_INTERRUPT_ENABLE    (!LCD_INTERRUPT_DISABLE)

#define LINE_DIR_HORIZONTAL       0x0
#define LINE_DIR_VERTICAL         0x1

/** 
  * @brief  LCD color  
  */
#define LCD_COLOR_WHITE          0xFFFFFF
#define LCD_COLOR_BLACK          0x000000
#define LCD_COLOR_GREY           0xC0C0C0
#define LCD_COLOR_BLUE           0x0000FF
#define LCD_COLOR_BLUE2          0x0000A0
#define LCD_COLOR_RED            0xFF0000
#define LCD_COLOR_MAGENTA        0xFF00FF
#define LCD_COLOR_GREEN          0x00FF00
#define LCD_COLOR_CYAN           0x00FFFF
#define LCD_COLOR_YELLOW         0xFFFF00


/*******************************************************************************
 * 全局变量声明
 ******************************************************************************/
extern volatile bool s_frameDone;
extern uint32_t s_psBufferLcd[2][APP_IMG_HEIGHT][APP_IMG_WIDTH];

/*******************************************************************************
 * 函数声明
 ******************************************************************************/
void LCD_BackLight_ON(void);
void LCD_Init(bool enableInterrupt);
void APP_FillFrameBuffer(uint32_t frameBuffer[APP_IMG_HEIGHT][APP_IMG_WIDTH]);
void LCD_InterruptConfig(void);

void LCD_SetColors(uint32_t TextColor, uint32_t BackColor); 
void LCD_GetColors(uint32_t *TextColor, uint32_t *BackColor);
void LCD_SetTextColor(uint32_t Color);
void LCD_SetBackColor(uint32_t Color);
void LCD_SetFont(sFONT *fonts);
sFONT *LCD_GetFont(void);
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, char Ascii);
void LCD_DispString(uint16_t Xpos, uint16_t Ypos, const uint8_t * pStr );
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr);
void LCD_ClearLine(uint16_t Line);
uint32_t LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);


void LCD_SetFrameBuffer(uint8_t index);
void LCD_SetDisplayBuffer(uint8_t index);
void PutPixel(uint16_t Xpos, uint16_t Ypos);
void LCD_Clear(uint32_t Color);
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, bool Direction);
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void LCD_DrawUniLine( uint16_t Xpos1, uint16_t Ypos1, uint16_t Xpos2, uint16_t Ypos2 );
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void LCD_DrawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);




#endif /* __BSP_LCD_H */
