#ifndef GPIO_H
#define GPIO_H

#include "Arduino.h"

#define GPIO0 ((4-1)*32+19)
#define GPIO1 ((4-1)*32+20)
#define GPIO2 ((4-1)*32+21)
#define GPIO3 ((4-1)*32+22)
#define GPIO4 ((4-1)*32+23)

class GPIO {
    private:
        int m_iPin;
        string m_sPath;
        
    public:
        GPIO(void);
        GPIO(int pin);
        
        void setPin(int pin);
        int getPin(void);   
        
        void setPath(int pin);
        string getPath(void);
        
        int setDirection(int dir);
        int getDirection(void);
        
        int setValue(int val);
        int getValue(void);
        
        int exportGPIO(void);
        int unexportGPIO(void);
};

#endif
