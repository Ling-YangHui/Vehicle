#include "self.h"

// 左侧PWM指针，右侧PWM指针，左侧目标速度，右侧目标速度，清除PID控制记录
void CaculatePWM_PID(int32_t *PWM_Left, int32_t *PWM_Right, int16_t LeftTargetSpeed, int16_t RightTargetSpeed, int ClearFlag)
{
    extern int16_t LeftSpeed, RightSpeed;
    extern const double Ki, Kd, Kp;
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
        DiffLeftSpeed = DeltaLeftSpeed - LastLeftSpeed;
        DiffRightSpeed = DeltaRightSpeed - LastRightSpeed;
    }
    LastLeftSpeed = DeltaLeftSpeed;
    LastRightSpeed = DeltaRightSpeed;
    *PWM_Left = Kp * DeltaLeftSpeed + Ki * IntLeftSpeed + Kd * DiffLeftSpeed;
    *PWM_Right = Kp * DeltaRightSpeed + Ki * IntRightSpeed + Kd * DiffRightSpeed;
    if (*PWM_Left < 0)
        *PWM_Left = 0;
    if (*PWM_Right < 0)
        *PWM_Right = 0;
    if (*PWM_Left > PIDMAX)
        *PWM_Left = PIDMAX;
    if (*PWM_Right > PIDMAX)
        *PWM_Right = PIDMAX;

    #ifdef DBG
    USART_PrintNum((int)*PWM_Left,"\r\n");
    USART_PrintNum((int)*PWM_Left,"\r\n:");
    USART_PrintNum((int)LeftSpeed,"\r\n:");
    USART_PrintNum((int)RightSpeed,"\r\n");
    #endif
    return;
}