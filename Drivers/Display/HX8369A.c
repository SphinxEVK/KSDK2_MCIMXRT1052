/*********************************************************************
----------------------------------------------------------------------
File        : HX8369A.c
Purpose     : 显示驱动配置
Editor      : Stack

---------------------------END-OF-HEADER------------------------------
*/

#include "fsl_iomuxc.h"
#include "fsl_gpio.h"

#include "HX8369A.H"

/*
	配置说明 :
	转换速率: 转换速率慢
	驱动强度: R0/6
	带宽配置 : LOW(50MHz)
	开漏配置: 关闭
	拉/保持器配置: 关闭
	拉/保持器选择: 保持器（上面已关闭，配置无效）
	上拉/下拉选择: 100K欧姆下拉（上面已关闭，配置无效）
	滞回器配置: 关闭
*/

#define LCD_SPI_CS(a)	if (a)		\
							GPIOB->BSRR = GPIO_Pin_0;	\
						else		\
							GPIOB->BRR  = GPIO_Pin_0;
#define SPI_DCLK(a)		if (a)		\
							GPIOB->BSRR = GPIO_Pin_1;	\
						else		\
							GPIOB->BRR  = GPIO_Pin_1;
#define SPI_SDA(a) 		if (a)		\
							GPIOB->BSRR = GPIO_Pin_2;	\
						else		\
							GPIOB->BRR  = GPIO_Pin_2;

//800*480
//#define  HDP 479  //Hsync Display period
//#define  HT  530	//Horizontal total period = (HT + 1) pixels
//#define  HPS 30   //Horizontal Sync Pulse Start Position = (HPS + 1) pixels
//#define  LPS 2	//Horizontal Display Period Start Position = LPS pixels
//#define  HPW 8    //Horizontal Sync Pulse Width = (HPW + 1) pixels

//#define  VDP 799 	//Vertical Display period
//#define  VT 825	//Vertical Total = (VT + 1) lines
//#define  VPS 16  	//Vertical Sync Pulse Start Position = VPS lines
//#define  FPS 1   	//Vertical Display Period Start Position = FPS lines
//#define  VPW 4  	//Vertical Sync Pulse Width = (VPW + 1) lines

/*********************************************************************
*
*       Driver Port functions
*
**********************************************************************
*/

static void HX8369A_SPI_IOMUXC_MUX_Config(void);
static void HX8369A_SPI_IOMUXC_PAD_Config(void);
static void HX8369A_SPI_GPIO_Mode_Config(void);

/* 所有引脚均使用同样的PAD配置 */
#define HX8369A_SPI_PAD_CONFIG_DATA		(IOMUXC_SW_PAD_CTL_PAD_SRE(0)| 		\
                                         IOMUXC_SW_PAD_CTL_PAD_DSE(6)| 		\
                                         IOMUXC_SW_PAD_CTL_PAD_SPEED(0)| 	\
                                         IOMUXC_SW_PAD_CTL_PAD_ODE(0) | 	\
                                         IOMUXC_SW_PAD_CTL_PAD_PKE(0)| 		\
                                         IOMUXC_SW_PAD_CTL_PAD_PUE(0)| 		\
                                         IOMUXC_SW_PAD_CTL_PAD_PUS(0)| 		\
                                         IOMUXC_SW_PAD_CTL_PAD_HYS(0))

/**
* @brief  初始化HX8369A_SPI相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void HX8369A_SPI_IOMUXC_MUX_Config(void)
{

  /* 设置引脚的复用模式为GPIO，不使用SION功能 */
  IOMUXC_SetPinMux(HX8369A_SPI_CS_IOMUXC, 0U);
  IOMUXC_SetPinMux(HX8369A_SPI_MOSI_IOMUXC, 0U);
  IOMUXC_SetPinMux(HX8369A_SPI_CLK_IOMUXC, 0U);
  IOMUXC_SetPinMux(HX8369A_RST_IOMUXC, 0U);
}

/**
* @brief  初始化HX8369A_SPI相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void HX8369A_SPI_IOMUXC_PAD_Config(void)
{
  IOMUXC_SetPinConfig(HX8369A_SPI_CS_IOMUXC, HX8369A_SPI_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(HX8369A_SPI_MOSI_IOMUXC, HX8369A_SPI_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(HX8369A_SPI_CLK_IOMUXC, HX8369A_SPI_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(HX8369A_RST_IOMUXC, HX8369A_SPI_PAD_CONFIG_DATA);
}


 /**
  * @brief  初始化HX8369A_SPI相关的GPIO模式
  * @param  无
  * @retval 无
  */
