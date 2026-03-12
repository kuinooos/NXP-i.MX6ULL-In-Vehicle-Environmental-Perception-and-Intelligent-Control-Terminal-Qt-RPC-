    
#include "gpio.h"

GPIO::GPIO(void)
{
    
}   

GPIO::GPIO(int pin) : m_iPin(pin)
{
    this->setPath(pin);
}   


void GPIO::setPin(int pin)
{
    this->m_iPin = pin; 
    this->setPath(pin);
}


int GPIO::getPin(void)
{
    return this->m_iPin; 
}

void GPIO::setPath(int pin)
{
    stringstream s;
    s << "/sys/class/gpio/gpio" << pin << "/"; 
    this->m_sPath = s.str();
}

string GPIO::getPath(void)
{
    return this->m_sPath;
}


int GPIO::setDirection(int dir)
{
    ofstream file((this->m_sPath + "direction").c_str());

    if (!file.is_open()){
       cout << "setDirection: write failed to open file" << endl;
       return -1;
   }
   
    if (dir)
        file << "in"; 
    else
        file << "out";  
    
   file.close();
   
   return 0;
}

int GPIO::getDirection(void)
{
    char buffer[10];
    
    ifstream file((this->m_sPath + "direction").c_str()); 
    
    if (!file.is_open()){
       cout << "getDirection: write failed to open file" << endl;
       return -1;
    }
    
    file >> buffer;
    
    file.close();
    
    if (!strcmp(buffer, "out")) //is out
        return 0;
    else
        return 1;
}

int GPIO::setValue(int val)
{
    ofstream file((this->m_sPath + "value").c_str());
    
    if (!file.is_open()){
       cout << "setValue: write failed to open file" << endl;
       return -1;
   }
   file << val;
   file.close();
   
   return 0;
}

int GPIO::getValue(void)
{
    char buffer[255];
    
    ifstream file((this->m_sPath + "value").c_str()); 
    
    if (!file.is_open()){
       cout << "getValue: write failed to open file" << endl;
       return -1;
    }
    
    file >> buffer;
    
    file.close();
    
    return atoi(buffer);
}

int GPIO::exportGPIO(void)
{
    ofstream file("/sys/class/gpio/export");
    
    if (!file.is_open()){
       cout << "exportGPIO: write failed to open file" << endl;
       return -1;
   }
   file << this->m_iPin;
   file.close();
   
   return 0;
}

int GPIO::unexportGPIO(void)
{
    ofstream file("/sys/class/gpio/unexport");
    
    if (!file.is_open()){
       cout << "unexportGPIO: write failed to open file" << endl;
       return -1;
   }
   file << this->m_iPin;
   file.close();
   
   return 0;
}
