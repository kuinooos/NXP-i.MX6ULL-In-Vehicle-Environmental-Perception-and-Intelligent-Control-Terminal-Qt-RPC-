
#include "motor.h"

MOTOR* MOTOR::g_motor = nullptr;

static void func1(int signal)
{
	while(MOTOR::g_motor){
		MOTOR::g_motor->motorStop();
		delete MOTOR::g_motor;
		MOTOR::g_motor = nullptr;
	}
    
    exit(0);
}


MOTOR::MOTOR()
{
    this->m_iPin1 = GPIO0;// 引脚初始化
    this->m_iPin2 = GPIO1;
    this->m_iPin3 = GPIO2;
    this->m_iPin4 = GPIO3;

    this->m_iSpeed = 4*2; // 初始速度设置为8

    // 初始化GPIO引脚		
    setPin(this->m_iPin1); 
    exportGPIO();
    usleep(25000);
    setDirection(0); 
    setValue(0);

    setPin(this->m_iPin2);
    exportGPIO();
    usleep(25000);
    setDirection(0); 
    setValue(0);
    
    setPin(this->m_iPin3);
    exportGPIO();
    usleep(25000);
    setDirection(0); 
    setValue(0);
    
    setPin(this->m_iPin4);
    exportGPIO();
    usleep(25000);
    setDirection(0); 
    setValue(0);

	// 注册信号处理程序
    signal(SIGINT, func1);
    signal(SIGTERM, func1); 
    signal(SIGKILL, func1); //无法捕获kill -9 ,应用层不能kill -9 退出，不然可能会导致GPIO输出形成回路，电机发热
}

MOTOR::~MOTOR(void)
{
	// 将所有引脚设置为0并取消导出
    setPin(this->m_iPin1);
    setValue(0);
    unexportGPIO();
    
    setPin(this->m_iPin2);
    setValue(0);
    unexportGPIO();
    
    setPin(this->m_iPin3);
    setValue(0);
    unexportGPIO();
    
    setPin(this->m_iPin4);
    setValue(0);
    unexportGPIO();
}

// 电机正转函数，angle为512时正转一圈
int MOTOR::motorForward(int angle)
{   
    int i = 0;
    for(i=0; i<angle; i++) 
    {

        setPin(this->m_iPin1);
        setValue(1);
        setPin(this->m_iPin2);
        setValue(1);
        setPin(this->m_iPin3);
        setValue(0);    
        setPin(this->m_iPin4);
        setValue(0);    
        msleep(this->m_iSpeed);
        
        setPin(this->m_iPin1);
        setValue(0);
        setPin(this->m_iPin2);
        setValue(1);
        setPin(this->m_iPin3);
        setValue(1);    
        setPin(this->m_iPin4);
        setValue(0);    
        msleep(this->m_iSpeed);
        
        setPin(this->m_iPin1);
        setValue(0);
        setPin(this->m_iPin2);
        setValue(0);
        setPin(this->m_iPin3);
        setValue(1);    
        setPin(this->m_iPin4);
        setValue(1);    
        msleep(this->m_iSpeed);
        
        setPin(this->m_iPin1);
        setValue(1);
        setPin(this->m_iPin2);
        setValue(0);
        setPin(this->m_iPin3);
        setValue(0);    
        setPin(this->m_iPin4);
        setValue(1);    
        msleep(this->m_iSpeed);


    }

    return 0;
}

int MOTOR::motorReverse(int angle)
{
    int i = 0;
    for(i=0; i<angle; i++) 
    {
        setPin(this->m_iPin1);
        setValue(1);
        setPin(this->m_iPin2);
        setValue(1);
        setPin(this->m_iPin3);
        setValue(0);    
        setPin(this->m_iPin4);
        setValue(0);    
        msleep(this->m_iSpeed);
        
        setPin(this->m_iPin1);
        setValue(1);
        setPin(this->m_iPin2);
        setValue(0);
        setPin(this->m_iPin3);
        setValue(0);    
        setPin(this->m_iPin4);
        setValue(1);    
        msleep(this->m_iSpeed);
        
        setPin(this->m_iPin1);
        setValue(0);
        setPin(this->m_iPin2);
        setValue(0);
        setPin(this->m_iPin3);
        setValue(1);    
        setPin(this->m_iPin4);
        setValue(1);    
        msleep(this->m_iSpeed);
        
        setPin(this->m_iPin1);
        setValue(0);
        setPin(this->m_iPin2);
        setValue(1);
        setPin(this->m_iPin3);
        setValue(1);    
        setPin(this->m_iPin4);
        setValue(0);    
        msleep(this->m_iSpeed);
    }
    
    return 0;
}

//停止电机
int MOTOR::motorStop(void)
{
    setPin(this->m_iPin1);
    setValue(0);
    setPin(this->m_iPin2);
    setValue(0);
    setPin(this->m_iPin3);
    setValue(0);    
    setPin(this->m_iPin4);
    setValue(0);    
    msleep(this->m_iSpeed);
    
    return 0;
}

int MOTOR::setSpeed(int speed)
{
    if (speed > 10)
        speed = 10;
    if (speed < 1)
        speed = 1;
    
    this->m_iSpeed = 4 * speed; //low
    
    return 0;
}

int MOTOR::getSpeed(void)
{
    return this->m_iSpeed/4;
}


