#ifndef _MOTOR_H
#define _MOTOR_H

#include "Arduino.h"
#include "gpio.h"

class MOTOR : public GPIO {
    private:
        int m_iPin1;    
        int m_iPin2;    
        int m_iPin3;    
        int m_iPin4;
        
        int m_iSpeed; //0-10    

    public:
        MOTOR();
        ~MOTOR(void);
        
        virtual int motorForward(int angle); //512一圈
        virtual int motorReverse(int angle);
        virtual int motorStop(void);
        
        virtual int setSpeed(int speed);
        virtual int getSpeed(void);
	static MOTOR* g_motor;
};

#endif 


