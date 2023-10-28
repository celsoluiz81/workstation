
#include"workstation_motion.h"
#include "workstation_cfg.h"

// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

void IRAM_ATTR motionHandler(void);

void WorkstationMotionInit(void){

  Serial.println("Initializing motion sensor"); 
 
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(WORKSTATION_MOTION_IN_PIN, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(WORKSTATION_MOTION_IN_PIN), motionHandler, RISING);
}

void WorkstationMotionMain(void){
    // Current time
    now = millis();
    
    if((now - lastTrigger) > DISPLAY_SLEEP_TIME ){
      digitalWrite(WORKSTATION_LCD_PIN_SWITCH, LOW);
    }
}


// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR motionHandler() {
  Serial.println("MOTION DETECTED!!!");
  digitalWrite(WORKSTATION_LCD_PIN_SWITCH, HIGH);
  lastTrigger = millis();
}
