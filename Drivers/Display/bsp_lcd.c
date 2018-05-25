/**
  ******************************************************************
  * @file    bsp_lcd.c
  * @author  fire
  * @version V2.0
  * @date    2018-xx-xx
  * @brief   lcd应用函数接口
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "fsl_elcdif.h" 
#include "fsl_clock.h"
#include "fsl_pxp.h"

	
#include "pad_config.h"  
#include "./lcd/bsp_lcd.h" 



/*******************************************************************************
 * 变量
 ******************************************************************************/

/* 帧中断标志 */
volatile bool s_frameDone = false;

/* 显存 */
AT_NONCACHEABLE_SECTION_ALIGN( uint32_t s_psBufferLcd[2][LCD_PIXEL_HEIGHT][LCD_PIXEL_WIDTH], FRAME_BUFFER_ALIGN);

/*用于存储当前选择的字体格式*/
static sFONT *LCD_Currentfonts = &Font24x48;
/* 用于存储当前字体颜色和字体背景颜色的变量*/
static uint32_t CurrentTextColor   = 0x00FFFFFF;
static uint32_t CurrentBackColor   = 0x00000000;

/* 指向当前的显存 */
static uint32_t CurrentFrameBuffer = (uint32_t)s_psBufferLcd[0];

/*******************************************************************************
 * 宏
 ******************************************************************************/
/* 所有引脚均使用同样的PAD配置 */
#define LCD_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
        驱动强度: R0/6 
        带宽配置 : medium(100MHz)
        开漏配置: 关闭 
        拉/保持器配置: 使能
        拉/保持器选择: 保持器
        上拉/下拉选择: 100K欧姆下拉(选择了保持器此配置无效)
        滞回器配置: 禁止 */
        
/*******************************************************************************
 * 声明
 ******************************************************************************/
static void LCD_IOMUXC_MUX_Config(void);
static void LCD_IOMUXC_PAD_Config(void);
static void LCD_ELCDIF_Config(void);


/**
* @brief  初始化LCD相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void LCD_IOMUXC_MUX_Config(void)
{
    /* 所有引脚均不开启SION功能 */
    /* 时序控制信号线 */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_00_LCD_CLK, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_01_LCD_ENABLE, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_02_LCD_HSYNC, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_03_LCD_VSYNC, 0U);
  
    /* RGB565数据信号线，
     DATA0~DATA4:B3~B7
     DATA5~DATA10:G2~G7
     DATA11~DATA15:R3~R7 */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_04_LCD_DATA00, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_05_LCD_DATA01, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_06_LCD_DATA02, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_07_LCD_DATA03, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_08_LCD_DATA04, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_09_LCD_DATA05, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_10_LCD_DATA06, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_11_LCD_DATA07, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_12_LCD_DATA08, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_13_LCD_DATA09, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_14_LCD_DATA10, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_15_LCD_DATA11, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_LCD_DATA12, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_01_LCD_DATA13, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_02_LCD_DATA14, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_03_LCD_DATA15, 0U);   
    
    /* LCD_BL背光控制信号线 */
    IOMUXC_SetPinMux(LCD_BL_IOMUXC, 0U); 
}


/**
* @brief  初始化LCD相关IOMUXC的PAD属性配置
* @param  无
* @retval 无
*/
static void LCD_IOMUXC_PAD_Config(void)
{  
    /* 所有引脚均使用同样的PAD配置 */
    /* 时序控制信号线 */
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_00_LCD_CLK,LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_01_LCD_ENABLE, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_02_LCD_HSYNC, LCD_PAD_CONFIG_DATA);  
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_03_LCD_VSYNC, LCD_PAD_CONFIG_DATA); 

    /* RGB565数据信号线，
     DATA0~DATA4:B3~B7
     DATA5~DATA10:G2~G7
     DATA11~DATA15:R3~R7 */
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_04_LCD_DATA00, LCD_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_05_LCD_DATA01, LCD_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_06_LCD_DATA02, LCD_PAD_CONFIG_DATA);  
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_07_LCD_DATA03, LCD_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_08_LCD_DATA04, LCD_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_09_LCD_DATA05, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_10_LCD_DATA06, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_11_LCD_DATA07, LCD_PAD_CONFIG_DATA);  
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_12_LCD_DATA08, LCD_PAD_CONFIG_DATA);  
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_13_LCD_DATA09, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_14_LCD_DATA10, LCD_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_15_LCD_DATA11, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_00_LCD_DATA12, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_01_LCD_DATA13, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_02_LCD_DATA14, LCD_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_03_LCD_DATA15, LCD_PAD_CONFIG_DATA); 
    
    /* LCD_BL背光控制信号线 */
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_15_GPIO1_IO15, LCD_PAD_CONFIG_DATA);
}

