#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include <WiFi.h>

typedef enum{
    WIFI_STATE_IDLE,
    WIFI_STATE_DISCONNECTED,
    WIFI_STATE_CONNECTED,
    WIFI_STATE_CONNECTING,
    WIFI_STATE_SCAN,
    WIFI_STATE_TIMEOUT
} workstation_wifi_state_t;



void WorkstationWifiScan(void);
void WorkstationWifiInit(void);

void WorkstationWifiMain(void);
bool WorkstationWifiGetConnectionStatus(void);

void WorkstationWifiGetIp(char* ip, int ip_length);
void WorkstationWifiGetSsid(char* ssid, int ssid_length);
void WorkstationWifiGetRssi(int* rssi);
#endif
