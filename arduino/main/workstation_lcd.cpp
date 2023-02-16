#include <Arduino.h>
#include <U8g2lib.h>
#include "workstation_lcd.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18, /* data=*/ 23, /* cs=*/ 2, /* reset=*/ 4);


void WorkstationLcd::WorkstationLcdInit (void) {
 
  u8g2.begin();
  u8g2_prepare();
 
}

void WorkstationLcd::u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

void WorkstationLcdWrite(uint posX, uint posY, char* message){
  u8g2.drawStr( posX, posY, message);
  u8g2.sendBuffer();
}

void WorkstationLcdMain(void){

  
}
