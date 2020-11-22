#include "self.h"

StringResult USART2_StringExam(void)
{
    extern USARTCache USARTCache_Struct;
    if (USARTCache_Struct.EndFlag == 0)
        return NORESULT;
    USARTCache_Struct.EndFlag = 0; // 已经读取，清空标志位置
    // 控制ESP8266字节串
    if (strstr(USARTCache_Struct.String, "OK") != NULL)
    {
        clearCache();
        return OK;
    }
    else if (strstr(USARTCache_Struct.String, "Error") != NULL)
    {
        clearCache();
        return ERRORBACK;
    }
    else if (strstr(USARTCache_Struct.String, "WIFI GOT IP") != NULL)
    {
        clearCache();
        return OK;
    }
    // 小车控制代码
    else if (strstr(USARTCache_Struct.String, "MOV Ahead F") != NULL)
    {
        clearCache();
        return FRONTFAST;
    }
    else if (strstr(USARTCache_Struct.String, "MOV Ahead S") != NULL)
    {
        clearCache();
        return FRONTSLOW;
    }
    else if (strstr(USARTCache_Struct.String, "MOV LEFT") != NULL)
    {
        clearCache();
        return LEFT;
    }
    else if (strstr(USARTCache_Struct.String, "MOV RIGHT") != NULL)
    {
        clearCache();
        return RIGHT;
    }
    else if (strstr(USARTCache_Struct.String, "RETURN") != NULL)
    {
        clearCache();
        return BACK;
    }
    else if (strstr(USARTCache_Struct.String, "STOP") != NULL)
    {
        clearCache();
        return STOP;
    }
    // 请求指令
    else if (strstr(USARTCache_Struct.String, "GET SPEED") != NULL)
    {
        clearCache();
        return GETSPEED;
    }
    clearCache();
    return ERRORBACK;
}

void USART2_IRQHandler() // 串口中断事件检查
{
    extern USARTCache USARTCache_Struct;
    extern StringResult NowStatus;
    if (USART_GetITStatus(USART2, USART_IT_RXNE)) // 检测到确实是读入时间发生
    {
        if ((USARTCache_Struct.String[USARTCache_Struct.StringLen++] = (char)USART_ReceiveData(USART2)) == '\n' || USARTCache_Struct.StringLen == 63)
        {
            // 读入并且自动 +1，检测读入结果是否为\n行尾标志符号
            USARTCache_Struct.EndFlag = 1;                                  // 如果行结束，结束标志设置为1
            USARTCache_Struct.String[USARTCache_Struct.StringLen++] = '\0'; // 之后的数据无效化
            NowStatus = USART2_StringExam();
        }
    }
}