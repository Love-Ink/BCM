/*
 * Display.h
 *
 *  Created on: Mar 28, 2023
 *      Author: bbhn2
 */

#ifndef APPLICATION_DISPLAY_H_
#define APPLICATION_DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//--------------------------------------------------------------------------------------------------
//  ͷ���ļ�����    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "TM1621.h"

//--------------------------------------------------------------------------------------------------
// @�궨�������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
//  ���� ��ʾ״̬ ����
#define PROJECT_DISPLAYSTATE_INDEX_RUN              (0)             // ����
#define PROJECT_DISPLAYSTATE_INDEX_WAIT             (1)             // ����
#define PROJECT_DISPLAYSTATE_INDEX_STOP             (2)             // ͣ��

//  ���� ��ʾ���� ����
// #define PROJECT_DISPLAYPARAM_INDEX_VOLTAGE          (PROJECT_DISPLAYDATA_VOLTAGE_SHIFT)     // ��ѹ
// #define PROJECT_DISPLAYPARAM_INDEX_CURRENT          (PROJECT_DISPLAYDATA_CURRENT_SHIFT)     // ����
// #define PROJECT_DISPLAYPARAM_INDEX_LEAKAGE          (PROJECT_DISPLAYDATA_LEAKAGE_SHIFT)     // ©��
// #define PROJECT_DISPLAYPARAM_INDEX_POWER            (PROJECT_DISPLAYDATA_POWER_SHIFT)       // ����
// #define PROJECT_DISPLAYPARAM_INDEX_TEMP             (PROJECT_DISPLAYDATA_TEMP_SHIFT)        // �¶�
//#define PROJECT_DISPLAYPARAM_INDEX_VOLTAGE          (1)     // ��ѹ
//#define PROJECT_DISPLAYPARAM_INDEX_CURRENT          (2)     // ����
//#define PROJECT_DISPLAYPARAM_INDEX_LEAKAGE          (3)     // ©��
//#define PROJECT_DISPLAYPARAM_INDEX_POWER            (4)     // ����
// #define PROJECT_DISPLAYPARAM_INDEX_ENERGYNOW        (PROJECT_DISPLAYDATA_ENERGYNOW_SHIFT)   // ��ǰ����
// #define PROJECT_DISPLAYPARAM_INDEX_ENERGYALL        (PROJECT_DISPLAYDATA_ENERGYALL_SHIFT)   // �ۼƵ���
// #define PROJECT_DISPLAYPARAM_INDEX_TEMPERATURE      (PROJECT_DISPLAYDATA_TEMPERATURE_SHIFT) // �¶�
// #define PROJECT_DISPLAYPARAM_INDEX_TIMEREAL         (PROJECT_DISPLAYDATA_TIMEREAL_SHIFT)    // ʵʱʱ��

#define PROJECT_ERRORFAULTCODE_LEAKAGE              (100)           // ��������� �豸���� ©�����
#define PROJECT_ERRORFAULTCODE_OVERLOAD             (101)           // ��������� �豸���� ���ع���
#define PROJECT_ERRORFAULTCODE_NULLLOAD             (102)           // ��������� �豸���� ���ع���
#define PROJECT_ERRORFAULTCODE_LOWVOLTAGE           (103)           // ��������� �豸���� Ƿѹ����
#define PROJECT_ERRORFAULTCODE_HIGHVOLTAGE          (104)           // ��������� �豸���� ��ѹ����
#define PROJECT_ERRORFAULTCODE_LACKPHASE            (105)           // ��������� �豸���� ȱ�����
#define PROJECT_ERRORFAULTCODE_UNBALANCE            (106)           // ��������� �豸���� ��ƽ��ȹ���
#define PROJECT_ERRORFAULTCODE_FASTTURN             (107)           // ��������� �豸���� Ƶ����ͣ����
#define PROJECT_ERRORFAULTCODE_LEAKWATER            (108)           // ��������� �豸���� ©ˮ����
#define PROJECT_ERRORFAULTCODE_OILCHANGE            (109)           // ��������� �豸���� ���͹���
#define PROJECT_ERRORFAULTCODE_TEMPERATURE          (110)           // ��������� �豸���� �¶ȹ���
#define PROJECT_ERRORFAULTCODE_FITTING              (111)           // ��������� �豸���� ��������
#define PROJECT_ERRORFAULTCODE_ELSEERROR            (112)           // ��������� �豸���� ��������

