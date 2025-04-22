#pragma once

#include <FreeRTOS.h>
#include <freertos/queue.h>
#include <models/PulleyPosition.h>

class Logic {
    public:

    static void init();
    static void run(void *pvParameters);
    static void sendCommand(PulleyPosition position);

    private:

    static QueueHandle_t commands;
};