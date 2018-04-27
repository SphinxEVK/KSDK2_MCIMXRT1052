#include "board.h"
//#include "bsp_delay.h"
#include "bsp_sdram.h"

/**
 * SDRAM模式寄存器
 */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0003)
#define SDRAM_MODEREG_BURST_LENGTH_PAGE          ((uint16_t)0x0007)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)


#ifndef NULL
  #define NULL  (void *)0
#endif

/* 内存数据总线宽度 */
typedef uint16_t databus_width;

static databus_width   memtest_databus(volatile databus_width * address);
static databus_width * memtest_addressbus(volatile databus_width * address_base, unsigned long length);
static databus_width * memtest_device(volatile databus_width * address_base, unsigned long length);

/*******************************************************************************
* 名    称: memtest_databus
* 入口参数:
*  > address - 测试地址
* 出口参数: 无
* 返 回 值: 成功返回0, 否则返回首次出错的参数(数据总线位)
* 描    述: 内存数据总线测试
* 注意事项: 该操作会摧毁内存指定地址的数据, Cache可能会影响测试的结果, 敬请注意
*******************************************************************************/
databus_width memtest_databus(volatile databus_width * address)
{
  databus_width pattern;

  for(pattern = 1; pattern != 0; pattern <<= 1)
  {
    *address = pattern;

    /* 注意Cache可能会影响测试结果的正确性 */
    if(*address != pattern)
    {
      return (pattern);
    }
  }

  return (0);
}

/*******************************************************************************
* 名    称: memtest_addressbus
* 入口参数:
*  > address_base - 起始测试地址
*  > length - 测试长度
* 出口参数: 无
* 返 回 值: 成功返回NULL, 否则返回首次出错的地址
* 描    述: 内存地址总线测试
* 注意事项: 该操作会摧毁内存中所有数据, Cache可能会影响测试的结果, 敬请注意
*******************************************************************************/
databus_width * memtest_addressbus(volatile databus_width * address_base, unsigned long length)
{
  unsigned long address_mask = (length / sizeof(databus_width) - 1);
  unsigned long offset;
  unsigned long test_offset;

  databus_width pattern     = (databus_width)0xAAAAAAAA;
  databus_width anti_patten = (databus_width)0x55555555;

  /* 写入初始默认值 */
  for(offset = 1; (offset & address_mask) != 0; offset <<= 1)
  {
    address_base[offset] = pattern;
  }

  /* 检查地址总线位是否总是为1 */
  test_offset = 0;
  address_base[test_offset] = anti_patten;

  for(offset = 1; (offset & address_mask) != 0; offset <<= 1)
  {
    if(address_base[offset] != pattern)
    {
      return ((databus_width *) &address_base[offset]);
    }
  }

  address_base[test_offset] = pattern;

  /* 检查地址总线位是否总是为0或短路 */
  for(test_offset = 1; (test_offset & address_mask) != 0; test_offset <<= 1)
  {
    address_base[test_offset] = anti_patten;

    if(address_base[0] != pattern)
    {
      return ((databus_width *) &address_base[test_offset]);
    }

    for(offset = 1; (offset & address_mask) != 0; offset <<= 1)
    {
      if((address_base[offset] != pattern) && (offset != test_offset))
      {
        return ((databus_width *) &address_base[test_offset]);
      }
    }

    address_base[test_offset] = pattern;
  }

  return (NULL);
}

/*******************************************************************************
* 名    称: memtest_device
* 入口参数:
*  > address_base - 起始测试地址
*  > length - 测试长度
* 出口参数: 无
* 返 回 值: 成功返回NULL, 否则返回首次出错的地址
* 描    述: 内存测试
* 注意事项: 该操作会摧毁内存中所有数据, Cache可能会影响测试的结果, 敬请注意
*******************************************************************************/
databus_width * memtest_device(volatile databus_width * address_base, unsigned long length)
{
  unsigned long offset;
  unsigned long word_length = length / sizeof(databus_width);

  databus_width pattern;
  databus_width anti_patten;

  /* 写入初始默认值 */
  for(pattern = 1, offset = 0; offset < word_length; pattern++, offset++)
  {
    address_base[offset] = pattern;
  }

  /* 检查每个地址存储数据的正确性并翻该转数据供下一步测试 */
  for(pattern = 1, offset = 0; offset < word_length; pattern++, offset++)
  {
    if(address_base[offset] != pattern)
    {
      return ((databus_width *) &address_base[offset]);
    }

    anti_patten = ~pattern;
    address_base[offset] = anti_patten;
  }

  /* 检查每个地址存储数据(已翻转)的正确性并清0 */
  for(pattern = 1, offset = 0; offset < word_length; pattern++, offset++)
  {
    anti_patten = ~pattern;

    if(address_base[offset] != anti_patten)
    {
      return ((databus_width *) &address_base[offset]);
    }
  }

  return (NULL);
}

/*******************************************************************************
* 名    称: memtest
* 入口参数: 无
* 出口参数: 无
* 返 回 值: 成功返回0
* 描    述: 内存测试
* 注意事项: 该操作会摧毁内存中所有数据, Cache可能会影响测试的结果, 敬请注意
*******************************************************************************/
int memtest(void)
{
#define BASE_ADDRESS  ((volatile databus_width *)SDRAM_ADDR)
#define NUM_BYTES     (SDRAM_SIZE)

  if((memtest_databus(BASE_ADDRESS) != 0) ||
     (memtest_addressbus(BASE_ADDRESS, NUM_BYTES) != NULL) ||
     (memtest_device(BASE_ADDRESS, NUM_BYTES) != NULL))
  {
    return (-1);
  }
  else
  {
    return (0);
  }

#undef BASE_ADDRESS
#undef NUM_BYTES
}

/*******************************************************************************
* 名    称: sdram_test
* 入口参数:
*  > data - 测试数据
* 出口参数: 无
* 返 回 值: 成功返回0, 否则返回错误码
* 描    述: SDRAM读写测试
* 注意事项: 该操作会摧毁SDRAM中保存的所有数据, 敬请注意
*******************************************************************************/
int32_t sdram_test(uint32_t data)
{
  /* 不允许编译器优化 */
  volatile uint32_t * write_uint32, *read_uint32;
  volatile uint32_t pattern;
  volatile uint32_t i;

  if(memtest() != 0)
  {
    return 0;
  }

  /* 四字节写入 */
  write_uint32 = (uint32_t *)SDRAM_ADDR;
  read_uint32 = (uint32_t *)SDRAM_ADDR;
  pattern = data & 0xFFFFFFFF;

  for(i = 0; i < (SDRAM_SIZE / sizeof(pattern)); i++)
  {
    *write_uint32++ = pattern;
  }

  /* 四字节读取 */
  for(i = 0; i < (SDRAM_SIZE / sizeof(pattern)); i++)
  {
    if(*read_uint32++ != ((uint32_t)pattern))
    {
      return 0;
    }
  }

  return 1;
}