/**
* @brief  初始化ELCDIF外设
* @param  无
* @retval 无
*/
static void LCD_ELCDIF_Config(void)
{	
    const elcdif_rgb_mode_config_t config = {
        .panelWidth = LCD_PIXEL_WIDTH,
        .panelHeight = LCD_PIXEL_HEIGHT,
        .hsw = APP_HSW,
        .hfp = APP_HFP,
        .hbp = APP_HBP,
        .vsw = APP_VSW,
        .vfp = APP_VFP,
        .vbp = APP_VBP,
        .polarityFlags = APP_POL_FLAGS,
        .bufferAddr = (uint32_t)s_psBufferLcd[0],
        .pixelFormat = kELCDIF_PixelFormatXRGB8888,
        .dataBus = APP_LCDIF_DATA_BUS,
    };
 
  ELCDIF_RgbModeInit(APP_ELCDIF, &config);
  ELCDIF_RgbModeStart(APP_ELCDIF);
}

/**
* @brief  初始化ELCDIF使用的时钟
* @param  无
* @retval 无
*/
void LCD_InitClock(void)
{
    /*
     * 要把帧率设置成60Hz，所以像素时钟频率为:
     * 水平像素时钟个数：(APP_IMG_WIDTH + APP_HSW + APP_HFP + APP_HBP ) 
     * 垂直像素时钟个数：(APP_IMG_HEIGHT + APP_VSW + APP_VFP + APP_VBP)
     * 
     * 像素时钟频率：(800 + 1 + 10 + 46) * (480 + 1 + 22 + 23) * 60 = 27.05M.
     * 本例子设置 LCDIF 像素时钟频率为 27M.
     */

    /*
     * 初始化 Video PLL.
     * Video PLL 输出频率为 
     * OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 108MHz.
     */
    clock_video_pll_config_t config = {
        .loopDivider = 36, .postDivider = 8, .numerator = 0, .denominator = 0,
    };

    CLOCK_InitVideoPll(&config);

    /*
     * 000 derive clock from PLL2
     * 001 derive clock from PLL3 PFD3
     * 010 derive clock from PLL5
     * 011 derive clock from PLL2 PFD0
     * 100 derive clock from PLL2 PFD1
     * 101 derive clock from PLL3 PFD1
     */
    /* 选择为vedio PLL*/
    CLOCK_SetMux(kCLOCK_LcdifPreMux, 2);

    /* 设置分频 */  
    CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 1);

    CLOCK_SetDiv(kCLOCK_LcdifDiv, 1);
}

/**
* @brief  初始化背光引脚并点亮
* @param  无
* @retval 无
*/
void LCD_BackLight_ON(void)
{    
    /* 背光，高电平点亮 */
    gpio_pin_config_t config = {
      kGPIO_DigitalOutput, 
      1,
      kGPIO_NoIntmode
    };

    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);
}



/**
* @brief  初始化液晶屏
* @param  enableInterrupt ：是否使能中断
* @retval 无
*/
void LCD_Init(bool enableInterrupt)
{
  *((uint32_t *)0x41044100) = 0x0000000f;
	*((uint32_t *)0x41044104) = 0x0000000f;

  LCD_IOMUXC_MUX_Config();
  LCD_IOMUXC_PAD_Config();
  LCD_ELCDIF_Config();
  LCD_InitClock();
  LCD_BackLight_ON();
  
  if(enableInterrupt)
  {
    LCD_InterruptConfig();
  }
}

