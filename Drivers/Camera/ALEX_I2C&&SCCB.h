#ifndef _ALEX_SCCB_H_
#define _ALEX_SCCB_H_

#include "appinc.h"

//SCCB ¹Ü½ÅÅäÖÃ
#define SCCB_SCL        GPIO_AD_B1_01 //lpi2c~SDA, GPIO1_IO17
#define SCCB_SDA        GPIO_AD_B1_00 //lpi2c~SCL, GPIO1_IO16

#define SCL_H()         (GPIO1->DR |=  (1U<<17U)) //GPIOE->PSOR = 1U << 11U;
#define SCL_L()         (GPIO1->DR &= ~(1U<<17U)) //GPIOE->PCOR = 1U << 11U;
#define SCL_DDR_OUT()   do{GPIO1->GDIR &= ~(1U<<17U); GPIO1->GDIR |= (1U<<17U);}while(0) //do{GPIOE->PDDR&=(uint32_t)~(1U<<11U);GPIOE->PDDR|=(uint32_t)(1U<<11U);}while(0) 
#define SCL_DDR_IN()    do{GPIO1->GDIR &= ~(1U<<17U);}while(0) //do{GPIOE->PDDR&=(uint32_t)~(1U<<11U);}while(0)
                                    
#define SDA_H()         (GPIO1->DR |=  (1U<<16U)) //GPIOE->PSOR = 1U << 10U;
#define SDA_L()         (GPIO1->DR &= ~(1U<<16U)) //GPIOE->PCOR = 1U << 10U;
#define SDA_IN()        (((GPIO1->DR)>>16U)&0x1U) //((GPIOE->PDIR&(1U<<10U))>>10U)
#define SDA_DDR_OUT()   do{GPIO1->GDIR &= ~(1U<<16U); GPIO1->GDIR |=  (1U<<16U);}while(0) //do{GPIOE->PDDR&=(uint32_t)~(1U<<10U);GPIOE->PDDR|=(uint32_t)(1U<<10U);}while(0)
#define SDA_DDR_IN()    do{GPIO1->GDIR &= ~(1U<<16U);}while(0) //do{GPIOE->PDDR&=(uint32_t)~(1U<<10U);}while(0)

#define SCCB_DELAY()    ALEX_CPU_Delay(600)

extern void SCCB_UnlockI2C(void);
extern void SCCB_Start(void);
extern void SCCB_Stop(void);
extern int16_t SCCB_WriteByte_soft(uint8_t SlaveAddress, uint8_t REG_Address, uint16_t REG_data);
extern int16_t SCCB_ReadByte_soft(uint8_t SlaveAddress, uint8_t REG_Address);

#endif