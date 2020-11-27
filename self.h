#ifndef __SELF_CAR__
#define __SELF_CAR__

#define PIDMAX 7000

typedef struct USARTCache
{
    char String[256];
    int StringLen;
    char EndFlag;
} USARTCache;
typedef enum StringResult
{
    NORESULT,
    OK,
    ERRORBACK,
    FRONTFAST,
    FRONTSLOW,
    LEFT,
    RIGHT,
    BACK,
    STOP,
    GETSPEED
} StringResult;

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"

#include "misc.h"
#include "SysTick.h"

#include "string.h"

#include "usartPrint.h"
#include "usart1Test.h"
#include "init.h"
#include "car_run.h"
#include "light.h"

void clearCache(void);

void Init_System(void);           // ???????
void Init_ESP8266WIFI(void);      // ???ESP8266 WIFI??
void USART2_IRQHandler(void);     // USART2??????
void PWM_ResetCCR(int, uint16_t); // ??TIM4 PWM??
StringResult USART2_StringExam();

void CaculatePWM_PID(int32_t *, int32_t *, int16_t, int16_t, int); // ??PID
uint16_t GET_TIM3CNT(void);                                            // ??TIM3???
uint16_t GET_TIM2CNT(void);                                            // ??TIM5???
void RST_TIM2_3CNT(void);                                              // ??TIM3?TIM5???

#endif