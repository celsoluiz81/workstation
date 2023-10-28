#ifndef WORKSTATION_LCD_H
#define WORKSTATION_LCD_H

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif



typedef enum{
    LCD_STATE_IDLE,
    LCD_STATE_BOOT,
    LCD_STATE_MAIN_MENU,
    LCD_STATE_NETWORK,
    LCD_STATE_AMBIENCE,
    LCD_STATE_CONTAINER,
    LCD_STATE_WEATHER,
    LCD_STATE_GARDEN,
} workstation_lcd_state_t;



void WorkstationLcdMain(void);  
void WorkstationLcdInit (void);
void WorsktationLcdMoveCursorUp(void);
void WorsktationLcdMoveCursorDown(void);
void WorsktationLcdEnterButtonPress(void);
void WorsktationLcdResetButton(void);

 
#endif
