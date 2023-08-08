#include "EEPROM.h"
#include "string.h"

#define EEPROM_TIME_WRADDR      0x00
#define EEPROM_CONFIG_WRADDR    0x80

#define CRC_TIME_PASSWORD       0x1111
#define CRC_CONFIG_PASSWORD     0x2222

struct EEPROM_TimeVal_str {
    uint16_t CRC_Num;
    Undulation_Time Undul_Time;
};

struct EEPROM_ConfigVal_str {
    uint16_t CRC_Num;
    Config_Val_Str ConfigVal;
};

const uint16_t crc16_tab[256]= {0x0,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
                                0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
                                0x1231,0x210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
                                0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
                                0x2462,0x3443,0x420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
                                0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
                                0x3653,0x2672,0x1611,0x630,0x76d7,0x66f6,0x5695,0x46b4,
                                0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
                                0x48c4,0x58e5,0x6886,0x78a7,0x840,0x1861,0x2802,0x3823,
                                0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
                                0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0xa50,0x3a33,0x2a12,
                                0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
                                0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0xc60,0x1c41,
                                0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
                                0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0xe70,
                                0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
                                0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
                                0x1080,0xa1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
                                0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
                                0x2b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
                                0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
                                0x34e2,0x24c3,0x14a0,0x481,0x7466,0x6447,0x5424,0x4405,
                                0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
                                0x26d3,0x36f2,0x691,0x16b0,0x6657,0x7676,0x4615,0x5634,
                                0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
                                0x5844,0x4865,0x7806,0x6827,0x18c0,0x8e1,0x3882,0x28a3,
                                0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
                                0x4a75,0x5a54,0x6a37,0x7a16,0xaf1,0x1ad0,0x2ab3,0x3a92,
                                0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
                                0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0xcc1,
                                0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
                                0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0xed1,0x1ef0
                                };
/***********************************************************
CRC16 Coding & Decoding G(X) = X^16+X^12+X^5+1
***********************************************************/
uint16_t CRC16_cal(uint8_t *ptr, uint32_t len, uint16_t crc_init)
{
    uint16_t crc,   oldcrc16;
    uint8_t  temp;
    crc = crc_init;
    while (len--!=0)
    {
        temp=(crc>>8)&0xff;
        oldcrc16=crc16_tab[*ptr^temp];
        crc=(crc<<8)^oldcrc16;
        ptr++;
    }

    return(crc);
}

void IIC_Init(u32 bound, u16 address)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    I2C_InitTypeDef I2C_InitTSturcture={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C2, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;//SCL/SDA-IIC2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTSturcture.I2C_ClockSpeed = bound;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitTSturcture.I2C_OwnAddress1 = address;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C2, &I2C_InitTSturcture);

    I2C_Cmd(I2C2, ENABLE );

    I2C_AcknowledgeConfig(I2C2, ENABLE );
}

void EEPROM_Clear () {
    struct EEPROM_TimeVal_str EEPROM_TimeVal = { 0 };
    struct EEPROM_ConfigVal_str EEPROM_ConfigVal = { 0 };

    AT24CXX_Write(EEPROM_TIME_WRADDR, (uint8_t *)&EEPROM_TimeVal, sizeof(struct EEPROM_TimeVal_str));
    AT24CXX_Write(EEPROM_CONFIG_WRADDR, (uint8_t *)&EEPROM_ConfigVal, sizeof(struct EEPROM_ConfigVal_str));
}

void EEPROM_Write_TimeVal() {
    struct EEPROM_TimeVal_str EEPROM_TimeVal = { 0 };
    EEPROM_TimeVal.CRC_Num = CRC16_cal((uint8_t *)&Undul_Time, sizeof(Undulation_Time), CRC_TIME_PASSWORD);
    memcpy(&EEPROM_TimeVal.Undul_Time, &Undul_Time, sizeof(Undulation_Time));
    AT24CXX_Write(EEPROM_TIME_WRADDR, (uint8_t *)&EEPROM_TimeVal, sizeof(struct EEPROM_TimeVal_str));
    printf("Write TimeVal OK! CRC : 0x%X\r\n", EEPROM_TimeVal.CRC_Num);
}   

