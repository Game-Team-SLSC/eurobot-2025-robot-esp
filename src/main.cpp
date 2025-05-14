#include <FreeRTOS.h>
#include <Logic.h>
#include <ArduinoCommunication.h>
#include <Pulley.h>

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  Serial.println("Initializing Pulley...");
  Pulley::init();
  
  Serial.println("Initializing Arduino Communication...");
  ArduinoCommunication::init();

  Serial.println("Initializing Logic...");
  Logic::init();

  configASSERT(xTaskCreatePinnedToCore(
    Pulley::run,
    "Pulley runner",
    4096,
    NULL,
    1,
    NULL,
    1
  ) == pdPASS);

  configASSERT(xTaskCreatePinnedToCore(
    ArduinoCommunication::check,
    "Arduino commands check",
    4096,
    NULL,
    1,
    NULL,
    1
  ) == pdPASS);
  
  configASSERT(xTaskCreatePinnedToCore(
    Logic::run,
    "Logic computing",
    4096,
    NULL,
    2,
    NULL,
    1
  ) == pdPASS);

  Serial.println("ESP Ready");
}

void loop() {}