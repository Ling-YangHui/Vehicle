#include "self.h"

void Init_System(void)
{
    /******����ʹ��******/
    // APB2��������ʹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIOAʱ��ʹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // GPIODʱ��ʹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // ��ӳ��ʹ��
    // APB1��������ʹ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART2����ʹ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   // TIM3ʹ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   // TIM4ʹ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   // TIM5ʹ��

    /******IO�ӿڳ�ʼ��******/
    GPIO_InitTypeDef GPIO_InitStruct;

    // Usart�ӿڳ�ʼ��
    // PA2: USART2 TX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   // �����������
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;         // pin2
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ����
    GPIO_Init(GPIOA, &GPIO_InitStruct);            // PA2: USART2 TX ��ʼ��
    // PA3: USART2 RX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; // RX����Ϊ��������
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;             //pin3
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;     // 50MHZ����
    GPIO_Init(GPIOA, &GPIO_InitStruct);                //PA3: USART2 RX ��ʼ��
    // �ⲿʱ�ӽӿڳ�ʼ��
    // PA15: TIM2 CH_1_REMAP for LEFTWHEEL
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;     // CH_1����Ϊ��������
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;        // pin0
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ����
    GPIO_Init(GPIOA, &GPIO_InitStruct);            // PA15: TIM5 CH_1_REMAP
    // PC6: TIM3 CH_1_REMAP fot RIGHTWHEEL
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;     // CH_1����Ϊ��������
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;         // pin6
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ����
    GPIO_Init(GPIOC, &GPIO_InitStruct);            // PC6: TIM2 CH_1_REMAP
    // �������PWM�ӿڳ�ʼ��
    // PD12: TIM4 CH_1 PWM
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   // �����������
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;        //pin12
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ����
    GPIO_Init(GPIOD, &GPIO_InitStruct);            // PD12: TIM4 CH_1 PWM ��ʼ��
    // PD13: TIM4 CH_2 PWM
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   // �����������
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;        //pin13
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ����
    GPIO_Init(GPIOD, &GPIO_InitStruct);            // PD13: TIM4 CH_2 PWM ��ʼ��
    // PD14: TIM4 CH_3 PWM
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   // �����������
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;        //pin14
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ����
    GPIO_Init(GPIOD, &GPIO_InitStruct);            // PD14: TIM4 CH_3 PWM ��ʼ��
    // PD15: TIM4 CH_4 PWM
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   // �����������
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;        //pin15
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ����
    GPIO_Init(GPIOD, &GPIO_InitStruct);            // PD15: TIM4 CH_ PWM ��ʼ��

    /******USART2��ʼ��******/
    USART_InitTypeDef USART_InitStruct;

    USART_InitStruct.USART_BaudRate = 115200;                                    // ������115200
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;                     // ֡����8
    USART_InitStruct.USART_Parity = USART_Parity_No;                             // ����żУ��λ
    USART_InitStruct.USART_StopBits = USART_StopBits_1;                          // ֹͣλ����1
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ��������
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 // ˫��ģʽ
    USART_Init(USART2, &USART_InitStruct);                                       //��ʼ��USART2

    USART_Cmd(USART2, ENABLE); // USART2����ʹ��

    /******TIM��ʼ��******/
    // TIM3��ʼ��
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_DeInit(TIM3);                                                           // �����ǰ������
    TIM_TimeBaseInitStruct.TIM_Period = 65535;                                  // 65535��֤����������¼�
    TIM_TimeBaseInitStruct.TIM_Prescaler = 1;                                   // ����Ƶ��ԭԭ������ȡ���ݳ���
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;                // ���ϼ���ģʽ
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;                    // ʱ�ӷָ�
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);                            // ����TIM3
    TIM_TIxExternalClockConfig(TIM3, TIM_TS_TI1FP1, TIM_ICPolarity_Rising, 15); // CH_1������
    TIM_Cmd(TIM3, ENABLE);                                                      // ʹ��TIM3
    // TIM2��ʼ��
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    TIM_DeInit(TIM2);                                                           // �����ǰ������
    TIM_TimeBaseInitStruct.TIM_Period = 65535;                                  // 65535��֤����������¼�
    TIM_TimeBaseInitStruct.TIM_Prescaler = 1;                                   // ����Ƶ��ԭԭ������ȡ���ݳ���
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;                // ���ϼ���ģʽ
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;                    // ʱ�ӷָ�
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);                            // ����TIM5
    TIM_TIxExternalClockConfig(TIM2, TIM_TS_TI1FP1, TIM_ICPolarity_Rising, 15); // CH_1������
    TIM_Cmd(TIM2, ENABLE);                                                      // ʹ��TIM5
    // TIM4��ʼ��
    TIM_TimeBaseInitStruct.TIM_Period = 10000;                   // ACC����ֵΪ10000�������ﵽ10000�Զ���0
    TIM_TimeBaseInitStruct.TIM_Prescaler = 1;                    // ����Ƶ
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;     // ʱ�ӷָ�
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);             // ����TIM4
    TIM_Cmd(TIM4, ENABLE);                                       // ʹ��TIM4

    /******TIM4 PWM��ʼ��******/
    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); // TIM4ȫ����ӳ��
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             // PWMģʽ1���ڼ�����С�ڷֽ��ߵ�ʱ�����Ϊ��Ч
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     // ��Ч��ƽ����Ϊ��
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; // �����Ч��
    TIM_OC1Init(TIM4, &TIM_OCInitStruct);                      // TIM4 CH_1 PWM
    TIM_OC2Init(TIM4, &TIM_OCInitStruct);                      // TIM4 CH_2 PWM
    TIM_OC3Init(TIM4, &TIM_OCInitStruct);                      // TIM4 CH_3 PWM
    TIM_OC4Init(TIM4, &TIM_OCInitStruct);                      // TIM4 CH_4 PWM
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);          // TIM CH_1 PWM ENABLE
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);          // TIM CH_2 PWM ENABLE
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);          // TIM CH_3 PWM ENABLE
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);          // TIM CH_4 PWM ENABLE
    for (int i = 1; i <= 4; i++)
        PWM_ResetCCR(i, 0); // PWMȫ����ʼ��Ϊ0

    /******�жϺ�����ʼ��******/
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  //USART2�ж�ʹ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�жϷ���2

    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn; // USART2�ж�ͨ��
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;   // �����ж�
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct); //�жϳ�ʼ��

    Init_USART1();
}

