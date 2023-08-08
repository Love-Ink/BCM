#include "ConfigParam.h"

void Application_CfgParam_Init() {
    // ConfigVal.Current = 15.0;
    // ConfigVal.Leakage_Current = 30.0;
    // ConfigVal.Preheating_Detection_Time = 7200;
    // ConfigVal.Preheating_Temp_Time = 900;
    // ConfigVal.Preheating_Time = 60;
    // ConfigVal.Temp_Max = 110.0;
    // ConfigVal.Temp_Min = 75.0;
    // ConfigVal.Upkeep_Detection_Systemopen_Time = 7200000;
    // ConfigVal.Upkeep_Detection_Time = 31536000;
    // ConfigVal.Voltage_Max = 450.0;
    // ConfigVal.Voltage_Min = 340.0;

    ConfigParameters_Creat(CFG_MODE_Preheating_Detection_Time_PAGE, &ConfigVal.Preheating_Detection_Time, 3600, PROJECT_TOTAOLTIME_SETMAX, PROJECT_TOTAOLTIME_SETMIN);
    ConfigParameters_Creat(CFG_MODE_Preheating_Temp_Time_PAGE, &ConfigVal.Preheating_Temp_Time, 60, PROJECT_DEVICETIME_SETMAX, PROJECT_DEVICETIME_SETMIN);
    ConfigParameters_Creat(CFG_MODE_Upkeep_Detection_Time_PAGE, &ConfigVal.Upkeep_Detection_Time, 86400, PROJECT_FITTING_SETMAX, PROJECT_FITTING_SETMIN);
    ConfigParameters_Creat(CFG_MODE_Upkeep_Detection_Systemopen_Time_PAGE, &ConfigVal.Upkeep_Detection_Systemopen_Time, 3600, PROJECT_SYSTEMOPEN_SETMAX, PROJECT_SYSTEMOPEN_SETMIN);
    ConfigParameters_Creat(CFG_MODE_Temp_Max_PAGE, &ConfigVal.Temp_Max, 1, PROJECT_TEMP_HIGH, PROJECT_TEMP_LOW);
    ConfigParameters_Creat(CFG_MODE_Temp_Min_PAGE, &ConfigVal.Temp_Min, 1, PROJECT_TEMP_HIGH, PROJECT_TEMP_LOW);
    ConfigParameters_Creat(CFG_MODE_Preheating_Time_PAGE, &ConfigVal.Preheating_Time, 1, PROJECT_WARMTIME_HIGH, PROJECT_WARMTIME_LOW);
    ConfigParameters_Creat(CFG_MODE_Voltage_Max_PAGE, &ConfigVal.Voltage_Max, 1, PROJECT_VOLTAGEPARAM_SETTINGHIGH, PROJECT_VOLTAGEPARAM_SETTINGLOW);
    ConfigParameters_Creat(CFG_MODE_Voltage_Min_PAGE, &ConfigVal.Voltage_Min, 1, PROJECT_VOLTAGEPARAM_SETTINGHIGH, PROJECT_VOLTAGEPARAM_SETTINGLOW);
    ConfigParameters_Creat(CFG_MODE_Current_Max_PAGE, &ConfigVal.Current, 1, PROJECT_CURRENTPARAM_SETTINGMAX, PROJECT_CURRENTPARAM_SETTINGMIN);
    ConfigParameters_Creat(CFG_MODE_Leakage_Current_Max_PAGE, &ConfigVal.Leakage_Current, 1, PROJECT_LEAKCURRENTPARAM_SETTINGMAX, PROJECT_LEAKCURRENTPARAM_SETTINGMIN);
}
