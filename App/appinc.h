#ifndef _ALEXINC_H_
#define _ALEXINC_H_

#include "stdint.h"
#include "stdbool.h"
#include "fsl_common.h"

#pragma optimize=none
static void ALEX_CPU_Delay(volatile uint32_t cnt)
{
    for (uint32_t i = 0; i < cnt; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

#pragma optimize=none
static void ALEX_lptmr_DelayMs(uint32_t cnt)
{
	return;
}

#endif