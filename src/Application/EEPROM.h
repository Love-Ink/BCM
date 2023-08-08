#ifndef __APPLICATION_EEPROM_H
#define __APPLICATION_EEPROM_H

#include "ch32v20x.h"
#include "main.h"

void Applicaiton_EEPROM_Init ();
void EEPROM_Write_ConfigVal();
void EEPROM_Write_TimeVal();
void EEPROM_Clear ();

#endif