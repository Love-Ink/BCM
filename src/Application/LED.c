#include "LED.h"

/**
 * @brief 设置从上到下第一个灯的状态
 * 
 * @param state 状态(1,0)
 */
void Set_LED_1(uint8_t state) {
    GPIO_WriteBit(GPIOA, GPIO_Pin_11, !state);
}

/**
 * @brief 设置从上到下第二个灯的状态
 * 
 * @param state 状态(1,0)
 */
void Set_LED_2(uint8_t state) {
    GPIO_WriteBit(GPIOA, GPIO_Pin_10, !state);
}

/**
 * @brief 设置从上到下第三个灯的状态
 * 
 * @param state 状态(1,0)
 */
void Set_LED_3(uint8_t state) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_15, !state);
}

/**
 * @brief 设置从上到下第四个灯的状态
 * 
 * @param state 状态(1,0)
 */
void Set_LED_4(uint8_t state) {
    GPIO_WriteBit(GPIOA, GPIO_Pin_8, !state);
}

/**
 * @brief 设置从上到下第五个灯的状态
 * 
 * @param state 状态(1,0)
 */
void Set_LED_5(uint8_t state) {
    GPIO_WriteBit(GPIOA, GPIO_Pin_9, !state);
}

void Set_BL(uint8_t state) {
    GPIO_WriteBit(GPIOA, GPIO_Pin_12, state);
}

void RunPage_Switch(uint8_t state) {
    Run_Page ++;
    if(Run_Page >= RUN_PAGE_NUMS) {
        Run_Page = 0;
    }
}

/**
 * @brief 初始化LED灯GPIO,配置LED灯为Blink设备
 * 
 */
void Application_LED_Init() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    gpio_init(GPIOA, GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_8 | GPIO_Pin_9, GPIO_Mode_Out_PP);
    gpio_init(GPIOB, GPIO_Pin_15, GPIO_Mode_Out_PP);
    gpio_init(GPIOA, GPIO_Pin_12, GPIO_Mode_Out_PP);

    BlinkDevice_Creat("LED1", Set_LED_1);
    BlinkDevice_Creat("LED2", Set_LED_2);
    BlinkDevice_Creat("LED3", Set_LED_3);
    BlinkDevice_Creat("LED4", Set_LED_4);
    BlinkDevice_Creat("LED5", Set_LED_5);
    BlinkDevice_Creat("BL", Set_BL);
    BlinkDevice_Creat("RunPage", RunPage_Switch);
}