/***************************中断相关******************************/
/**
* @brief  配置ELCDIF中断
* @param  无
* @retval 无
*/
void LCD_InterruptConfig(void)
{
  /* 使能中断 */
  EnableIRQ(LCDIF_IRQn);
   
  /* 配置ELCDIF为CurFrameDoneInterrupt中断 */
  ELCDIF_EnableInterrupts(APP_ELCDIF, kELCDIF_CurFrameDoneInterruptEnable);
}

/**
* @brief  ELCDIF中断服务函数
* @param  无
* @retval 无
*/
void LCDIF_IRQHandler(void)
{
    uint32_t intStatus;

    intStatus = ELCDIF_GetInterruptStatus(APP_ELCDIF);

    ELCDIF_ClearInterruptStatus(APP_ELCDIF, intStatus);

    if (intStatus & kELCDIF_CurFrameDone)
    {
        s_frameDone = true;
    }

    /* 以下部分是为 ARM 的勘误838869添加的, 
       该错误影响 Cortex-M4, Cortex-M4F内核，       
       立即存储覆盖重叠异常，导致返回操作可能会指向错误的中断
        CM7不受影响，此处保留该代码
    */  
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/***************************显示应用相关******************************/

/***************************显示字符相关******************************/

/**
  * @brief  设置字体的颜色及字体的背景颜色
  * @param  TextColor: 字体颜色
  * @param  BackColor: 字体的背景颜色
  * @retval None
  */
void LCD_SetColors(uint32_t TextColor, uint32_t BackColor) 
{
  CurrentTextColor = TextColor; 
  CurrentBackColor = BackColor;
}

/**
  * @brief 获取当前设置的字体颜色和字体的背景颜色
  * @param  TextColor: 指向字体颜色的指针
  * @param  BackColor: 指向字体背景颜色的指针
  * @retval None
  */
void LCD_GetColors(uint32_t *TextColor, uint32_t *BackColor)
{
  *TextColor = CurrentTextColor;
  *BackColor = CurrentBackColor;
}

/**
  * @brief  设置字体颜色
  * @param  Color: 字体颜色
  * @retval None
  */
void LCD_SetTextColor(uint32_t Color)
{
  CurrentTextColor = Color;
}

/**
  * @brief  设置字体的背景颜色
  * @param  Color: 字体的背景颜色
  * @retval None
  */
void LCD_SetBackColor(uint32_t Color)
{
  CurrentBackColor = Color;
}

/**
  * @brief  设置字体格式(英文)
  * @param  fonts: 选择要设置的字体格式
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

/**
  * @brief  获取当前字体格式(英文)
  * @param  None.
  * @retval 当前应用的格式
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}


/**
  * @brief  在显示器上显示一个英文字符
  * @param  Xpos ：字符的起始X坐标
  * @param  Ypos ：字符的起始Y坐标
  * @param  Ascii: 要显示的字符的ASCII码
  * @retval None
  */
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, char Ascii)
{
  uint16_t fontLength;	
  uint16_t page, column;

	uint16_t relativePositon;
	uint8_t *pfont;
  
  uint32_t yBufferPos = 0;
  uint32_t xPixelPos = 0;
  
  /*yBufferPos表示当前行的显存偏移位置*/
  yBufferPos = Ypos*LCD_PIXEL_WIDTH*APP_BPP;
  
  /*xpixelPos表示部分像素点位置
    APP_BPP*xPixelPos + yBufferPos 就是当前像素点的显存位置
  */
  xPixelPos += Xpos;
	
	//对ascii码表偏移（字模表不包含ASCII表的前32个非图形符号）
	relativePositon = Ascii - ' ';
	
	//每个字模的字节数
	fontLength = (LCD_Currentfonts->Width*LCD_Currentfonts->Height)/8;
		
	//字模首地址
	/*ascii码表偏移值乘以每个字模的字节数，求出字模的偏移位置*/
	pfont = (uint8_t *)&LCD_Currentfonts->table[relativePositon * fontLength];
	
  //每个字模有LCD_Currentfonts->Height行，遍历每一行
  for ( page = 0; page < LCD_Currentfonts->Height; page++ )
	{    
    //每个字模有LCD_Currentfonts->Width/8 个字节，遍历每个字节
    for ( column = 0; column < LCD_Currentfonts->Width/8; column++ ) 
		{	
      uint8_t bitCount = 0;

      //每个字节有8个数据位，遍历每个数据位
      for(bitCount=0; bitCount<8; bitCount++)
      {
        if(*pfont & (0x80>>bitCount))
        {
         //字体色
          #if 0
            //RGB888显示方式  
            *(__IO uint16_t*)(CurrentFrameBuffer + (APP_BPP*xPixelPos) + yBufferPos) = (0x00FFFF & CurrentTextColor);        //GB
            *(__IO uint8_t*)(CurrentFrameBuffer + (APP_BPP*xPixelPos) + yBufferPos+2) = (0x00FF0000 & CurrentTextColor) >> 16; //R
          #else
            //XRGB8888显示方式
            *(__IO uint32_t*)(CurrentFrameBuffer + (APP_BPP*xPixelPos) + yBufferPos) = CurrentTextColor;        //XRGB
          #endif          
        }
        else
        {
          //背景色
         #if 0
            //RGB888显示方式 
            *(__IO uint16_t*)(CurrentFrameBuffer + (APP_BPP*xPixelPos) + yBufferPos) = (0x00FFFF & CurrentBackColor);        //GB
            *(__IO uint8_t*)(CurrentFrameBuffer + (APP_BPP*xPixelPos) + yBufferPos+2) = (0x00FF0000 & CurrentBackColor) >> 16; //R
         #else
            //XRGB8888显示方式
            *(__IO uint32_t*)(CurrentFrameBuffer + (APP_BPP*xPixelPos) + yBufferPos) = CurrentBackColor;        //XRGB
         #endif
        }
        /*指向当前行的下一个点*/
        xPixelPos++;		
      }
      
      /* 指向字模数据的一下个字节 */
      pfont++;
    }      
    /*显示完一行*/
    /*指向字符显示矩阵下一行的第一个像素点*/
    xPixelPos += (LCD_PIXEL_WIDTH - LCD_Currentfonts->Width);		
  }
}

