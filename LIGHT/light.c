#include "self.h"

void ControlLight(StringResult light)
{
    if (light == RIGHT)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    }
    if (light == LEFT)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
    }
    if (light == BACK)
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    }
    else
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
    }
    return;
}