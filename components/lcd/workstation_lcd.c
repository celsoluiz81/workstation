/*
  Docs:
  https://github.com/nkolban/esp32-snippets/tree/master/hardware/displays/U8G2
  https://github.com/olikraus/u8g2/wiki/Porting-to-new-MCU-platform#esp32-espidf
  https://github.com/olikraus/u8g2/wiki/u8g2reference#clearbuffer
  https://github.com/pcbreflux/espressif/blob/master/esp32/app/ESP32_GPS_Clock/main/oled_SSD1306.c
*/

#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "workstation_lcd.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "../../main/workstation_cfg.h"
#include "../u8g2/csrc/u8g2.h"
#include "../u8g2/csrc/u8g2_esp32_hal.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_err.h"


static const char *TAG = "LCD";
int bufferSent = 0;

u8g2_t u8g2; // a structure which will contain all the data for one display

#define LCD_HOST  SPI3_HOST
#define EXAMPLE_LCD_H_RES              248
#define EXAMPLE_LCD_V_RES              64
// Bit number used to represent command and parameter
#define EXAMPLE_LCD_CMD_BITS           8
#define EXAMPLE_LCD_PARAM_BITS         8

#define EXAMPLE_LVGL_TICK_PERIOD_MS    2



esp_lcd_panel_handle_t panel_handle = NULL;
static void lcd_task(void* arg);

uint8_t  LcdWakeupDisplay(gpio_num_t lcd_power_pin){
  return gpio_set_level(lcd_power_pin, 1);
}

uint8_t  LcdSleepDisplay(gpio_num_t lcd_power_pin){
  return gpio_set_level(lcd_power_pin, 0);
}

void WorkstationLcdInit(void){

/*
  u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;

	u8g2_esp32_hal.clk   = WORKSTATION_LCD_PIN_E;
	u8g2_esp32_hal.mosi  = WORKSTATION_LCD_PIN_RW;
  u8g2_esp32_hal.dc  = WORKSTATION_LCD_PIN_RW;
	u8g2_esp32_hal.cs    = WORKSTATION_LCD_PIN_RS;
	u8g2_esp32_hal.reset = WORKSTATION_LCD_PIN_RST;
  
	u8g2_esp32_hal_init(u8g2_esp32_hal);

  u8g2_Setup_st7920_128x64_f(
		&u8g2,
		U8G2_R0,
		u8g2_esp32_spi_byte_cb,
		u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure

  ESP_LOGI(TAG, "Initializing Display");
  u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,

  u8g2_SetPowerSave(&u8g2, 0); // wake up display
 */
  

    ESP_LOGI(TAG, "Turn off LCD backlight");
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << WORKSTATION_LCD_PIN_SWITCH
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    ESP_LOGI(TAG, "Initialize SPI bus");
   spi_bus_config_t buscfg = {
       .sclk_io_num = WORKSTATION_LCD_PIN_E,
       .mosi_io_num = WORKSTATION_LCD_PIN_RW,
       .miso_io_num = -1,
       .quadwp_io_num = -1,
       .quadhd_io_num = -1,
       .max_transfer_sz = EXAMPLE_LCD_H_RES * 80 * sizeof(uint16_t),
   };
   ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));
  ESP_LOGI(TAG, "Initializing LCD Task");

  ESP_LOGI(TAG, "Install panel IO");
   esp_lcd_panel_io_handle_t io_handle = NULL;
   esp_lcd_panel_io_spi_config_t io_config = {
       .dc_gpio_num = WORKSTATION_LCD_PIN_RW,
       .cs_gpio_num = WORKSTATION_LCD_PIN_RS,
       .pclk_hz = LCD_PIXEL_CLOCK_HZ,
       .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,
       .lcd_param_bits = EXAMPLE_LCD_PARAM_BITS,
       .spi_mode = 0,
       .trans_queue_depth = 10
   };

   // Attach the LCD to the SPI bus
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));


   esp_lcd_panel_dev_config_t panel_config = {
       .reset_gpio_num = WORKSTATION_LCD_PIN_RST,
       .bits_per_pixel = 16,
     };
     // Initialize the LCD configuration
    //ESP_ERROR_CHECK(u8x8_d_st7920_128x64(io_handle, &panel_config, &panel_handle));


     ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));


    // user can flush pre-defined pattern to the screen before we turn on the screen or backlight
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    
  xTaskCreate(lcd_task, "lcd_task", 4096, NULL, 10, NULL);
}

void WorkstationLcdSetup(void){
  ESP_LOGI(TAG, "Setting up Display");
  ESP_LOGI(TAG, "Turn on LCD backlight");

  ESP_ERROR_CHECK(gpio_set_direction(WORKSTATION_LCD_PIN_SWITCH,GPIO_MODE_OUTPUT)); 
 
  // Turn off backlight to avoid unpredictable display on the LCD screen while initializing
    // the LCD panel driver. (Different LCD screens may need different levels)
    ESP_ERROR_CHECK(gpio_set_level(WORKSTATION_LCD_PIN_SWITCH, 0));

    // Reset the display
    //ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));

    // Initialize LCD panel
    //ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    // Turn on backlight (Different LCD screens may need different levels)
    ESP_ERROR_CHECK(gpio_set_level(WORKSTATION_LCD_PIN_SWITCH, 1));

}

void WorkstationLcdMain(void){
  

  u8g2_ClearBuffer(&u8g2);
  u8g2_SetFont(&u8g2,u8g2_font_fur20_tr);
  u8g2_DrawStr(&u8g2,2,22,"Hello World!");
  u8g2_DrawFrame(&u8g2,3,7,25,15);
 
  u8g2_SendBuffer(&u8g2);
    
  
  ESP_LOGI(TAG, "Sent buffer");
}

static void lcd_task(void* arg){
  
  while (1) {
    WorkstationLcdMain();
    //vTaskDelay(10);
  }
}
