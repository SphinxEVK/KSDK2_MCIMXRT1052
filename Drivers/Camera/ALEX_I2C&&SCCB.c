#include "fsl_gpio.h"
#include "ALEX_I2C&&SCCB.h"

#define _SCCB_SOFT_ 1

#if _SCCB_SOFT_
/*!
 *  @brief      SCCB起始信号
 *  @since      v5.0
 */
void SCCB_Start(void)
{
  
    SCL_L();
    SDA_H();
    SCCB_DELAY();
    SCL_H();
    SCCB_DELAY();
    SDA_L();
    SCCB_DELAY();
    SCL_L();
    SCCB_DELAY();
}

void SCCB_Stop(void)
{
    SCL_L();
    //SCCB_DELAY();
    SDA_L();
    SCCB_DELAY();
    SCL_H();
    SCCB_DELAY();
    SDA_H();
    SCCB_DELAY();
    SCL_L();
    SCCB_DELAY();
}

/*!
 *  @brief      SCCB应答信号
 *  @since      v5.0
 */
static void SCCB_Ack(void)
{
    SCL_L();
    SCCB_DELAY();
    SDA_L();
    SCCB_DELAY();
    SCL_H();
    SCCB_DELAY();
    SCL_L();
    SCCB_DELAY();
}

/*!
 *  @brief      SCCB无应答信号
 *  @since      v5.0
 */
static void SCCB_NoAck(void)
{
    SCL_L();
    SCCB_DELAY();
    SDA_H();
    SCCB_DELAY();
    SCL_H();
    SCCB_DELAY();
    SCL_L();
    SCCB_DELAY();
}

/*!
 *  @brief      SCCB 等待应答
 *  @return     应答结果（0表示无应答，1表示有应答）
 *  @since      v5.0
 */
int SCCB_WaitAck(void)
{
    SCL_L();
    //SDA_H();
    SDA_DDR_IN();

    SCL_H();

    SCCB_DELAY();

    if(SDA_IN())           //应答为高电平，异常，通信失败
    {
        SCL_L();
        SCCB_DELAY();
        SDA_DDR_OUT();
        return 0;
    }
    SCL_L();
    SCCB_DELAY();
    SDA_DDR_OUT();
    return 1;
}

/*!
 *  @brief      SCCB 发送的数据
 *  @param      SendByte    需要发送的数据
 *  @since      v5.0
 */
void SCCB_SendByte(uint8_t SendByte)
{
    uint8_t i = 8;
    SCL_L();
    SCCB_DELAY();
    while(i--)
    {

        if(SendByte & 0x80)     //SDA 输出数据
        {
            SDA_H();
        }
        else
        {
            SDA_L();
        }
        SendByte <<= 1;
        SCCB_DELAY();
        SCL_H();                //SCL 拉高，采集信号
        SCCB_DELAY();
        SCL_L();
        SCCB_DELAY();
        //SCL 时钟线拉低
        //SCCB_DELAY();
    }
    //SCL_L();
}

/*!
 *  @brief      接收SCCB总线的数据
 *  @return     接收到的数据
 *  @since      v5.0
 */
int SCCB_ReceiveByte(void)
{
    uint8_t i = 8;
    uint8_t ReceiveByte = 0;

    //SDA_H();
    //SCCB_DELAY();
    SDA_DDR_IN();

    while(i--)
    {
        ReceiveByte <<= 1;
        SCL_L();
        SCCB_DELAY();
        SCL_H();
        SCCB_DELAY();

        if(SDA_IN())
        {
            ReceiveByte |= 0x01;
        }
    }
    SCL_L();
    SCCB_DELAY();
    SDA_DDR_OUT();
    return ReceiveByte;
}

// ---------------------------------------------------------------------------------------- //

void SCCB_UnlockI2C()
{
    //SCCB_Stop();
    
    SCCB_DELAY();
    SCCB_DELAY();
    
    for(uint8_t i=0; i<9; i++)
    {
        SCL_L();
        SCCB_DELAY();
        SCL_H();
        SCCB_DELAY();
    }
    
    SCCB_DELAY();
}

int16_t SCCB_WriteByte_soft(uint8_t SlaveAddress, uint8_t REG_Address, uint16_t REG_data)
{
    SCCB_SendByte( SlaveAddress<<1|0 );                    /* 器件地址 */
    if(!SCCB_WaitAck())
    {
        SCCB_Stop();
        return 0;
    }
    SCCB_SendByte(REG_Address);   /* 设置低起始地址 */
    SCCB_WaitAck();
    SCCB_SendByte((uint8_t)(REG_data>>8));
    SCCB_WaitAck();
    SCCB_SendByte((uint8_t)REG_data&0x00FF);
    SCCB_WaitAck();
    return 1;
}


int16_t SCCB_ReadByte_soft(uint8_t SlaveAddress, uint8_t REG_Address)
{
    uint16_t pBuffer = 0;
    SCCB_Start();
    SCCB_SendByte(SlaveAddress<<1|0);         /* 器件地址 */
    if(!SCCB_WaitAck())
    {
        SCCB_Stop();
        return -1;
    }
    SCCB_SendByte(REG_Address);           /* 设置低起始地址 */
    SCCB_WaitAck();
    SCCB_DELAY();
    SCCB_Start();
    SCCB_SendByte(SlaveAddress<<1|1);               /* 器件地址 */

    if(!SCCB_WaitAck())
    {
        SCCB_Stop();
        return -3;
    }
    
    pBuffer = SCCB_ReceiveByte();       // 只读一个数据
    SCCB_Ack();
    pBuffer <<= 8;
    pBuffer |=SCCB_ReceiveByte();
    SCCB_NoAck();
    
    return pBuffer;
}
#endif

