#pragma once

#include <TMCStepper.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <PulleyPosition.h>

class Pulley {
    public:
        
    static void init();
    static void run(void *pvParameters);
    static void check(void *pvParameters);

    static void setTarget(PulleyPosition target);

    private:
    
    static TMC2209Stepper driverL, driverR;
    static AccelStepper stepperL, stepperR;
    static MultiStepper steppers;
};