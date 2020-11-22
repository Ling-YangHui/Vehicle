#include "self.h"

USARTCache USARTCache_Struct;
const double Kp = 20, Ki = 0.01, Kd = 20; // PID系数
uint16_t LeftSpeed, RightSpeed;
StringResult NowStatus, PastStatus;

int main()
{
    StringResult ExamResult;
    Init_System();
    Init_ESP8266WIFI();
    while (1)
    {
        if (NowStatus != NORESULT)
        {

        }
    }
}



void clearCache()
{
    memset(USARTCache_Struct.String, 0, 64); // 清空缓冲区
    USARTCache_Struct.StringLen = 0;
    return;
}

void CaculatePWM_PID(uint16_t *PWM_Left, uint16_t *PWM_Right, uint16_t LeftTargetSpeed, uint16_t RightTargetSpeed, int ClearFlag)
{
    static int32_t IntLeftSpeed = 0;
    static int32_t IntRightSpeed = 0;
    static uint16_t LastLeftSpeed = 0;
    static uint16_t LastRightSpeed = 0;
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