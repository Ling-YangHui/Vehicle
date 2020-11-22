#include "self.h"

void Move(int16_t LeftSpeed, int16_t LeftPWM, int16_t RightSpeed, int16_t RightPWM, uint8_t ReverseLeft, uint8_t ReverseRight)
{
    CaculatePWM_PID(&LeftPWM, &RightPWM, LeftSpeed, RightSpeed, 0);
    if (ReverseLeft)
    {
        PWM_ResetCCR(2, LeftPWM);
        PWM_ResetCCR(1, 0);
    }
    else
    {
        PWM_ResetCCR(1, LeftPWM);
        PWM_ResetCCR(2, 0);
    }
    if (ReverseRight)
    {
        PWM_ResetCCR(4, RightPWM);
        PWM_ResetCCR(3, 0);
    }
    else
    {
        PWM_ResetCCR(3, RightPWM);
        PWM_ResetCCR(4, 0);
    }
    return;
}

void FrontFast()
{
    Move(60, 5000, 60, 5000, 0, 0);
}

void FrontSlow()
{
    Move(40, 3500, 40, 3500, 0, 0);
}

void Left()
{
    Move(30, 2500, 30, 2500, 1, 0);
}

void Right()
{
    Move(30, 2500, 30, 2500, 0, 1);
}

void Back()
{
    Move(30, 2500, 30, 2500, 1, 1);
}

void Stop()
{
    Move(0, 0, 0, 0, 0, 0);
}