
#ifndef LCD_H
#define LCD_H

uint8_t  LcdWakeupDisplay(gpio_num_t lcd_power_pin);
uint8_t  LcdSleepDisplay(gpio_num_t lcd_power_pin);

void WorkstationLcdInit(void);
void WorkstationLcdSetup(void);

#endif /* LCD_H */
