#ifndef __APPLICATION_KEY_H
#define __APPLICATION_KEY_H

#include "main.h"

extern uint16_t Key[3];

void Application_Key_Init();
void Get_Key_state(uint16_t ms);
#endif