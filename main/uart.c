#include "esp_log.h"
#include "driver/uart.h"

#define UART_LOG_TICK 200
#define GPS_BAUD 115200
#define GPS_TX 41
#define GPS_RX 40
#define GPS_RTS 6 // unused pin
#define GPS_CTS 7 // unused pin
#define GPS_UART UART_NUM_1
#define GPS_LOG_TICK 33
#define GPS_LOG_STACK_SIZE 2048


void init_uart(int dev_uart, int baud, int tx_pin, int rx_pin) {
    uart_config_t uart_config = {
        .baud_rate = baud,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_driver_install(dev_uart, 1024, 0, 0, NULL, 0);
    uart_param_config(dev_uart, &uart_config);
    uart_set_pin(dev_uart, tx_pin, rx_pin, GPS_RTS, GPS_CTS);
}

int get_buffered_data_len(int dev_uart){
    int length = 0;
    uart_get_buffered_data_len(dev_uart, (size_t*)&length);
    return length;
}

void get_uart_data(int dev_uart, uint8_t *buffer, int len){
    int length = 0;
    length = uart_read_bytes(dev_uart, buffer, len, 1 / portTICK_PERIOD_MS);
}

void send_uart_data(int dev_uart, char* str_data) {
    uart_write_bytes(dev_uart, (const char*)str_data, strlen(str_data));
    printf("  To UART >>");
    log_text(str_data, strlen(str_data));
    printf("\n");
}

void log_uart_data(int dev_uart) {
    int buffered_data_len = 0;
    uint8_t data[64];

    buffered_data_len = get_buffered_data_len(dev_uart);
    if (buffered_data_len > 0) {
        get_uart_data(dev_uart, data, buffered_data_len);
        printf("From UART << ");
        log_text(data, buffered_data_len);
        printf("\n");
    } else {
        // printf("GPS_UART_2 >> ");
        // printf("none\n");
    }
}

void log_text(char* str_data, int len) {
    for (int i=0; i < len; i++) {
        if (str_data[i] == '\r') {
            printf("\\r");
        } else if (str_data[i] == '\n') {
            printf("\\n");
        } else {
            printf("%c", str_data[i]);
        }
    }
}

void gps_log_task(void *arg) {
    init_uart(GPS_UART, GPS_BAUD, GPS_TX, GPS_RX);

    char* cmd = "";

    // vTaskDelay(pdMS_TO_TICKS(1000));
    // char* cmd_uart_b_enable = "$JASC,GPGGA,1,PORTB\r\n";
    // send_uart_data(GPS_UART, cmd_uart_b_enable);

    vTaskDelay(pdMS_TO_TICKS(4000));
    cmd = "$JETHERNET,MODE,STATIC,192.168.5.88,255.255.255.0.\r\n";
    send_uart_data(GPS_UART, cmd);

    vTaskDelay(pdMS_TO_TICKS(4000));
    cmd = "$>JETHERNET,WEBUI,ON\r\n";
    send_uart_data(GPS_UART, cmd);

    vTaskDelay(pdMS_TO_TICKS(4000));
    cmd = "$>JETHERNET\r\n";
    send_uart_data(GPS_UART, cmd);

    vTaskDelay(pdMS_TO_TICKS(4000));
    cmd = "$>JETHERNET\r\n";
    send_uart_data(GPS_UART, cmd);

    while (1) {
        log_uart_data(GPS_UART);
        vTaskDelay(pdMS_TO_TICKS(UART_LOG_TICK));
    }
    vTaskDelete(NULL);
}

void setup_uart() {
    xTaskCreate(gps_log_task, "gps_log_task", GPS_LOG_STACK_SIZE, NULL, 10, NULL);
}
