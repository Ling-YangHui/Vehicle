#include "self.h"

USARTCache USARTCache_Struct;
const double Kp = 20, Ki = 0.01, Kd = 20; // PID系数
uint16_t LeftSpeed, RightSpeed;
StringResult NowStatus, PastStatus;

int main()
{
    Init_System();
    Init_ESP8266WIFI();
    while (1)
    {
        if (NowStatus != NORESULT)
        {
            if (NowStatus != PastStatus && (NowStatus != NORESULT) && (NowStatus != ERRORBACK) && (NowStatus != OK) && (NowStatus != GETSPEED))
            {
                PastStatus = NowStatus;
            }
            switch (NowStatus)
            {
            case FRONTFAST:

            default:
                break;
            }
        }
    }
}



void clearCache()
{
    memset(USARTCache_Struct.String, 0, 64); // 清空缓冲区
    USARTCache_Struct.StringLen = 0;
    return;
}

// 左侧PWM指针，右侧PWM指针，左侧目标速度，右侧目标速度，清除PID控制记录
void CaculatePWM_PID(int16_t *PWM_Left, int16_t *PWM_Right, int16_t LeftTargetSpeed, int16_t RightTargetSpeed, int ClearFlag)
{
    static int32_t IntLeftSpeed = 0;
    static int32_t IntRightSpeed = 0;
    static int16_t LastLeftSpeed = 0;
    static int16_t LastRightSpeed = 0;
    if (ClearFlag) // 变换状态，重置所有的参数
    {
        IntLeftSpeed = 0;
        IntRightSpeed = 0;
        LastLeftSpeed = 0;
        LastRightSpeed = 0;
        return;
    }
    int32_t DiffLeftSpeed, DiffRightSpeed;
    DiffLeftSpeed = 0;
    DiffRightSpeed = 0;
    RST_TIM2_3CNT();            // 清空TIM3/5->CNT寄存器
    SysTick_DelaySync(30);      // 延迟30ms，这里的定时器是硬件定时器不会和中断冲突
    LeftSpeed = GET_TIM3CNT();  // 获取左边速度
    RightSpeed = GET_TIM2CNT(); // 获取右边速度
    // 比例
    int32_t DeltaLeftSpeed = LeftTargetSpeed - LeftSpeed;
    int32_t DeltaRightSpeed = RightTargetSpeed - RightSpeed;
    // 积分
    IntLeftSpeed += DeltaLeftSpeed;
    IntRightSpeed += DeltaRightSpeed;
    // 微分
    if (LastLeftSpeed != 0)
    {
        DiffLeftSpeed = LeftSpeed - LastLeftSpeed;
        DiffRightSpeed = RightSpeed - LastRightSpeed;
    }
    LastLeftSpeed = LeftSpeed;
    LastRightSpeed = RightSpeed;
    *PWM_Left = Kp * DeltaLeftSpeed + Ki * IntLeftSpeed + Kd * DiffLeftSpeed;
    *PWM_Right = Kp * DeltaRightSpeed + Ki * IntRightSpeed + Kd * DiffRightSpeed;
    if (*PWM_Left < 0)
        PWM_Left = 0;
    if (*PWM_Right < 0)
        PWM_Right = 0;
    if (*PWM_Left > 9000)
        *PWM_Left = 9000;
    if (*PWM_Right > 9000)
        *PWM_Right = 9000;
    return;
}

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