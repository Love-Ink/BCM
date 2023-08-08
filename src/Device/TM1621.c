//--------------------------------------------------------------------------------------------------
// @文件包含引用    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------

#include "TM1621.h"                                 // 包含外设头文件 TM1621-32x4位显示RAMLCD驱动器
#include <stdio.h>
#include "public.h"

#include <ch32v20x_rcc.h>
//--------------------------------------------------------------------------------------------------
// @宏定义宏声明    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
// TM1621 标识符
#define TM1621_FLAG_CMD                   (0x04)          // TM1621 标识 命令标识符 b100
#define TM1621_FLAG_DATA                  (0x05)          // TM1621 标识 数据标识符 b101

// TM1621 命令
#define TM1621_CMD_SYSEN                  (0x0002)        // TM1621 命令 打开系统振荡器
#define TM1621_CMD_SYSDIS                 (0x0000)        // TM1621 命令 关闭系统振荡器和LCD偏压发生器

#define TM1621_CMD_WDTDIS                 (0x000A)        // TM1621 命令 WDT看门狗溢出标志位输出 禁止
#define TM1621_CMD_WDTEN                  (0x000E)        // TM1621 命令 WDT看门狗溢出标志位输出 允许

#define TM1621_CMD_LCDON                  (0x0006)        // TM1621 命令 LCD偏压发生器 打开
#define TM1621_CMD_LCDOFF                 (0x0004)        // TM1621 命令 LCD偏压发生器 关闭

#define TM1621_CMD_XTAL32K                (0x0028)        // TM1621 命令 系统时钟源选择 外部晶振32.768KHZ
#define TM1621_CMD_RC256K                 (0x0030)        // TM1621 命令 系统时钟源选择 片内RC振荡器256KHz
#define TM1621_CMD_EXT256K                (0x0038)        // TM1621 命令 系统时钟源选择 外部时钟源256KHz

#define TM1621_CMD_BIASCOM                (0x0040)        // TM1621 命令 设置公共口和偏压的命令

#define TM1621_CMD_BIASCOM_MASK_1P2       (0x0000)        // TM1621 命令 设置公共口和偏压的命令 掩码 1/2偏压选项
#define TM1621_CMD_BIASCOM_MASK_1P3       (0x0002)        // TM1621 命令 设置公共口和偏压的命令 掩码 1/3偏压选项

#define TM1621_CMD_BIASCOM_MASK_COM2      (0x0000)        // TM1621 命令 设置公共口和偏压的命令 掩码 使用2个公共口
#define TM1621_CMD_BIASCOM_MASK_COM3      (0x0008)        // TM1621 命令 设置公共口和偏压的命令 掩码 使用3个公共口
#define TM1621_CMD_BIASCOM_MASK_COM4      (0x0010)        // TM1621 命令 设置公共口和偏压的命令 掩码 使用4个公共口

#define TM1621_CMD_TOPT                   (0x01C0)        // TM1621 命令 模式选择 测试模式
#define TM1621_CMD_TNORMAL                (0x01C6)        // TM1621 命令 模式选择 普通模式

//--------------------------------------------------------------------------------------------------
// @变量定义声明    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
TM1621_Handle_t     Project_TM1621;
//--------------------------------------------------------------------------------------------------
// @函数声明调用    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
uint8_t TM1621_Init(TM1621_Handle_t *TM1621_Handle);
static void TM1621_WriteBit_MSB(TM1621_Handle_t *TM1621_Handle, uint32_t Data, uint8_t Count);
static void TM1621_WriteBit_LSB(TM1621_Handle_t *TM1621_Handle, uint32_t Data, uint8_t Count);
void TM1621_WriteCommad(TM1621_Handle_t *TM1621_Handle, uint16_t Commad);
void TM1621_WriteByte(TM1621_Handle_t *TM1621_Handle, uint8_t Address, uint8_t Byte);
void TM1621_WriteBuffer(TM1621_Handle_t *TM1621_Handle, uint8_t Address, uint8_t *Buffer, uint8_t Count);
void TM1621_ShowAll(TM1621_Handle_t *TM1621_Handle);
void TM1621_ClearAll(TM1621_Handle_t *TM1621_Handle);

