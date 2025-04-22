#include "Pulley.h"
#include <GlobalSettings.h>
#include <Logic.h>

TMC2209Stepper Pulley::driverL = TMC2209Stepper(&Serial1, 0.11f, 0b01);
TMC2209Stepper Pulley::driverR = TMC2209Stepper(&Serial2, 0.11f, 0b01);

AccelStepper Pulley::stepperL = AccelStepper(AccelStepper::DRIVER, STEPPER_L_STEP_PIN, STEPPER_L_DIR_PIN);
AccelStepper Pulley::stepperR = AccelStepper(AccelStepper::DRIVER, STEPPER_R_STEP_PIN, STEPPER_R_DIR_PIN);

MultiStepper Pulley::steppers = MultiStepper();

const int STEPS_PER_MM = (MOTOR_STEPS_PER_REV * MICROSTEPS) / (2 * PI * PULLEY_DIAMETER);

long positions[3][2] = {
    {0, 0}, // 0 mm
    {static_cast<long>(PULLEY_TRANS_HEIGHT * STEPS_PER_MM), static_cast<long>(PULLEY_DIAMETER * STEPS_PER_MM)}, // 20 mm
    {static_cast<long>(PULLEY_UP_HEIGHT * STEPS_PER_MM), static_cast<long>(PULLEY_UP_HEIGHT * STEPS_PER_MM)}, // 100
};

void Pulley::init() {
    Serial1.begin(ARDUINO_BAUD, SERIAL_8N1, STEPPER_L_RX_PIN, STEPPER_L_TX_PIN);
    Serial2.begin(ARDUINO_BAUD, SERIAL_8N1, STEPPER_R_RX_PIN, STEPPER_R_TX_PIN);

    driverL.begin();
    driverL.rms_current(900);
    driverL.microsteps(0);
    driverL.en_spreadCycle(true);
    driverL.pwm_autoscale(true);

    stepperL.setEnablePin(STEPPER_ENABLE_PIN); // Enable pin
    stepperL.setPinsInverted(false, false, true); // Invert direction pin
    stepperL.setMaxSpeed(PULLEY_SPEED * STEPS_PER_MM);

    driverR.begin();
    driverR.rms_current(900);
    driverR.microsteps(0);
    driverR.en_spreadCycle(true);
    driverR.pwm_autoscale(true);
    driverR.shaft(true);

    stepperR.setEnablePin(STEPPER_ENABLE_PIN); // Enable pin
    stepperR.setPinsInverted(false, false, true); // Invert direction pin
    stepperR.setMaxSpeed(PULLEY_SPEED * STEPS_PER_MM);

    steppers.addStepper(stepperL);
    steppers.addStepper(stepperR);

    steppers.moveTo(positions[DOWN_POS]);
    steppers.runSpeedToPosition();

    PulleyPosition target = PulleyPosition::DOWN_POS;
    PulleyPosition newTarget = PulleyPosition::DOWN_POS;
}

void Pulley::run(void *pvParameters) {
    for(;;) {
        steppers.run();
    }
}

void Pulley::check(void *pvParameters) {
    bool sentSignal = false;
    for(;;) {
        if (stepperL.distanceToGo() == 0 && stepperR.distanceToGo() == 0) {
            if (!sentSignal) {
                sentSignal = true;
                Logic::sendFeedback();
            }
        } else {
            sentSignal = false;
        }
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
}

void Pulley::setTarget(PulleyPosition newTarget) {
    steppers.moveTo(positions[newTarget]);
}