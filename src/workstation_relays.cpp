#include <Arduino.h>
#include "workstation_cfg.h"
#include "workstation_relays.h"

boolean relay_status_1;
boolean relay_status_2;
boolean relay_status_3;
boolean relay_status_4;

void WorkstationRelaysInit(void){
  Serial.println("Initializing relays"); 

  relay_status_1 = false; 
  relay_status_2 = false;
  relay_status_3 = false;
  relay_status_4 = false;
 
  // Set relay pin modes
  pinMode(WORKSTATION_RELAY_PIN_1, OUTPUT);
  pinMode(WORKSTATION_RELAY_PIN_2, OUTPUT);
  pinMode(WORKSTATION_RELAY_PIN_3, OUTPUT);
  pinMode(WORKSTATION_RELAY_PIN_4, OUTPUT);

  // Set pins to low
  digitalWrite(WORKSTATION_RELAY_PIN_1, LOW);
  digitalWrite(WORKSTATION_RELAY_PIN_2, LOW);
  digitalWrite(WORKSTATION_RELAY_PIN_3, LOW);
  digitalWrite(WORKSTATION_RELAY_PIN_4, LOW);
}

void WorkstationRelaysMain(void){
  
}

void WorkstationRelaysActivateRelay1(){
  relay_status_1 = true;
  digitalWrite(WORKSTATION_RELAY_PIN_1, HIGH);
  if(DEBUG_MODE){
      Serial.println("Relay 1 Active"); 
  }
}

void WorkstationRelaysDeactivateRelay1(){
  relay_status_1 = false;
  digitalWrite(WORKSTATION_RELAY_PIN_1, LOW);
  if(DEBUG_MODE){
      Serial.println("Relay 1 Inactive"); 
  }
}
void WorkstationRelaysActivateRelay2(){
  relay_status_2 = true;
  digitalWrite(WORKSTATION_RELAY_PIN_2, HIGH);
  if(DEBUG_MODE){
      Serial.println("Relay 2 Active"); 
  }
}

void WorkstationRelaysDeactivateRelay2(){
  relay_status_2 = false;
  digitalWrite(WORKSTATION_RELAY_PIN_2, LOW);
  if(DEBUG_MODE){
      Serial.println("Relay 2 Inactive"); 
  }
}
void WorkstationRelaysActivateRelay3(){
  relay_status_3 = true;
  digitalWrite(WORKSTATION_RELAY_PIN_3, HIGH);
  if(DEBUG_MODE){
      Serial.println("Relay 3 Active"); 
  }
}

void WorkstationRelaysDeactivateRelay3(){
  relay_status_3 = false;
  digitalWrite(WORKSTATION_RELAY_PIN_3, LOW);
  if(DEBUG_MODE){
      Serial.println("Relay 3 Inactive"); 
  }
}
void WorkstationRelaysActivateRelay4(){
  relay_status_4 = true;
  digitalWrite(WORKSTATION_RELAY_PIN_4, HIGH);
  if(DEBUG_MODE){
      Serial.println("Relay 4 Active"); 
  }
}

void WorkstationRelaysDeactivateRelay4(){
  relay_status_4 = false;
  digitalWrite(WORKSTATION_RELAY_PIN_4, LOW);
  if(DEBUG_MODE){
      Serial.println("Relay 4 Inactive"); 
  }
}

boolean WorkstationRelaysReadRelay1Status(){
  return relay_status_1;
}
boolean WorkstationRelaysReadRelay2Status(){
  return relay_status_2;
}
boolean WorkstationRelaysReadRelay3Status(){
  return relay_status_3;
}
boolean WorkstationRelaysReadRelay4Status(){
  return relay_status_4;
}