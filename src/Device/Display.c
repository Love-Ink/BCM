/*
 * Display.c
 *
 *  Created on: Mar 28, 2023
 *      Author: bbhn2
 */

//--------------------------------------------------------------------------------------------------
// @文件包含引用    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
#include "Display.h"

//--------------------------------------------------------------------------------------------------
// @宏定义宏声明    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// @变量定义声明    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
extern TM1621_Handle_t Project_TM1621;

Project_DisplayData_t MyDisplayData;

const struct Project_MySEGx4_FontCode_t     // 结构体名 SEGx4_CODE
{
       uint8_t Char;                        // 编码
       uint8_t Code;                        // 编码内容
} MySEGx4_FontCode[] =
{
    {'.',0x80},                               // .的编码方式，
    {'0',0x3F},                               // 0的编码方式
    {'1',0x06},                               // 1的编码方式
    {'2',0x5B},                               // 2的编码方式
    {'3',0x4F},                               // 3的编码方式
    {'4',0x66},                               // 4的编码方式
    {'5',0x6D},                               // 5的编码方式
    {'6',0x7D},                               // 6的编码方式
    {'7',0x07},                               // 7的编码方式
    {'8',0x7F},                               // 8的编码方式
    {'9',0x6F},                               // 9的编码方式
    {'-',0x40},                               // -的编码方式
    {'_',0x08},                               // _的编码方式
    {'*',0x01},                               // *的编码方式（*表示数码管A位置点亮）
    {' ',0x00},                               //  的编码方式
    {'a',0x5F},                               // a的编码方式
    {'b',0x7C},                               // b的编码方式
    {'c',0x58},                               // c的编码方式
    {'g',0x6F},                               // g的编码方式
    {'n',0x54},                               // n的编码方式
    {'d',0x5E},                               // d的编码方式
    {'q',0x67},                               // q的编码方式
    {'o',0x5C},                               // o的编码方式
    {'t',0x78},                               // t的编码方式
    {'u',0x1C},                               // u的编码方式
    {'A',0x77},                               // A的编码方式
    {'C',0x39},                               // C的编码方式
    {'E',0x79},                               // E的编码方式
    {'F',0x71},                               // F的编码方式
    {'G',0x3D},                               // G的编码方式
    {'H',0x76},                               // H的编码方式
    {'L',0x38},                               // L的编码方式
    {'N',0x37},                               // N的编码方式
    {'P',0x73}                                // P的编码方式
};

//--------------------------------------------------------------------------------------------------
// @函数声明调用    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
static void SEGx4_ShowData(uint8_t *Data);
static void SEGx4_ShowString(uint8_t X,uint8_t *String);
static void SEGx4_ShowNumber_Integer(uint8_t X, int16_t IntegerNumber, uint8_t Count);
static void SEGx4_ShowNumber_Float(uint8_t X, float FloatNumber, uint8_t Count1, uint8_t Count2);

//--------------------------------------------------------------------------------------------------
// @操作系统任务    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------