//  ���� ��ʾ���� ����
#define PROJECT_DISPLAYFAULT_MASK_NULL              (PROJECT_FAULT_NULL_MASK)               // ��
#define PROJECT_DISPLAYFAULT_MASK_LEAKAGE           (PROJECT_FAULT_LEAKAGE_MASK)            // ©��
#define PROJECT_DISPLAYFAULT_MASK_OVERLOAD          (PROJECT_FAULT_OVERLOAD_MASK)           // ����
#define PROJECT_DISPLAYFAULT_MASK_NULLLOAD          (PROJECT_FAULT_NULLLOAD_MASK)           // ����
#define PROJECT_DISPLAYFAULT_MASK_LOWVOLTAGE        (PROJECT_FAULT_LOWVOLTAGE_MASK)         // Ƿѹ
#define PROJECT_DISPLAYFAULT_MASK_HIGHVOLTAGE       (PROJECT_FAULT_HIGHVOLTAGE_MASK)        // ��ѹ
#define PROJECT_DISPLAYFAULT_MASK_LACKPHASE         (PROJECT_FAULT_LACKPHASE_MASK)          // ȱ��
#define PROJECT_DISPLAYFAULT_MASK_UNBALANCE         (PROJECT_FAULT_UNBALANCE_MASK)          // ��ƽ���
#define PROJECT_DISPLAYFAULT_MASK_FASTTURN          (PROJECT_FAULT_FASTTURN_MASK)           // Ƶ����ͣ
#define PROJECT_DISPLAYFAULT_MASK_LEAKWATER         (PROJECT_FAULT_LEAKWATER_MASK)          // ©ˮ
#define PROJECT_DISPLAYFAULT_MASK_OILCHANGE         (PROJECT_FAULT_OILCHANGE_MASK)          // ����
#define PROJECT_DISPLAYFAULT_MASK_TEMPERATURE       (PROJECT_FAULT_TEMPERATURE_MASK)        // �¶�
#define PROJECT_DISPLAYFAULT_MASK_ELSEERROR         (PROJECT_FAULT_ELSEERROR_MASK)          // ����

//--------------------------------------------------------------------------------------------------
// @������������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
//  ���ö������ṹ�����ϵ�֧��
#if 1
#if defined(__CC_ARM)
    #pragma push
    #pragma anon_unions
#elif defined(__ICCARM__)
    #pragma language=extended
#elif defined(__GNUC__)
    /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
    /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
    #pragma warning 586
#elif defined ( __CSMC__ )
    /* anonymous unions are enabled by default */
#else
    #warning Not supported compiler type
