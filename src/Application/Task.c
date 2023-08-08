#include "Task.h"

uint16_t Error_Page = 0, Run_Page = 0;

void Display_Cfg_Page(uint16_t Page, uint32_t Val);
void Display_Error_Page(uint16_t Page);
void Display_Run_Page(uint16_t Page);
void Display_Clear_Param(void);
void Motor_Control(uint8_t state);
void Solenoid_valve_Control(uint8_t state);

void Display_Task_Func(uint16_t ms) {
    if(App_State.Cfg_Mode == 1) {
        Display_Cfg_Page(ConfigParameters_GetID(), ConfigParameters_GetVal());
    } else {
        if(App_State.Error_State == 1) {
            Display_Error_Page(Error_Page);
        } else {
            Display_Run_Page(Run_Page);
            MyDisplayData.Fault_Null = 1;
            MyDisplayData.Param_Fault = 1;
        }
    }

    if((App_State.Run_Flag == 1)) {
        if(App_State.Motor_Flag == 1) {
            MyDisplayData.State_Stop = 0;
            MyDisplayData.State_Standby = 0;
            //显示运行
            MyDisplayData.State_Run = 1;
        } else {
            MyDisplayData.State_Run = 0;
            MyDisplayData.State_Stop = 0;
            //显示待机
            MyDisplayData.State_Standby = 1;
        }
    } else {
        MyDisplayData.State_Run = 0;
        MyDisplayData.State_Standby = 0;
        //显示停机
        MyDisplayData.State_Stop = 1;
    }

    TM1621_WriteBuffer(&Project_TM1621, 0, MyDisplayData.DataArray, 8);
}

void Device_Control_Task_Func(uint16_t ms) {
    if(App_State.Motor_Flag == 1) {
        Motor_Control(1);
        BlinkDevice_Config("LED1", 10, 0, 0);
    } else {
        Motor_Control(0);
        BlinkDevice_Config("LED1", 0, 10, 0);
    }

    if(App_State.Solenoid_valve_Flag == 1) {
        Solenoid_valve_Control(1);
        BlinkDevice_Config("LED2", 10, 0 ,0);
    } else {
        Solenoid_valve_Control(0);
        BlinkDevice_Config("LED2", 0, 10, 0);
    }

    if(App_State.Upkeep_Flag == 1) {
        BlinkDevice_Config("LED3", 10, 0 ,0);
    } else {
        BlinkDevice_Config("LED3", 0, 10, 0);
    }

    if(App_State.Preheating_Flag == 1) {
        BlinkDevice_Config("LED4", 10, 0 ,0);
    } else {
        BlinkDevice_Config("LED4", 0, 10, 0);
    }

    if(App_State.Error_State != 0) {
        BlinkDevice_Config("LED5", 10, 0 ,0);
    } else {
        BlinkDevice_Config("LED5", 0, 10, 0);
    }
}

void Run_State_Control_Task_Func (uint16_t ms) {
    App_State.Pressure_Switch_State = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
    if(App_State.Error_State != 0) {
        App_State.Motor_Flag = 0;
        App_State.Solenoid_valve_Flag = 0;
    } else {
        if(App_State.Upkeep_Flag == 2) {
            App_State.Run_Flag = 0;
        }

        if(App_State.Run_Flag == 1) {
            App_State.Motor_Flag = !App_State.Pressure_Switch_State;
        } else {
            App_State.Motor_Flag = 0;
            App_State.Solenoid_valve_Flag = 0;
        }
    }
}

void Temp_Preheating_Task_Delete() {
    App_State.Preheating_Flag = 0;
}


void Temp_Preheating_Task_Func(uint16_t ms) {
    if(App_State.Preheating_Flag == 2) {
        if(ADC_Measuring.Temperature <= 70.0) { //低于70 开始进行防冷凝水措施
            Task_Cfg_Life(Task_ID_Prheating_Temp, 0, Temp_Preheating_Task_Delete);  //寿命无限长
            App_State.Preheating_Flag = 3;
        } 
    } else if (App_State.Preheating_Flag == 3){ //预热模式后处理-防冷凝水中 3
        if(ADC_Measuring.Temperature >= 80.0) {
            App_State.Preheating_Flag = 0;
            Task_Cfg_Life(Task_ID_Prheating_Temp, 5, Temp_Preheating_Task_Delete);  //寿命倒计时 10秒
        }
    } else {
        //
    }
}

