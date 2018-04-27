/*
** ###################################################################
**     Processors:          MIMXRT1052CVL5B
**                          MIMXRT1052DVL6B
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          MCUXpresso Compiler
**
**     Reference manual:    IMXRT1050RM Rev.1, 03/2018
**     Version:             rev. 0.1, 2017-01-10
**     Build:               b180307
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     The Clear BSD License
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2018 NXP
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without
**     modification, are permitted (subject to the limitations in the
**     disclaimer below) provided that the following conditions are met:
**
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**
**     * Neither the name of the copyright holder nor the names of its
**       contributors may be used to endorse or promote products derived from
**       this software without specific prior written permission.
**
**     NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
**     GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
**     HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
**     WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
**     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
**     LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
**     CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
**     SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
**     BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
**     WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
**     OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
**     IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 0.1 (2017-01-10)
**         Initial version.
**
** ###################################################################
*/

/*!
 * @file MIMXRT1052
 * @version 0.1
 * @date 2017-01-10
 * @brief Device specific configuration file for MIMXRT1052 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "fsl_device_registers.h"



/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

#if (defined(XIP_BOOT_HEADER_DCD_ENABLE) && (XIP_BOOT_HEADER_DCD_ENABLE == 1))
/* ----------------------------------------------------------------------------
   -- SDRAM_WaitIpCmdDone()
   ---------------------------------------------------------------------------- */
void SDRAM_WaitIpCmdDone(void)
{
  uint32_t reg;
  do
  {
    reg = SEMC->INTR;
  }while((reg & 0x3) == 0);
}

/* ----------------------------------------------------------------------------
   -- SDRAM_Init()
   ---------------------------------------------------------------------------- */