//==================================================================================================
// @函数功能: 4位数码管 字库转显示
// @函数标记: 外设驱动函数
// @函数说明:
//--------------------------------------------------------------------------------------------------
// @输入参量: 无
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
static void SEGx4_ShowData(uint8_t *Data)
{
    // 第1位数码管
    {
        // A段
        if(Data[0] & 0x01) MyDisplayData.SEG_1_A = 1;
        else               MyDisplayData.SEG_1_A = 0;
        // B段
        if(Data[0] & 0x02) MyDisplayData.SEG_1_B = 1;
        else               MyDisplayData.SEG_1_B = 0;
        // C段
        if(Data[0] & 0x04) MyDisplayData.SEG_1_C = 1;
        else               MyDisplayData.SEG_1_C = 0;
        // D段
        if(Data[0] & 0x08) MyDisplayData.SEG_1_D = 1;
        else               MyDisplayData.SEG_1_D = 0;
        // E段
        if(Data[0] & 0x10) MyDisplayData.SEG_1_E = 1;
        else               MyDisplayData.SEG_1_E = 0;
        // F段
        if(Data[0] & 0x20) MyDisplayData.SEG_1_F = 1;
        else               MyDisplayData.SEG_1_F = 0;
        // G段
        if(Data[0] & 0x40) MyDisplayData.SEG_1_G = 1;
        else               MyDisplayData.SEG_1_G = 0;
        // DP段
        if(Data[0] & 0x80) MyDisplayData.SEG_1_DP = 1;
        else               MyDisplayData.SEG_1_DP = 0;
    }

    // 第2位数码管
    {
        // A段
        if(Data[1] & 0x01) MyDisplayData.SEG_2_A = 1;
        else               MyDisplayData.SEG_2_A = 0;
        // B段
        if(Data[1] & 0x02) MyDisplayData.SEG_2_B = 1;
        else               MyDisplayData.SEG_2_B = 0;
        // C段
        if(Data[1] & 0x04) MyDisplayData.SEG_2_C = 1;
        else               MyDisplayData.SEG_2_C = 0;
        // D段
        if(Data[1] & 0x08) MyDisplayData.SEG_2_D = 1;
        else               MyDisplayData.SEG_2_D = 0;
        // E段
        if(Data[1] & 0x10) MyDisplayData.SEG_2_E = 1;
        else               MyDisplayData.SEG_2_E = 0;
        // F段
        if(Data[1] & 0x20) MyDisplayData.SEG_2_F = 1;
        else               MyDisplayData.SEG_2_F = 0;
        // G段
        if(Data[1] & 0x40) MyDisplayData.SEG_2_G = 1;
        else               MyDisplayData.SEG_2_G = 0;
        // DP段
        if(Data[1] & 0x80) MyDisplayData.SEG_2_DP = 1;
        else               MyDisplayData.SEG_2_DP = 0;
    }

    // 第3位数码管
    {
        // A段
        if(Data[2] & 0x01) MyDisplayData.SEG_3_A = 1;
        else               MyDisplayData.SEG_3_A = 0;
        // B段
        if(Data[2] & 0x02) MyDisplayData.SEG_3_B = 1;
        else               MyDisplayData.SEG_3_B = 0;
        // C段
        if(Data[2] & 0x04) MyDisplayData.SEG_3_C = 1;
        else               MyDisplayData.SEG_3_C = 0;
        // D段
        if(Data[2] & 0x08) MyDisplayData.SEG_3_D = 1;
        else               MyDisplayData.SEG_3_D = 0;
        // E段
        if(Data[2] & 0x10) MyDisplayData.SEG_3_E = 1;
        else               MyDisplayData.SEG_3_E = 0;
        // F段
        if(Data[2] & 0x20) MyDisplayData.SEG_3_F = 1;
        else               MyDisplayData.SEG_3_F = 0;
        // G段
        if(Data[2] & 0x40) MyDisplayData.SEG_3_G = 1;
        else               MyDisplayData.SEG_3_G = 0;
        // DP段
        if(Data[2] & 0x80) MyDisplayData.SEG_3_DP = 1;
        else               MyDisplayData.SEG_3_DP = 0;
    }

    // 第4位数码管
    {
        // A段
        if(Data[3] & 0x01) MyDisplayData.SEG_4_A = 1;
        else               MyDisplayData.SEG_4_A = 0;
        // B段
        if(Data[3] & 0x02) MyDisplayData.SEG_4_B = 1;
        else               MyDisplayData.SEG_4_B = 0;
        // C段
        if(Data[3] & 0x04) MyDisplayData.SEG_4_C = 1;
        else               MyDisplayData.SEG_4_C = 0;
        // D段
        if(Data[3] & 0x08) MyDisplayData.SEG_4_D = 1;
        else               MyDisplayData.SEG_4_D = 0;
        // E段
        if(Data[3] & 0x10) MyDisplayData.SEG_4_E = 1;
        else               MyDisplayData.SEG_4_E = 0;
        // F段
        if(Data[3] & 0x20) MyDisplayData.SEG_4_F = 1;
        else               MyDisplayData.SEG_4_F = 0;
        // G段
        if(Data[3] & 0x40) MyDisplayData.SEG_4_G = 1;
        else               MyDisplayData.SEG_4_G = 0;
        // DP段
        if(Data[3] & 0x80) MyDisplayData.SEG_4_DP = 1;
        else               MyDisplayData.SEG_4_DP = 0;
    }
}

