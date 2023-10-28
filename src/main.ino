#include "workstation_cfg.h"
#include "workstation_wifi.h"
#include "workstation_lcd.h"
#include "workstation_motion.h"
//#include "workstation_joystick.h"
#include "workstation_relays.h"
#include "workstation_keypad.h"
#include "workstation_rgb.h"
#include "workstation_uv_light.h"

void setup() {

  Serial.begin(115200);
  delay(1000);


  //Turn lcd on 
  WorkstationLcdInit();  
  
  //Connect Wifi
  //const char* ssid =  "AppleTalk";
  //const char* password = "Benicio08092017!";
  WorkstationWifiInit();

  //Motion
  WorkstationMotionInit();

  //Joystick
  //WorkstationJoystickInit();

  //Relays
  WorkstationRelaysInit();

  //Keypad
  WorkstationKeypadInit();

  //RGB
  WorkstationRgbInit();

  //UV Light
  WorkstationUvInit();
}

void loop() {
  WorkstationLcdMain();
  WorkstationWifiMain();
  WorkstationMotionMain();
  //WorkstationJoystickMain();
  WorkstationRelaysMain();
  WorkstationKeypadMain();
  WorkstationRgbMain();
  WorkstationUvMain();
  delay(10);
}