void SDRAM_Init(void)
{
  /* Clock init */
  //CCM->CCGR3 |= CCM_CCGR3_CG2(3);//SEMC clock enable
  //CCM->CCGR4 |= CCM_CCGR4_CG1(3);//IOMUXC clock enable
  CCM->CCGR0 = 0xFFFFFFFF;
  CCM->CCGR1 = 0xFFFFFFFF;
  CCM->CCGR2 = 0xFFFFFFFF;
  CCM->CCGR3 = 0xFFFFFFFF;
  CCM->CCGR4 = 0xFFFFFFFF;
  CCM->CCGR5 = 0xFFFFFFFF;
  CCM->CCGR5 = 0xFFFFFFFF;
  
  
  CCM_ANALOG->PLL_SYS = 0x00002001;//Enable PLL output, Fout=Fref*22
  CCM_ANALOG->PFD_528 = 0x001d0000;
  CCM->CBCDR = 0x00010D40;
   
  /* SDRAM IO MUX Config */
  IOMUXC->SW_MUX_CTL_PAD[0] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[1] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[2] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[3] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[4] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[5] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[6] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[7] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[8] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[9] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[10] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[11] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[12] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[13] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[14] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[15] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[16] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[17] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[18] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[19] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[20] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[21] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[22] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[23] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[24] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[25] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[26] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[27] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[28] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[29] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[30] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[31] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[32] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[33] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[34] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[35] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[36] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[37] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[38] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[39] = 0x00000010;
  IOMUXC->SW_MUX_CTL_PAD[40] = 0x00000000;
  IOMUXC->SW_MUX_CTL_PAD[41] = 0x00000000;
  
  /* SDRAM IO Pad Config */
  IOMUXC->SW_PAD_CTL_PAD[0] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[1] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[2] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[3] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[4] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[5] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[6] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[7] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[8] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[9] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[10] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[11] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[12] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[13] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[14] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[15] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[16] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[17] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[18] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[19] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[20] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[21] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[22] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[23] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[24] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[25] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[26] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[27] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[28] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[29] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[30] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[31] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[32] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[33] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[34] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[35] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[36] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[37] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[38] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[39] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[40] = 0x000110F9;
  IOMUXC->SW_PAD_CTL_PAD[41] = 0x000110F9;
  
  SEMC->MCR = 0x1000E000;
  SEMC->BMCR0 = 0x00030524;
  SEMC->BMCR1 = 0x06030524;
  SEMC->BR[0] = 0x8000001B;
  SEMC->BR[1] = 0x90000021;
  SEMC->IOCR = 0x00000008;
  SEMC->SDRAMCR0 = 0x00000B27;
  SEMC->SDRAMCR1 = 0x00100100;
  SEMC->SDRAMCR2 = 0x00020201;
  SEMC->SDRAMCR3 = 0x08193D0E;
  SEMC->DBICR0 = 0x00000021;
  SEMC->DBICR1 = 0x00888888;
  SEMC->IPCR1 = 0x00000002;
  SEMC->IPCR2 = 0x00000000;
  
  /* Config SDR Controller Registers */
  SEMC->MCR = 0x10000004;
  SEMC->BMCR0 = 0x00030524;
  SEMC->BMCR1 = 0x06030524;
  SEMC->BR[0] = 0x8000001B;
  SEMC->BR[1] = 0x8200001B;
  SEMC->BR[2] = 0x8400001B;
  SEMC->BR[3] = 0x8600001B;
  SEMC->BR[4] = 0x90000021;
  SEMC->BR[5] = 0xA0000019;
  SEMC->BR[6] = 0xA8000017;
  SEMC->BR[7] = 0xA900001B;
  SEMC->BR[8] = 0x00000021;
  SEMC->IOCR = 0x000079A8;//SEMC_CCSX0 as NOR CE, SEMC_CSX1 as PSRAM CE, SEMC_CSX2 as NAND CE, SEMC_CSX3 as DBI CE
    
  // __writeMemory32(0x00000008, 0x402F0004, "Memory"); // IOCR, SEMC_CCSX0 as SDRAM_CS1
  SEMC->SDRAMCR0 = 0x00000F31;
  SEMC->SDRAMCR1 = 0x00652922;
  SEMC->SDRAMCR2 = 0x00010920;
  SEMC->SDRAMCR3 = 0x50210A08;
  
  SEMC->DBICR0 = 0x00000021;
  SEMC->DBICR1 = 0x00888888;
  SEMC->IPCR1 = 0x00000002;
  SEMC->IPCR2 = 0x00000000;
  
  SEMC->IPCR0 = 0x80000000;
  SEMC->IPCMD = 0xA55A000F;//SD_CC_IPREA
  SDRAM_WaitIpCmdDone();
  SEMC->IPCR0 = 0x80000000;
  SEMC->IPCMD = 0xA55A000C;//SD_CC_IAF
  SDRAM_WaitIpCmdDone();
  SEMC->IPCR0 = 0x80000000;
  SEMC->IPCMD = 0xA55A000C;//SD_CC_IAF
  SDRAM_WaitIpCmdDone();
  SEMC->IPTXDAT = 0x00000033;
  SEMC->IPCR0 = 0x80000000;
  SEMC->IPCMD = 0xA55A000A;//SD_CC_IMS
  SDRAM_WaitIpCmdDone();
  
  SEMC->SDRAMCR3 = 0x50210A09;//enable sdram self refresh again after initialization done.
}
#endif

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit (void) {
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
  SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access */
#endif /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

#if defined(__MCUXPRESSO)
    extern uint32_t g_pfnVectors[];  // Vector table defined in startup code
    SCB->VTOR = (uint32_t)g_pfnVectors;
#endif

/* Watchdog disable */

#if (DISABLE_WDOG)
    if (WDOG1->WCR & WDOG_WCR_WDE_MASK)
    {
        WDOG1->WCR &= ~WDOG_WCR_WDE_MASK;
    }
    if (WDOG2->WCR & WDOG_WCR_WDE_MASK)
    {
        WDOG2->WCR &= ~WDOG_WCR_WDE_MASK;
    }
    RTWDOG->CNT = 0xD928C520U; /* 0xD928C520U is the update key */
    RTWDOG->TOVAL = 0xFFFF;
    RTWDOG->CS = (uint32_t) ((RTWDOG->CS) & ~RTWDOG_CS_EN_MASK) | RTWDOG_CS_UPDATE_MASK;
#endif /* (DISABLE_WDOG) */

    /* Disable Systick which might be enabled by bootrom */
    if (SysTick->CTRL & SysTick_CTRL_ENABLE_Msk)
    {
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    }

/* Enable instruction and data caches */
#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    if (SCB_CCR_IC_Msk != (SCB_CCR_IC_Msk & SCB->CCR)) {
        SCB_EnableICache();
    }
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    if (SCB_CCR_DC_Msk != (SCB_CCR_DC_Msk & SCB->CCR)) {
        SCB_EnableDCache();
    }
#endif
	
#if (defined(XIP_BOOT_HEADER_DCD_ENABLE) && (XIP_BOOT_HEADER_DCD_ENABLE == 1))
	SDRAM_Init();
