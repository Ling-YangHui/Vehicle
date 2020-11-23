#include "self.h"

// ���PWMָ�룬�Ҳ�PWMָ�룬���Ŀ���ٶȣ��Ҳ�Ŀ���ٶȣ����PID���Ƽ�¼
void CaculatePWM_PID(int16_t *PWM_Left, int16_t *PWM_Right, int16_t LeftTargetSpeed, int16_t RightTargetSpeed, int ClearFlag)
{
    extern uint16_t LeftSpeed, RightSpeed;
    extern const double Ki, Kd, Kp;
    static int32_t IntLeftSpeed = 0;
    static int32_t IntRightSpeed = 0;
    static int16_t LastLeftSpeed = 0;
    static int16_t LastRightSpeed = 0;
    if (ClearFlag) // �任״̬���������еĲ���
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
    RST_TIM2_3CNT();            // ���TIM3/5->CNT�Ĵ���
    SysTick_DelaySync(30);      // �ӳ�30ms������Ķ�ʱ����Ӳ����ʱ��������жϳ�ͻ
    LeftSpeed = GET_TIM3CNT();  // ��ȡ����ٶ�
    RightSpeed = GET_TIM2CNT(); // ��ȡ�ұ��ٶ�
    // ����
    int32_t DeltaLeftSpeed = LeftTargetSpeed - LeftSpeed;
    int32_t DeltaRightSpeed = RightTargetSpeed - RightSpeed;
    // ����
    IntLeftSpeed += DeltaLeftSpeed;
    IntRightSpeed += DeltaRightSpeed;
    // ΢��
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
    if (*PWM_Left > PIDMAX)
        *PWM_Left = PIDMAX;
    if (*PWM_Right > PIDMAX)
        *PWM_Right = PIDMAX;
        
    USART_PrintNum((int)PWM_Left,"\r\n");
    USART_PrintNum((int)PWM_Right,"\r\n");
    return;
}