//==================================================================================================
// @函数功能: 4位数码管 显示字符串
// @函数标记: 外设驱动函数
// @函数说明:
//--------------------------------------------------------------------------------------------------
// @输入参量: X - 显示位置  取值范围 - 0~3
//           String - 字符串  取值范围 - ASCII码字符串
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
static void SEGx4_ShowString(uint8_t X,uint8_t *String)
{
    uint8_t i = 0,j;                          // 定义局部变量
    uint8_t FontCount;                        // 定义局部变量 用于记录字符字库个数
    uint8_t SEGx4_DataArray[4] = {0x00};

    // 计算字符库 字符数量
    FontCount = sizeof(MySEGx4_FontCode)/sizeof(MySEGx4_FontCode[0]);

    while(*String)
    {
        for(j=0; j<FontCount; j++)                  // 循环查询数码管字模位置
        {
            if(MySEGx4_FontCode[j].Char == *String)   // 判断字符的编码是否与输入字符相等
            {
                // 防止数组溢出
                if((i+X) >= 4)
                {
                    return;
                }
                SEGx4_DataArray[i+X] = MySEGx4_FontCode[j].Code;
                i++;
                break;
            }
        }
        String++;
    }
    SEGx4_ShowData(SEGx4_DataArray);
}

//==================================================================================================
// @函数功能: 4位数码管 显示整型数字
// @函数标记: 外设驱动函数
// @函数说明:
//--------------------------------------------------------------------------------------------------
// @输入参量: X - 显示位置  取值范围 - 0~3
//           IntegerNumber - 整型数字
//           Count - 整数部分位数
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
static void SEGx4_ShowNumber_Integer(uint8_t X, int16_t IntegerNumber, uint8_t Count)
{
    uint8_t i = 0,j;                                // 定义局部变量 用于循环
    uint8_t Postion_Sign = 0;                       // 定义局部变量 用于标识数字的正负 0为正数 1为负数
    uint8_t Number_Integer_Array[4];                // 定义局部数组 用于存储整数位各位数据
    uint8_t FontCount;                              // 定义局部变量 用于记录字符字库个数
    uint8_t SEGx4_DataArray[4] = {0x00};

    // 判断是否是负数
    if(IntegerNumber < 0)
    {
        // 增加负号位置
        SEGx4_DataArray[X] = 0x40;

        Postion_Sign = 1;                           // 正负标识位 置位
        IntegerNumber = 0 - IntegerNumber;          // 负数变换正数操作
    }

    Number_Integer_Array[0] = IntegerNumber/1000%10 + 0x30;
    Number_Integer_Array[1] = IntegerNumber/100 %10 + 0x30;
    Number_Integer_Array[2] = IntegerNumber/10  %10 + 0x30;
    Number_Integer_Array[3] = IntegerNumber/1   %10 + 0x30;

    // 计算字符库 字符数量
    FontCount = sizeof(MySEGx4_FontCode)/sizeof(MySEGx4_FontCode[0]);

    for(i=0; i<Count; i++)
    {
        for(j=0; j<FontCount; j++)                  // 循环查询数码管字模位置
        {
            if(MySEGx4_FontCode[j].Char == Number_Integer_Array[4 - Count + i])
            {
                // 防止数组溢出
                if((i + X + Postion_Sign) >= 4)
                {
                    return;
                }
                SEGx4_DataArray[i + X + Postion_Sign] = MySEGx4_FontCode[j].Code;
                break;
            }
        }
    }

    SEGx4_ShowData(SEGx4_DataArray);
}

