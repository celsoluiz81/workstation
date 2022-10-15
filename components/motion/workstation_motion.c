#include <stdbool.h>
#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "../../main/workstation_cfg.h"
#include "esp_log.h"
#include "../lcd/include/workstation_lcd.h"
#include "workstation_motion.h"

static const char *TAG = "Motion";

/* Queues*/
static QueueHandle_t gpio_evt_queue = NULL;
static QueueHandle_t motion_timer_queue = NULL;

/* Timers */
gptimer_handle_t workstationMotionDetectionTimer;

/* Variables */
typedef struct {
    uint64_t event_count;
} motion_queue_element_t;


static void motion_isr_handler(void* arg);
static void motion_task(void* arg);
static bool WorkstationMotionTimerCallback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);

void WorkstationMotionInit(void){

  /* Initialize queues */
  motion_timer_queue = xQueueCreate(10, sizeof(motion_queue_element_t));
  if (!motion_timer_queue) {
      ESP_LOGE(TAG, "Creating motion queue failed");
      return;
  }

  gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
  if (!gpio_evt_queue) {
      ESP_LOGE(TAG, "Creating gpio queue failed");
      return;
  }

  ESP_LOGI(TAG, "Initializing motion task!");
  xTaskCreate(motion_task, "motion_task", 2048, NULL, 9, NULL);
}

void WorkstationMotionSetup(void){
  ESP_LOGI(TAG, "Setting up motion timers and interrupts!");
  /* Configure Motion GPIO */
  gpio_set_intr_type(WORKSTATION_MOTION_IN_PIN, GPIO_INTR_ANYEDGE);

  gpio_set_direction(WORKSTATION_MOTION_IN_PIN, GPIO_MODE_INPUT);
  gpio_wakeup_enable(WORKSTATION_MOTION_IN_PIN, GPIO_INTR_HIGH_LEVEL);
  gpio_isr_handler_add(WORKSTATION_MOTION_IN_PIN,motion_isr_handler,  (void*) WORKSTATION_MOTION_IN_PIN);

  /* Init timers */
  gptimer_config_t motion_timer_config = {
    .clk_src = GPTIMER_CLK_SRC_DEFAULT,
    .direction = GPTIMER_COUNT_UP,
    .resolution_hz = 1 * 1000 * 1000, // 1MHz, 1 tick = 1us
  };

  gptimer_alarm_config_t motion_timer_alarm_config = {
    .reload_count = 0, // counter will reload with 0 on alarm event
    .alarm_count = DISPLAY_SLEEP_TIME, // alarm target = 1s @resolution 1MHz
    .flags.auto_reload_on_alarm = true
  };

  gptimer_event_callbacks_t cbs = {
    .on_alarm = &WorkstationMotionTimerCallback, // register user callback
  };

  ESP_ERROR_CHECK(gptimer_new_timer(&motion_timer_config, &workstationMotionDetectionTimer));
  ESP_ERROR_CHECK(gptimer_set_alarm_action(workstationMotionDetectionTimer, &motion_timer_alarm_config));
  ESP_ERROR_CHECK(gptimer_register_event_callbacks(workstationMotionDetectionTimer, &cbs, motion_timer_queue));
  ESP_ERROR_CHECK(gptimer_enable(workstationMotionDetectionTimer));
  ESP_ERROR_CHECK(gptimer_start(workstationMotionDetectionTimer));
}

void WorkstationMotionMain(void){

}

static void motion_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void motion_task(void* arg)
{
    uint32_t io_num;
    while(1) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
          switch (io_num) {
            case WORKSTATION_MOTION_IN_PIN:
              printf("Motion detected\n");
              LcdWakeupDisplay(WORKSTATION_LCD_PIN_SWITCH);
              gpio_set_level(LED_GPIO, 1); //For debugging only
              ESP_ERROR_CHECK(gptimer_set_raw_count(workstationMotionDetectionTimer, 0)); //restart timer
            break;
          }
        }
        vTaskDelay(10);
    }
}

/*
* WorkstationMotionTimerCallback
* This function is called every time the motion timer bursts
* :return bool
*/
static bool IRAM_ATTR WorkstationMotionTimerCallback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
  ESP_LOGI(TAG, "Motion timer expired!");
  BaseType_t high_task_awoken = pdFALSE;
  LcdSleepDisplay(WORKSTATION_LCD_PIN_SWITCH);
  gptimer_disable(workstationMotionDetectionTimer);
  // return whether we need to yield at the end of ISR
  return (high_task_awoken == pdTRUE);
}

bool ReadMotionStatus(void){

  return false;
}
