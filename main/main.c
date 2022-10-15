#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "workstation_cfg.h"
#include "esp_log.h"
#include "../components/comm/include/workstation_comm.h"
#include "../components/motion/include/workstation_motion.h"
#include "../components/joystick/include/workstation_joystick.h"
#include "../components/lcd/include/workstation_lcd.h"

static const char *TAG = "Main";

/* Prototypes */
void Init(void);
void Setup(void);

void app_main(void){
  esp_log_level_set("*", ESP_LOG_VERBOSE);
  Init();
  Setup();
}

void Init(void){
  ESP_LOGI(TAG, "Initializing the workstation!" );

  //WorkstationUartInit();
  WorkstationLcdInit();
  //WorkstationMotionInit();
  //WorkstationJoystickInit();

}

void Setup(void){

  /* Set the GPIO as a push/pull output */
  gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

  //WorkstationUartSetup();
  WorkstationLcdSetup();
  //WorkstationMotionSetup();
  //WorkstationJoystickSetup();
}
