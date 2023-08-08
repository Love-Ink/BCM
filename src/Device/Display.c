/*
 * Display.c
 *
 *  Created on: Mar 28, 2023
 *      Author: bbhn2
 */

//--------------------------------------------------------------------------------------------------
// @�ļ���������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
#include "Display.h"

//--------------------------------------------------------------------------------------------------
// @�궨�������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// @������������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
extern TM1621_Handle_t Project_TM1621;

Project_DisplayData_t MyDisplayData;

const struct Project_MySEGx4_FontCode_t     // �ṹ���� SEGx4_CODE
{
       uint8_t Char;                        // ����
       uint8_t Code;                        // ��������
} MySEGx4_FontCode[] =
{
    {'.',0x80},                               // .�ı��뷽ʽ��
    {'0',0x3F},                               // 0�ı��뷽ʽ
    {'1',0x06},                               // 1�ı��뷽ʽ
    {'2',0x5B},                               // 2�ı��뷽ʽ
    {'3',0x4F},                               // 3�ı��뷽ʽ
    {'4',0x66},                               // 4�ı��뷽ʽ
    {'5',0x6D},                               // 5�ı��뷽ʽ
    {'6',0x7D},                               // 6�ı��뷽ʽ
    {'7',0x07},                               // 7�ı��뷽ʽ
    {'8',0x7F},                               // 8�ı��뷽ʽ
    {'9',0x6F},                               // 9�ı��뷽ʽ
    {'-',0x40},                               // -�ı��뷽ʽ
    {'_',0x08},                               // _�ı��뷽ʽ
    {'*',0x01},                               // *�ı��뷽ʽ��*��ʾ�����Aλ�õ�����
    {' ',0x00},                               //  �ı��뷽ʽ
    {'a',0x5F},                               // a�ı��뷽ʽ
    {'b',0x7C},                               // b�ı��뷽ʽ
    {'c',0x58},                               // c�ı��뷽ʽ
    {'g',0x6F},                               // g�ı��뷽ʽ
    {'n',0x54},                               // n�ı��뷽ʽ
    {'d',0x5E},                               // d�ı��뷽ʽ
    {'q',0x67},                               // q�ı��뷽ʽ
    {'o',0x5C},                               // o�ı��뷽ʽ
    {'t',0x78},                               // t�ı��뷽ʽ
    {'u',0x1C},                               // u�ı��뷽ʽ
    {'A',0x77},                               // A�ı��뷽ʽ
    {'C',0x39},                               // C�ı��뷽ʽ
    {'E',0x79},                               // E�ı��뷽ʽ
    {'F',0x71},                               // F�ı��뷽ʽ
    {'G',0x3D},                               // G�ı��뷽ʽ
    {'H',0x76},                               // H�ı��뷽ʽ
    {'L',0x38},                               // L�ı��뷽ʽ
    {'N',0x37},                               // N�ı��뷽ʽ
    {'P',0x73}                                // P�ı��뷽ʽ
};

//--------------------------------------------------------------------------------------------------
// @������������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
static void SEGx4_ShowData(uint8_t *Data);
static void SEGx4_ShowString(uint8_t X,uint8_t *String);
static void SEGx4_ShowNumber_Integer(uint8_t X, int16_t IntegerNumber, uint8_t Count);
static void SEGx4_ShowNumber_Float(uint8_t X, float FloatNumber, uint8_t Count1, uint8_t Count2);

//--------------------------------------------------------------------------------------------------
// @����ϵͳ����    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------


