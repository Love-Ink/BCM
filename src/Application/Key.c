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
        case Short_Process :  //短按
            if(App_State.Cfg_Mode == 1) {
                ConfigParameters_PlusVal();  //当前所指向的配置参数加
            } else {
                if(App_State.Run_Flag == 0) {
                    App_State.Run_Flag = 1;  //电机运行
                    BlinkDevice_Config("BL", 100, 0, 0); //背光开启
                }
            }
            break;
        case Long_Process : //长按
            ;
            break;
        default : break;
    }
    return 0;
}

uint8_t Key2_Event_Func(FrameWork_ProcessTypes_Index Process_Types) {
        printf("key2 In\r\n");
    switch(Process_Types) {
        case Short_Process :    //短按
            if(App_State.Cfg_Mode == 1) {
                ConfigParameters_ReduceVal();   //当前所指向的配置参数减
            } else {
                if(App_State.Run_Flag == 1) {   //当处在运行时
                    App_State.Run_Flag = 0;     //电机关闭
                    BlinkDevice_Config("BL", 0, 100, 0); //背光关闭
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
        case Short_Process :    //短按
            if(App_State.Cfg_Mode == 1)     //配置模式下
                ConfigParameters_OrderSwitch(); //当前配置参数切换
            break;
        case Long_Process :     //长按
            if(App_State.Run_Flag == 0) {   //停机模式下
                if(App_State.Cfg_Mode == 0) {
                    App_State.Cfg_Mode = 1; //进入配置模式
                    ConfigParameters_GoHeadPage();  //配置参数切换到第一个
                    Task_Suspend(12, Task_State_Suspend);   //挂起 任务ID 为 12 的任务
                    Task_Suspend(13, Task_State_Suspend);   //挂起 任务ID 为 13 的任务
                    BlinkDevice_Config("BL", 100, 0, 0);    //开启背光
                } else {
                    App_State.Cfg_Mode = 0; //退出配置模式
                    EEPROM_Write_ConfigVal();   //写入配置参数信息
                    Task_Suspend(12, Task_State_Run);   //就绪 任务ID 为 12 的任务
                    Task_Suspend(13, Task_State_Run);   //就绪 任务ID 为 13 的任务
                    BlinkDevice_Config("BL", 0, 100, 0);    //关闭背光
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


