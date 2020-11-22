#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "usartPrint.h"

#include "SysTick.h"

#include "string.h"

#include "self.h"
#include "usart1Test.h"

int pow(int basic, int power)
{
    int result = 1;
    for (int i = 0;i < power;i ++)
        result *= basic;
    return result;
}
void USART_SendChar(char c)
{
    USART_ClearFlag(USART2, USART_FLAG_TC);
    USART_SendData(USART2, c);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    USART1_SendChar(c);
    return;
}
void USART_PrintStr(char *string)
{
    while (*string != '\0')
    {
        USART_SendChar(*string);
        string ++;
    }
    return;
}
void USART_PrintStrWithEnding(char *string, char *ending)
{
    USART_PrintStr(string);
    USART_PrintStr(ending);
    return;
}
void USART_PrintNum(int num, char *ending) // 最大支持int长度为65536
{
    if (num < 0)
    {
        num *= -1;
        USART_SendChar('-');
    }
    int firstNumFlag = 0;
    for (int i = 4; i >= 0; i--)
    {
        char c = (num / pow(10, i)) % 10 + '0';
        if (c != '0' && firstNumFlag == 0)
            firstNumFlag = 1;
        if (firstNumFlag)
            USART_SendChar(c);
        if (i == 0 && firstNumFlag == 0)
            USART_SendChar(c);
    }
    USART_PrintStr(ending);
    return;
}
void USART_PrintCallbackStr(char *string)
{
    USART_PrintStrWithEnding("AT+CIPSEND=","\0");
    USART_PrintNum(strlen(string) + 2,"\r\n");
    SysTick_DelaySync(100);
    USART_PrintStr(string);
    return;
}
char *Int_To_String(int num, char *string)
{
    int stringTop = 0;
    if (num < 0)
    {
        num *= -1;
        string[stringTop++] = '-';
    }
    int firstNumFlag = 0;
    for (int i = 4; i >= 0; i--)
    {
        char c = (num / pow(10, i)) % 10 + '0';
        if (c != '0' && firstNumFlag == 0)
            firstNumFlag = 1;
        if (firstNumFlag)
            string[stringTop++] = c;
        if (i == 0 && firstNumFlag == 0)
            string[stringTop ++] = '0';
    }
    string[stringTop ++] = '\0';
    return string;
}