#include "Key.h"

uint16_t Key[3] = {0};
uint16_t Display_CfgMode_Page = 0;


uint8_t Get_Key1_State_Func() {
    return (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8));
}
uint8_t Get_Key2_State_Func() {
    return (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13));
}
uint8_t Get_Key3_State_Func() {
    return (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12));
}

uint8_t Key1_Event_Func(FrameWork_ProcessTypes_Index Process_Types) {
    printf("key1 In\r\n");
    switch(Process_Types) {
        case Short_Process :
            if(App_State.Cfg_Mode == 1) {
                ConfigParameters_PlusVal();
            } else {
                if(App_State.Run_Flag == 0) {
                    App_State.Run_Flag = 1;
                    BlinkDevice_Config("BL", 100, 0, 0);
                }
            }
            break;
        case Long_Process :
            ;
            break;
        default : break;
    }
    return 0;
}

uint8_t Key2_Event_Func(FrameWork_ProcessTypes_Index Process_Types) {
        printf("key2 In\r\n");
    switch(Process_Types) {
        case Short_Process :
            if(App_State.Cfg_Mode == 1) {
                ConfigParameters_ReduceVal();
            } else {
                if(App_State.Run_Flag == 1) {
                    App_State.Run_Flag = 0;
                    BlinkDevice_Config("BL", 0, 100, 0);
                }
            }
            break;
        case Long_Process :
            ;
            break;
        default : break;
    }
    return 0;
}

uint8_t Key3_Event_Func(FrameWork_ProcessTypes_Index Process_Types) {
    printf("key3 In\r\n");
    switch (Process_Types) {
        case Short_Process :
            if(App_State.Cfg_Mode == 1)
                ConfigParameters_OrderSwitch();
            break;
        case Long_Process :
            if(App_State.Run_Flag == 0) {
                if(App_State.Cfg_Mode == 0) {
                    App_State.Cfg_Mode = 1;
                    ConfigParameters_GoHeadPage();
                    Task_Suspend(12, Task_State_Suspend);
                    Task_Suspend(13, Task_State_Suspend);
                    BlinkDevice_Config("BL", 100, 0, 0);
                } else {
                    App_State.Cfg_Mode = 0;
                    EEPROM_Write_ConfigVal();
                    Task_Suspend(12, Task_State_Run);
                    Task_Suspend(13, Task_State_Run);
                    BlinkDevice_Config("BL", 0, 100, 0);
                }
            }
            break;
        default : break;
    }
    return 0;
}


void Get_Key_state(uint16_t ms) {
    Key[0] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);
    Key[1] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
    Key[2] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
}

/**
 * @brief 初始化 按键 GPIO ，并将Key注册为Key设备
 * 
 */
void Application_Key_Init() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    gpio_init(GPIOB, GPIO_Pin_8 | GPIO_Pin_13 | GPIO_Pin_12, GPIO_Mode_IPU);
    gpio_init(GPIOA, GPIO_Pin_4, GPIO_Mode_IN_FLOATING);
    Creat_Key(0, 1000, Get_Key1_State_Func, Key1_Event_Func);
    Creat_Key(0, 1000, Get_Key2_State_Func, Key2_Event_Func);
    Creat_Key(0, 1000, Get_Key3_State_Func, Key3_Event_Func);

}


