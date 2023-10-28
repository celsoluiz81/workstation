#ifndef WORKSTATION_UV_LIGHT_H
#define WORKSTATION_UV_LIGHT_H



typedef enum{
    RGB_STATE_UV_IDLE,
    RGB_STATE_UV_ACTIVE,
} workstation_uv_state_t;


void WorkstationUvInit(void);
void WorkstationUvMain(void);
void WorkstationUvLightSwitchOn();
void WorkstationUvLightSwitchOff();
void WorkstationUvSetMainState(workstation_uv_state_t state);
void WorkstationUvSetTimer(int time);
bool WorkstationUVReadTimerStatus(void);
bool WorkstationUVReadUvLightStatus(void);
#endif