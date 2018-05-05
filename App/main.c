/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "SEGGER_RTT.h"

#include "bsp_sdram.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
void delay(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* The PIN status */
volatile bool g_pinSet = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 8000000UL; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin, clock, debug console init */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    //BOARD_InitDebugConsole();

    /* Print a note to terminal. */
    SEGGER_RTT_printf(0, "\r\n GPIO Driver example\r\n");
    SEGGER_RTT_printf(0, "\r\n The LED is blinking.\r\n");

    /* Init output LED GPIO. */
    GPIO_PinInit(GPIO1, 15U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 1, kGPIO_NoIntmode});
	
	//uint8_t string[10] = {0x00};
	//uint32_t vendorID = QSPIFlash_25Q128JVSQ.init((void*)FlexSPI_AHB_BASE, 0, 0);
	//QSPIFlash_25Q128JVSQ.write(FlexSPI_AHB_BASE, sizeof("HELLOWORLD"), "HELLOWORLD");
	//memcpy(string, (uint8_t*)FlexSPI_AHB_BASE, sizeof("HELLOWORLD"));
	
	//uint8_t str[] = {0x01, 0x02, 0x03, 0x04};
	
#if (defined(USE_EXTERNAL_SDRAM) && (USE_EXTERNAL_SDRAM == 1))	
	BOARD_InitSEMC();
	//SCB_DisableDCache();
    //SCB_DisableICache();
	if(1==sdram_test(0x12ABCDEF))
	{
		//sdram test success
		SEGGER_RTT_printf(0, "\r\n sdram test success\r\n");
	}
	else
	{
		//sdram test failed
		SEGGER_RTT_printf(0, "\r\n sdram test failed\r\n");
	}
	SCB_EnableDCache();
	SCB_EnableICache();
#endif

    while (1)
    {
        delay();
        GPIO_WritePinOutput(GPIO1, 15U, !GPIO_PinRead(GPIO1, 15U));
    }
}
