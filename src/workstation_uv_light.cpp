#include "Arduino.h"
#include "workstation_cfg.h"
#include "workstation_uv_light.h"
#include "workstation_relays.h"


workstation_uv_state_t workstation_uv_state;
int uv_timer;
int timer_begin_time;
bool uv_light_status;
bool uv_timer_status;


void WorkstationUvInit(void){
      Serial.println("Initializingrelays UV-Light"); 

      uv_timer=0;
      timer_begin_time=0;
      uv_light_status = false;
      uv_timer_status = false;

      workstation_uv_state=RGB_STATE_UV_IDLE;
}


void WorkstationUvMain(void){

    switch (workstation_uv_state)
    {
        case RGB_STATE_UV_IDLE:
            if(WorkstationRelaysReadRelay2Status()){
                    if(DEBUG_MODE){
                        Serial.println("Shutting down UV Light..."); 
                    }
                    WorkstationRelaysDeactivateRelay2();
                    uv_light_status = false;
                    timer_begin_time = 0;
                    uv_timer_status = false;
            }       
            /* code */
        break;
        case RGB_STATE_UV_ACTIVE:
            if(uv_light_status==false){
                uv_light_status = true;
                WorkstationRelaysActivateRelay2();
                if(DEBUG_MODE){
                            Serial.println("Turning On UV Light..."); 
                    }
            }
            if(uv_timer_status){
                if((millis()-timer_begin_time)>=(uv_timer*1000)){
                    workstation_uv_state=RGB_STATE_UV_IDLE;
                }
            }
        
        /* code */
        break;
    
    default:
        break;
    }
}


void WorkstationUvLightSwitchOn(){
    workstation_uv_state=RGB_STATE_UV_ACTIVE;
}

void WorkstationUvLightSwitchOff(){
    workstation_uv_state=RGB_STATE_UV_IDLE;
}


void WorkstationUvSetMainState(workstation_uv_state_t state){
    workstation_uv_state=state;
}

void WorkstationUvSetTimer(int time){
    timer_begin_time=millis();
    uv_timer = time;
    uv_timer_status=true;
}

bool WorkstationUVReadTimerStatus(){
    return uv_timer_status;
}

bool WorkstationUVReadUvLightStatus(){
    return uv_light_status;
}