//==================================================================================================
// @函数功能: 4位数码管 显示浮点数字
// @函数标记: 外设驱动函数
// @函数说明:
//--------------------------------------------------------------------------------------------------
// @输入参量: X - 显示位置  取值范围 - 0~3
//           FloatNumber - 浮点数字  取值范围 - 0.000~999.9
//           Count1 - 显示整数部分位数  取值范围 - 1~3
//           Count2 - 显示小数部分位数  取值范围 - 1~3
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
static void SEGx4_ShowNumber_Float(uint8_t X, float FloatNumber, uint8_t Count1, uint8_t Count2)
{
    uint8_t i = 0,j;                                // 定义局部变量 用于函数循环计数
    uint8_t Postion_Sign = 0;                       // 定义局部变量 用于标识数字的正负 0为正数 1为负数
    uint8_t Number_Integer_Array[4 - 1];            // 定义局部数组 用于存储整数位各位数据
    uint8_t Number_Decimal_Array[4 - 1];            // 定义局部数组 用于存储小数位各位数据
    uint8_t FontCount;                              // 定义局部变量 用于记录字符字库个数
    uint8_t SEGx4_DataArray[4] = {0x00};

    uint32_t  Number_Integer = 0;                   // 定义局部变量 表示浮点数的 整数部分
    uint32_t  Number_Decimal = 0;                   // 定义局部变量 表示浮点数的 小数部分

    // 判断是否是负数
    if(FloatNumber < 0)
    {
        // 增加负号位置
        SEGx4_DataArray[X] = 0x40;

        Postion_Sign = 1;                           // 正负标识位 置位
        FloatNumber = 0 - FloatNumber;              // 负数变换正数操作
    }

    // 取整数部分
    Number_Integer = (uint32_t)FloatNumber;
    // 取小数部分，1e2表示100
    // 注意此处的取值和数码管的位数有直接关系，需要手动修改
    Number_Decimal = (uint32_t)((FloatNumber - Number_Integer + 0.005)*1e2);
    if(Number_Decimal < 100) Number_Decimal *= 10;

    Number_Integer_Array[0] = Number_Integer/100%10+0x30;
    Number_Integer_Array[1] = Number_Integer/ 10%10+0x30;
    Number_Integer_Array[2] = Number_Integer/  1%10+0x30;

    Number_Decimal_Array[0] = Number_Decimal/100%10+0x30;
    Number_Decimal_Array[1] = Number_Decimal/ 10%10+0x30;
    Number_Decimal_Array[2] = Number_Decimal/  1%10+0x30;

    // 计算字符库 字符数量
    FontCount = sizeof(MySEGx4_FontCode)/sizeof(MySEGx4_FontCode[0]);

    // 提取整数部分
    for(i=0; i<Count1; i++)
    {
        for(j=0; j<FontCount; j++)                  // 循环查询数码管字模位置
        {
            if(MySEGx4_FontCode[j].Char == Number_Integer_Array[4 - 1 - Count1 + i])
            {
                // 防止数组溢出
                if((i + X + Postion_Sign) >= 4)
                {
                    return;
                }
                SEGx4_DataArray[i + X + Postion_Sign] = MySEGx4_FontCode[j].Code;
                break;
            }
        }
    }

    // 增加小数点位置
    SEGx4_DataArray[Count1 + X + Postion_Sign - 1] |= 0x80;

    // 提取小数部分
    for(i=0; i<Count2; i++)
    {
        for(j=0; j<FontCount; j++)                  // 循环查询数码管字模位置
        {
            if(MySEGx4_FontCode[j].Char == Number_Decimal_Array[i])
            {
                // 防止数组溢出
                if((i + Count1 + X + Postion_Sign) >= 4)
                {
                    return;
                }
                SEGx4_DataArray[i + Count1 + X + Postion_Sign] = MySEGx4_FontCode[j].Code;
                break;
            }
        }
    }

    SEGx4_ShowData(SEGx4_DataArray);
}


//==================================================================================================
// @函数功能: 工程 液晶屏显示 数码管 显示字符串
// @函数标记: 外设驱动函数
// @函数说明:
//--------------------------------------------------------------------------------------------------
// @输入参量: X - 显示位置  取值范围 - 0~3
//           String - 字符串  取值范围 - ASCII码字符串
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void Project_Display_Segment_ShowString(uint8_t X,uint8_t *String)
{
    SEGx4_ShowString(X, String);

    TM1621_WriteBuffer(&Project_TM1621, 0, MyDisplayData.DataArray, 8);
}

