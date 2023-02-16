idf_component_register(SRCS "workstation_lcd.c"
                       INCLUDE_DIRS "include"
                       REQUIRES mbedtls driver esp_lcd u8g2)
