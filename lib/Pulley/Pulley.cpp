#include "Pulley.h"
#include <GlobalSettings.h>
#include <Logic.h>

TMC2209Stepper Pulley::driverL = TMC2209Stepper(&Serial1, 0.11f, 0b01);
TMC2209Stepper Pulley::driverR = TMC2209Stepper(&Serial2, 0.11f, 0b01);

FastAccelStepperEngine Pulley::engine = FastAccelStepperEngine();

FastAccelStepper *Pulley::stepperL = NULL;
FastAccelStepper *Pulley::stepperR = NULL;

QueueHandle_t Pulley::targetMailbox = xQueueCreate(1, sizeof(PulleyPosition));

const int STEPS_PER_MM = (MOTOR_STEPS_PER_REV * MICROSTEPS) / (PI * PULLEY_DIAMETER);

int positions[3][2] = {
    {0, 0}, // 0 mm
    {static_cast<long>(PULLEY_TRANS_HEIGHT * STEPS_PER_MM), -static_cast<long>(PULLEY_DIAMETER * STEPS_PER_MM)}, // 20 mm
    {static_cast<long>(PULLEY_UP_HEIGHT * STEPS_PER_MM), -static_cast<long>(PULLEY_UP_HEIGHT * STEPS_PER_MM)}, // 100 mm
};

void Pulley::init() {
    Serial1.begin(ARDUINO_BAUD, SERIAL_8N1, STEPPER_L_RX_PIN, STEPPER_L_TX_PIN);
    Serial2.begin(ARDUINO_BAUD, SERIAL_8N1, STEPPER_R_RX_PIN, STEPPER_R_TX_PIN);

    driverL.begin();
    driverL.rms_current(900);
    driverL.microsteps(MICROSTEPS);
    driverL.en_spreadCycle(true);
    driverL.pwm_autoscale(true);

    engine.init();
    stepperL = engine.stepperConnectToPin(STEPPER_L_STEP_PIN, DRIVER_RMT);
    stepperL->setDirectionPin(STEPPER_L_DIR_PIN);
    stepperL->setEnablePin(STEPPER_ENABLE_PIN); // Enable pin
    stepperL->setSpeedInHz(STEPS_PER_MM * PULLEY_SPEED);
    stepperL->setAcceleration(PULLEY_ACCEL * STEPS_PER_MM);
    stepperL->enableOutputs();
    
    driverR.begin();
    driverR.rms_current(900);
    driverR.microsteps(MICROSTEPS);
    driverR.en_spreadCycle(true);
    driverR.pwm_autoscale(true);


    stepperR = engine.stepperConnectToPin(STEPPER_R_STEP_PIN);
    stepperR->setDirectionPin(STEPPER_R_DIR_PIN);
    stepperR->setEnablePin(STEPPER_ENABLE_PIN); // Enable pin
    stepperR->setSpeedInHz(STEPS_PER_MM * PULLEY_SPEED);
    stepperR->setAcceleration(PULLEY_ACCEL * STEPS_PER_MM);
    stepperR->enableOutputs();

    setTarget(DOWN_POS);
}

void Pulley::run(void *pvParameters) {
    bool sentSignal = true;
    PulleyPosition target;

    for(;;) {
        PulleyPosition newTarget;

        if ((xQueueReceive(targetMailbox, &newTarget, 0) == pdTRUE) && (newTarget != target)) {
            Serial.println(newTarget);
            target = newTarget;
            sentSignal = false;
            stepperL->moveTo(positions[target][0]);
            stepperR->moveTo(positions[target][1]);
        }

        if (!stepperL->isRunning() && !stepperR->isRunning()) {
            if (!sentSignal) {
                sentSignal = true;
                Logic::sendFeedback(target);
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

}

void Pulley::setTarget(PulleyPosition newTarget) {
    xQueueOverwrite(targetMailbox, (void *)&newTarget);
}