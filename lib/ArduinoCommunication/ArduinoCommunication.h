#pragma once

#include <SoftwareSerial.h>

class ArduinoCommunication {
    public:
        
    static void init();
    static void check(void *pvParameters);
        
    private:

    static EspSoftwareSerial::UART *arduinoSerial;
};