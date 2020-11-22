#include "self.h"

StringResult USART2_StringExam(void)
{
    extern USARTCache USARTCache_Struct;
    if (USARTCache_Struct.EndFlag == 0)
        return NORESULT;
    USARTCache_Struct.EndFlag = 0; // �Ѿ���ȡ����ձ�־λ��
    // ����ESP8266�ֽڴ�
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
    // С�����ƴ���
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
    // ����ָ��
    else if (strstr(USARTCache_Struct.String, "GET SPEED") != NULL)
    {
        clearCache();
        return GETSPEED;
    }
    clearCache();
    return ERRORBACK;
}

void USART2_IRQHandler() // �����ж��¼����
{
    extern USARTCache USARTCache_Struct;
    extern StringResult NowStatus;
    if (USART_GetITStatus(USART2, USART_IT_RXNE)) // ��⵽ȷʵ�Ƕ���ʱ�䷢��
    {
        if ((USARTCache_Struct.String[USARTCache_Struct.StringLen++] = (char)USART_ReceiveData(USART2)) == '\n' || USARTCache_Struct.StringLen == 63)
        {
            // ���벢���Զ� +1�����������Ƿ�Ϊ\n��β��־����
            USARTCache_Struct.EndFlag = 1;                                  // ����н�����������־����Ϊ1
            USARTCache_Struct.String[USARTCache_Struct.StringLen++] = '\0'; // ֮���������Ч��
            NowStatus = USART2_StringExam();
        }
    }
}