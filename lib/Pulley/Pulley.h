#pragma once

#include <TMCStepper.h>
#include <PulleyPosition.h>
#include <FastAccelStepper.h>
#include <FreeRTOS.h>
#include <freertos/queue.h>

class Pulley {
    public:
        
    static void init();
    static void run(void *pvParameters);

    static void setTarget(PulleyPosition target);

    private:
    
    static QueueHandle_t targetMailbox;
    static TMC2209Stepper driverL, driverR;
    static FastAccelStepperEngine engine;
    static FastAccelStepper *stepperL, *stepperR; 
};