#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Arduino.h"

#include "storage.c"
#include "router.c"
#include "led.c"
#include "uart.c"
#include "webserver.c"


static const char *TAG_MAIN = "Tower PANDAHAT";

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);

    setup_storage();
    setup_router();
    setup_led();
    setup_uart();
    setup_webserver();

    initArduino();
    pinMode(9, OUTPUT);
    digitalWrite(9, HIGH);

    while (true) {
        //
    }
}