/**
 * @brief  在显示器上显示中英文字符串,超出液晶宽度时会自动换行。
 * @param  Xpos ：字符的起始X坐标
 * @param  Ypos ：字符的起始Y坐标
 * @param  pStr ：要显示的字符串的首地址
 * @retval 无
 */
void LCD_DispString(uint16_t Xpos, uint16_t Ypos, const uint8_t * pStr )
{
	while( * pStr != '\0' )
	{	
    /*自动换行*/
    if ( ( Xpos + LCD_Currentfonts->Width ) > LCD_PIXEL_WIDTH )
    {
      Xpos = 0;
      Ypos += LCD_Currentfonts->Height;
    }
    
    if ( ( Ypos + LCD_Currentfonts->Height ) > LCD_PIXEL_HEIGHT )
    {
      Xpos = 0;
      Ypos = 0;
    }			
        
    /* 显示单个字符 */
    LCD_DisplayChar(Xpos,Ypos,*pStr);
    
    Xpos += LCD_Currentfonts->Width;
  
    pStr ++;
  }		
} 

/**
  * @brief  显示字符串(英文)
  * @param  Line: 根据当前字体而变的行号
  *     @arg Line(1),Line(2)等
  * @param  *ptr: 要显示的字符串
  * @retval None
  */
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{  
  uint16_t refcolumn = 0;
  /* 循环显示字符，直至遇到字符串结束符
    或直到单行显示不下字符
  */
  while ((refcolumn < LCD_PIXEL_WIDTH) && ((*ptr != 0) & 
    (((refcolumn + LCD_Currentfonts->Width) & 0xFFFF) >= LCD_Currentfonts->Width)))
  {
    /* 显示单个字符 */
    LCD_DisplayChar(refcolumn,Line , *ptr);
    /* 偏移字符显示位置 */
    refcolumn += LCD_Currentfonts->Width;
    /* 指向下一个字符 */
    ptr++;
  }
}

