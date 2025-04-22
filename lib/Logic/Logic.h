#pragma once

#include <FreeRTOS.h>
#include <freertos/queue.h>
#include <PulleyPosition.h>

class Logic {
    public:

    static void init();
    static void run(void *pvParameters);
    static void sendCommand(PulleyPosition position);
    static void sendFeedback();

    private:

    static QueueHandle_t commands;
};