#endif

  SystemInitHook();
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate (void) {

    uint32_t freq;
    uint32_t PLL1MainClock;
    uint32_t PLL2MainClock;

    /* Periph_clk2_clk ---> Periph_clk */
    if (CCM->CBCDR & CCM_CBCDR_PERIPH_CLK_SEL_MASK)
    {
        switch (CCM->CBCMR & CCM_CBCMR_PERIPH_CLK2_SEL_MASK)
        {
            /* Pll3_sw_clk ---> Periph_clk2_clk ---> Periph_clk */
            case CCM_CBCMR_PERIPH_CLK2_SEL(0U):
                if(CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_BYPASS_MASK)
                {
                    freq = (((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_BYPASS_CLK_SRC_MASK) >> CCM_ANALOG_PLL_USB1_BYPASS_CLK_SRC_SHIFT) == 0U) ?
                           CPU_XTAL_CLK_HZ : CPU_CLK1_HZ;
                }
                else
                {
                    freq = (CPU_XTAL_CLK_HZ * ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_DIV_SELECT_MASK) ? 22U : 20U));
                }
                break;

            /* Osc_clk ---> Periph_clk2_clk ---> Periph_clk */
            case CCM_CBCMR_PERIPH_CLK2_SEL(1U):
                freq = CPU_XTAL_CLK_HZ;
                break;

            case CCM_CBCMR_PERIPH_CLK2_SEL(2U):
                freq = (((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_BYPASS_CLK_SRC_MASK) >> CCM_ANALOG_PLL_SYS_BYPASS_CLK_SRC_SHIFT) == 0U) ?
                   CPU_XTAL_CLK_HZ : CPU_CLK1_HZ;

            case CCM_CBCMR_PERIPH_CLK2_SEL(3U):
            default:
                freq = 0U;
                break;
        }

        freq /= (((CCM->CBCDR & CCM_CBCDR_PERIPH_CLK2_PODF_MASK) >> CCM_CBCDR_PERIPH_CLK2_PODF_SHIFT) + 1U);
    }
    /* Pre_Periph_clk ---> Periph_clk */
    else
    {
        /* check if pll is bypassed */
        if(CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_BYPASS_MASK)
        {
            PLL1MainClock = (((CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_BYPASS_CLK_SRC_MASK) >> CCM_ANALOG_PLL_ARM_BYPASS_CLK_SRC_SHIFT) == 0U) ?
                   CPU_XTAL_CLK_HZ : CPU_CLK1_HZ;
        }
        else
        {
            PLL1MainClock = ((CPU_XTAL_CLK_HZ * ((CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK) >>
                                             CCM_ANALOG_PLL_ARM_DIV_SELECT_SHIFT)) >> 1U);
        }

        /* check if pll is bypassed */
        if(CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_BYPASS_MASK)
        {
            PLL2MainClock = (((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_BYPASS_CLK_SRC_MASK) >> CCM_ANALOG_PLL_SYS_BYPASS_CLK_SRC_SHIFT) == 0U) ?
                   CPU_XTAL_CLK_HZ : CPU_CLK1_HZ;
        }
        else
        {
            PLL2MainClock = (CPU_XTAL_CLK_HZ * ((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_DIV_SELECT_MASK) ? 22U : 20U));
        }
        PLL2MainClock += ((uint64_t)CPU_XTAL_CLK_HZ * ((uint64_t)(CCM_ANALOG->PLL_SYS_NUM)));
		PLL2MainClock /= ((uint64_t)(CCM_ANALOG->PLL_SYS_DENOM));


        switch (CCM->CBCMR & CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK)
        {
            /* PLL2 ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(0U):
                freq = PLL2MainClock;
                break;

            /* PLL2 PFD2 ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(1U):
                freq = PLL2MainClock / ((CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD2_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD2_FRAC_SHIFT) * 18U;
                break;

            /* PLL2 PFD0 ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(2U):
                freq = PLL2MainClock / ((CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD0_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD0_FRAC_SHIFT) * 18U;
                break;

            /* PLL1 divided(/2) ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(3U):
                freq = PLL1MainClock / (((CCM->CACRR & CCM_CACRR_ARM_PODF_MASK) >> CCM_CACRR_ARM_PODF_SHIFT) + 1U);
                break;

            default:
                freq = 0U;
                break;
        }
    }

    SystemCoreClock = (freq / (((CCM->CBCDR & CCM_CBCDR_AHB_PODF_MASK) >> CCM_CBCDR_AHB_PODF_SHIFT) + 1U));

}

/* ----------------------------------------------------------------------------
   -- SystemInitHook()
   ---------------------------------------------------------------------------- */

__attribute__ ((weak)) void SystemInitHook (void) {
  /* Void implementation of the weak function. */
}