/**
  * @brief  清除指定行的字符
  * @param  Line: 要清除的行,注意LINE宏是根据当前字体而变的
  *     @arg LINE(1),LINE(2)
  * @retval None
  */
void LCD_ClearLine(uint16_t Line)
{
  uint16_t refcolumn = 0;
  /* 循环显示至屏幕最右侧 */
  while ((refcolumn < LCD_PIXEL_WIDTH) && 
    (((refcolumn + LCD_Currentfonts->Width)& 0xFFFF) >= LCD_Currentfonts->Width))
  {
    /* 显示空格（相当于清除的效果） */
    LCD_DisplayChar(refcolumn, Line, ' ');
    /* 偏移字符显示位置 */
    refcolumn += LCD_Currentfonts->Width;
  }
}

/**
  * @brief  设置显示坐标
  * @param  Xpos: x坐标
  * @param  Ypos: y坐标
  * @retval 显存的地址
  */
uint32_t LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{  
  return CurrentFrameBuffer + APP_BPP*(Xpos + (LCD_PIXEL_WIDTH*Ypos));
}

/***************************显示图形相关******************************/
/**
  * @brief 选择当前要操作的显存区域
  * @param  index: 0或1
  * @retval None
  */
void LCD_SetFrameBuffer(uint8_t index)
{
  CurrentFrameBuffer = (uint32_t)s_psBufferLcd[index];
}

/**
  * @brief 设置后面要显示的显存区域
  * @param  index: 0或1
  * @retval None
  */
void LCD_SetDisplayBuffer(uint8_t index)
{
  /* 设置ELCDIF的下一个缓冲区地址 */
  ELCDIF_SetNextBufferAddr(APP_ELCDIF, (uint32_t)s_psBufferLcd[index]);

}

/**
  * @brief 使用当前颜色在指定的位置绘制一个像素点
  * @param  Xpos: x坐标
  * @param  Ypos: y坐标
  * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
  * @retval None
  */
void PutPixel(uint16_t Xpos, uint16_t Ypos)
{   
	if ( ( Xpos < LCD_PIXEL_WIDTH ) && ( Ypos < LCD_PIXEL_HEIGHT ) )
  {
		*(uint32_t *)(CurrentFrameBuffer + APP_BPP*(Xpos + (LCD_PIXEL_WIDTH*Ypos))) = CurrentTextColor;
	}
}

/**
  * @brief  以当前背景颜色清除整个屏幕
  * @param  无
  * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
  * @retval 无
  */
void LCD_Clear(uint32_t Color)
{
  /* 清除缓冲区内容 */
  uint16_t page, column;  
  
  /* 指向矩形第一个像素点的显存位置 */
  uint32_t *pRectImage = (uint32_t*)CurrentFrameBuffer ;
  
  /* 遍历每一行 */
  for ( page = 0; page < LCD_PIXEL_HEIGHT; page++ )
  {    
    /* 遍历每一列 */
    for ( column = 0; column < LCD_PIXEL_WIDTH; column++ ) 
    {	
      *pRectImage = Color;
      
      /* 指向下一个像素点的显存位置 */
      pRectImage++;
    }      
  }
}

