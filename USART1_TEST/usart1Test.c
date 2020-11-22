#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"

#include "self.h"

void Init_USART1()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;

    // Usart�ӿڳ�ʼ��
    // PA9: USART1 TX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   // �����������
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;         // pin9
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ����
    GPIO_Init(GPIOA, &GPIO_InitStruct);            // PA9: USART1 TX ��ʼ��
    // PA10: USART1 RX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; // RX����Ϊ��������
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;             //pin10
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;     // 50MHZ����
    GPIO_Init(GPIOA, &GPIO_InitStruct);                //PA10: USART1 RX ��ʼ��

    /******USART1��ʼ��******/
    USART_InitTypeDef USART_InitStruct;

    USART_InitStruct.USART_BaudRate = 115200;                                    // ������115200
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;                     // ֡����8
    USART_InitStruct.USART_Parity = USART_Parity_No;                             // ����żУ��λ
    USART_InitStruct.USART_StopBits = USART_StopBits_1;                          // ֹͣλ����1
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ��������
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 // ˫��ģʽ
    USART_Init(USART1, &USART_InitStruct);                                       //��ʼ��USART2

    USART_Cmd(USART1, ENABLE); // USART1����ʹ��

    /******�жϺ�����ʼ��******/
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //USART2�ж�ʹ��

    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn; // USART2�ж�ͨ��
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;   // �����ж�
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStruct); //�жϳ�ʼ��
}

void USART1_IRQHandler() // �����ж��¼����
{
    extern USARTCache USARTCache_Struct;
    if (USART_GetITStatus(USART1, USART_IT_RXNE)) // ��⵽ȷʵ�Ƕ���ʱ�䷢��
    {
        if ((USARTCache_Struct.String[USARTCache_Struct.StringLen++] = (char)USART_ReceiveData(USART1)) == '\n' || USARTCache_Struct.StringLen == 63)
        {
            // ���벢���Զ� +1�����������Ƿ�Ϊ\n��β��־����
            USARTCache_Struct.EndFlag = 1;                                  // ����н�����������־����Ϊ1
            USARTCache_Struct.String[USARTCache_Struct.StringLen++] = '\0'; // ֮���������Ч��
        }
    }
}

void USART1_SendChar(char c)
{
    USART_ClearFlag(USART1, USART_FLAG_TC);
    USART_SendData(USART1, c);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}