//==================================================================================================
// @��������: 4λ����� �ֿ�ת��ʾ
// @�������: ������������
// @����˵��:
//--------------------------------------------------------------------------------------------------
// @�������: ��
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
static void SEGx4_ShowData(uint8_t *Data)
{
    // ��1λ�����
    {
        // A��
        if(Data[0] & 0x01) MyDisplayData.SEG_1_A = 1;
        else               MyDisplayData.SEG_1_A = 0;
        // B��
        if(Data[0] & 0x02) MyDisplayData.SEG_1_B = 1;
        else               MyDisplayData.SEG_1_B = 0;
        // C��
        if(Data[0] & 0x04) MyDisplayData.SEG_1_C = 1;
        else               MyDisplayData.SEG_1_C = 0;
        // D��
        if(Data[0] & 0x08) MyDisplayData.SEG_1_D = 1;
        else               MyDisplayData.SEG_1_D = 0;
        // E��
        if(Data[0] & 0x10) MyDisplayData.SEG_1_E = 1;
        else               MyDisplayData.SEG_1_E = 0;
        // F��
        if(Data[0] & 0x20) MyDisplayData.SEG_1_F = 1;
        else               MyDisplayData.SEG_1_F = 0;
        // G��
        if(Data[0] & 0x40) MyDisplayData.SEG_1_G = 1;
        else               MyDisplayData.SEG_1_G = 0;
        // DP��
        if(Data[0] & 0x80) MyDisplayData.SEG_1_DP = 1;
        else               MyDisplayData.SEG_1_DP = 0;
    }

    // ��2λ�����
    {
        // A��
        if(Data[1] & 0x01) MyDisplayData.SEG_2_A = 1;
        else               MyDisplayData.SEG_2_A = 0;
        // B��
        if(Data[1] & 0x02) MyDisplayData.SEG_2_B = 1;
        else               MyDisplayData.SEG_2_B = 0;
        // C��
        if(Data[1] & 0x04) MyDisplayData.SEG_2_C = 1;
        else               MyDisplayData.SEG_2_C = 0;
        // D��
        if(Data[1] & 0x08) MyDisplayData.SEG_2_D = 1;
        else               MyDisplayData.SEG_2_D = 0;
        // E��
        if(Data[1] & 0x10) MyDisplayData.SEG_2_E = 1;
        else               MyDisplayData.SEG_2_E = 0;
        // F��
        if(Data[1] & 0x20) MyDisplayData.SEG_2_F = 1;
        else               MyDisplayData.SEG_2_F = 0;
        // G��
        if(Data[1] & 0x40) MyDisplayData.SEG_2_G = 1;
        else               MyDisplayData.SEG_2_G = 0;
        // DP��
        if(Data[1] & 0x80) MyDisplayData.SEG_2_DP = 1;
        else               MyDisplayData.SEG_2_DP = 0;
    }

    // ��3λ�����
    {
        // A��
        if(Data[2] & 0x01) MyDisplayData.SEG_3_A = 1;
        else               MyDisplayData.SEG_3_A = 0;
        // B��
        if(Data[2] & 0x02) MyDisplayData.SEG_3_B = 1;
        else               MyDisplayData.SEG_3_B = 0;
        // C��
        if(Data[2] & 0x04) MyDisplayData.SEG_3_C = 1;
        else               MyDisplayData.SEG_3_C = 0;
        // D��
        if(Data[2] & 0x08) MyDisplayData.SEG_3_D = 1;
        else               MyDisplayData.SEG_3_D = 0;
        // E��
        if(Data[2] & 0x10) MyDisplayData.SEG_3_E = 1;
        else               MyDisplayData.SEG_3_E = 0;
        // F��
        if(Data[2] & 0x20) MyDisplayData.SEG_3_F = 1;
        else               MyDisplayData.SEG_3_F = 0;
        // G��
        if(Data[2] & 0x40) MyDisplayData.SEG_3_G = 1;
        else               MyDisplayData.SEG_3_G = 0;
        // DP��
        if(Data[2] & 0x80) MyDisplayData.SEG_3_DP = 1;
        else               MyDisplayData.SEG_3_DP = 0;
    }

    // ��4λ�����
    {
        // A��
        if(Data[3] & 0x01) MyDisplayData.SEG_4_A = 1;
        else               MyDisplayData.SEG_4_A = 0;
        // B��
        if(Data[3] & 0x02) MyDisplayData.SEG_4_B = 1;
        else               MyDisplayData.SEG_4_B = 0;
        // C��
        if(Data[3] & 0x04) MyDisplayData.SEG_4_C = 1;
        else               MyDisplayData.SEG_4_C = 0;
        // D��
        if(Data[3] & 0x08) MyDisplayData.SEG_4_D = 1;
        else               MyDisplayData.SEG_4_D = 0;
        // E��
        if(Data[3] & 0x10) MyDisplayData.SEG_4_E = 1;
        else               MyDisplayData.SEG_4_E = 0;
        // F��
        if(Data[3] & 0x20) MyDisplayData.SEG_4_F = 1;
        else               MyDisplayData.SEG_4_F = 0;
        // G��
        if(Data[3] & 0x40) MyDisplayData.SEG_4_G = 1;
        else               MyDisplayData.SEG_4_G = 0;
        // DP��
        if(Data[3] & 0x80) MyDisplayData.SEG_4_DP = 1;
        else               MyDisplayData.SEG_4_DP = 0;
    }
}