/**
  * @brief 显示一条直线
  * @param Xpos: 直线起点的x坐标
  * @param Ypos: 直线起点的y坐标
  * @param Length: 直线的长度
  * @param Direction: 直线的方向，可输入
      @arg LINE_DIR_HORIZONTAL(水平方向) 
      @arg LINE_DIR_VERTICAL(垂直方向).
  * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
  * @retval None
  */
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, bool Direction)
{
  uint16_t index; 
  
  uint16_t realLength;
  
  /* 指向直线第一个像素点的显存位置 */
  uint32_t *pLineImage = (uint32_t*)(CurrentFrameBuffer + APP_BPP*(Xpos + (LCD_PIXEL_WIDTH*Ypos)));

  if(Direction == LINE_DIR_HORIZONTAL)
  {
    realLength = LCD_PIXEL_WIDTH-Xpos-Length > 0 ? Length : LCD_PIXEL_WIDTH - Xpos;
    
    /* 遍历每一列 */
    for ( index = 0; index < realLength; index++ ) 
    {
        *pLineImage = CurrentTextColor;
        
        /* 指向下一个像素点的显存位置 */
        pLineImage++;
    }
  }
  else
  {
    realLength = LCD_PIXEL_HEIGHT-Ypos-Length > 0 ? Length : LCD_PIXEL_HEIGHT - Ypos;
    
    /* 遍历每一行 */
    for ( index = 0; index < realLength; index++ ) 
    {
        *pLineImage = CurrentTextColor;
        
        /* 指向下一个像素点的显存位置 */
        pLineImage += LCD_PIXEL_WIDTH;
    }
  }   
}

/**
 * @brief  在液晶屏上使用 Bresenham 算法画线段（基于两点） 
 * @param  Xpos1 ：线段的一个端点X坐标
 * @param  Ypos1 ：线段的一个端点Y坐标
 * @param  Xpos2 ：线段的另一个端点X坐标
 * @param  Ypos2 ：线段的另一个端点Y坐标
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_DrawUniLine ( uint16_t Xpos1, uint16_t Ypos1, uint16_t Xpos2, uint16_t Ypos2 )
{
	uint16_t us; 
	uint16_t usX_Current, usY_Current;
	
	int32_t lError_X = 0, lError_Y = 0, lDelta_X, lDelta_Y, lDistance; 
	int32_t lIncrease_X, lIncrease_Y; 	
	
	
	lDelta_X = Xpos2 - Xpos1; //计算坐标增量 
	lDelta_Y = Ypos2 - Ypos1; 
	
	usX_Current = Xpos1; 
	usY_Current = Ypos1; 
	
	
	if ( lDelta_X > 0 ) 
		lIncrease_X = 1; //设置单步方向 
	
	else if ( lDelta_X == 0 ) 
		lIncrease_X = 0;//垂直线 
	
	else 
  { 
    lIncrease_X = -1;
    lDelta_X = - lDelta_X;
  } 

	
	if ( lDelta_Y > 0 )
		lIncrease_Y = 1; 
	
	else if ( lDelta_Y == 0 )
		lIncrease_Y = 0;//水平线 
	
	else 
  {
    lIncrease_Y = -1;
    lDelta_Y = - lDelta_Y;
  } 

	
	if (  lDelta_X > lDelta_Y )
		lDistance = lDelta_X; //选取基本增量坐标轴 
	
	else 
		lDistance = lDelta_Y; 

	
	for ( us = 0; us <= lDistance + 1; us ++ )//画线输出 
	{  
		PutPixel ( usX_Current, usY_Current );//画点 
		
		lError_X += lDelta_X ; 
		lError_Y += lDelta_Y ; 
		
		if ( lError_X > lDistance ) 
		{ 
			lError_X -= lDistance; 
			usX_Current += lIncrease_X; 
		}  
		
		if ( lError_Y > lDistance ) 
		{ 
			lError_Y -= lDistance; 
			usY_Current += lIncrease_Y; 
		} 
		
	}  	
	
}   

/**
  * @brief  绘制空心矩形
  * @param  Xpos ：矩形左上角端点X坐标
  * @param  Ypos ：矩形左上角端点Y坐标
  * @param  Width ：矩形宽
  * @param  Height ：矩形高
  * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
  * @retval 无
  */
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{  
  uint16_t realHeight,realWidth;
  
  realHeight = LCD_PIXEL_HEIGHT-Ypos-Height > 0 ? Height : LCD_PIXEL_HEIGHT - Ypos;
  realWidth = LCD_PIXEL_WIDTH-Xpos-Width > 0 ? Width : LCD_PIXEL_WIDTH - Xpos;
  
  LCD_DrawLine(Xpos, Ypos, realWidth, LINE_DIR_HORIZONTAL);
  LCD_DrawLine(Xpos, Ypos, realHeight, LINE_DIR_VERTICAL);
  LCD_DrawLine(Xpos + realWidth - 1, Ypos, realHeight, LINE_DIR_VERTICAL);
  LCD_DrawLine(Xpos, Ypos + realHeight - 1, realWidth, LINE_DIR_HORIZONTAL);
}