//==================================================================================================
// @函数功能: TM1621 结构体初始化
// @函数标记: 外设驱动函数
// @函数说明: 无
//--------------------------------------------------------------------------------------------------
// @输入参量: 无
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void TM1621_StructInit(TM1621_Handle_t *TM1621_Handle)
{
    TM1621_Handle->Clock  = TM1621_CLOCK_RC256K;    // 时钟选择 默认为片内RC振荡器256KHz
    TM1621_Handle->Bias   = TM1621_BIAS_1P3;        // 偏压选项 默认为1/3偏置电压
    TM1621_Handle->Common = TM1621_COM_4;           // 公共口   默认为使用4个公共口

    TM1621_Handle->CS_GPIO_ResetBit_Function   = NULL;
    TM1621_Handle->CS_GPIO_SetBit_Function     = NULL;
    TM1621_Handle->WR_GPIO_ResetBit_Function   = NULL;
    TM1621_Handle->WR_GPIO_SetBit_Function     = NULL;
    TM1621_Handle->RD_GPIO_ResetBit_Function   = NULL;
    TM1621_Handle->RD_GPIO_SetBit_Function     = NULL;
    TM1621_Handle->DATA_GPIO_ResetBit_Function = NULL;
    TM1621_Handle->DATA_GPIO_SetBit_Function   = NULL;
    TM1621_Handle->DELAY_nUS_Function          = NULL;
}

//==================================================================================================
// @函数功能: TM1621 初始化
// @函数标记: 外设驱动函数
// @函数说明: 无
//--------------------------------------------------------------------------------------------------
// @输入参量: 无
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
uint8_t TM1621_Init(TM1621_Handle_t *TM1621_Handle)
{
    uint16_t Temp = 0x0000;                         // 定义局部变量 用于变量临时存储

//    // 断言检查参数
//    Assert_Param(NULL != TM1621_Handle->CS_GPIO_ResetBit_Function);         // 要求函数为非空指针
//    Assert_Param(NULL != TM1621_Handle->CS_GPIO_SetBit_Function);           // 要求函数为非空指针
//    Assert_Param(NULL != TM1621_Handle->WR_GPIO_ResetBit_Function);         // 要求函数为非空指针
//    Assert_Param(NULL != TM1621_Handle->WR_GPIO_SetBit_Function);           // 要求函数为非空指针
//    Assert_Param(NULL != TM1621_Handle->RD_GPIO_ResetBit_Function);         // 要求函数为非空指针
//    Assert_Param(NULL != TM1621_Handle->RD_GPIO_SetBit_Function);           // 要求函数为非空指针
//    Assert_Param(NULL != TM1621_Handle->DATA_GPIO_ResetBit_Function);       // 要求函数为非空指针
//    Assert_Param(NULL != TM1621_Handle->DATA_GPIO_SetBit_Function);         // 要求函数为非空指针
//    Assert_Param(NULL != TM1621_Handle->DELAY_nUS_Function);                // 要求函数为非空指针

    // 端口引脚初始化
    TM1621_Handle->CS_GPIO_SetBit_Function();       // 片选引脚引脚 输出高电平
    TM1621_Handle->WR_GPIO_SetBit_Function();       // 写引脚 输出高电平

    TM1621_Handle->DELAY_nUS_Function(1000);        // 延迟一段时间

    // 计算偏置电压数值
    if(TM1621_BIAS_1P2 == TM1621_Handle->Bias)
    {
        Temp |= TM1621_CMD_BIASCOM_MASK_1P2;
    }
    else if(TM1621_BIAS_1P3 == TM1621_Handle->Bias)
    {
        Temp |= TM1621_CMD_BIASCOM_MASK_1P3;
    }
    // 计算公共口数值
    if(TM1621_COM_2 == TM1621_Handle->Common)
    {
        Temp |= TM1621_CMD_BIASCOM_MASK_COM2;
    }
    else if(TM1621_COM_3 == TM1621_Handle->Common)
    {
        Temp |= TM1621_CMD_BIASCOM_MASK_COM3;
    }
    else if(TM1621_COM_4 == TM1621_Handle->Common)
    {
        Temp |= TM1621_CMD_BIASCOM_MASK_COM4;
    }

    // 功能性能初始化
    TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_BIASCOM | Temp);   // 写命令 设置公共口和偏压的命令

    if(TM1621_CLOCK_XTAL32K == TM1621_Handle->Clock)
    {
        TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_XTAL32K);      // 写命令 系统时钟源选择 外部晶振32.768KHZ
    }
    else if(TM1621_CLOCK_RC256K == TM1621_Handle->Clock)
    {
        TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_RC256K);       // 写命令 系统时钟源选择 片内RC振荡器256KHz
    }
    else if(TM1621_CLOCK_EXT256K == TM1621_Handle->Clock)
    {
        TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_EXT256K);      // 写命令 系统时钟源选择 外部时钟源256KHz
    }

    TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_SYSDIS);           // 写命令 关闭系统振荡器和LCD偏压发生器
    TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_WDTDIS);           // 写命令 WDT看门狗溢出标志位输出 禁止

    TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_SYSEN);            // 写命令 打开系统振荡器
    TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_TNORMAL);          // 写命令 模式选择 普通模式
    TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_LCDON);            // 写命令 LCD偏压发生器 打开