//==================================================================================================
// @��������: 4λ����� ��ʾ�ַ���
// @�������: ������������
// @����˵��:
//--------------------------------------------------------------------------------------------------
// @�������: X - ��ʾλ��  ȡֵ��Χ - 0~3
//           String - �ַ���  ȡֵ��Χ - ASCII���ַ���
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
static void SEGx4_ShowString(uint8_t X,uint8_t *String)
{
    uint8_t i = 0,j;                          // ����ֲ�����
    uint8_t FontCount;                        // ����ֲ����� ���ڼ�¼�ַ��ֿ����
    uint8_t SEGx4_DataArray[4] = {0x00};

    // �����ַ��� �ַ�����
    FontCount = sizeof(MySEGx4_FontCode)/sizeof(MySEGx4_FontCode[0]);

    while(*String)
    {
        for(j=0; j<FontCount; j++)                  // ѭ����ѯ�������ģλ��
        {
            if(MySEGx4_FontCode[j].Char == *String)   // �ж��ַ��ı����Ƿ��������ַ����
            {
                // ��ֹ�������
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
// @��������: 4λ����� ��ʾ��������
// @�������: ������������
// @����˵��:
//--------------------------------------------------------------------------------------------------
// @�������: X - ��ʾλ��  ȡֵ��Χ - 0~3
//           IntegerNumber - ��������
//           Count - ��������λ��
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
static void SEGx4_ShowNumber_Integer(uint8_t X, int16_t IntegerNumber, uint8_t Count)
{
    uint8_t i = 0,j;                                // ����ֲ����� ����ѭ��
    uint8_t Postion_Sign = 0;                       // ����ֲ����� ���ڱ�ʶ���ֵ����� 0Ϊ���� 1Ϊ����
    uint8_t Number_Integer_Array[4];                // ����ֲ����� ���ڴ洢����λ��λ����
    uint8_t FontCount;                              // ����ֲ����� ���ڼ�¼�ַ��ֿ����
    uint8_t SEGx4_DataArray[4] = {0x00};

    // �ж��Ƿ��Ǹ���
    if(IntegerNumber < 0)
    {
        // ���Ӹ���λ��
        SEGx4_DataArray[X] = 0x40;

        Postion_Sign = 1;                           // ������ʶλ ��λ
        IntegerNumber = 0 - IntegerNumber;          // �����任��������
    }

    Number_Integer_Array[0] = IntegerNumber/1000%10 + 0x30;
    Number_Integer_Array[1] = IntegerNumber/100 %10 + 0x30;
    Number_Integer_Array[2] = IntegerNumber/10  %10 + 0x30;
    Number_Integer_Array[3] = IntegerNumber/1   %10 + 0x30;

    // �����ַ��� �ַ�����
    FontCount = sizeof(MySEGx4_FontCode)/sizeof(MySEGx4_FontCode[0]);

    for(i=0; i<Count; i++)
    {
        for(j=0; j<FontCount; j++)                  // ѭ����ѯ�������ģλ��
        {
            if(MySEGx4_FontCode[j].Char == Number_Integer_Array[4 - Count + i])
            {
                // ��ֹ�������
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
// @��������: 4λ����� ��ʾ��������
// @�������: ������������
// @����˵��:
//--------------------------------------------------------------------------------------------------
// @�������: X - ��ʾλ��  ȡֵ��Χ - 0~3
//           FloatNumber - ��������  ȡֵ��Χ - 0.000~999.9
//           Count1 - ��ʾ��������λ��  ȡֵ��Χ - 1~3
//           Count2 - ��ʾС������λ��  ȡֵ��Χ - 1~3
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
static void SEGx4_ShowNumber_Float(uint8_t X, float FloatNumber, uint8_t Count1, uint8_t Count2)
{
    uint8_t i = 0,j;                                // ����ֲ����� ���ں���ѭ������
    uint8_t Postion_Sign = 0;                       // ����ֲ����� ���ڱ�ʶ���ֵ����� 0Ϊ���� 1Ϊ����
    uint8_t Number_Integer_Array[4 - 1];            // ����ֲ����� ���ڴ洢����λ��λ����
    uint8_t Number_Decimal_Array[4 - 1];            // ����ֲ����� ���ڴ洢С��λ��λ����
    uint8_t FontCount;                              // ����ֲ����� ���ڼ�¼�ַ��ֿ����
    uint8_t SEGx4_DataArray[4] = {0x00};

    uint32_t  Number_Integer = 0;                   // ����ֲ����� ��ʾ�������� ��������
    uint32_t  Number_Decimal = 0;                   // ����ֲ����� ��ʾ�������� С������

    // �ж��Ƿ��Ǹ���
    if(FloatNumber < 0)
    {
        // ���Ӹ���λ��
        SEGx4_DataArray[X] = 0x40;

        Postion_Sign = 1;                           // ������ʶλ ��λ
        FloatNumber = 0 - FloatNumber;              // �����任��������
    }

    // ȡ��������
    Number_Integer = (uint32_t)FloatNumber;
    // ȡС�����֣�1e2��ʾ100
    // ע��˴���ȡֵ������ܵ�λ����ֱ�ӹ�ϵ����Ҫ�ֶ��޸�
    Number_Decimal = (uint32_t)((FloatNumber - Number_Integer + 0.005)*1e2);
    if(Number_Decimal < 100) Number_Decimal *= 10;

    Number_Integer_Array[0] = Number_Integer/100%10+0x30;
    Number_Integer_Array[1] = Number_Integer/ 10%10+0x30;
    Number_Integer_Array[2] = Number_Integer/  1%10+0x30;

    Number_Decimal_Array[0] = Number_Decimal/100%10+0x30;
    Number_Decimal_Array[1] = Number_Decimal/ 10%10+0x30;
    Number_Decimal_Array[2] = Number_Decimal/  1%10+0x30;

    // �����ַ��� �ַ�����
    FontCount = sizeof(MySEGx4_FontCode)/sizeof(MySEGx4_FontCode[0]);

    // ��ȡ��������
    for(i=0; i<Count1; i++)
    {
        for(j=0; j<FontCount; j++)                  // ѭ����ѯ�������ģλ��
        {
            if(MySEGx4_FontCode[j].Char == Number_Integer_Array[4 - 1 - Count1 + i])
            {
                // ��ֹ�������
                if((i + X + Postion_Sign) >= 4)
                {
                    return;
                }
                SEGx4_DataArray[i + X + Postion_Sign] = MySEGx4_FontCode[j].Code;
                break;
            }
        }
    }

    // ����С����λ��
    SEGx4_DataArray[Count1 + X + Postion_Sign - 1] |= 0x80;

    // ��ȡС������
    for(i=0; i<Count2; i++)
    {
        for(j=0; j<FontCount; j++)                  // ѭ����ѯ�������ģλ��
        {
            if(MySEGx4_FontCode[j].Char == Number_Decimal_Array[i])
            {
                // ��ֹ�������
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
// @��������: ���� Һ������ʾ ����� ��ʾ�ַ���
// @�������: ������������
// @����˵��:
//--------------------------------------------------------------------------------------------------
// @�������: X - ��ʾλ��  ȡֵ��Χ - 0~3
//           String - �ַ���  ȡֵ��Χ - ASCII���ַ���
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void Project_Display_Segment_ShowString(uint8_t X,uint8_t *String)
{
    SEGx4_ShowString(X, String);

    TM1621_WriteBuffer(&Project_TM1621, 0, MyDisplayData.DataArray, 8);
}

//==================================================================================================
// @��������: ���� Һ������ʾ ����� ��ʾ��������
// @�������: ������������
// @����˵��:
//--------------------------------------------------------------------------------------------------
// @�������: X - ��ʾλ��  ȡֵ��Χ - 0~3
//           String - �ַ���  ȡֵ��Χ - ASCII���ַ���
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void Project_Display_Segment_ShowIntegerNumber(uint8_t X, int16_t IntegerNumber, uint8_t Count)
{
    SEGx4_ShowNumber_Integer(X, IntegerNumber, Count);

    TM1621_WriteBuffer(&Project_TM1621, 0, MyDisplayData.DataArray, 8);
}

//==================================================================================================
// @��������: ���� Һ������ʾ ����� ��ʾ��������
// @�������: ������������
// @����˵��:
//--------------------------------------------------------------------------------------------------
// @�������: X - ��ʾλ��  ȡֵ��Χ - 0~3
//           String - �ַ���  ȡֵ��Χ - ASCII���ַ���
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void Project_Display_Segment_ShowFloatNumber(uint8_t X, float FloatNumber, uint8_t Count1, uint8_t Count2)
{
    SEGx4_ShowNumber_Float(X, FloatNumber, Count1, Count2);

    TM1621_WriteBuffer(&Project_TM1621, 0, MyDisplayData.DataArray, 8);

}

//==================================================================================================
// @��������: ���� Һ������ʾ ����� ��ʾ����ַ���������
// @�������: ������������
// @����˵��:
//--------------------------------------------------------------------------------------------------
// @�������: X - ��ʾλ��  ȡֵ��Χ - 0~3
//           String - �ַ���  ȡֵ��Χ - ASCII���ַ���
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void Project_Display_Segment_ShowStringNumber(uint8_t X, uint8_t *String, uint16_t Number)
{
    uint8_t ShowString[128] = {0};                  // ����ֲ����� ���ں�������ַ���

    if(sprintf((char *)ShowString, "%s%d\r\n", String, Number)>=0)
    {
        Project_Display_Segment_ShowString(X, ShowString);
    }
}


//==================================================================================================
// @��������: ���� Һ������ʾ �������
// @�������: Ӧ��ִ�к���
// @����˵��: ���������״�ִ��
//--------------------------------------------------------------------------------------------------
// @�������: State -
// @�������: ��
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
// // @��������: ���� Һ������ʾ
// // @�������: Ӧ��ִ�к���
// // @����˵��:
// //--------------------------------------------------------------------------------------------------
// // @�������: ��
// // @�������: ��
// //--------------------------------------------------------------------------------------------------
// //  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
// //==================================================================================================
// void Project_Display_Param(uint8_t ParamType, int32_t ParamData)
// {
//     // �����ʾ �������� ����
//     MyDisplayData.Param_Mode = 0;
//     MyDisplayData.Param_Total = 0;
//     MyDisplayData.Param_Time = 0;
//     MyDisplayData.Param_Voltage = 0;
//     MyDisplayData.Param_Current = 0;
//     MyDisplayData.Param_Power = 0;
//     MyDisplayData.Param_Leakage = 0;
//     MyDisplayData.Param_Energy = 0;
//     MyDisplayData.Param_Temp = 0;

//     // �����ʾ ��λ ����
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
//         // ��ʾ��ѹ����
//         case PROJECT_DISPLAYPARAM_INDEX_VOLTAGE:
//         {
//             MyDisplayData.Param_Voltage = 1;        // ��ʾ��ѹ����

//             // ���˼·����ʾ3λ����λ+1λС��λ��ParamDataΪ�Ŵ�100��ֵ����Ҫ��д100������ʾ
//             SEGx4_ShowNumber_Float(0, (float)(ParamData)/100.0, 3, 1);

//             MyDisplayData.Unit_V = 1;               // ��ʾ��ѹ��λ����λΪV
//         } break;
//         // ��ʾ��������
//         case PROJECT_DISPLAYPARAM_INDEX_CURRENT:
//         {
//             MyDisplayData.Param_Current = 1;        // ��ʾ��������

//             // ���˼·����ʾ2λ����λ+2λС��λ��ParamDataΪ�Ŵ�100��ֵ����Ҫ��д100������ʾ
//             SEGx4_ShowNumber_Float(0, (float)(ParamData)/100.0, 2, 2);

//             MyDisplayData.Unit_A = 1;               // ��ʾ������λ����λΪA
//         } break;
//         // ��ʾ©������
//         case PROJECT_DISPLAYPARAM_INDEX_LEAKAGE:
//         {
//             // ���˼·: ©�����ɡ�©��+�����������������
//             MyDisplayData.Param_Leakage = 1;        // ��ʾ©�����
//             MyDisplayData.Param_Current = 1;

//             // ���˼·����ʾ2λ����λ+1λС��λ��ParamDataΪ�Ŵ�100��ֵ����Ҫ��д100������ʾ
//             SEGx4_ShowNumber_Float(0, (float)(ParamData)/100.0, 3, 1);

//             MyDisplayData.Unit_M = 1;               // ��ʾ©�絥λ����λΪmA
//             MyDisplayData.Unit_A = 1;
//         } break;
//         //��ʾ�¶�����
//         case PROJECT_DISPLAYPARAM_INDEX_TEMP:
//         {
//             // ���˼·: ��ʾ�¶ȱ���
//             MyDisplayData.Param_Temp = 1;        // ��ʾ�¶ȱ���

//             // ���˼·����ʾ2λ����λ+1λС��λ��ParamDataΪ�Ŵ�100��ֵ����Ҫ��д100������ʾ
//             SEGx4_ShowNumber_Float(0, (float)(ParamData)/100.0, 3, 1);

//             MyDisplayData.Unit_C = 1;
//         } break;
// //
// //        // ��ʾ��������
// //        case PROJECT_DISPLAYPARAM_INDEX_POWER:
// //        {
// //            MyDisplayData.Param_Power = 1;          // ��ʾ���ʱ���
// //
// //            // ���˼·����ʾ4λ����λ��ParamDataΪ�Ŵ�10��ֵ����Ҫ��д10������ʾ
// //            SEGx4_ShowNumber_Integer(0, (float)(*((uint16_t *)ParamData))/10, 4);
// //
// //            MyDisplayData.Unit_W = 1;               // ��ʾ���ʵ�λ����λΪW
// //        } break;

//         default:
//         {

//         } break;
//     }
//     TM1621_WriteBuffer(&Project_TM1621, 0, MyDisplayData.DataArray, 8);
// }

//�豸״̬��ʾ����
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