//==================================================================================================
// @函数功能: 工程 液晶屏显示 数码管 显示整型数字
// @函数标记: 外设驱动函数
// @函数说明:
//--------------------------------------------------------------------------------------------------
// @输入参量: X - 显示位置  取值范围 - 0~3
//           String - 字符串  取值范围 - ASCII码字符串
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void Project_Display_Segment_ShowIntegerNumber(uint8_t X, int16_t IntegerNumber, uint8_t Count)
{
    SEGx4_ShowNumber_Integer(X, IntegerNumber, Count);

    TM1621_WriteBuffer(&Project_TM1621, 0, MyDisplayData.DataArray, 8);
}

//==================================================================================================
// @函数功能: 工程 液晶屏显示 数码管 显示浮点数字
// @函数标记: 外设驱动函数
// @函数说明:
//--------------------------------------------------------------------------------------------------
// @输入参量: X - 显示位置  取值范围 - 0~3
//           String - 字符串  取值范围 - ASCII码字符串
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void Project_Display_Segment_ShowFloatNumber(uint8_t X, float FloatNumber, uint8_t Count1, uint8_t Count2)
{
    SEGx4_ShowNumber_Float(X, FloatNumber, Count1, Count2);

    TM1621_WriteBuffer(&Project_TM1621, 0, MyDisplayData.DataArray, 8);

}

//==================================================================================================
// @函数功能: 工程 液晶屏显示 数码管 显示混合字符串与数字
// @函数标记: 外设驱动函数
// @函数说明:
//--------------------------------------------------------------------------------------------------
// @输入参量: X - 显示位置  取值范围 - 0~3
//           String - 字符串  取值范围 - ASCII码字符串
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void Project_Display_Segment_ShowStringNumber(uint8_t X, uint8_t *String, uint16_t Number)
{
    uint8_t ShowString[128] = {0};                  // 定义局部变量 用于函数结果字符串

    if(sprintf((char *)ShowString, "%s%d\r\n", String, Number)>=0)
    {
        Project_Display_Segment_ShowString(X, ShowString);
    }
}


//==================================================================================================
// @函数功能: 工程 液晶屏显示 清除内容
// @函数标记: 应用执行函数
// @函数说明: 主函数中首次执行
//--------------------------------------------------------------------------------------------------
// @输入参量: State -
// @输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void Project_Display_Clear(uint8_t State)
{
    if(0 == State)
    {
        MyDisplayData.Data = 0x0000000000000000;
    }
    else if(1 == State)
    {
        MyDisplayData.Data = 0xFFFFFFFFFFFFFFFF;
    }
    TM1621_WriteBuffer(&Project_TM1621, 0, MyDisplayData.DataArray, 8);
}


// //==================================================================================================
// // @函数功能: 工程 液晶屏显示
// // @函数标记: 应用执行函数
// // @函数说明:
// //--------------------------------------------------------------------------------------------------
// // @输入参量: 无
// // @输出参量: 无
// //--------------------------------------------------------------------------------------------------
// //  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
// //==================================================================================================
// void Project_Display_Param(uint8_t ParamType, int32_t ParamData)
// {
//     // 清除显示 数据名称 区域
//     MyDisplayData.Param_Mode = 0;
//     MyDisplayData.Param_Total = 0;
//     MyDisplayData.Param_Time = 0;
//     MyDisplayData.Param_Voltage = 0;
//     MyDisplayData.Param_Current = 0;
//     MyDisplayData.Param_Power = 0;
//     MyDisplayData.Param_Leakage = 0;
//     MyDisplayData.Param_Energy = 0;
//     MyDisplayData.Param_Temp = 0;

//     // 清除显示 单位 区域
//     MyDisplayData.Unit_V = 0;
//     MyDisplayData.Unit_A = 0;
//     MyDisplayData.Unit_M = 0;
//     MyDisplayData.Unit_K = 0;
//     MyDisplayData.Unit_W = 0;
//     MyDisplayData.Unit_h = 0;
//     MyDisplayData.Unit_C = 0;
//     MyDisplayData.Unit_1000 = 0;