/**
  * @brief  绘制实心矩形
  * @param  Xpos ：矩形左上角端点X坐标
  * @param  Ypos ：矩形左上角端点Y坐标
  * @param  Width ：矩形宽
  * @param  Height ：矩形高
  * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
  * @retval 无
  */
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  uint16_t page, column; 
  
  uint16_t realHeight,realWidth;
  
  realHeight = LCD_PIXEL_HEIGHT-Ypos-Height > 0 ? Height : LCD_PIXEL_HEIGHT - Ypos;
  realWidth = LCD_PIXEL_WIDTH-Xpos-Width > 0 ? Width : LCD_PIXEL_WIDTH - Xpos;
  
  /* 指向矩形第一个像素点的显存位置 */
  uint32_t *pRectImage = (uint32_t*)(CurrentFrameBuffer + APP_BPP*(Xpos + (LCD_PIXEL_WIDTH*Ypos)));
  
  /* 遍历每一行 */
  for ( page = 0; page < realHeight; page++ )
  {    
    /* 遍历每一列 */
    for ( column = 0; column < realWidth; column++ ) 
    {	
      *pRectImage = CurrentTextColor;
      
      /* 指向下一个像素点的显存位置 */
      pRectImage++;
    }      
    /*显示完一行*/
    /*指向下一行的第一个像素点的显存位置*/
    pRectImage += (LCD_PIXEL_WIDTH - realWidth);		
  }
}

/**
 * @brief  绘制一个空心圆
 * @param  Xpos: 圆心X坐标
 * @param  Ypos: 圆心Y坐标
 * @param  Radius: 半径
 * @retval None
 */
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
   int x = -Radius, y = 0, err = 2-2*Radius, e2;
   do {
       *(__IO uint32_t*) (CurrentFrameBuffer + (APP_BPP*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor;
       *(__IO uint32_t*) (CurrentFrameBuffer + (APP_BPP*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor;
       *(__IO uint32_t*) (CurrentFrameBuffer + (APP_BPP*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;
       *(__IO uint32_t*) (CurrentFrameBuffer + (APP_BPP*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;

       e2 = err;
       if (e2 <= y) {
           err += ++y*2+1;
           if (-x == y && e2 <= x) e2 = 0;
       }
       if (e2 > x) err += ++x*2+1;
   }
   while (x <= 0);
}

/**
 * @brief  绘制一个实心圆
 * @param  Xpos: 圆心X坐标
 * @param  Ypos: 圆心Y坐标
 * @param  Radius: 半径
 * @retval None
 */
void LCD_DrawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{  
  /* 绘制实心圆需要增加的操作 */
   int32_t  D;    /* Decision Variable */
   uint32_t  CurX;/* Current X Value */
   uint32_t  CurY;/* Current Y Value */

   D = 3 - (Radius << 1);

   CurX = 0;
   CurY = Radius;

   while (CurX <= CurY)
   {
     if(CurY > 0)
     {
       LCD_DrawLine(Xpos - CurX, Ypos - CurY, 2*CurY, LINE_DIR_VERTICAL);
       LCD_DrawLine(Xpos + CurX, Ypos - CurY, 2*CurY, LINE_DIR_VERTICAL);
     }

     if(CurX > 0)
     {
       LCD_DrawLine(Xpos - CurY, Ypos - CurX, 2*CurX, LINE_DIR_VERTICAL);
       LCD_DrawLine(Xpos + CurY, Ypos - CurX, 2*CurX, LINE_DIR_VERTICAL);
     }
     if (D < 0)
     {
       D += (CurX << 2) + 6;
     }
     else
     {
       D += ((CurX - CurY) << 2) + 10;
       CurY--;
     }
     CurX++; 
    }
 
   LCD_DrawCircle(Xpos, Ypos, Radius);

}
/*********************************************END OF FILE**********************/
