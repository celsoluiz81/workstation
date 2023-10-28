#ifndef WORKSTATION_RGB_H
#define WORKSTATION_RGB_H



typedef enum{
    RGB_STATE_IDLE,
    RGB_STATE_LIGHTSHOW,
    RGB_STATE_BACKLIGHT
} workstation_rgb_state_t;

struct rgbColor{
    char title[20]; 
    int red;
    int green;
    int blue;
};

void WorkstationRgbInit(void);
void WorkstationRgbMain(void);
void WorkstationRgbSetColor(int hexValue);
void WorkstationRgbSetMainState(workstation_rgb_state_t state);

#endif