void Init_ESP8266WIFI(void)
{
    extern StringResult NowStatus;
    do
    {
        USART_PrintStrWithEnding("AT+RST", "\r\n"); // ����ESP8266
        SysTick_DelaySync(5000);                    // �����ӳٱȽϳ�����Ҫ5000ms�Ժ��ٶ�ȡ
    } while (NowStatus != OK);
    NowStatus = NORESULT;
    clearCache();
    do
    {
        USART_PrintStrWithEnding("AT+CIPMODE=1", "\r\n"); // ����CIPģʽ
        SysTick_DelaySync(2000);                          // �ӳ�2000ms
    } while (NowStatus != OK);
    NowStatus = NORESULT;
    clearCache();
    do
    {
        USART_PrintStrWithEnding("AT+CWMODE=1", "\r\n"); // ����CWģʽ
        SysTick_DelaySync(2000);                         //�ӳ�2000ms
    } while (NowStatus != OK);
    NowStatus = NORESULT;
    clearCache();
    do
    {
        USART_PrintStrWithEnding("AT+CIPSTART=\"TCP\",\"192.168.43.1\",7000", "\r\n");
        // ����TCP������
        SysTick_DelaySync(2000); // �ӳ�2000ms
    } while (NowStatus != OK);
    NowStatus = NORESULT;
    clearCache();
    USART_PrintStrWithEnding("AT+CIPSEND","\r\n");
    SysTick_DelaySync(2000);
    clearCache();
    USART_PrintCallbackStr("START"); // ��������ָ��
}