static void HX8369A_SPI_GPIO_Mode_Config(void)
{
  /* 定义gpio初始化配置结构体 */
  gpio_pin_config_t hx_spi_config;

   /**HX8369A GPIO配置 **/
  hx_spi_config.direction = kGPIO_DigitalOutput; //输出模式
  hx_spi_config.outputLogic =  1;                //默认高电平
  hx_spi_config.interruptMode = kGPIO_NoIntmode; //不使用中断

  /* 初始化 HX8369A GPIO. */
  GPIO_PinInit(HX8369A_SPI_CS_GPIO, HX8369A_SPI_CS_GPIO_PIN, &hx_spi_config);
  GPIO_PinInit(HX8369A_SPI_MOSI_GPIO, HX8369A_SPI_MOSI_GPIO_PIN, &hx_spi_config);
  GPIO_PinInit(HX8369A_SPI_CLK_GPIO, HX8369A_SPI_CLK_GPIO_PIN, &hx_spi_config);
  GPIO_PinInit(HX8369A_RST_GPIO, HX8369A_RST_GPIO_PIN, &hx_spi_config);
}

/**
  * @brief  初始化控制HX8369A_SPI的IO
  * @param  无
  * @retval 无
  */
void HX8369A_SPI_GPIO_Config(void)
{
  /* 初始化GPIO复用、属性、模式 */
  HX8369A_SPI_IOMUXC_MUX_Config();
  HX8369A_SPI_IOMUXC_PAD_Config();
  HX8369A_SPI_GPIO_Mode_Config();
}


void LCD_delay(uint8_t j)
{
	volatile uint16_t i;
	while(j--)
		for(i=7200;i>0;i--);
}

void LCD_WriteByteSPI(uint8_t byte)
{
	uint8_t n;

	for(n=0; n<8; n++)
	{
		if(byte&0x80)
		{
			HX8369A_SPI_MOSI(1)
		}
		else
		{
			HX8369A_SPI_MOSI(0)
		}

		byte<<= 1;

		HX8369A_SPI_CLK(0);
		HX8369A_SPI_CLK(1);
	}
}
void SPI_WriteComm(uint16_t CMD)
{
	HX8369A_SPI_CS(0);
	HX8369A_SPI_MOSI(0);
	HX8369A_SPI_CLK(0);
	HX8369A_SPI_CLK(1);
	LCD_WriteByteSPI(CMD);
	HX8369A_SPI_CS(1);
}
void SPI_WriteData(uint16_t tem_data)
{
	HX8369A_SPI_CS(0);
	HX8369A_SPI_MOSI(1);
	HX8369A_SPI_CLK(0);
	HX8369A_SPI_CLK(1);
	LCD_WriteByteSPI(tem_data);
	HX8369A_SPI_CS(1);
}

