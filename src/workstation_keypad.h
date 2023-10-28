#ifndef WORKSTATION_KEYPAD_H
#define WORKSTATION_KEYPAD_H
#include <Arduino.h>


void WorkstationKeypadMain(void);  
void WorkstationKeypadInit (void);

#define LEFT_BUTTON_LOW 0
#define LEFT_BUTTON_HIGH 0
#define TOP_BUTTON_LOW   100
#define TOP_BUTTON_HIGH  400
#define BOTTOM_BUTTON_LOW   1000
#define BOTTOM_BUTTON_HIGH  1300
#define RIGHT_BUTTON_LOW   1600
#define RIGHT_BUTTON_HIGH  1900
#define ENTER_BUTTON_LOW   2500
#define ENTER_BUTTON_HIGH  2800
#define IDLE_BUTTON_LOW   4000
#define IDLE_BUTTON_HIGH  4200

typedef enum{
    IDLE_BUTTON=255,
    LEFT_BUTTON=0,
    ENTER_BUTTON=80,
    RIGHT_BUTTON=82,
    BOTTOM_BUTTON=83,
    TOP_BUTTON=84
} workstation_keypad_button_t;

#endif