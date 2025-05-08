#include <ArduinoCommunication.h>
#include <GlobalSettings.h>
#include <SoftwareSerial.h>
#include <Logic.h>

EspSoftwareSerial::UART ArduinoCommunication::arduinoSerial;

void ArduinoCommunication::init() {
    arduinoSerial.begin(ARDUINO_BAUD, EspSoftwareSerial::SWSERIAL_8N1, ARDUINO_TX, ARDUINO_RX);   
}

void ArduinoCommunication::check(void *pvParameters) {
    for(;;) {
        if (arduinoSerial.available()) {
            PulleyPosition pos;
            arduinoSerial.read((uint8_t*)&pos, sizeof(PulleyPosition));
            Logic::sendCommand(pos);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    
}

void ArduinoCommunication::sendFeedback(PulleyPosition position) {
    arduinoSerial.write(position);
    arduinoSerial.flush();
}