//    TM1621_ClearAll(TM1621_Handle);                                 // 清除屏幕所有内容

    return TM1621_SUCCESS;
}

//==================================================================================================
// @函数功能: TM1621 发送位 大端方式
// @函数标记: 外设驱动函数
// @函数说明: 大端方式为高位先发送，低位后发送
//--------------------------------------------------------------------------------------------------
// @输入参量: Byte - 待发送字节
// @输入参量: Count - 待发送位数长度
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
static void TM1621_WriteBit_MSB(TM1621_Handle_t *TM1621_Handle, uint32_t Data, uint8_t Count)
{
    int8_t i = 0;                                   // 定义局部变量 用于函数循环计数

    for(i=(Count-1); i>=0; --i)
    {
        TM1621_Handle->WR_GPIO_ResetBit_Function(); // 写引脚 输出低电平

        if((Data >> i) & 0x01)                      // 判断高位数据是否为1
        {
            TM1621_Handle->DATA_GPIO_SetBit_Function();             // 数据引脚 输出高电平
        }
        else
        {
            TM1621_Handle->DATA_GPIO_ResetBit_Function();           // 数据引脚 输出低电平
        }
        TM1621_Handle->DELAY_nUS_Function(1);       // 延迟一段时间
        TM1621_Handle->WR_GPIO_SetBit_Function();   // 写引脚 输出高电平
        TM1621_Handle->DELAY_nUS_Function(1);       // 延迟一段时间
    }

    TM1621_Handle->WR_GPIO_ResetBit_Function();     // 写引脚 输出低电平
    TM1621_Handle->DATA_GPIO_ResetBit_Function();   // 数据引脚 输出低电平
}

//==================================================================================================
// @函数功能: TM1621 发送位 小端方式
// @函数标记: 外设驱动函数
// @函数说明: 大端方式为低位先发送，高位后发送
//--------------------------------------------------------------------------------------------------
// @输入参量: Byte - 待发送字节
// @输入参量: Count - 待发送位数长度
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
static void TM1621_WriteBit_LSB(TM1621_Handle_t *TM1621_Handle, uint32_t Data, uint8_t Count)
{
    uint8_t i = 0;                                  // 定义局部变量 用于函数循环计数

    for(i=0; i<Count; i++)
    {
        TM1621_Handle->WR_GPIO_ResetBit_Function(); // 写引脚 输出低电平

        if((Data >> i) & 0x01)                      // 判断高位数据是否为1
        {
            TM1621_Handle->DATA_GPIO_SetBit_Function();             // 数据引脚 输出高电平
        }
        else
        {
            TM1621_Handle->DATA_GPIO_ResetBit_Function();           // 数据引脚 输出低电平
        }
        TM1621_Handle->DELAY_nUS_Function(1);       // 延迟一段时间
        TM1621_Handle->WR_GPIO_SetBit_Function();   // 写引脚 输出高电平
        TM1621_Handle->DELAY_nUS_Function(1);       // 延迟一段时间
    }

    TM1621_Handle->WR_GPIO_ResetBit_Function();     // 写引脚 输出低电平
    TM1621_Handle->DATA_GPIO_ResetBit_Function();   // 数据引脚 输出低电平
}