void EEPROM_Write_ConfigVal() {
    struct EEPROM_ConfigVal_str EEPROM_ConfigVal = { 0 };
    EEPROM_ConfigVal.CRC_Num = CRC16_cal((uint8_t *)&ConfigVal, sizeof(Config_Val_Str), CRC_CONFIG_PASSWORD);
    memcpy(&EEPROM_ConfigVal.ConfigVal, &ConfigVal, sizeof(Config_Val_Str));
    AT24CXX_Write(EEPROM_CONFIG_WRADDR, (uint8_t *)&EEPROM_ConfigVal, sizeof(struct EEPROM_ConfigVal_str));
    printf("Write ConfigVal OK! CRC : 0x%X\r\n", EEPROM_ConfigVal.CRC_Num);
}

void Applicaiton_EEPROM_Init () {

    struct EEPROM_TimeVal_str EEPROM_TimeVal = { 0 };
    struct EEPROM_ConfigVal_str EEPROM_ConfigVal = { 0 };

    IIC_Init(100000, 0xA0);

    AT24CXX_Read(EEPROM_TIME_WRADDR, (uint8_t *)&EEPROM_TimeVal, sizeof(struct EEPROM_TimeVal_str));
    if(CRC16_cal((uint8_t *)&EEPROM_TimeVal.Undul_Time, sizeof(Undulation_Time), CRC_TIME_PASSWORD) == EEPROM_TimeVal.CRC_Num) {
        memcpy(&Undul_Time, &EEPROM_TimeVal.Undul_Time, sizeof(Undulation_Time));
    } else {
        Undul_Time.Preheating_Temp_Time = 0;
        Undul_Time.RTC_Time = 0;
        Undul_Time.Run_Temp_Time = 0;
        Undul_Time.System_Open_Time = 0;
        EEPROM_TimeVal.CRC_Num = CRC16_cal((uint8_t *)&Undul_Time, sizeof(Undulation_Time), CRC_TIME_PASSWORD);
        memcpy(&EEPROM_TimeVal.Undul_Time, &Undul_Time, sizeof(Undulation_Time));
        AT24CXX_Write(EEPROM_TIME_WRADDR, (uint8_t *)&EEPROM_TimeVal, sizeof(struct EEPROM_TimeVal_str));
    }

    AT24CXX_Read(EEPROM_CONFIG_WRADDR, (uint8_t *)&EEPROM_ConfigVal, sizeof(struct EEPROM_ConfigVal_str));
    if(CRC16_cal((uint8_t *)&EEPROM_ConfigVal.ConfigVal, sizeof(Config_Val_Str), CRC_CONFIG_PASSWORD) == EEPROM_ConfigVal.CRC_Num) {
        memcpy(&ConfigVal, &EEPROM_ConfigVal.ConfigVal, sizeof(Config_Val_Str));
    } else {
        ConfigVal.Current = 15;
        ConfigVal.Leakage_Current = 30;
        ConfigVal.Preheating_Detection_Time = 7200;
        ConfigVal.Preheating_Temp_Time = 900;
        ConfigVal.Preheating_Time = 60;
        ConfigVal.Temp_Max = 110;
        ConfigVal.Temp_Min = 75;
        ConfigVal.Upkeep_Detection_Systemopen_Time = 7200000;
        ConfigVal.Upkeep_Detection_Time = 31536000;
        ConfigVal.Voltage_Max = 450;
        ConfigVal.Voltage_Min = 340;
        EEPROM_ConfigVal.CRC_Num = CRC16_cal((uint8_t *)&ConfigVal, sizeof(Config_Val_Str), CRC_TIME_PASSWORD);
        memcpy(&EEPROM_ConfigVal.ConfigVal, &ConfigVal, sizeof(Config_Val_Str));
        AT24CXX_Write(EEPROM_CONFIG_WRADDR, (uint8_t *)&EEPROM_ConfigVal, sizeof(struct EEPROM_ConfigVal_str));
    }


}