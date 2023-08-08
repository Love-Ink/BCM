//--------------------------------------------------------------------------------------------------
// @�ļ���������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------

#include "TM1621.h"                                 // ��������ͷ�ļ� TM1621-32x4λ��ʾRAMLCD������
#include <stdio.h>
#include "public.h"

#include <ch32v20x_rcc.h>
//--------------------------------------------------------------------------------------------------
// @�궨�������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
// TM1621 ��ʶ��
#define TM1621_FLAG_CMD                   (0x04)          // TM1621 ��ʶ �����ʶ�� b100
#define TM1621_FLAG_DATA                  (0x05)          // TM1621 ��ʶ ���ݱ�ʶ�� b101

// TM1621 ����
#define TM1621_CMD_SYSEN                  (0x0002)        // TM1621 ���� ��ϵͳ����
#define TM1621_CMD_SYSDIS                 (0x0000)        // TM1621 ���� �ر�ϵͳ������LCDƫѹ������

#define TM1621_CMD_WDTDIS                 (0x000A)        // TM1621 ���� WDT���Ź������־λ��� ��ֹ
#define TM1621_CMD_WDTEN                  (0x000E)        // TM1621 ���� WDT���Ź������־λ��� ����

#define TM1621_CMD_LCDON                  (0x0006)        // TM1621 ���� LCDƫѹ������ ��
#define TM1621_CMD_LCDOFF                 (0x0004)        // TM1621 ���� LCDƫѹ������ �ر�

#define TM1621_CMD_XTAL32K                (0x0028)        // TM1621 ���� ϵͳʱ��Դѡ�� �ⲿ����32.768KHZ
#define TM1621_CMD_RC256K                 (0x0030)        // TM1621 ���� ϵͳʱ��Դѡ�� Ƭ��RC����256KHz
#define TM1621_CMD_EXT256K                (0x0038)        // TM1621 ���� ϵͳʱ��Դѡ�� �ⲿʱ��Դ256KHz

#define TM1621_CMD_BIASCOM                (0x0040)        // TM1621 ���� ���ù����ں�ƫѹ������

#define TM1621_CMD_BIASCOM_MASK_1P2       (0x0000)        // TM1621 ���� ���ù����ں�ƫѹ������ ���� 1/2ƫѹѡ��
#define TM1621_CMD_BIASCOM_MASK_1P3       (0x0002)        // TM1621 ���� ���ù����ں�ƫѹ������ ���� 1/3ƫѹѡ��

#define TM1621_CMD_BIASCOM_MASK_COM2      (0x0000)        // TM1621 ���� ���ù����ں�ƫѹ������ ���� ʹ��2��������
#define TM1621_CMD_BIASCOM_MASK_COM3      (0x0008)        // TM1621 ���� ���ù����ں�ƫѹ������ ���� ʹ��3��������
#define TM1621_CMD_BIASCOM_MASK_COM4      (0x0010)        // TM1621 ���� ���ù����ں�ƫѹ������ ���� ʹ��4��������

#define TM1621_CMD_TOPT                   (0x01C0)        // TM1621 ���� ģʽѡ�� ����ģʽ
#define TM1621_CMD_TNORMAL                (0x01C6)        // TM1621 ���� ģʽѡ�� ��ͨģʽ

