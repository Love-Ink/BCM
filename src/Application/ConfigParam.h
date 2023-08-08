#ifndef __APPLICATION_CONFIGPARAM_H
#define __APPLICATION_CONFIGPARAM_H

#include "main.h"

#define PROJECT_FITTING_TOO_LONG                    (1296000)         //保养超时15天

#define PROJECT_VOLTAGEPARAM_SETTINGLOW             (300)           // 电压下限100V
#define PROJECT_VOLTAGEPARAM_SETTINGHIGH            (500)           // 电压上限500V
#define PROJECT_CURRENTPARAM_SETTINGMIN             (5)            // 电流下限5A
#define PROJECT_CURRENTPARAM_SETTINGMAX             (50)            // 电流上限50A
#define PROJECT_LEAKCURRENTPARAM_SETTINGMIN         (10)            // 漏电流下限10mA
#define PROJECT_LEAKCURRENTPARAM_SETTINGMAX         (30)            // 漏电流上限30mA
#define PROJECT_TEMP_LOW                            (40)            // 温度下限50度
#define PROJECT_TEMP_HIGH                           (150)           // 温度上限150度
#define PROJECT_WARMTIME_LOW                        (1)             // 预热时间下限1秒
#define PROJECT_WARMTIME_HIGH                       (3600)            // 预热时间上限60分钟
//#define PROJECT_TOTAlTIME_SET                       (180000)         //设备运行时间
//#define PROJECT_DEVICETIME_SET                      (18000)          //温度正常运行时间
#define PROJECT_TOTAOLTIME_SETMAX                   (3596400)         //检测周期上限为999小时
#define PROJECT_TOTAOLTIME_SETMIN                   (3600)            //检测周期下限为1小时
#define PROJECT_FITTING_SETMAX                      (31536000)        //检测保养周期时间上限365天(RTC运行时间超过)
#define PROJECT_FITTING_SETMIN                      (86400)           //检测保养周期时间下限1天(RTC运行时间超过)
#define PROJECT_SYSTEMOPEN_SETMAX                   (7200000)         //检测保养周期时间上限2000小时(开机运行超过runflag==1)
#define PROJECT_SYSTEMOPEN_SETMIN                   (36000)           //检测保养周期时间下限10小时(开机运行超过runflag==1)
#define PROJECT_DEVICETIME_SETMAX                   (59940)           //温度正常时间上限为999分钟
#define PROJECT_DEVICETIME_SETMIN                   (60)             //温度正常时间下限为1分钟

void Application_CfgParam_Init();

#endif