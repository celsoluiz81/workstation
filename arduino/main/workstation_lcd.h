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

  U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18, /* data=*/ 23, /* cs=*/ 2, /* reset=*/ 4);
  
class WorkstationLcd {
  private:
  workstation_lcd_state_t lcdMainState = LCD_STATE_IDLE;
  void u8g2_prepare(void);
  public:
  WorkstationLcd(){
     
  }
  void WorkstationLcdInit (void);
 U8G2_ST7920_128X64_F_SW_SPI u8g2;
}; 

typedef enum{
    LCD_STATE_IDLE,
    LCD_STATE_BOOT,
    LCD_STATE_MAIN_MENU,
    LCD_STATE_NETWORK
} workstation_lcd_state_t;

 void WorkstationLcdWrite(uint posX, uint posY, char* message);

 
#endif
