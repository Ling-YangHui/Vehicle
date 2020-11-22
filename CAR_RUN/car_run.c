#include "self.h"

void Move(int16_t LeftSpeed, int16_t LeftPWM, int16_t RightSpeed, int16_t RightPWM, uint8_t Reverse)
{
    CaculatePWM_PID(&LeftPWM, &RightPWM, LeftSpeed, RightSpeed, 0);
}