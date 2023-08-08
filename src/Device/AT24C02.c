/*
 *@Note
 I2C�ӿڲ���EEPROM���裺
 I2C1_SCL(PB10)��I2C1_SDA(PB11)��

 �������裺
 READ EEPROM��Start + 0xA0 + 8bit Data Address + Start + 0xA1 + Read Data + Stop.
 WRITE EERPOM��Start + 0xA0 + 8bit Data Address + Write Data + Stop.

*/

#include "debug.h"

/**********************************************************************
*@Note:
AT24Cxx��

READ EEPROM��Start + 0xA0 + 8bit Data Address + Start + 0xA1 + Read Data + Stop.
WRITE EERPOM��Start + 0xA0 + 8bit Data Address + Write Data + Stop.
*******************************************************************************/
/* EERPOM DATA ADDRESS Length Definition */
#define Address_8bit  0
#define Address_16bit  1

/* EERPOM DATA ADDRESS Length Selection */
#define Address_Lenth   Address_8bit
//#define Address_Lenth   Address_16bit

/* Global define */
#define SIZE sizeof(TEXT_Buffer)

/* Global Variable */
const u8 TEXT_Buffer[]={"CH32F10x I2C TEST"};


/*********************************************************************
 * @fn      AT24CXX_ReadOneByte
 *
 * @brief   Read one data from EEPROM.
 *
 * @param   ReadAddr - Read frist address.
 *
 * @return  temp - Read data.
 */
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
    u8 temp=0;

    while( I2C_GetFlagStatus( I2C2, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( I2C2, ENABLE );

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C2, 0XA0, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

#if (Address_Lenth  == Address_8bit)
    I2C_SendData( I2C2, (u8)(ReadAddr&0x00FF) );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#elif (Address_Lenth  == Address_16bit)
    I2C_SendData( I2C2, (u8)(ReadAddr>>8) );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_SendData( I2C2, (u8)(ReadAddr&0x00FF) );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#endif

    I2C_GenerateSTART( I2C2, ENABLE );

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C2, 0XA0, I2C_Direction_Receiver );

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) );
    while( I2C_GetFlagStatus( I2C2, I2C_FLAG_RXNE ) ==  RESET ) {
        I2C_AcknowledgeConfig( I2C2, DISABLE );
    }

    temp = I2C_ReceiveData( I2C2 );
    I2C_GenerateSTOP( I2C2, ENABLE );

    return temp;
}

/*********************************************************************
 * @fn      AT24CXX_WriteOneByte
 *
 * @brief   Write one data to EEPROM.
 *
 * @param   WriteAddr - Write frist address.
 *
 * @return  DataToWrite - Write data.
 */
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
    while( I2C_GetFlagStatus( I2C2, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( I2C2, ENABLE );

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C2, 0XA0, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

#if (Address_Lenth  == Address_8bit)
    I2C_SendData( I2C2, (u8)(WriteAddr&0x00FF) );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#elif (Address_Lenth  == Address_16bit)
    I2C_SendData( I2C2, (u8)(WriteAddr>>8) );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_SendData( I2C2, (u8)(WriteAddr&0x00FF) );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#endif

    if( I2C_GetFlagStatus( I2C2, I2C_FLAG_TXE ) !=  RESET )
    {
        I2C_SendData( I2C2, DataToWrite );
    }

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
    I2C_GenerateSTOP( I2C2, ENABLE );
}

/*********************************************************************
 * @fn      AT24CXX_Read
 *
 * @brief   Read multiple data from EEPROM.
 *
 * @param   ReadAddr - Read frist address. (AT24c02: 0~255)
 *          pBuffer - Read data.
 *          NumToRead - Data number.
 *
 * @return  none
 */
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
    while(NumToRead)
    {
        *pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}

/*********************************************************************
 * @fn      AT24CXX_Write
 *
 * @brief   Write multiple data to EEPROM.
 *
 * @param   WriteAddr - Write frist address. (AT24c02: 0~255)
 *          pBuffer - Write data.
 *          NumToWrite - Data number.
 *
 * @return  none
 */
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
    while(NumToWrite--)
    {
        AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
        WriteAddr++;
        pBuffer++;
        Delay_Ms(2);
    }
}

