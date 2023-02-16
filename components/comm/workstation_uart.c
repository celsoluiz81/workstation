#include <stdio.h>
#include "driver/uart.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "workstation_comm.h"
#include "../../main/workstation_cfg.h"

static const char *TAG = "UART";

static void comm_task(void* arg);

uart_config_t uart_config;
int intr_alloc_flags;
uint8_t* RxBUffer;
uint8_t* TxBUffer;

void WorkstationUartInit(void){

  //start all tasks
  xTaskCreate(comm_task, "comm_task", 2048, NULL, 10, NULL);

  /* Configure parameters of an UART driver,
   * communication pins and install the driver */
  uart_config_t config = {
         .baud_rate =UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };

  uart_config = config;

  intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
  intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif


  // Configure a temporary buffer for the incoming data
  RxBUffer = (uint8_t *) malloc(UART_RX_BUFFER_SIZE+1);

}

void WorkstationUartSetup(void){
  /* Setup Uart driver and pin */
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, UART_TXD_PIN, UART_RXD_PIN, UART_RTS_PIN, UART_CTS_PIN));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, UART_RX_BUFFER_SIZE, 0, 0, NULL, intr_alloc_flags));
}

void WorkstationUartMain(void)
{
    const int rxBytes = uart_read_bytes(UART_NUM_1, RxBUffer, UART_RX_BUFFER_SIZE, 1000 / portTICK_PERIOD_MS);

    if(rxBytes > 0){
      ESP_LOGI(TAG, "Recv str: %s", (char *) RxBUffer);

      uint8_t messageId = RxBUffer[0];
      RxBUffer[rxBytes] = 0;
      ESP_LOGI(TAG, "Read %d bytes: '%s'", rxBytes, RxBUffer);
      ESP_LOG_BUFFER_HEXDUMP(TAG, RxBUffer, rxBytes, ESP_LOG_INFO);
      switch (messageId) {
        case UART_TEST_MESSAGE:

        break;
        case UART_CLAMP_STATE_MESSAGE:

        break;
        case UART_STATE_SEND_MSG:

        break;

      }
    }
}

uint8_t SendUartMessage(const char * data){
  uint8_t length = sizeof(data);
  return uart_write_bytes(UART_PORT_NUM, (const char *) data, length);
}

static void comm_task(void* arg){
  while (1) {
    WorkstationUartMain();
  }
  free(RxBUffer);
}