//==================================================================================================
// @函数功能: TM1621 写命令
// @函数标记: 外设驱动函数
// @函数说明: 无
//--------------------------------------------------------------------------------------------------
// @输入参量: Commad - 控制命令
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void TM1621_WriteCommad(TM1621_Handle_t *TM1621_Handle, uint16_t Commad)
{


    TM1621_Handle->CS_GPIO_ResetBit_Function();     // 片选引脚 输出低电平
    TM1621_Handle->DELAY_nUS_Function(1);           // 延迟一段时间

    TM1621_WriteBit_MSB(TM1621_Handle, TM1621_FLAG_CMD, 3);         // 写前3位标识符
    TM1621_WriteBit_MSB(TM1621_Handle, Commad, 9);                  // 写后9位控制命令符

    TM1621_Handle->CS_GPIO_SetBit_Function();       // 片选引脚 输出高电平
    TM1621_Handle->DELAY_nUS_Function(1);           // 延迟一段时间


}

//==================================================================================================
// @函数功能: TM1621 写1个数据
// @函数标记: 外设驱动函数
// @函数说明: 指定地址上写数据，8位分成2个SEG位
//--------------------------------------------------------------------------------------------------
// @输入参量: Address - 地址  取值范围 - 0~31
// @输入参量: Byte - 字节  取值范围 - 低4位有效
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void TM1621_WriteByte(TM1621_Handle_t *TM1621_Handle, uint8_t Address, uint8_t Byte)
{


    TM1621_Handle->CS_GPIO_ResetBit_Function();     // 片选引脚 输出低电平
    TM1621_Handle->DELAY_nUS_Function(1);           // 延迟一段时间

    TM1621_WriteBit_MSB(TM1621_Handle, TM1621_FLAG_DATA, 3);        // 执行写数据操作 写前3位标识符
    TM1621_WriteBit_MSB(TM1621_Handle, Address, 6);                 // 设置6位地址 从高位开始写数据 分别为(A5-A4-A3-A2-A1-A0)

    TM1621_WriteBit_LSB(TM1621_Handle, Byte, 8);                    // 写入单次4位数据

    TM1621_Handle->CS_GPIO_SetBit_Function();       // 片选引脚 输出高电平
    TM1621_Handle->DELAY_nUS_Function(1);           // 延迟一段时间

}

//==================================================================================================
// @函数功能: TM1621 写多个数据
// @函数标记: 外设驱动函数
// @函数说明: 指定地址上写数据，8位分成2个SEG位
//--------------------------------------------------------------------------------------------------
// @输入参量: TM1621_Handle - TM1621句柄
// @输入参量: Address - 地址  取值范围 - 0~31
// @输入参量: Buffer - 多字节首地址  取值范围 - 低4位有效
// @输入参量: Count - 字节数量  取值范围 - 0~31
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void TM1621_WriteBuffer(TM1621_Handle_t *TM1621_Handle, uint8_t Address, uint8_t *Buffer, uint8_t Count)
{
    uint8_t i=0;                                    // 定义局部变量 用于函数循环计数

    TM1621_Handle->CS_GPIO_ResetBit_Function();     // 片选引脚 输出低电平
    TM1621_Handle->DELAY_nUS_Function(1);           // 延迟一段时间

    TM1621_WriteBit_MSB(TM1621_Handle, TM1621_FLAG_DATA, 3);        // 执行写数据操作 写前3位标识符
    TM1621_WriteBit_MSB(TM1621_Handle, Address, 6);                 // 设置6位地址 从高位开始写数据 分别为(A5-A4-A3-A2-A1-A0)

    for(i=0; i<Count; i++)
    {
        TM1621_WriteBit_LSB(TM1621_Handle, *Buffer, 8);             // 写入单次4位数据
        Buffer++;                                                   // 指针移动下一位
    }

    TM1621_Handle->CS_GPIO_SetBit_Function();       // 片选引脚 输出高电平
    TM1621_Handle->DELAY_nUS_Function(1);           // 延迟一段时间

}

//==================================================================================================
// @函数功能: TM1621 显示所有内容
// @函数标记: 外设驱动函数
// @函数说明: 无
//--------------------------------------------------------------------------------------------------
// @输入参量: 无
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void TM1621_ShowAll(TM1621_Handle_t *TM1621_Handle)
{
    uint8_t i=0;                                    // 定义局部变量 用于函数循环计数



    TM1621_Handle->CS_GPIO_ResetBit_Function();     // 片选引脚 输出低电平
    TM1621_Handle->DELAY_nUS_Function(1);           // 延迟一段时间

    TM1621_WriteBit_MSB(TM1621_Handle, TM1621_FLAG_DATA, 3);        // 执行写数据操作
    TM1621_WriteBit_MSB(TM1621_Handle, 0, 6);                       // 设置6位地址为b000000
    for (i=0; i<16; i++)
    {
        TM1621_WriteBit_LSB(TM1621_Handle, 0xFF, 8);                // 连续写入32次4位数据
    }
    TM1621_Handle->CS_GPIO_SetBit_Function();       // 片选引脚 输出高电平
    TM1621_Handle->DELAY_nUS_Function(1);           // 延迟一段时间


}

