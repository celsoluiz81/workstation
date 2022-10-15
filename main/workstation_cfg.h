#ifndef WORKSTATION_CFG_H
#define WORKSTATION_CFG_H

#include "driver/gpio.h"

/* Motion detection Pins */
#define WORKSTATION_MOTION_IN_PIN     GPIO_NUM_32

/* LCD Pins */
#define WORKSTATION_LCD_PIN_RS        GPIO_NUM_4 /* CS */
#define WORKSTATION_LCD_PIN_RW        GPIO_NUM_19 /* MOSI */
#define WORKSTATION_LCD_PIN_E         GPIO_NUM_18 /* CLK */
#define WORKSTATION_LCD_PIN_RST       GPIO_NUM_3
#define WORKSTATION_LCD_PIN_SWITCH    GPIO_NUM_22

/* Joystick Pins */
#define WORKSTATION_JOYSTICK_PIN_CLK  GPIO_NUM_34
#define WORKSTATION_JOYSTICK_PIN_DT   GPIO_NUM_35
#define WORKSTATION_JOYSTICK_PIN_SW   GPIO_NUM_33

#define DISPLAY_SLEEP_TIME            (5 * 60 * 1000 * 1000) /* five minutes */

#define ESP_INTR_FLAG_DEFAULT         0

/* LED PIN for testing */
#define LED_GPIO                      GPIO_NUM_2

/* UART Buffer Size for communication with Arduino */
#define UART_BUFFER_SIZE              1024
#define UART_PORT_NUM                 0
#define UART_BAUD_RATE                115200
#define TASK_STACK_SIZE               2048
#define UART_TXD_PIN                  GPIO_NUM_1
#define UART_RXD_PIN                  GPIO_NUM_3
#define UART_RTS_PIN                  GPIO_NUM_4  /* check */
#define UART_CTS_PIN                  GPIO_NUM_36 /* check */

#endif /* WORKSTATION_CFG_H */
