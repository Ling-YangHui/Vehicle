#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"

#include "self.h"

void Init_USART1()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;

    // Usart接口初始化
    // PA9: USART1 TX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   // 复用推挽输出
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;         // pin9
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ高速
    GPIO_Init(GPIOA, &GPIO_InitStruct);            // PA9: USART1 TX 初始化
    // PA10: USART1 RX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; // RX设置为浮空输入
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;             //pin10
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;     // 50MHZ高速
    GPIO_Init(GPIOA, &GPIO_InitStruct);                //PA10: USART1 RX 初始化

    /******USART1初始化******/
    USART_InitTypeDef USART_InitStruct;

    USART_InitStruct.USART_BaudRate = 115200;                                    // 波特率115200
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;                     // 帧长度8
    USART_InitStruct.USART_Parity = USART_Parity_No;                             // 无奇偶校验位
    USART_InitStruct.USART_StopBits = USART_StopBits_1;                          // 停止位长度1
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 // 双工模式
    USART_Init(USART1, &USART_InitStruct);                                       //初始化USART2

    USART_Cmd(USART1, ENABLE); // USART1串口使能

    /******中断函数初始化******/
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //USART2中断使能

    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn; // USART2中断通道
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;   // 允许中断
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStruct); //中断初始化
}

void USART1_IRQHandler() // 串口中断事件检查
{
    extern USARTCache USARTCache_Struct;
    if (USART_GetITStatus(USART1, USART_IT_RXNE)) // 检测到确实是读入时间发生
    {
        if ((USARTCache_Struct.String[USARTCache_Struct.StringLen++] = (char)USART_ReceiveData(USART1)) == '\n' || USARTCache_Struct.StringLen == 63)
        {
            // 读入并且自动 +1，检测读入结果是否为\n行尾标志符号
            USARTCache_Struct.EndFlag = 1;                                  // 如果行结束，结束标志设置为1
            USARTCache_Struct.String[USARTCache_Struct.StringLen++] = '\0'; // 之后的数据无效化
        }
    }
}

void USART1_SendChar(char c)
{
    USART_ClearFlag(USART1, USART_FLAG_TC);
    USART_SendData(USART1, c);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}