//==================================================================================================
// @函数功能: TM1621 清空所有内容
// @函数标记: 外设驱动函数
// @函数说明: 无
//--------------------------------------------------------------------------------------------------
// @输入参量: 无
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void TM1621_ClearAll(TM1621_Handle_t *TM1621_Handle)
{
    uint8_t i=0;                                    // 定义局部变量 用于函数循环计数



    TM1621_Handle->CS_GPIO_ResetBit_Function();     // 片选引脚 输出低电平
    TM1621_Handle->DELAY_nUS_Function(1);           // 延迟一段时间

    TM1621_WriteBit_MSB(TM1621_Handle, TM1621_FLAG_DATA, 3);        // 执行写数据操作
    TM1621_WriteBit_MSB(TM1621_Handle, 0, 6);                       // 设置6位地址为b000000

    for (i=0; i<16; i++)
    {
        TM1621_WriteBit_LSB(TM1621_Handle, 0x00, 8);                // 连续写入32次4位数据
    }
    TM1621_Handle->CS_GPIO_SetBit_Function();       // 片选引脚 输出高电平
    TM1621_Handle->DELAY_nUS_Function(1);           // 延迟一段时间


}

void TM1621_CS_ResetBit(void) {
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);
}

void TM1621_CS_SetBit(void) {
    GPIO_SetBits(GPIOB, GPIO_Pin_6);
}

void TM1621_WR_ResetBit(void) {
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

void TM1621_WR_SetBit(void) {
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

void TM1621_DATA_ResetBit(void) {
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);
}

void TM1621_DATA_SetBit(void) {
    GPIO_SetBits(GPIOB, GPIO_Pin_4);
}

void Application_TM1612_Init() {

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    gpio_init(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_Out_PP);

    TM1621_StructInit(&Project_TM1621);
    Project_TM1621.Clock  = TM1621_CLOCK_RC256K;                                     // TM1621 时钟选择 设置为片内RC振荡器256KHz
    Project_TM1621.Bias   = TM1621_BIAS_1P3;                                         // TM1621 偏压选项 设置为1/3偏置
    Project_TM1621.Common = TM1621_COM_4;                                            // TM1621 公共口 设置为使用4个公共口
    Project_TM1621.CS_GPIO_ResetBit_Function   = TM1621_CS_ResetBit;                  // TM1621 CS/片选引脚 GPIO清除低电平函数
    Project_TM1621.CS_GPIO_SetBit_Function     = TM1621_CS_SetBit;                    // TM1621 CS/片选引脚 GPIO置位高电平函数
    Project_TM1621.WR_GPIO_ResetBit_Function   = TM1621_WR_ResetBit;                  // TM1621 WR/写引脚 GPIO清除低电平函数
    Project_TM1621.WR_GPIO_SetBit_Function     = TM1621_WR_SetBit;                    // TM1621 WR/写引脚 GPIO置位高电平函数
    Project_TM1621.RD_GPIO_ResetBit_Function   = NULL;                               // TM1621 RD/读引脚 GPIO清除低电平函数
    Project_TM1621.RD_GPIO_SetBit_Function     = NULL;                               // TM1621 RD/读引脚 GPIO置位高电平函数
    Project_TM1621.DATA_GPIO_ResetBit_Function = TM1621_DATA_ResetBit;                  // TM1621 DATA/数据引脚 GPIO清除低电平函数
    Project_TM1621.DATA_GPIO_SetBit_Function   = TM1621_DATA_SetBit;                    // TM1621 DATA/数据引脚 GPIO置位高电平函数
    Project_TM1621.DELAY_nUS_Function = (void (*)(uint16_t))Delay_Us;                // TM1621 DELAY延迟微秒函数
    TM1621_Init(&Project_TM1621);

}
//========================================== 文 件 结 束 ===========================================