void Preheating_Task_Func(uint16_t ms) {
    if(App_State.Preheating_Flag == 0) {
        if(Undul_Time.Run_Temp_Time >= ConfigVal.Preheating_Temp_Time) {
            //预热后的操作
            return ;
        } else {
            App_State.Preheating_Flag = 1;
            Task_Cfg_Time(Task_ID_Prheating, 2000, 2000); //变换周期 进入预热
        }
    } else if(App_State.Preheating_Flag == 1) { //预热模式开启 1
        if(Undul_Time.Preheating_Temp_Time >= ConfigVal.Preheating_Time ) { //如果达到预热时间
            App_State.Preheating_Flag = 2; //预热模式后处理—防冷凝水监测 2
            CreatTask_Func(Task_ID_Prheating_Temp, 2000, Temp_Preheating_Task_Func); //创建临时任务-防冷凝水
            Task_Cfg_Life(Task_ID_Prheating_Temp, 450, Temp_Preheating_Task_Delete); //设置寿命 15分钟
        }
    } else {
        //
    }
}

void Upkeep_Task_Func(uint16_t ms) {
    if(App_State.Upkeep_Flag == 0) {
        App_State.Upkeep_Flag = 1; //提醒保养
    } else {
        if(Undul_Time.RTC_Time >= ConfigVal.Upkeep_Detection_Time + PROJECT_FITTING_TOO_LONG) {
            App_State.Upkeep_Flag = 2;
        }
    }
}

void Motor_Control(uint8_t state) {
    if(state) {
        GPIO_WriteBit(GPIOB, GPIO_Pin_14, 1);
        GPIO_WriteBit(GPIOB, GPIO_Pin_3, 1);
        GPIO_WriteBit(GPIOA, GPIO_Pin_14, 1);
    } else {
        GPIO_WriteBit(GPIOB, GPIO_Pin_14, 0);
        Delay_Ms(30);
        GPIO_WriteBit(GPIOB, GPIO_Pin_3, 0);
        Delay_Ms(30);
        GPIO_WriteBit(GPIOA, GPIO_Pin_14, 0);
        Delay_Ms(30);
    }
}

void Solenoid_valve_Control(uint8_t state) {
    if(state) {
        GPIO_WriteBit(GPIOB, GPIO_Pin_7, 1);
    } else {
        GPIO_WriteBit(GPIOB, GPIO_Pin_7, 0);
    }
}

