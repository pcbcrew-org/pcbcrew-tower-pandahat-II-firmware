#define EXT_LED1 16
#define EXT_LED2 17

void led1_blink(void *pvParams) {
    gpio_pad_select_gpio(EXT_LED1);
    gpio_set_direction (EXT_LED1, GPIO_MODE_OUTPUT);
    while (1) {
        gpio_set_level(EXT_LED1, 0);
        vTaskDelay(500/portTICK_RATE_MS);
        gpio_set_level(EXT_LED1, 1);
        vTaskDelay(500/portTICK_RATE_MS);
    }
}

void led2_blink(void *pvParams) {
    gpio_pad_select_gpio(EXT_LED2);
    gpio_set_direction (EXT_LED2, GPIO_MODE_OUTPUT);
    while (1) {
        gpio_set_level(EXT_LED2, 0);
        vTaskDelay(133/portTICK_RATE_MS);
        gpio_set_level(EXT_LED2, 1);
        vTaskDelay(133/portTICK_RATE_MS);
    }
}

void setup_led(void)
{
    xTaskCreate(&led1_blink,"LED_BLINK",4*1024,NULL,4,NULL);
    xTaskCreate(&led2_blink,"LED_BLINK",4*1024,NULL,4,NULL);
}
