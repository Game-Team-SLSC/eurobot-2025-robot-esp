#pragma once

#include <Arduino.h>

// CALIBRATING

#define PULLEY_UP_HEIGHT 275 // mm
#define PULLEY_TRANS_HEIGHT 40 // mm
#define PULLEY_ZERO 0 // mm
#define PULLEY_SPEED 200 // mm/s
#define PULLEY_ACCEL 230 // mm/s2

// COMMUNICATION

#define ARDUINO_RX D5
#define ARDUINO_TX D6

#define ARDUINO_BAUD 9600

// STEPPERS

#define STEPPER_ENABLE_PIN D0

#define STEPPER_L_STEP_PIN D1
#define STEPPER_L_DIR_PIN D2
#define STEPPER_L_TX_PIN D3
#define STEPPER_L_RX_PIN D4

#define STEPPER_R_STEP_PIN D10
#define STEPPER_R_DIR_PIN D9
#define STEPPER_R_TX_PIN D8
#define STEPPER_R_RX_PIN D7

#define PULLEY_DIAMETER 15.f // mm
#define MOTOR_STEPS_PER_REV 400
#define MICROSTEPS 4