void Display_Cfg_Page(uint16_t Page, uint32_t Val) {
    switch (Page) {
        case CFG_MODE_Preheating_Time_PAGE : //预热时间
            Display_Clear_Param();
            MyDisplayData.Param_Time = 1;        // 显示时间标题
            Val = Val;       //转化秒
            Project_Display_Segment_ShowIntegerNumber(0,Val,4);//显示小时数
            break;
        case CFG_MODE_Preheating_Temp_Time_PAGE : //温度正常时间
            Display_Clear_Param();
            MyDisplayData.Param_Time = 1;        // 显示时间标题
            MyDisplayData.Unit_M = 1;
            Val = Val/60;       //转化分钟数
            Project_Display_Segment_ShowIntegerNumber(0,Val,4);//显示分钟数
            break;
        case CFG_MODE_Preheating_Detection_Time_PAGE : //预热检测周期
            Display_Clear_Param();
            MyDisplayData.Param_Time = 1;        // 显示时间标题
            MyDisplayData.Unit_h = 1;
            Val = Val/3600;       //转化小时数
            Project_Display_Segment_ShowIntegerNumber(0,Val,4);//显示分钟数
            break;
        case CFG_MODE_Upkeep_Detection_Time_PAGE : //保养检测周期
            Display_Clear_Param();
            MyDisplayData.Param_Time = 1;        // 显示时间标题
            Val = Val/86400;       //转化天数
            Project_Display_Segment_ShowIntegerNumber(0,Val,4);//显示天数
            break;
        case CFG_MODE_Upkeep_Detection_Systemopen_Time_PAGE : //保养检测周期
            Display_Clear_Param();
            MyDisplayData.Param_Time = 1;        // 显示时间标题
            MyDisplayData.Unit_h = 1;            
            Val = Val/3600;       //转化小时数
            Project_Display_Segment_ShowIntegerNumber(0,Val,4);//显示小时数            
            break;
        case CFG_MODE_Temp_Max_PAGE :
            Display_Clear_Param(); 
            MyDisplayData.Param_Temp = 1;
            MyDisplayData.Unit_C = 1;
            Project_Display_Segment_ShowIntegerNumber(0,Val,4);//显示最高温度
            break;
        case CFG_MODE_Temp_Min_PAGE :
            Display_Clear_Param(); 
            MyDisplayData.Param_Temp = 1;
            MyDisplayData.Unit_C = 1;
            Project_Display_Segment_ShowIntegerNumber(0,Val,4);//显示最低温度            
            break;
        case CFG_MODE_Voltage_Max_PAGE :
            Display_Clear_Param(); 
            MyDisplayData.Param_Voltage = 1;
            MyDisplayData.Unit_V = 1;
            Project_Display_Segment_ShowIntegerNumber(0,Val,4);//显示最高电压              
            break;
        case CFG_MODE_Voltage_Min_PAGE :
            Display_Clear_Param(); 
            MyDisplayData.Param_Voltage = 1;
            MyDisplayData.Unit_V = 1;
            Project_Display_Segment_ShowIntegerNumber(0,Val,4);//显示最低电压              
            break;
        case CFG_MODE_Current_Max_PAGE :
            Display_Clear_Param(); 
            MyDisplayData.Param_Current = 1;
            MyDisplayData.Unit_A = 1;
            Project_Display_Segment_ShowIntegerNumber(0,Val,4);//显示最大电流                   
            break;
        case CFG_MODE_Leakage_Current_Max_PAGE :
            Display_Clear_Param(); 
            MyDisplayData.Param_Current = 1;
            MyDisplayData.Param_Leakage = 1;
            MyDisplayData.Unit_A = 1;
            MyDisplayData.Unit_M = 1;
            Project_Display_Segment_ShowIntegerNumber(0,Val,4);//显示最大漏电流                      
            break;
        default :break;
    }
}

void Display_Clear_Param() {
    MyDisplayData.Param_Time    = 0;        // 清除时间标题
    MyDisplayData.Param_Current = 0;        // 清除电流标题
    MyDisplayData.Param_Temp    = 0;        // 清除温度标题
    MyDisplayData.Param_Voltage = 0;        // 清除电压标题
    MyDisplayData.Param_Leakage = 0;        // 清除漏电标题
    MyDisplayData.Unit_V = 0;               // 清除电压单位，单位为V
    MyDisplayData.Unit_A = 0;               // 清除电流单位，单位为A
    MyDisplayData.Unit_C = 0;               // 清除温度单位，单位为C
    MyDisplayData.Unit_M = 0;               // 清除分钟单位M
    MyDisplayData.Unit_h = 0;               // 清除小时单位h
}

void Display_Clear_Error_Param() {
    MyDisplayData.Param_Fault    = 0;       // 清除故障标题
    MyDisplayData.Fault_ChangeOil = 0;      // 清除换油标题
    MyDisplayData.Fault_Else = 0;           // 清除其他故障标题
    MyDisplayData.Fault_HighVoltage = 0;    // 清除超压标题
    MyDisplayData.Fault_Lackphase = 0;      // 清除缺相标题
    MyDisplayData.Fault_Leakage = 0;        // 清除漏电标题
    MyDisplayData.Fault_LowVoltage = 0;     // 清除欠压标题
    MyDisplayData.Fault_Null = 0;           // 清除无标题
    MyDisplayData.Fault_Overload = 0;       // 清除过载标题
    MyDisplayData.Param_Temp = 0;           // 清除温度标题
}