//--------------------------------------------------------------------------------------------------
// @������������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
TM1621_Handle_t     Project_TM1621;
//--------------------------------------------------------------------------------------------------
// @������������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
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
// @��������: TM1621 �ṹ���ʼ��
// @�������: ������������
// @����˵��: ��
//--------------------------------------------------------------------------------------------------
// @�������: ��
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void TM1621_StructInit(TM1621_Handle_t *TM1621_Handle)
{
    TM1621_Handle->Clock  = TM1621_CLOCK_RC256K;    // ʱ��ѡ�� Ĭ��ΪƬ��RC����256KHz
    TM1621_Handle->Bias   = TM1621_BIAS_1P3;        // ƫѹѡ�� Ĭ��Ϊ1/3ƫ�õ�ѹ
    TM1621_Handle->Common = TM1621_COM_4;           // ������   Ĭ��Ϊʹ��4��������

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
// @��������: TM1621 ��ʼ��
// @�������: ������������
// @����˵��: ��
//--------------------------------------------------------------------------------------------------
// @�������: ��
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
uint8_t TM1621_Init(TM1621_Handle_t *TM1621_Handle)
{
    uint16_t Temp = 0x0000;                         // ����ֲ����� ���ڱ�����ʱ�洢

//    // ���Լ�����
//    Assert_Param(NULL != TM1621_Handle->CS_GPIO_ResetBit_Function);         // Ҫ����Ϊ�ǿ�ָ��
//    Assert_Param(NULL != TM1621_Handle->CS_GPIO_SetBit_Function);           // Ҫ����Ϊ�ǿ�ָ��
//    Assert_Param(NULL != TM1621_Handle->WR_GPIO_ResetBit_Function);         // Ҫ����Ϊ�ǿ�ָ��
//    Assert_Param(NULL != TM1621_Handle->WR_GPIO_SetBit_Function);           // Ҫ����Ϊ�ǿ�ָ��
//    Assert_Param(NULL != TM1621_Handle->RD_GPIO_ResetBit_Function);         // Ҫ����Ϊ�ǿ�ָ��
//    Assert_Param(NULL != TM1621_Handle->RD_GPIO_SetBit_Function);           // Ҫ����Ϊ�ǿ�ָ��
//    Assert_Param(NULL != TM1621_Handle->DATA_GPIO_ResetBit_Function);       // Ҫ����Ϊ�ǿ�ָ��
//    Assert_Param(NULL != TM1621_Handle->DATA_GPIO_SetBit_Function);         // Ҫ����Ϊ�ǿ�ָ��
//    Assert_Param(NULL != TM1621_Handle->DELAY_nUS_Function);                // Ҫ����Ϊ�ǿ�ָ��

    // �˿����ų�ʼ��
    TM1621_Handle->CS_GPIO_SetBit_Function();       // Ƭѡ�������� ����ߵ�ƽ
    TM1621_Handle->WR_GPIO_SetBit_Function();       // д���� ����ߵ�ƽ

    TM1621_Handle->DELAY_nUS_Function(1000);        // �ӳ�һ��ʱ��

    // ����ƫ�õ�ѹ��ֵ
    if(TM1621_BIAS_1P2 == TM1621_Handle->Bias)
    {
        Temp |= TM1621_CMD_BIASCOM_MASK_1P2;
    }
    else if(TM1621_BIAS_1P3 == TM1621_Handle->Bias)
    {
        Temp |= TM1621_CMD_BIASCOM_MASK_1P3;
    }
    // ���㹫������ֵ
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

    // �������ܳ�ʼ��
    TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_BIASCOM | Temp);   // д���� ���ù����ں�ƫѹ������

    if(TM1621_CLOCK_XTAL32K == TM1621_Handle->Clock)
    {
        TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_XTAL32K);      // д���� ϵͳʱ��Դѡ�� �ⲿ����32.768KHZ
    }
    else if(TM1621_CLOCK_RC256K == TM1621_Handle->Clock)
    {
        TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_RC256K);       // д���� ϵͳʱ��Դѡ�� Ƭ��RC����256KHz
    }
    else if(TM1621_CLOCK_EXT256K == TM1621_Handle->Clock)
    {
        TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_EXT256K);      // д���� ϵͳʱ��Դѡ�� �ⲿʱ��Դ256KHz
    }

    TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_SYSDIS);           // д���� �ر�ϵͳ������LCDƫѹ������
    TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_WDTDIS);           // д���� WDT���Ź������־λ��� ��ֹ

    TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_SYSEN);            // д���� ��ϵͳ����
    TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_TNORMAL);          // д���� ģʽѡ�� ��ͨģʽ
    TM1621_WriteCommad(TM1621_Handle, TM1621_CMD_LCDON);            // д���� LCDƫѹ������ ��

//    TM1621_ClearAll(TM1621_Handle);                                 // �����Ļ��������

    return TM1621_SUCCESS;
}