#endif
#endif
typedef union
{
    struct
    {
        // SEG0����Ļ����9���ֱ�ΪS9��S16��S20��P4����COM1��COM2��COM3��COM4��˳��
        uint64_t Param_Temp:1;              // �¶� ��ӦS9
        uint64_t Unit_C:1;                  // ��λ �� �¶ȵ�λ ��ӦS16
        uint64_t Unit_V:1;                  // ��λ V ��ѹ��λ ��ӦS20
        uint64_t SEG_4_DP:1;                // ����� ��4λDP�� ��ӦP4

        // SEG1����Ļ����10���ֱ�ΪS10��S15��S17��S21����COM1��COM2��COM3��COM4��˳��
        uint64_t Function_Debug:1;          // ���Թ��� ��ӦS10
        uint64_t Unit_1000:1;               // ��λ 1000�� ��ӦS15
        uint64_t Unit_K:1;                  // ��λ K ���KW/KWh ����/���ܵ�λ ��ӦS17
        uint64_t Unit_M:1;                  // ��λ M ���MA ������λ ��ӦS21

        // SEG2����Ļ����11���ֱ�ΪS11��S19��S18��S22����COM1��COM2��COM3��COM4��˳��
        uint64_t Function_Match:1;          // ��Թ��� ��ӦS11
        uint64_t Unit_h:1;                  // ��λ h KWh ����/ʱ�䵥λ ��ӦS19
        uint64_t Unit_W:1;                  // ��λ W ���KW/KWh ����/���ܵ�λ ��ӦS18
        uint64_t Unit_A:1;                  // ��λ A ������λ ��ӦS22

        // SEG3����Ļ����12���ֱ�ΪS8��S7��S6��S5����COM1��COM2��COM3��COM4��˳��
        uint64_t Param_Time:1;              // ʱ�� ��ӦS8
        uint64_t Param_Energy:1;            // ���� ��ӦS7
        uint64_t Param_Power:1;             // ���� ��ӦS6
        uint64_t Param_Current:1;           // ���� ��ӦS5

        // SEG4����Ļ����13���ֱ�ΪS1��S2��S3��S4����COM1��COM2��COM3��COM4��˳��
        uint64_t Param_Mode:1;              // ģʽ ��ӦS1
        uint64_t Param_Total:1;             // �ۼ� ��ӦS2
        uint64_t Param_Voltage:1;           // ��ѹ ��ӦS3
        uint64_t Param_Leakage:1;           // ©�� ��ӦS4

        // SEG5����Ļ����14���ֱ�Ϊ4B��4G��4C��COL����COM1��COM2��COM3��COM4��˳��
        uint64_t SEG_4_B:1;                 // ����� ��4λB�� ��Ӧ4B
        uint64_t SEG_4_G:1;                 // ����� ��4λG�� ��Ӧ4G
        uint64_t SEG_4_C:1;                 // ����� ��4λC�� ��Ӧ4C
        uint64_t SEG_Time:1;                // ����� ʱ��� ��ӦCOL

        // SEG6����Ļ����15���ֱ�Ϊ4A��4F��4E��4D����COM1��COM2��COM3��COM4��˳��
        uint64_t SEG_4_A:1;                 // ����� ��4λA�� ��Ӧ4A
        uint64_t SEG_4_F:1;                 // ����� ��4λF�� ��Ӧ4F
        uint64_t SEG_4_E:1;                 // ����� ��4λE�� ��Ӧ4E
        uint64_t SEG_4_D:1;                 // ����� ��4λD�� ��Ӧ4D

        // SEG7����Ļ����16���ֱ�Ϊ3B��3G��3C��P3����COM1��COM2��COM3��COM4��˳��
        uint64_t SEG_3_B:1;                 // ����� ��3λB�� ��Ӧ3B
        uint64_t SEG_3_G:1;                 // ����� ��3λG�� ��Ӧ3G
        uint64_t SEG_3_C:1;                 // ����� ��3λC�� ��Ӧ3C
        uint64_t SEG_3_DP:1;                // ����� ��3λDP�� ��ӦP3

        // SEG8����Ļ����17���ֱ�Ϊ3A��3F��3E��3D����COM1��COM2��COM3��COM4��˳��
        uint64_t SEG_3_A:1;                 // ����� ��3λA�� ��Ӧ3A
        uint64_t SEG_3_F:1;                 // ����� ��3λF�� ��Ӧ3F
        uint64_t SEG_3_E:1;                 // ����� ��3λE�� ��Ӧ3E
        uint64_t SEG_3_D:1;                 // ����� ��3λD�� ��Ӧ3D

        // SEG9����Ļ����18���ֱ�Ϊ2B��2G��2C��P2����COM1��COM2��COM3��COM4��˳��
        uint64_t SEG_2_B:1;                 // ����� ��2λB�� ��Ӧ2B
        uint64_t SEG_2_G:1;                 // ����� ��2λG�� ��Ӧ2G
        uint64_t SEG_2_C:1;                 // ����� ��2λC�� ��Ӧ2C
        uint64_t SEG_2_DP:1;                // ����� ��2λDP�� ��ӦP2

        // SEG10����Ļ����19���ֱ�Ϊ2A��2F��2E��2D����COM1��COM2��COM3��COM4��˳��
        uint64_t SEG_2_A:1;                 // ����� ��2λA�� ��Ӧ2A
        uint64_t SEG_2_F:1;                 // ����� ��2λF�� ��Ӧ2F
        uint64_t SEG_2_E:1;                 // ����� ��2λE�� ��Ӧ2E
        uint64_t SEG_2_D:1;                 // ����� ��2λD�� ��Ӧ2D

        // SEG11����Ļ����20���ֱ�Ϊ1B��1G��1C��P1����COM1��COM2��COM3��COM4��˳��
        uint64_t SEG_1_B:1;                 // ����� ��1λB�� ��Ӧ1B
        uint64_t SEG_1_G:1;                 // ����� ��1λG�� ��Ӧ1G
        uint64_t SEG_1_C:1;                 // ����� ��1λC�� ��Ӧ1C
        uint64_t SEG_1_DP:1;                // ����� ��1λDP�� ��ӦP1

        // SEG12����Ļ����1���ֱ�Ϊ1A��1F��1E��1D����COM1��COM2��COM3��COM4��˳��
        uint64_t SEG_1_A:1;                 // ����� ��1λA�� ��Ӧ1A
        uint64_t SEG_1_F:1;                 // ����� ��1λF�� ��Ӧ1F
        uint64_t SEG_1_E:1;                 // ����� ��1λE�� ��Ӧ1E
        uint64_t SEG_1_D:1;                 // ����� ��1λD�� ��Ӧ1D

        // SEG13����Ļ����2���ֱ�ΪS12��S13��S14��S23����COM1��COM2��COM3��COM4��˳��
        uint64_t State_Run:1;               // ���� ��ӦS12
        uint64_t State_Standby:1;           // ���� ��ӦS13
        uint64_t State_Stop:1;              // ͣ�� ��ӦS14
        uint64_t Param_Fault:1;             // ���� ��ӦS23

        // SEG14����Ļ����3���ֱ�ΪS27��S26��S25��S24����COM1��COM2��COM3��COM4��˳��
        uint64_t Fault_LowVoltage:1;        // Ƿѹ���� ��ӦS27
        uint64_t Fault_Overload:1;          // ���ع��� ��ӦS26
        uint64_t Fault_Leakage:1;           // ©����� ��ӦS25
        uint64_t Fault_Null:1;              // �޹��� ��ӦS24

        // SEG15����Ļ����4���ֱ�ΪS28��S29��S30��S31����COM1��COM2��COM3��COM4��˳��
        uint64_t Fault_HighVoltage:1;       // ��ѹ���� ��ӦS28
        uint64_t Fault_Lackphase:1;         // ȱ����� ��ӦS29
        uint64_t Fault_Else:1;              // �������� ��ӦS30
        uint64_t Fault_ChangeOil:1;         // ���͹��� ��ӦS31

    };
    struct
    {
        uint8_t  DataArray[8];
    };
    struct
    {
        uint64_t Data;
    };
} Project_DisplayData_t;