void Display_Error_Page(uint16_t Page) {
    switch (Page) {
        case ERROR_PAGE_LOWVOLTAGE :
            Display_Clear_Error_Param(); 
            MyDisplayData.Param_Fault  = 1;
            MyDisplayData.Fault_LowVoltage = 1;
            Project_Display_Segment_ShowStringNumber(0, (uint8_t *)"E", PROJECT_ERRORFAULTCODE_LOWVOLTAGE);
            break;
        case ERROR_PAGE_HIGHVOLTAGE :
            Display_Clear_Error_Param(); 
            MyDisplayData.Param_Fault  = 1;
            MyDisplayData.Fault_HighVoltage = 1;
            Project_Display_Segment_ShowStringNumber(0, (uint8_t *)"E", PROJECT_ERRORFAULTCODE_HIGHVOLTAGE);
            break;
        case ERROR_PAGE_LEAKAGE :
            Display_Clear_Error_Param(); 
            MyDisplayData.Param_Fault  = 1;
            MyDisplayData.Fault_Leakage = 1;
            Project_Display_Segment_ShowStringNumber(0, (uint8_t *)"E", PROJECT_ERRORFAULTCODE_LEAKAGE);
            break;
        case ERROR_PAGE_OVERLOAD : 
            Display_Clear_Error_Param(); 
            MyDisplayData.Param_Fault  = 1;
            MyDisplayData.Fault_Overload = 1;
            Project_Display_Segment_ShowStringNumber(0, (uint8_t *)"E", PROJECT_ERRORFAULTCODE_OVERLOAD);
            break;
        case ERROR_PAGE_TEMPERATURE :
            Display_Clear_Error_Param(); 
            MyDisplayData.Param_Fault  = 1;
            MyDisplayData.Fault_Else = 1;
            MyDisplayData.Param_Temp = 1;
            Project_Display_Segment_ShowStringNumber(0, (uint8_t *)"E", PROJECT_ERRORFAULTCODE_TEMPERATURE);
            break;
        case ERROR_PAGE_UNBALANCE :
            Display_Clear_Error_Param(); 
            MyDisplayData.Param_Fault = 1;
            MyDisplayData.Fault_Else = 1;
            Project_Display_Segment_ShowStringNumber(0, (uint8_t *)"E", PROJECT_ERRORFAULTCODE_UNBALANCE);
            break;
        default :break;
    }
}

void Display_Run_Page(uint16_t Page) {
    static int8_t back = -1;
    if(back == Page) {
        return ;
    } else {
        back = Page;
    }
    switch(Page) {
        case RUN_PAGE_VOLTAGE   :
            Display_Clear_Param();
            MyDisplayData.Param_Voltage = 1;
            MyDisplayData.Unit_V = 1;
            Project_Display_Segment_ShowFloatNumber(0, ADC_Measuring.Voltage, 3, 1);
            break;
        case RUN_PAGE_LEAKAGE   :
            Display_Clear_Param();
            MyDisplayData.Param_Current = 1;
            MyDisplayData.Param_Leakage = 1;
            MyDisplayData.Unit_M = 1;
            MyDisplayData.Unit_A = 1;
            Project_Display_Segment_ShowFloatNumber(0, ADC_Measuring.Leak_Current, 3, 1);
            break;
        case RUN_PAGE_TEMP      :
            Display_Clear_Param();
            MyDisplayData.Param_Temp = 1;
            MyDisplayData.Unit_C = 1;
            Project_Display_Segment_ShowFloatNumber(0, ADC_Measuring.Temperature, 3, 1);
            break;
        case RUN_PAGE_CURRENT   :
            Display_Clear_Param();
            MyDisplayData.Param_Current = 1;
            MyDisplayData.Unit_A = 1;
            Project_Display_Segment_ShowFloatNumber(0, ADC_Measuring.Average_Current, 2, 2);
            break;
        default :break;
    }
}