//==================================================================================================
// @��������: TM1621 ����λ ��˷�ʽ
// @�������: ������������
// @����˵��: ��˷�ʽΪ��λ�ȷ��ͣ���λ����
//--------------------------------------------------------------------------------------------------
// @�������: Byte - �������ֽ�
// @�������: Count - ������λ������
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
static void TM1621_WriteBit_MSB(TM1621_Handle_t *TM1621_Handle, uint32_t Data, uint8_t Count)
{
    int8_t i = 0;                                   // ����ֲ����� ���ں���ѭ������

    for(i=(Count-1); i>=0; --i)
    {
        TM1621_Handle->WR_GPIO_ResetBit_Function(); // д���� ����͵�ƽ

        if((Data >> i) & 0x01)                      // �жϸ�λ�����Ƿ�Ϊ1
        {
            TM1621_Handle->DATA_GPIO_SetBit_Function();             // �������� ����ߵ�ƽ
        }
        else
        {
            TM1621_Handle->DATA_GPIO_ResetBit_Function();           // �������� ����͵�ƽ
        }
        TM1621_Handle->DELAY_nUS_Function(1);       // �ӳ�һ��ʱ��
        TM1621_Handle->WR_GPIO_SetBit_Function();   // д���� ����ߵ�ƽ
        TM1621_Handle->DELAY_nUS_Function(1);       // �ӳ�һ��ʱ��
    }

    TM1621_Handle->WR_GPIO_ResetBit_Function();     // д���� ����͵�ƽ
    TM1621_Handle->DATA_GPIO_ResetBit_Function();   // �������� ����͵�ƽ
}

//==================================================================================================
// @��������: TM1621 ����λ С�˷�ʽ
// @�������: ������������
// @����˵��: ��˷�ʽΪ��λ�ȷ��ͣ���λ����
//--------------------------------------------------------------------------------------------------
// @�������: Byte - �������ֽ�
// @�������: Count - ������λ������
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
static void TM1621_WriteBit_LSB(TM1621_Handle_t *TM1621_Handle, uint32_t Data, uint8_t Count)
{
    uint8_t i = 0;                                  // ����ֲ����� ���ں���ѭ������

    for(i=0; i<Count; i++)
    {
        TM1621_Handle->WR_GPIO_ResetBit_Function(); // д���� ����͵�ƽ

        if((Data >> i) & 0x01)                      // �жϸ�λ�����Ƿ�Ϊ1
        {
            TM1621_Handle->DATA_GPIO_SetBit_Function();             // �������� ����ߵ�ƽ
        }
        else
        {
            TM1621_Handle->DATA_GPIO_ResetBit_Function();           // �������� ����͵�ƽ
        }
        TM1621_Handle->DELAY_nUS_Function(1);       // �ӳ�һ��ʱ��
        TM1621_Handle->WR_GPIO_SetBit_Function();   // д���� ����ߵ�ƽ
        TM1621_Handle->DELAY_nUS_Function(1);       // �ӳ�һ��ʱ��
    }

    TM1621_Handle->WR_GPIO_ResetBit_Function();     // д���� ����͵�ƽ
    TM1621_Handle->DATA_GPIO_ResetBit_Function();   // �������� ����͵�ƽ
}

//==================================================================================================
// @��������: TM1621 д����
// @�������: ������������
// @����˵��: ��
//--------------------------------------------------------------------------------------------------
// @�������: Commad - ��������
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void TM1621_WriteCommad(TM1621_Handle_t *TM1621_Handle, uint16_t Commad)
{


    TM1621_Handle->CS_GPIO_ResetBit_Function();     // Ƭѡ���� ����͵�ƽ
    TM1621_Handle->DELAY_nUS_Function(1);           // �ӳ�һ��ʱ��

    TM1621_WriteBit_MSB(TM1621_Handle, TM1621_FLAG_CMD, 3);         // дǰ3λ��ʶ��
    TM1621_WriteBit_MSB(TM1621_Handle, Commad, 9);                  // д��9λ���������

    TM1621_Handle->CS_GPIO_SetBit_Function();       // Ƭѡ���� ����ߵ�ƽ
    TM1621_Handle->DELAY_nUS_Function(1);           // �ӳ�һ��ʱ��


}

