#include <stdio.h>
#include <unistd.h>
#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "workstation_joystick.h"
#include "../../main/workstation_cfg.h"

static const char *TAG = "JOYSTICK";
volatile int counter = 0;

static QueueHandle_t joystick_evt_queue = NULL;

static void ReadEncoder(void);
static void joystick_task(void* arg);
static void joystick_isr_handler(void* arg);

void WorkstationJoystickInit(void){

  //Setup Queue
  joystick_evt_queue = xQueueCreate(10, sizeof(uint32_t));
  if (!joystick_evt_queue) {
      ESP_LOGE(TAG, "Creating gpio queue failed");
      return;
  }

    xTaskCreate(joystick_task, "joystick_task", 1024, NULL, 10, NULL);
}

void WorkstationJoystickSetup(void){

  /* Configure GPIO interrupt for encoder CLK pin */
  gpio_set_intr_type(WORKSTATION_JOYSTICK_PIN_CLK, GPIO_INTR_ANYEDGE);
  gpio_set_direction(WORKSTATION_JOYSTICK_PIN_CLK, GPIO_MODE_INPUT);
  gpio_wakeup_enable(WORKSTATION_JOYSTICK_PIN_CLK, GPIO_INTR_HIGH_LEVEL);
  gpio_isr_handler_add(WORKSTATION_JOYSTICK_PIN_CLK,joystick_isr_handler,  (void*) WORKSTATION_JOYSTICK_PIN_CLK);

  /* Configure GPIO interrupt for encoder DT pin */
  gpio_set_intr_type(WORKSTATION_JOYSTICK_PIN_DT, GPIO_INTR_ANYEDGE);
  gpio_set_direction(WORKSTATION_JOYSTICK_PIN_DT, GPIO_MODE_INPUT);
  gpio_wakeup_enable(WORKSTATION_JOYSTICK_PIN_DT, GPIO_INTR_HIGH_LEVEL);
  gpio_isr_handler_add(WORKSTATION_JOYSTICK_PIN_DT,joystick_isr_handler,  (void*) WORKSTATION_JOYSTICK_PIN_DT);

  /* Configure GPIO interrupt for encoder DT pin */
  gpio_set_intr_type(WORKSTATION_JOYSTICK_PIN_SW, GPIO_INTR_ANYEDGE);
  gpio_pullup_en(WORKSTATION_JOYSTICK_PIN_SW);
  gpio_set_direction(WORKSTATION_JOYSTICK_PIN_SW, GPIO_MODE_INPUT);
  gpio_wakeup_enable(WORKSTATION_JOYSTICK_PIN_SW, GPIO_INTR_HIGH_LEVEL);
  gpio_isr_handler_add(WORKSTATION_JOYSTICK_PIN_SW,joystick_isr_handler,  (void*) WORKSTATION_JOYSTICK_PIN_SW);

}

void WorkstationJoystickMain(void){
  static int lastCounter = 0;

  // If count has changed print the new value to serial
  if(counter != lastCounter){

    lastCounter = counter;
    ESP_LOGI(TAG, "counter is now:%d", lastCounter);
  }
}

void ReadEncoder(void) {
  // Encoder interrupt routine for both pins. Updates counter
  // if they are valid and have rotated a full indent
  ESP_LOGI(TAG, "Read Encoder");
  static uint8_t old_AB = 3;  // Lookup table index
  static int8_t encval = 0;   // Encoder value
  static const int8_t enc_states[]  = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Lookup table

  old_AB <<=2;  // Remember previous state

  if (gpio_get_level(WORKSTATION_JOYSTICK_PIN_CLK)) old_AB |= 0x02; // Add current state of pin A
  if (gpio_get_level(WORKSTATION_JOYSTICK_PIN_DT)) old_AB |= 0x01; // Add current state of pin B

  encval += enc_states[( old_AB & 0x0f )];

  // Update counter if encoder has rotated a full indent, that is at least 4 steps
  if( encval > 3 ) {        // Four steps forward
    counter++;              // Increase counter
    encval = 0;
  }
  else if( encval < -3 ) {  // Four steps backwards
   counter--;               // Decrease counter
   encval = 0;
  }
}

static void joystick_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(joystick_evt_queue, &gpio_num, NULL);
}

static void joystick_task(void* arg)
{
    uint32_t io_num;
    while(1) {
        WorkstationJoystickMain();
        if(xQueueReceive(joystick_evt_queue, &io_num, portMAX_DELAY)) {
          ReadEncoder();
        }

        vTaskDelay(10);
    }
}
