#include "workstation_cfg.h"
#include "workstation_wifi.h"
#include "workstation_lcd.h"

void setup() {
  // put your setup code here, to run once:
  const char* ssid =  "AppleTalk";
  const char* password = "Benicio08092017!";

  WorkstationLcd lcd;
  
  lcd::WorkstationLcdInit();
  
  WorkstationWifiSetup(ssid,password, lcd);
  
}

void loop() {
  
}
