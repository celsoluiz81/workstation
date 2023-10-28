#include "workstation_wifi.h"
#include "workstation_cfg.h"

workstation_wifi_state_t wifiMainState;
char workstation_ip;
bool workstation_connected;
int  start_time;
bool workstation_wifi_boot;

const char* ssid = "AppleTalk";
const char* password = "Benicio08092017!";
String hostname = "Periscope (ESP32)";

void WorkstationWifiConnected(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiStationGotIp(WiFiEvent_t event, WiFiEventInfo_t info);

void WorkstationWifiInit(void)
{
    WiFi.onEvent(WorkstationWifiConnected, ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFiStationGotIp, ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    Serial.println("Setup done");
    wifiMainState=WIFI_STATE_IDLE;
}


void WorkstationWifiMain(void){

  int now;
  
  switch(wifiMainState){
    case WIFI_STATE_IDLE:
      
      if((workstation_wifi_boot==false) && (WORKSTATION_WIFI_AUTOCONNECT)){
        wifiMainState = WIFI_STATE_CONNECT;
      }
      if(DEBUG_MODE){
        Serial.println("Wifi: Idle");
      }

    break;
    case WIFI_STATE_CONNECT:
      if(WiFi.status() == WL_CONNECTED){
        wifiMainState=WIFI_STATE_CONNECTED;
      }
      else{
        WiFi.mode(WIFI_STA); //Optional
        WiFi.disconnect();
        delay(100);
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
        WiFi.setHostname(hostname.c_str()); //define hostname
        WiFi.begin(ssid, password);
        start_time = millis();
        wifiMainState=WIFI_STATE_CONNECTING;
      }
    break;
    case WIFI_STATE_DISCONNECTED:
       if(DEBUG_MODE){
        Serial.println("Wifi disconnected.");
      }
      if((workstation_wifi_boot==true) && (WORKSTATION_WIFI_AUTOCONNECT)){
        wifiMainState = WIFI_STATE_CONNECT;
        Serial.println("Attempting to reconnect...");
      }
    break;
    case WIFI_STATE_CONNECTED:
      if(WiFi.status() != WL_CONNECTED){
        wifiMainState=WIFI_STATE_DISCONNECTED;
      }
    break;
    case WIFI_STATE_CONNECTING:
        if(WiFi.status() == WL_CONNECTED){
          wifiMainState=WIFI_STATE_CONNECTED;
        }
        else{
          Serial.print(".");
          // Current time
          now = millis();
          if(now-start_time>=WIFI_CONNECT_TIMEOUT){
            wifiMainState=WIFI_STATE_TIMEOUT;
          }
        }   
    break;
    case WIFI_STATE_SCAN:

    break;
    case WIFI_STATE_TIMEOUT:
      Serial.println("Wifi connect timed out!");
      if((workstation_wifi_boot==true) && (WORKSTATION_WIFI_AUTOCONNECT)){
        wifiMainState = WIFI_STATE_CONNECT;
        Serial.println("Attempting to reconnect...");
      }
      else{
        wifiMainState=WIFI_STATE_IDLE;
      }
      
    break;
  }
  
}

bool WorkstationWifiGetConnectionStatus(void){
  return workstation_connected;
}

void WorkstationWifiConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  wifiMainState=WIFI_STATE_CONNECTED;
  workstation_connected = true; 
  Serial.println("\nConnected to the WiFi network"); 
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  //Serial.println(info.system_event_sta_disconnected_t.reason);
  wifiMainState=WIFI_STATE_DISCONNECTED;
  workstation_connected = false; 
}

void WiFiStationGotIp(WiFiEvent_t event, WiFiEventInfo_t info){
    workstation_ip = WiFi.localIP(); 
    Serial.println("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}


void WorkstationWifiScan(void)
{
    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
        }
    }
    Serial.println("");

    // Wait a bit before scanning again
    delay(5000);
}

void WorkstationWifiGetIp(char* ip, int ip_length){
  memcpy(ip,WiFi.localIP().toString().c_str(),ip_length);
}

void WorkstationWifiGetSsid(char* ssid, int ssid_length){
  memcpy(ssid,WiFi.SSID().c_str(),ssid_length);
}

void WorkstationWifiGetRssi(int* rssi){
  *rssi = WiFi.RSSI();
 
}
