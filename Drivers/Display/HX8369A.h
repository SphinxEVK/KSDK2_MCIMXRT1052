/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2011  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.12 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : HX8369A.h
Purpose     : Display driver configuration file
----------------------------------------------------------------------
*/

#ifndef __HX8369A_H
#define	__HX8369A_H

#include "fsl_common.h"

/*********************************************************
* HX8369A_SPI GPIO端口、引脚号及IOMUXC复用宏定义
*********************************************************/
#define HX8369A_SPI_CS_GPIO						GPIO1
#define HX8369A_SPI_CS_GPIO_PIN 		    	(11U)
#define HX8369A_SPI_CS_IOMUXC					IOMUXC_GPIO_AD_B0_11_GPIO1_IO11

#define HX8369A_SPI_MOSI_GPIO					GPIO1
#define HX8369A_SPI_MOSI_GPIO_PIN				(17U)
#define HX8369A_SPI_MOSI_IOMUXC					IOMUXC_GPIO_AD_B1_01_GPIO1_IO17

#define HX8369A_SPI_CLK_GPIO					GPIO1
#define HX8369A_SPI_CLK_GPIO_PIN				(16U)
#define HX8369A_SPI_CLK_IOMUXC					IOMUXC_GPIO_AD_B1_00_GPIO1_IO16

#define HX8369A_RST_GPIO						GPIO1
#define HX8369A_RST_GPIO_PIN 		   	   		(2U)
#define HX8369A_RST_IOMUXC			        	IOMUXC_GPIO_AD_B0_02_GPIO1_IO02

/* 使用标准的固件库控制IO
使用范例：CORE_BOARD_LED(ON); */

#define HX8369A_SPI_CS(a)	if (a)		\
								GPIO_PinWrite(HX8369A_SPI_CS_GPIO, HX8369A_SPI_CS_GPIO_PIN, 1U);\
							else		\
								GPIO_PinWrite(HX8369A_SPI_CS_GPIO, HX8369A_SPI_CS_GPIO_PIN, 0U);

#define HX8369A_SPI_MOSI(a)	if (a)		\
								GPIO_PinWrite(HX8369A_SPI_MOSI_GPIO, HX8369A_SPI_MOSI_GPIO_PIN, 1U);\
							else		\
								GPIO_PinWrite(HX8369A_SPI_MOSI_GPIO, HX8369A_SPI_MOSI_GPIO_PIN, 0U);

#define HX8369A_SPI_CLK(a)	if (a)		\
								GPIO_PinWrite(HX8369A_SPI_CLK_GPIO, HX8369A_SPI_CLK_GPIO_PIN, 1U);\
							else		\
								GPIO_PinWrite(HX8369A_SPI_CLK_GPIO, HX8369A_SPI_CLK_GPIO_PIN, 0U);

#define HX8369A_RST(a)		if (a)		\
								GPIO_PinWrite(HX8369A_RST_GPIO, HX8369A_RST_GPIO_PIN, 1U);\
							else		\
								GPIO_PinWrite(HX8369A_RST_GPIO, HX8369A_RST_GPIO_PIN, 0U);


///* 直接操作寄存器的方法控制IO */
//#define digitalHi(p,i)		  {p->DR |= (1U << i);}	  //输出为高电平
//#define digitalLo(p,i)		 	{p->DR &= ~(1U << i);}  //输出低电平
//#define digitalToggle(p,i)  {p->DR ^= (1U<<i);}     //输出反转状态

//void WriteComm(uint16_t CMD);
//void WriteData(uint16_t tem_data);
extern void HX8369A_LCD_Init(void);


#endif /* HX8369A_H */

/*************************** End of file ****************************/
