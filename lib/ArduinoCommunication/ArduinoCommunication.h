#pragma once

#include <SoftwareSerial.h>
#include <PulleyPosition.h>

class ArduinoCommunication {
    public:
        
    static void init();
    static void check(void *pvParameters);
    static void sendFeedback(PulleyPosition position);
        
    private:

    static EspSoftwareSerial::UART arduinoSerial;
};