//==================================================================================================
// @��������: TM1621 д1������
// @�������: ������������
// @����˵��: ָ����ַ��д���ݣ�8λ�ֳ�2��SEGλ
//--------------------------------------------------------------------------------------------------
// @�������: Address - ��ַ  ȡֵ��Χ - 0~31
// @�������: Byte - �ֽ�  ȡֵ��Χ - ��4λ��Ч
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void TM1621_WriteByte(TM1621_Handle_t *TM1621_Handle, uint8_t Address, uint8_t Byte)
{


    TM1621_Handle->CS_GPIO_ResetBit_Function();     // Ƭѡ���� ����͵�ƽ
    TM1621_Handle->DELAY_nUS_Function(1);           // �ӳ�һ��ʱ��

    TM1621_WriteBit_MSB(TM1621_Handle, TM1621_FLAG_DATA, 3);        // ִ��д���ݲ��� дǰ3λ��ʶ��
    TM1621_WriteBit_MSB(TM1621_Handle, Address, 6);                 // ����6λ��ַ �Ӹ�λ��ʼд���� �ֱ�Ϊ(A5-A4-A3-A2-A1-A0)

    TM1621_WriteBit_LSB(TM1621_Handle, Byte, 8);                    // д�뵥��4λ����

    TM1621_Handle->CS_GPIO_SetBit_Function();       // Ƭѡ���� ����ߵ�ƽ
    TM1621_Handle->DELAY_nUS_Function(1);           // �ӳ�һ��ʱ��

}

//==================================================================================================
// @��������: TM1621 д�������
// @�������: ������������
// @����˵��: ָ����ַ��д���ݣ�8λ�ֳ�2��SEGλ
//--------------------------------------------------------------------------------------------------
// @�������: TM1621_Handle - TM1621���
// @�������: Address - ��ַ  ȡֵ��Χ - 0~31
// @�������: Buffer - ���ֽ��׵�ַ  ȡֵ��Χ - ��4λ��Ч
// @�������: Count - �ֽ�����  ȡֵ��Χ - 0~31
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void TM1621_WriteBuffer(TM1621_Handle_t *TM1621_Handle, uint8_t Address, uint8_t *Buffer, uint8_t Count)
{
    uint8_t i=0;                                    // ����ֲ����� ���ں���ѭ������

    TM1621_Handle->CS_GPIO_ResetBit_Function();     // Ƭѡ���� ����͵�ƽ
    TM1621_Handle->DELAY_nUS_Function(1);           // �ӳ�һ��ʱ��

    TM1621_WriteBit_MSB(TM1621_Handle, TM1621_FLAG_DATA, 3);        // ִ��д���ݲ��� дǰ3λ��ʶ��
    TM1621_WriteBit_MSB(TM1621_Handle, Address, 6);                 // ����6λ��ַ �Ӹ�λ��ʼд���� �ֱ�Ϊ(A5-A4-A3-A2-A1-A0)

    for(i=0; i<Count; i++)
    {
        TM1621_WriteBit_LSB(TM1621_Handle, *Buffer, 8);             // д�뵥��4λ����
        Buffer++;                                                   // ָ���ƶ���һλ
    }

    TM1621_Handle->CS_GPIO_SetBit_Function();       // Ƭѡ���� ����ߵ�ƽ
    TM1621_Handle->DELAY_nUS_Function(1);           // �ӳ�һ��ʱ��

}

