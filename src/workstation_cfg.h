#ifndef WORKSTATION_CFG_H
#define WORKSTATION_CFG_H

/* Motion detection Pins */
#define WORKSTATION_MOTION_IN_PIN     GPIO_NUM_32

/* LCD Pins */
#define WORKSTATION_LCD_PIN_RS        GPIO_NUM_5 /* CS */
#define WORKSTATION_LCD_PIN_RW        GPIO_NUM_23 /* MOSI */
#define WORKSTATION_LCD_PIN_E         GPIO_NUM_18 /* CLK */
#define WORKSTATION_LCD_PIN_RST       GPIO_NUM_22
#define WORKSTATION_LCD_PIN_SWITCH    GPIO_NUM_21
#define LCD_PIXEL_CLOCK_HZ            (20 * 1000 * 1000)

/* Keypad Pins*/
#define WORKSTATION_KEYPAD_PIN_IN       GPIO_NUM_33
#define WORKSTATION_KEYPAD_KEY_PRESS_WAIT 160 /* ms */

/* RGB LEDS */
#define WORKSTATION_RGB_RED              GPIO_NUM_16
#define WORKSTATION_RGB_BLUE            GPIO_NUM_17
#define WORKSTATION_RGB_GREEN           GPIO_NUM_19

/*Relay */
#define WORKSTATION_RELAY_PIN_1       GPIO_NUM_12
#define WORKSTATION_RELAY_PIN_2       GPIO_NUM_13
#define WORKSTATION_RELAY_PIN_3       GPIO_NUM_14
#define WORKSTATION_RELAY_PIN_4       GPIO_NUM_15

#define WORKSTATION_BACKLIGHT_PIN     WORKSTATION_RELAY_PIN_1  
#define WORKSTATION_UV_LIGHT_PIN      WORKSTATION_RELAY_PIN_1   

#define DISPLAY_SLEEP_TIME            (5 * 60 * 1000 ) /* five minutes */

#define ESP_INTR_FLAG_DEFAULT         0

/* LED PIN for testing */
#define LED_GPIO                      GPIO_NUM_2


#define WIFI_CONNECT_TIMEOUT           10 * 100 *10 /* 10 seconds) */
#define SPLASH_SCREEN_DELAY           1000 /* ms */
#define DEBUG_MODE                     true

#endif /* WORKSTATION_CFG_H */
