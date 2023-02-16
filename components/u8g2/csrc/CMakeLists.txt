idf_component_register(SRCS "u8g2_esp32_hal.c" "workstation_uart.c"
                       INCLUDE_DIRS "include"
                       REQUIRES mbedtls driver)