/****************************************************************************
* 名    称：static void HX8369A_LCD_Init(void)
* 功    能：4.3寸屏(HX8369A  480x800)的初始化
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/

void HX8369A_LCD_Init(void)
{
	HX8369A_SPI_GPIO_Config();
	HX8369A_SPI_CS(1);
	LCD_delay(20);
	HX8369A_SPI_CS(0);

	//=================== TK043F1168 ===================//
	SPI_WriteComm(0x2D);
	for(uint32_t i=0; i<=63; i++)
	{
		SPI_WriteData(i*8);
	}

	for(uint32_t i=0; i<=63; i++)
	{
		SPI_WriteData(i*4);
	}

	for(uint32_t i=0; i<=63; i++)
	{
		SPI_WriteData(i*8);
	}

	SPI_WriteComm(0x01);	//Software Reset
	LCD_delay(120);


	SPI_WriteComm(0xB9);	//Set_EXTC
	SPI_WriteData(0xFF);
	SPI_WriteData(0x83);
	SPI_WriteData(0x69);

	SPI_WriteComm(0xB0);	//Set internal OSC
	SPI_WriteData(0x01);	//Enable internal OSC
	SPI_WriteData(0x0F);	//Freq = 15Mhz*125.8%

	SPI_WriteComm(0xB1);	//Set Power
	SPI_WriteData(0x85);
	SPI_WriteData(0x00);
	SPI_WriteData(0x34);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x00);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x2A);
	SPI_WriteData(0x32);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x01);	//update VBIAS
	SPI_WriteData(0x23);
	SPI_WriteData(0x01);
	SPI_WriteData(0xE6);
	SPI_WriteData(0xE6);
	SPI_WriteData(0xE6);
	SPI_WriteData(0xE6);
	SPI_WriteData(0xE6);

	SPI_WriteComm(0xB2);	// SET Display 480x800
	SPI_WriteData(0x00);
	SPI_WriteData(0x2B);	//0x2B
	SPI_WriteData(0x0A);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x70);
	SPI_WriteData(0x00);
	SPI_WriteData(0xFF);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x03);
	SPI_WriteData(0x03);
	SPI_WriteData(0x00);
	SPI_WriteData(0x01);

	SPI_WriteComm(0xB4);
	SPI_WriteData(0x00);
	SPI_WriteData(0x18);
	SPI_WriteData(0x80);
	SPI_WriteData(0x10);
	SPI_WriteData(0x01);
	SPI_WriteComm(0xB6);	// SET VCOM
	SPI_WriteData(0x2C);
	SPI_WriteData(0x2C);

	SPI_WriteComm(0xD5);	//SET GIP
	SPI_WriteData(0x00);
	SPI_WriteData(0x05);
	SPI_WriteData(0x03);
	SPI_WriteData(0x00);
	SPI_WriteData(0x01);
	SPI_WriteData(0x09);
	SPI_WriteData(0x10);
	SPI_WriteData(0x80);
	SPI_WriteData(0x37);
	SPI_WriteData(0x37);
	SPI_WriteData(0x20);
	SPI_WriteData(0x31);
	SPI_WriteData(0x46);
	SPI_WriteData(0x8A);
	SPI_WriteData(0x57);
	SPI_WriteData(0x9B);
	SPI_WriteData(0x20);
	SPI_WriteData(0x31);
	SPI_WriteData(0x46);
	SPI_WriteData(0x8A);
	SPI_WriteData(0x57);
	SPI_WriteData(0x9B);
	SPI_WriteData(0x07);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x02);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE0);	//SET GAMMA
	SPI_WriteData(0x00);
	SPI_WriteData(0x08);
	SPI_WriteData(0x0D);
	SPI_WriteData(0x2D);
	SPI_WriteData(0x34);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x19);
	SPI_WriteData(0x38);
	SPI_WriteData(0x09);
	SPI_WriteData(0x0E);
	SPI_WriteData(0x0E);
	SPI_WriteData(0x12);
	SPI_WriteData(0x14);
	SPI_WriteData(0x12);
	SPI_WriteData(0x14);
	SPI_WriteData(0x13);
	SPI_WriteData(0x19);
	SPI_WriteData(0x00);
	SPI_WriteData(0x08);
	SPI_WriteData(0x0D);
	SPI_WriteData(0x2D);
	SPI_WriteData(0x34);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x19);
	SPI_WriteData(0x38);
	SPI_WriteData(0x09);
	SPI_WriteData(0x0E);
	SPI_WriteData(0x0E);
	SPI_WriteData(0x12);
	SPI_WriteData(0x14);
	SPI_WriteData(0x12);
	SPI_WriteData(0x14);
	SPI_WriteData(0x13);
	SPI_WriteData(0x19);

	SPI_WriteComm(0xC1);	//set DGC
	SPI_WriteData(0x01);	//enable DGC function
	SPI_WriteData(0x02);	//SET R-GAMMA
	SPI_WriteData(0x08);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1A);
	SPI_WriteData(0x22);
	SPI_WriteData(0x2A);
	SPI_WriteData(0x31);
	SPI_WriteData(0x36);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x48);
	SPI_WriteData(0x51);
	SPI_WriteData(0x58);
	SPI_WriteData(0x60);
	SPI_WriteData(0x68);
	SPI_WriteData(0x70);
	SPI_WriteData(0x78);
	SPI_WriteData(0x80);
	SPI_WriteData(0x88);
	SPI_WriteData(0x90);
	SPI_WriteData(0x98);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA7);
	SPI_WriteData(0xAF);
	SPI_WriteData(0xB6);
	SPI_WriteData(0xBE);
	SPI_WriteData(0xC7);
	SPI_WriteData(0xCE);
	SPI_WriteData(0xD6);
	SPI_WriteData(0xDE);
	SPI_WriteData(0xE6);
	SPI_WriteData(0xEF);
	SPI_WriteData(0xF5);
	SPI_WriteData(0xFB);
	SPI_WriteData(0xFC);
	SPI_WriteData(0xFE);
	SPI_WriteData(0x8C);
	SPI_WriteData(0xA4);
	SPI_WriteData(0x19);
	SPI_WriteData(0xEC);
	SPI_WriteData(0x1B);
	SPI_WriteData(0x4C);
	SPI_WriteData(0x40);
	SPI_WriteData(0x02);	//SET G-Gamma
	SPI_WriteData(0x08);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1A);
	SPI_WriteData(0x22);
	SPI_WriteData(0x2A);
	SPI_WriteData(0x31);
	SPI_WriteData(0x36);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x48);
	SPI_WriteData(0x51);
	SPI_WriteData(0x58);
	SPI_WriteData(0x60);
	SPI_WriteData(0x68);
	SPI_WriteData(0x70);
	SPI_WriteData(0x78);
	SPI_WriteData(0x80);
	SPI_WriteData(0x88);
	SPI_WriteData(0x90);
	SPI_WriteData(0x98);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA7);
	SPI_WriteData(0xAF);
	SPI_WriteData(0xB6);
	SPI_WriteData(0xBE);
	SPI_WriteData(0xC7);
	SPI_WriteData(0xCE);
	SPI_WriteData(0xD6);
	SPI_WriteData(0xDE);
	SPI_WriteData(0xE6);
	SPI_WriteData(0xEF);
	SPI_WriteData(0xF5);
	SPI_WriteData(0xFB);
	SPI_WriteData(0xFC);
	SPI_WriteData(0xFE);
	SPI_WriteData(0x8C);
	SPI_WriteData(0xA4);
	SPI_WriteData(0x19);
	SPI_WriteData(0xEC);
	SPI_WriteData(0x1B);
	SPI_WriteData(0x4C);
	SPI_WriteData(0x40);
	SPI_WriteData(0x02);	//SET B-Gamma
	SPI_WriteData(0x08);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1A);
	SPI_WriteData(0x22);
	SPI_WriteData(0x2A);
	SPI_WriteData(0x31);
	SPI_WriteData(0x36);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x48);
	SPI_WriteData(0x51);
	SPI_WriteData(0x58);
	SPI_WriteData(0x60);
	SPI_WriteData(0x68);
	SPI_WriteData(0x70);
	SPI_WriteData(0x78);
	SPI_WriteData(0x80);
	SPI_WriteData(0x88);
	SPI_WriteData(0x90);
	SPI_WriteData(0x98);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA7);
	SPI_WriteData(0xAF);
	SPI_WriteData(0xB6);
	SPI_WriteData(0xBE);
	SPI_WriteData(0xC7);
	SPI_WriteData(0xCE);
	SPI_WriteData(0xD6);
	SPI_WriteData(0xDE);
	SPI_WriteData(0xE6);
	SPI_WriteData(0xEF);
	SPI_WriteData(0xF5);
	SPI_WriteData(0xFB);
	SPI_WriteData(0xFC);
	SPI_WriteData(0xFE);
	SPI_WriteData(0x8C);
	SPI_WriteData(0xA4);
	SPI_WriteData(0x19);
	SPI_WriteData(0xEC);
	SPI_WriteData(0x1B);
	SPI_WriteData(0x4C);
	SPI_WriteData(0x40);

	SPI_WriteComm(0x11);	//Sleep Out

	LCD_delay(120);

	SPI_WriteComm(0x2A);	//Set Column Address (2Ah)
	SPI_WriteData(0x00);	//SC[15:8]
	SPI_WriteData(0x00);	//SC[7:0]
	SPI_WriteData(0x01);	//EC[15:8]
	SPI_WriteData(0xDF);	//EC[7:0]

	SPI_WriteComm(0x2B);	//Set Row Address (2Bh)
	SPI_WriteData(0x00);	//SC[15:8]
	SPI_WriteData(0x00);	//SC[7:0]
	SPI_WriteData(0x03);	//EC[15:8]
	SPI_WriteData(0x1F);	//EC[7:0]

	SPI_WriteComm(0x36);	//Set_address_mode (36h)
	SPI_WriteData(0x68);	//BGR
	//SPI_WriteData(0x00);	//RGB

	SPI_WriteComm(0x2C);	//Write_memory_start (2Ch)

	LCD_delay(10);

	SPI_WriteComm(0x20);	//Exit_inversion_mode (20h)

	SPI_WriteComm(0x13);	//Return to Normal mode (13h)

	LCD_delay(10);

	SPI_WriteComm(0x3A);	//Set_pixel_format (3Ah)
	SPI_WriteData(0x77);	//24 Bit/Pixel
	//SPI_WriteData(0x66);	//18 Bit/Pixel
	//SPI_WriteData(0x55);	//16 Bit/Pixel

	SPI_WriteComm(0xCC);	//SETPANEL (CCh)
	SPI_WriteData(0xF8);

	SPI_WriteComm(0x11);

	LCD_delay(120);

	SPI_WriteComm(0x29);	//Set_display_on (29h)
}

/*************************** End of file ****************************/
