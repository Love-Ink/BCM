/*
 * TM1621.h
 *
 *  Created on: Mar 28, 2023
 *      Author: bbhn2
 */

#ifndef APPLICATION_TM1621_H_
#define APPLICATION_TM1621_H_

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//--------------------------------------------------------------------------------------------------
// @�ļ���������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
#include <stdint.h>
#include "ch32v20x.h"
//--------------------------------------------------------------------------------------------------
// @�궨�������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
// TM1621 ����궨��
#define TM1621_SUCCESS                      (0)             // TM1621 �ɹ�
#define TM1621_FAILURE                      (1)             // TM1621 ʧ��
#define TM1621_UNKNOWN                      (2)             // TM1621 δ֪

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TM1621 ʱ��Դѡ��궨��
#define TM1621_CLOCK_XTAL32K                (0)             // TM1621 ϵͳʱ��Դѡ�� �ⲿ����32.768KHZ
#define TM1621_CLOCK_RC256K                 (1)             // TM1621 ϵͳʱ��Դѡ�� Ƭ��RC����256KHz
#define TM1621_CLOCK_EXT256K                (2)             // TM1621 ϵͳʱ��Դѡ�� �ⲿʱ��Դ256KHz

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TM1621 ƫѹ��ѹѡ��궨��
#define TM1621_BIAS_1P2                     (0)             // TM1621 ƫѹ��ѹѡ�� 1/2ƫ�õ�ѹ
#define TM1621_BIAS_1P3                     (1)             // TM1621 ƫѹ��ѹѡ�� 1/3ƫ�õ�ѹ

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TM1621 ������ѡ��궨��
#define TM1621_COM_2                        (0)             // TM1621 ������ѡ�� ʹ��2��������
#define TM1621_COM_3                        (1)             // TM1621 ������ѡ�� ʹ��3��������
#define TM1621_COM_4                        (2)             // TM1621 ������ѡ�� ʹ��4��������

//--------------------------------------------------------------------------------------------------
// @������������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
typedef struct
{
    uint8_t Clock:2;                                // TM1621 ʱ��ѡ��  ȡֵ��Χ - �����TM1621_CLOCK�궨��˵��
    uint8_t Bias:2;                                 // TM1621 ƫѹѡ��  ȡֵ��Χ - �����TM1621_BIAS�궨��˵��
    uint8_t Common:2;                               // TM1621 ������ѡ��  ȡֵ��Χ - �����TM1621_COM�궨��˵��

    void (*CS_GPIO_ResetBit_Function)(void);        // TM1621 CS/Ƭѡ���� GPIO����͵�ƽ����
    void (*CS_GPIO_SetBit_Function)(void);          // TM1621 CS/Ƭѡ���� GPIO��λ�ߵ�ƽ����
    void (*WR_GPIO_ResetBit_Function)(void);        // TM1621 WR/д���� GPIO����͵�ƽ����
    void (*WR_GPIO_SetBit_Function)(void);          // TM1621 WR/д���� GPIO��λ�ߵ�ƽ����
    void (*RD_GPIO_ResetBit_Function)(void);        // TM1621 RD/������ GPIO����͵�ƽ����
    void (*RD_GPIO_SetBit_Function)(void);          // TM1621 RD/������ GPIO��λ�ߵ�ƽ����
    void (*DATA_GPIO_ResetBit_Function)(void);      // TM1621 DATA/�������� GPIO����͵�ƽ����
    void (*DATA_GPIO_SetBit_Function)(void);        // TM1621 DATA/�������� GPIO��λ�ߵ�ƽ����
    void (*DELAY_nUS_Function)(uint16_t Time);      // TM1621 DELAY�ӳ�΢�뺯��
} TM1621_Handle_t;

extern TM1621_Handle_t     Project_TM1621;
//--------------------------------------------------------------------------------------------------
// @������������    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
// TM1621 ��ʼ��
void TM1621_StructInit(TM1621_Handle_t *TM1621_Handle);
uint8_t TM1621_Init(TM1621_Handle_t *TM1621_Handle);

// TM1621 д����
void TM1621_WriteByte(TM1621_Handle_t *TM1621_Handle, uint8_t Address, uint8_t Byte);
void TM1621_WriteBuffer(TM1621_Handle_t *TM1621_Handle, uint8_t Address, uint8_t *Buffer, uint8_t Count);

// TM1621 ��ʾ/��� ��Ļ
void TM1621_ShowAll(TM1621_Handle_t *TM1621_Handle);
void TM1621_ClearAll(TM1621_Handle_t *TM1621_Handle);

void Application_TM1612_Init();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif /* APPLICATION_TM1621_H_ */
