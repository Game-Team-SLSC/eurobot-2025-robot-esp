#include <Logic.h>
#include <Pulley.h>
#include <ArduinoCommunication.h>

QueueHandle_t Logic::commands = NULL;

void Logic::init() {
    commands = xQueueCreate(10, sizeof(int));
}

void Logic::sendCommand(PulleyPosition position) {
    xQueueOverwrite(commands, (void *)&position);
}

void Logic::sendFeedback(PulleyPosition position) {
    ArduinoCommunication::sendFeedback(position);
}

void Logic::run(void *pvParameters) {
    PulleyPosition pos;
    for(;;) {
        if (xQueueReceive(commands, &pos, 0) == pdTRUE) {
            Pulley::setTarget(pos);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}