//     switch(ParamType)
//     {
//         // 显示电压内容
//         case PROJECT_DISPLAYPARAM_INDEX_VOLTAGE:
//         {
//             MyDisplayData.Param_Voltage = 1;        // 显示电压标题

//             // 设计思路：显示3位整数位+1位小数位，ParamData为放大100数值，需要缩写100倍后显示
//             SEGx4_ShowNumber_Float(0, (float)(ParamData)/100.0, 3, 1);

//             MyDisplayData.Unit_V = 1;               // 显示电压单位，单位为V
//         } break;
//         // 显示电流内容
//         case PROJECT_DISPLAYPARAM_INDEX_CURRENT:
//         {
//             MyDisplayData.Param_Current = 1;        // 显示电流标题

//             // 设计思路：显示2位整数位+2位小数位，ParamData为放大100数值，需要缩写100倍后显示
//             SEGx4_ShowNumber_Float(0, (float)(ParamData)/100.0, 2, 2);

//             MyDisplayData.Unit_A = 1;               // 显示电流单位，单位为A
//         } break;
//         // 显示漏电内容
//         case PROJECT_DISPLAYPARAM_INDEX_LEAKAGE:
//         {
//             // 设计思路: 漏电流由“漏”+“电流”两部分组成
//             MyDisplayData.Param_Leakage = 1;        // 显示漏电标题
//             MyDisplayData.Param_Current = 1;

//             // 设计思路：显示2位整数位+1位小数位，ParamData为放大100数值，需要缩写100倍后显示
//             SEGx4_ShowNumber_Float(0, (float)(ParamData)/100.0, 3, 1);

//             MyDisplayData.Unit_M = 1;               // 显示漏电单位，单位为mA
//             MyDisplayData.Unit_A = 1;
//         } break;
//         //显示温度内容
//         case PROJECT_DISPLAYPARAM_INDEX_TEMP:
//         {
//             // 设计思路: 显示温度标题
//             MyDisplayData.Param_Temp = 1;        // 显示温度标题

//             // 设计思路：显示2位整数位+1位小数位，ParamData为放大100数值，需要缩写100倍后显示
//             SEGx4_ShowNumber_Float(0, (float)(ParamData)/100.0, 3, 1);

//             MyDisplayData.Unit_C = 1;
//         } break;
// //
// //        // 显示功率内容
// //        case PROJECT_DISPLAYPARAM_INDEX_POWER:
// //        {
// //            MyDisplayData.Param_Power = 1;          // 显示功率标题
// //
// //            // 设计思路：显示4位整数位，ParamData为放大10数值，需要缩写10倍后显示
// //            SEGx4_ShowNumber_Integer(0, (float)(*((uint16_t *)ParamData))/10, 4);
// //
// //            MyDisplayData.Unit_W = 1;               // 显示功率单位，单位为W
// //        } break;

//         default:
//         {

//         } break;
//     }
//     TM1621_WriteBuffer(&Project_TM1621, 0, MyDisplayData.DataArray, 8);
// }

//设备状态显示函数
void Project_Display_State(uint8_t StateType)
{
    MyDisplayData.State_Run = 0;
    MyDisplayData.State_Standby = 0;
    MyDisplayData.State_Stop = 0;

    switch(StateType)
    {
        case PROJECT_DISPLAYSTATE_INDEX_RUN:
        {
            //Project_LED_SetState(&Project_LED_BL, 1);
            MyDisplayData.State_Run = 1;
        } break;
        case PROJECT_DISPLAYSTATE_INDEX_WAIT:
        {
            MyDisplayData.State_Standby = 1;
        } break;
        case PROJECT_DISPLAYSTATE_INDEX_STOP:
        {
            MyDisplayData.State_Stop = 1;
        } break;
        default:
        {

        } break;
    }
    TM1621_WriteBuffer(&Project_TM1621, 0, MyDisplayData.DataArray, 8);
}


