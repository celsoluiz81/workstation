#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H
#include "workstation_lcd.h"

void WorkstationWifiScan(void);
void WorkstationWifiSetup(const char*  ssid, const char*  password, WorkstationLcd lcd);

#endif