extern Project_DisplayData_t MyDisplayData;

#if 1
#if defined(__CC_ARM)
    #pragma pop
#elif defined(__ICCARM__)
    #pragma language=extended
#elif defined(__GNUC__)
    /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
    /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
    #pragma warning restore
#elif defined ( __CSMC__ )
    /* anonymous unions are enabled by default */
#else
    #warning Not supported compiler type
#endif
#endif
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//--------------------------------------------------------------------------------------------------
// @������������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
// ���� Һ������ʾ �������
void Project_Display_Clear(uint8_t State);

// ���� Һ������ʾ ��ʾָ������
void Project_Display_State(uint8_t State);                                  // ���� Һ������ʾ ���״̬
void Project_Display_Param(uint8_t ParamType, int32_t ParamData);           // ���� Һ������ʾ ��������
//void Project_ErrorDisplay(void);                                            // ���� Һ������ʾ ��������
void Project_Display_Mode(uint8_t Mode);                                    // ���� Һ������ʾ ģʽ
void Project_Display_Match(uint8_t Enable);                                 // ���� Һ������ʾ ƥ��
void Project_Display_OilChange(uint8_t Enable);                             // ���� Һ������ʾ ����
void Project_Display_NormalModeContent(uint16_t FaultType);                 // ���� Һ������ʾ ��������ģʽ��ģʽ����
void Project_Display_ShowControlState(uint8_t WorkState);                   // ���� Һ������ʾ ����״̬

// ���� Һ������ʾ �������ʾ����
void Project_Display_Segment_ShowString(uint8_t X,uint8_t *String);                                             // ���� Һ������ʾ ����� ��ʾ�ַ���
void Project_Display_Segment_ShowIntegerNumber(uint8_t X, int16_t IntegerNumber, uint8_t Count);                // ���� Һ������ʾ ����� ��ʾ����
void Project_Display_Segment_ShowFloatNumber(uint8_t X, float FloatNumber, uint8_t Count1, uint8_t Count2);     // ���� Һ������ʾ ����� ��ʾ��������
void Project_Display_Segment_ShowStringNumber(uint8_t X, uint8_t *String, uint16_t Number);                     // ���� Һ������ʾ ����� ��ʾ����ַ���������

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif /* APPLICATION_DISPLAY_H_ */

//========================================== �� �� �� �� ===========================================

