#include "Arduino.h"
#include "workstation_rgb.h"
#include "workstation_cfg.h"
#include "workstation_relays.h"

bool rgb_status = false;
int red_lcd_value=0;
int green_lcd_value=0;
int blue_lcd_value=0;

workstation_rgb_state_t rgbMainState;
rgbColor current_color;

void WorkstationRgbInit(void){
     Serial.println("Initializing RGB Lights"); 
  
    // Set relay pin modes
    pinMode(WORKSTATION_RGB_RED, OUTPUT);
    pinMode(WORKSTATION_RGB_GREEN, OUTPUT);
    pinMode(WORKSTATION_RGB_BLUE, OUTPUT);
  
    // Set pins to low
    digitalWrite(WORKSTATION_RGB_RED, LOW);
    digitalWrite(WORKSTATION_RGB_GREEN, LOW);
    digitalWrite(WORKSTATION_RGB_BLUE, LOW);

    rgbMainState = RGB_STATE_IDLE;
}

void WorkstationRgbMain(void){
    rgb_status = WorkstationRelaysReadRelay1Status();
    if(!rgb_status){
        rgbMainState = RGB_STATE_IDLE;
    }

    switch (rgbMainState)
        {
        case RGB_STATE_IDLE:
            if(rgb_status){
                rgbMainState = RGB_STATE_BACKLIGHT;
            }

            break;
        case RGB_STATE_LIGHTSHOW:
            Serial.println("Running Lightshow");
            break;
        case RGB_STATE_BACKLIGHT:
            if(DEBUG_MODE){
            Serial.print("Backlights ON:0x");
            Serial.print(current_color.red); 
             Serial.print(current_color.green); 
              Serial.println(current_color.blue); 
            }
            analogWrite(WORKSTATION_RGB_RED,current_color.red);
            analogWrite(WORKSTATION_RGB_GREEN,current_color.green);
            analogWrite(WORKSTATION_RGB_BLUE,current_color.blue);
        break;
        }
}

void WorkstationRgbSetMainState(workstation_rgb_state_t state){
    rgbMainState = state;
}

void WorkstationRgbSetColor(int hexValue){
    current_color.red =  ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
    current_color.green= ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
    current_color.blue= ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte

    rgbMainState = RGB_STATE_BACKLIGHT;
}
