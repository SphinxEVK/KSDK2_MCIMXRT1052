#ifndef __BSP_SDRAM_H__
#define __BSP_SDRAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SDRAM_ADDR  0x81E00000ul
#define SDRAM_SIZE  0x001FFFFFul

__ramfunc int32_t sdram_test(uint32_t data);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_SDRAM_H__ */
