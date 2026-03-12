#include "curtain.h"

void curtain_init(void)
{
    MOTOR::g_motor = new MOTOR();
    MOTOR::g_motor->setSpeed(2);
}

void curtain_control(int on)
{
    if (on)//0关1开
    {
         MOTOR::g_motor->motorForward(512); //正转一圈
    	 MOTOR::g_motor->motorStop(); //停止
    }
    else
    {
         MOTOR::g_motor->motorReverse(512); //反转一圈
         MOTOR::g_motor->motorStop(); //停止 
    }
}