#include "self.h"

void Init_System(void)
{
    /******总线使能******/
    // APB2高速总线使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIOA时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // GPIOD时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 重映射使能
    // APB1低速总线使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART2串口使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   // TIM3使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   // TIM4使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   // TIM5使能

    /******IO接口初始化******/
    GPIO_InitTypeDef GPIO_InitStruct;

    // Usart接口初始化
    // PA2: USART2 TX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   // 复用推挽输出
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;         // pin2
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ高速
    GPIO_Init(GPIOA, &GPIO_InitStruct);            // PA2: USART2 TX 初始化
    // PA3: USART2 RX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; // RX设置为浮空输入
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;             //pin3
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;     // 50MHZ高速
    GPIO_Init(GPIOA, &GPIO_InitStruct);                //PA3: USART2 RX 初始化
    // 外部时钟接口初始化
    // PA15: TIM2 CH_1_REMAP for LEFTWHEEL
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;     // CH_1设置为浮空输入
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;        // pin0
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ高速
    GPIO_Init(GPIOA, &GPIO_InitStruct);            // PA15: TIM5 CH_1_REMAP
    // PC6: TIM3 CH_1_REMAP fot RIGHTWHEEL
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;     // CH_1设置为浮空输入
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;         // pin6
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ高速
    GPIO_Init(GPIOC, &GPIO_InitStruct);            // PC6: TIM2 CH_1_REMAP
    // 轮子输出PWM接口初始化
    // PD12: TIM4 CH_1 PWM
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   // 复用推挽输出
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;        //pin12
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ高速
    GPIO_Init(GPIOD, &GPIO_InitStruct);            // PD12: TIM4 CH_1 PWM 初始化
    // PD13: TIM4 CH_2 PWM
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   // 复用推挽输出
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;        //pin13
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ高速
    GPIO_Init(GPIOD, &GPIO_InitStruct);            // PD13: TIM4 CH_2 PWM 初始化
    // PD14: TIM4 CH_3 PWM
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   // 复用推挽输出
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;        //pin14
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ高速
    GPIO_Init(GPIOD, &GPIO_InitStruct);            // PD14: TIM4 CH_3 PWM 初始化
    // PD15: TIM4 CH_4 PWM
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   // 复用推挽输出
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;        //pin15
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ高速
    GPIO_Init(GPIOD, &GPIO_InitStruct);            // PD15: TIM4 CH_ PWM 初始化

    /******USART2初始化******/
    USART_InitTypeDef USART_InitStruct;

    USART_InitStruct.USART_BaudRate = 115200;                                    // 波特率115200
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;                     // 帧长度8
    USART_InitStruct.USART_Parity = USART_Parity_No;                             // 无奇偶校验位
    USART_InitStruct.USART_StopBits = USART_StopBits_1;                          // 停止位长度1
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 // 双工模式
    USART_Init(USART2, &USART_InitStruct);                                       //初始化USART2

    USART_Cmd(USART2, ENABLE); // USART2串口使能

    /******TIM初始化******/
    // TIM3初始化
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_DeInit(TIM3);                                                           // 清除当前的设置
    TIM_TimeBaseInitStruct.TIM_Period = 65535;                                  // 65535保证不发生溢出事件
    TIM_TimeBaseInitStruct.TIM_Prescaler = 1;                                   // 不分频，原原本本读取数据长度
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;                // 向上计数模式
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;                    // 时钟分割
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);                            // 设置TIM3
    TIM_TIxExternalClockConfig(TIM3, TIM_TS_TI1FP1, TIM_ICPolarity_Rising, 15); // CH_1上升沿
    TIM_Cmd(TIM3, ENABLE);                                                      // 使能TIM3
    // TIM2初始化
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    TIM_DeInit(TIM2);                                                           // 清除当前的设置
    TIM_TimeBaseInitStruct.TIM_Period = 65535;                                  // 65535保证不发生溢出事件
    TIM_TimeBaseInitStruct.TIM_Prescaler = 1;                                   // 不分频，原原本本读取数据长度
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;                // 向上计数模式
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;                    // 时钟分割
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);                            // 设置TIM5
    TIM_TIxExternalClockConfig(TIM2, TIM_TS_TI1FP1, TIM_ICPolarity_Rising, 15); // CH_1上升沿
    TIM_Cmd(TIM2, ENABLE);                                                      // 使能TIM5
    // TIM4初始化
    TIM_TimeBaseInitStruct.TIM_Period = 10000;                   // ACC上限值为10000，计数达到10000自动归0
    TIM_TimeBaseInitStruct.TIM_Prescaler = 1;                    // 不分频
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分割
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);             // 设置TIM4
    TIM_Cmd(TIM4, ENABLE);                                       // 使能TIM4

    /******TIM4 PWM初始化******/
    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); // TIM4全部重映射
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             // PWM模式1，在计数器小于分界线的时候输出为有效
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     // 有效电平定义为高
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; // 输出有效化
    TIM_OC1Init(TIM4, &TIM_OCInitStruct);                      // TIM4 CH_1 PWM
    TIM_OC2Init(TIM4, &TIM_OCInitStruct);                      // TIM4 CH_2 PWM
    TIM_OC3Init(TIM4, &TIM_OCInitStruct);                      // TIM4 CH_3 PWM
    TIM_OC4Init(TIM4, &TIM_OCInitStruct);                      // TIM4 CH_4 PWM
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);          // TIM CH_1 PWM ENABLE
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);          // TIM CH_2 PWM ENABLE
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);          // TIM CH_3 PWM ENABLE
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);          // TIM CH_4 PWM ENABLE
    for (int i = 1; i <= 4; i++)
        PWM_ResetCCR(i, 0); // PWM全部初始化为0

    /******中断函数初始化******/
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  //USART2中断使能
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组2

    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn; // USART2中断通道
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;   // 允许中断
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct); //中断初始化

    Init_USART1();
}

void Init_ESP8266WIFI(void)
{
    extern StringResult NowStatus;
    do
    {
        USART_PrintStrWithEnding("AT+RST", "\r\n"); // 重置ESP8266
        SysTick_DelaySync(5000);                    // 这里延迟比较长，需要5000ms以后再读取
    } while (NowStatus != OK);
    NowStatus = NORESULT;
    clearCache();
    do
    {
        USART_PrintStrWithEnding("AT+CIPMODE=1", "\r\n"); // 设置CIP模式
        SysTick_DelaySync(2000);                          // 延迟2000ms
    } while (NowStatus != OK);
    NowStatus = NORESULT;
    clearCache();
    do
    {
        USART_PrintStrWithEnding("AT+CWMODE=1", "\r\n"); // 设置CW模式
        SysTick_DelaySync(2000);                         //延迟2000ms
    } while (NowStatus != OK);
    NowStatus = NORESULT;
    clearCache();
    do
    {
        USART_PrintStrWithEnding("AT+CIPSTART=\"TCP\",\"192.168.43.1\",7000", "\r\n");
        // 连接TCP服务器
        SysTick_DelaySync(2000); // 延迟2000ms
    } while (NowStatus != OK);
    NowStatus = NORESULT;
    clearCache();
    USART_PrintStrWithEnding("AT+CIPSEND","\r\n");
    SysTick_DelaySync(2000);
    clearCache();
    USART_PrintCallbackStr("START"); // 发送启动指令
}