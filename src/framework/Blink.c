#include "Blink.h"
#include "string.h"
#include "stdlib.h"

static Blink_Info *Blink_head = NULL; 

/**
 * @brief 创建Blink设备
 * 
 * @param Name Blink设备名
 * @param ioctrl 控制函数
 */
void BlinkDevice_Creat(const char *Name, void(*ioctrl)(uint8_t state)) {  
    if(strlen((const char *)Name) == 0) return ;
    if(ioctrl == NULL) return ;

    Blink_Info *Creat_Blink = malloc(sizeof(Blink_Info));
    memset(Creat_Blink, 0, sizeof(Blink_Info));
    
    memcpy(Creat_Blink->Name, Name, strlen((const char *)Name));
    Creat_Blink->Blink_Cfg.ioctrl = ioctrl;

    if(Blink_head == NULL) {
        Blink_head = Creat_Blink;
    } else {
        Blink_Info *Temp = Blink_head;
        while(Temp->Blink_next != NULL) Temp = Temp->Blink_next;
        Temp->Blink_next = Creat_Blink;
    }
}

/**
 * @brief Blink设备扫描
 * 
 */
void BlinkDevice_Scan(uint16_t ms) {
    if(Blink_head == NULL) return ;

    Blink_Info *Blink_Device = Blink_head;
    while(Blink_Device != NULL) {
        if (Blink_Device->Blink_State.OnTime == 0) {

        } else if (Get_TaskTick() - Blink_Device->Blink_State.tick < Blink_Device->Blink_State.OnTime) {
            if (!Blink_Device->Blink_State.State) {
                Blink_Device->Blink_State.State = true;
                Blink_Device->Blink_Cfg.ioctrl(true);
            }
        } else if (Get_TaskTick() - Blink_Device->Blink_State.tick < Blink_Device->Blink_State.OffTime) {    
            if (Blink_Device->Blink_State.State) {
                Blink_Device->Blink_State.State = false;
                Blink_Device->Blink_Cfg.ioctrl(false);
            }
        } else {
            Blink_Device->Blink_State.tick = Get_TaskTick();
            if (Blink_Device->Blink_State.repeats) {
                if (++Blink_Device->Blink_State.count >= Blink_Device->Blink_State.repeats) {
                    Blink_Device->Blink_State.OnTime = 0;
                    Blink_Device->Blink_Cfg.ioctrl(false);
                    Blink_Device->Blink_State.State = false;
                }
            }
        }

        Blink_Device = Blink_Device->Blink_next;
    }
}

/**
 * @brief 设置Blink设备
 * 
 * @param Name Blink设备名
 * @param OnTime 开启时间(一个单位与SysTick进入周期有关)
 * @param OffTime 关闭时间(一个单位与SysTick进入周期有关)
 * @param repeats 重复次数
 */
void BlinkDevice_Config(const char *Name, uint16_t OnTime, uint16_t OffTime, uint8_t repeats) {
    if(Blink_head == NULL) return ;
    if(strlen((const char *)Name) == 0) return ;

    Blink_Info *Blink_Device = Blink_head;
    while(Blink_Device != NULL) {
        if(strcmp((const char *)Blink_Device->Name, (const char *)Name) == 0) {
            Blink_Device->Blink_State.OnTime  = OnTime;
            Blink_Device->Blink_State.OffTime = OffTime + OnTime;                  
            Blink_Device->Blink_State.repeats = repeats;
            Blink_Device->Blink_State.tick    = Get_TaskTick();
            Blink_Device->Blink_State.count   = 0;
            if (OnTime  == 0) {
                Blink_Device->Blink_Cfg.ioctrl(false);
                Blink_Device->Blink_State.State  = false;
            } 
        }
        Blink_Device = Blink_Device->Blink_next;
    }
}   

void BlinkDevice_Show(uint16_t ms) {
    if(Blink_head == NULL) return ;
    Blink_Info *Blink_Device = Blink_head;
    printf("====================\r\n");
    while(Blink_Device != NULL) {
        printf("%d,%d,%d,%d\r\n",Blink_Device->Blink_State.OnTime,Blink_Device->Blink_State.OffTime,Blink_Device->Blink_State.repeats,Blink_Device->Blink_State.State);
        Blink_Device = Blink_Device->Blink_next;
    }
    printf("====================\r\n");
}