#include "self.h"
#define DB

USARTCache USARTCache_Struct;
const double Kp = 100, Ki = 10, Kd = 100; // PID系数
int16_t LeftSpeed, RightSpeed;
StringResult NowStatus = NORESULT, PastStatus;
char ReturnOrder[6][16] = {"FrontFast", "Front", "Left", "Right", "Back", "Stop"};

int main()
{
    Init_System();
    Init_ESP8266WIFI();
    PastStatus = STOP;
    while (1)
    {
        if (NowStatus != PastStatus && (NowStatus != NORESULT) && (NowStatus != ERRORBACK) && (NowStatus != OK) && (NowStatus != GETSPEED))
        {
            PastStatus = NowStatus;
            CaculatePWM_PID(NULL, NULL, 0, 0, 1);
            USART_PrintStrWithEnding(ReturnOrder[PastStatus - 3], "\r\n");
            ControlLight(PastStatus);
        }
        switch (PastStatus)
        {
        case FRONTFAST:
            FrontFast();
            break;
        case FRONTSLOW:
            FrontSlow();
            break;
        case LEFT:
            Left();
            break;
        case RIGHT:
            Right();
            break;
        case BACK:
            Back();
            break;
        case STOP:
            Stop();
            break;
        default:
            break;
        }
        if (NowStatus == GETSPEED)
        {
            USART_PrintStrWithEnding("LeftSpeed", ": ");
            USART_PrintNum((int)LeftSpeed, "\r\n");
            USART_PrintStrWithEnding("RightSpeed", ":");
            USART_PrintNum((int)RightSpeed, "\r\n");
            NowStatus = OK;
        }
    }
}

void clearCache()
{
    memset(USARTCache_Struct.String, 0, 64); // 清空缓冲区
    USARTCache_Struct.StringLen = 0;
    return;
}

// 1：左前，2：左后，3：右前，4：右后
void PWM_ResetCCR(int Channel, uint16_t PWM) // 重新设置各个PWM的输出值
{
    switch (Channel)
    {
    case 1:
        TIM4->CCR1 = PWM;
        break;
    case 2:
        TIM4->CCR2 = PWM;
        break;
    case 3:
        TIM4->CCR3 = PWM;
        break;
    case 4:
        TIM4->CCR4 = PWM;
        break;
    }
    return;
}

uint16_t GET_TIM3CNT()
{
    return TIM3->CNT;
}

uint16_t GET_TIM2CNT()
{
    return TIM2->CNT;
}
void RST_TIM2_3CNT()
{
    TIM3->CNT = 0;
    TIM2->CNT = 0;
    return;
}