//==================================================================================================
// @��������: TM1621 ��ʾ��������
// @�������: ������������
// @����˵��: ��
//--------------------------------------------------------------------------------------------------
// @�������: ��
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void TM1621_ShowAll(TM1621_Handle_t *TM1621_Handle)
{
    uint8_t i=0;                                    // ����ֲ����� ���ں���ѭ������



    TM1621_Handle->CS_GPIO_ResetBit_Function();     // Ƭѡ���� ����͵�ƽ
    TM1621_Handle->DELAY_nUS_Function(1);           // �ӳ�һ��ʱ��

    TM1621_WriteBit_MSB(TM1621_Handle, TM1621_FLAG_DATA, 3);        // ִ��д���ݲ���
    TM1621_WriteBit_MSB(TM1621_Handle, 0, 6);                       // ����6λ��ַΪb000000
    for (i=0; i<16; i++)
    {
        TM1621_WriteBit_LSB(TM1621_Handle, 0xFF, 8);                // ����д��32��4λ����
    }
    TM1621_Handle->CS_GPIO_SetBit_Function();       // Ƭѡ���� ����ߵ�ƽ
    TM1621_Handle->DELAY_nUS_Function(1);           // �ӳ�һ��ʱ��


}

//==================================================================================================
// @��������: TM1621 �����������
// @�������: ������������
// @����˵��: ��
//--------------------------------------------------------------------------------------------------
// @�������: ��
// @�������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void TM1621_ClearAll(TM1621_Handle_t *TM1621_Handle)
{
    uint8_t i=0;                                    // ����ֲ����� ���ں���ѭ������



    TM1621_Handle->CS_GPIO_ResetBit_Function();     // Ƭѡ���� ����͵�ƽ
    TM1621_Handle->DELAY_nUS_Function(1);           // �ӳ�һ��ʱ��

    TM1621_WriteBit_MSB(TM1621_Handle, TM1621_FLAG_DATA, 3);        // ִ��д���ݲ���
    TM1621_WriteBit_MSB(TM1621_Handle, 0, 6);                       // ����6λ��ַΪb000000

    for (i=0; i<16; i++)
    {
        TM1621_WriteBit_LSB(TM1621_Handle, 0x00, 8);                // ����д��32��4λ����
    }
    TM1621_Handle->CS_GPIO_SetBit_Function();       // Ƭѡ���� ����ߵ�ƽ
    TM1621_Handle->DELAY_nUS_Function(1);           // �ӳ�һ��ʱ��


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
    Project_TM1621.Clock  = TM1621_CLOCK_RC256K;                                     // TM1621 ʱ��ѡ�� ����ΪƬ��RC����256KHz
    Project_TM1621.Bias   = TM1621_BIAS_1P3;                                         // TM1621 ƫѹѡ�� ����Ϊ1/3ƫ��
    Project_TM1621.Common = TM1621_COM_4;                                            // TM1621 ������ ����Ϊʹ��4��������
    Project_TM1621.CS_GPIO_ResetBit_Function   = TM1621_CS_ResetBit;                  // TM1621 CS/Ƭѡ���� GPIO����͵�ƽ����
    Project_TM1621.CS_GPIO_SetBit_Function     = TM1621_CS_SetBit;                    // TM1621 CS/Ƭѡ���� GPIO��λ�ߵ�ƽ����
    Project_TM1621.WR_GPIO_ResetBit_Function   = TM1621_WR_ResetBit;                  // TM1621 WR/д���� GPIO����͵�ƽ����
    Project_TM1621.WR_GPIO_SetBit_Function     = TM1621_WR_SetBit;                    // TM1621 WR/д���� GPIO��λ�ߵ�ƽ����
    Project_TM1621.RD_GPIO_ResetBit_Function   = NULL;                               // TM1621 RD/������ GPIO����͵�ƽ����
    Project_TM1621.RD_GPIO_SetBit_Function     = NULL;                               // TM1621 RD/������ GPIO��λ�ߵ�ƽ����
    Project_TM1621.DATA_GPIO_ResetBit_Function = TM1621_DATA_ResetBit;                  // TM1621 DATA/�������� GPIO����͵�ƽ����
    Project_TM1621.DATA_GPIO_SetBit_Function   = TM1621_DATA_SetBit;                    // TM1621 DATA/�������� GPIO��λ�ߵ�ƽ����
    Project_TM1621.DELAY_nUS_Function = (void (*)(uint16_t))Delay_Us;                // TM1621 DELAY�ӳ�΢�뺯��
    TM1621_Init(&Project_TM1621);

}
//========================================== �� �� �� �� ===========================================
