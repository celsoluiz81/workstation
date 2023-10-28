#include "workstation_cfg.h"
#include "workstation_lcd.h"
#include "workstation_wifi.h"
#include "workstation_rgb.h"
#include "workstation_relays.h"
#include "workstation_uv_light.h"

#include <GEM_u8g2.h>
#include <KeyDetector.h>

#define WORKSTATION_NETWORK_INFO_ICON_X_POS     115
#define WORKSTATION_NETWORK_INFO_ICON_Y_POS     0 


workstation_lcd_state_t lcdMainState;
bool wifi_connection;
bool system_booted = false;
byte joystick_status = GEM_KEY_NONE;
/* RGB Variables*/
bool backlight_status = false;
int current_rgb_color;
int blue = 0x0000FF;
int red =0xFF0000;
int green = 0x00FF00;
int light_show=0x280781;

int fifteen_min = 900;
int thirty_min = 1800;
int one_hour = 3600;
int two_hours = 7200;
int three_hours = 10800;
int manual=0;

/* UV-Light */
bool uv_light_switch_status=false;
int uv_light_timer;
/* Network Variables */
char workstation_lcd_ip[17]="000.000.000.000"; 
char workstation_lcd_ssid[17]="--";
int workstation_lcd_rssi=0;

void u8g2_prepare(void);
void WorkstationLcdDrawNetworkInfo(void);
void WorkstationLcdNetworkMenu(void);
void WorkstationLcdMainMenu(void);
void WorkstationLcdAmbienceMenu(void);
void WorktstationLcdSetState(GEMCallbackData callbackData);
void WorkstationLcdAmbienceBacklightSetColor(GEMCallbackData data);
void WorkstationLcdAmbienceBacklightSetStatus(GEMCallbackData data);
void WorkstationLcdAmbienceUvLightSetStatus(GEMCallbackData data);
void WorkstationLcdAmbienceUvLightSetTimer(GEMCallbackData data);

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ WORKSTATION_LCD_PIN_E, /* data=*/ WORKSTATION_LCD_PIN_RW, /* cs=*/ WORKSTATION_LCD_PIN_RS, /* reset=*/ WORKSTATION_LCD_PIN_RST);

SelectOptionInt rgb_options_array[] = {{"Blue", blue}, {"Green", green},{"Red", red},{"Light Show", light_show}};
GEMSelect ambienceBacklightColorSelect(sizeof(rgb_options_array)/sizeof(SelectOptionInt), rgb_options_array);

SelectOptionInt uv_options_array[] = {{"Off", -1},{"15 Min", fifteen_min}, {"30 Min", thirty_min},{"1 Hour", one_hour},{"2 Hours", two_hours}, {"3 Hours", three_hours}};
GEMSelect ambienceUvLightSelect(sizeof(uv_options_array)/sizeof(SelectOptionInt), uv_options_array);

//Page Definitions
GEMPage menuPageMain("Main Menu");
GEMPage menuPageNetwork("Network");
GEMPage menuPageAmbience("Ambience");
//GEMPage menuPageContainer("Container");
//GEMPage menuPageGarden("Garden");
//GEMPage menuPageWeather("Weather");
GEMPage menuPageAmbienceBacklight("Backlight");
GEMPage menuPageAmbienceUVLight("UV Light");
GEMPage menuPageAmbienceSpotLight("Spotlight");

// Menu Item Definitions
GEMItem menuItemAmbience("Ambience", menuPageAmbience);
//GEMItem menuItemContainer("Container", menuPageContainer);
//GEMItem menuItemGarden("Garden", menuPageGarden);
GEMItem menuItemNetwork("Network", menuPageNetwork);
//GEMItem menuItemWeather("Weather", menuPageWeather);


GEMItem menuItemAmbienceBacklight("Backlight", menuPageAmbienceBacklight);
GEMItem menuItemAmbienceBacklightColorSelect("Color:", current_rgb_color, ambienceBacklightColorSelect, &WorkstationLcdAmbienceBacklightSetColor);
GEMItem menuItemAmbienceBacklightSwitch("Switch:",backlight_status, &WorkstationLcdAmbienceBacklightSetStatus, backlight_status);

GEMItem menuItemAmbienceUvLightSwitch("Switch:",uv_light_switch_status, &WorkstationLcdAmbienceUvLightSetStatus, uv_light_switch_status);
GEMItem menuItemAmbienceUvLightTimerSelect("Timer:", uv_light_timer, ambienceUvLightSelect, &WorkstationLcdAmbienceUvLightSetTimer);

GEMItem menuItemAmbienceUVLight("UV Light", menuPageAmbienceUVLight);
GEMItem menuItemAmbienceSpotLight("Spotlight", menuPageAmbienceSpotLight);
GEMItem menuItemNetworkSsid("SSID:", workstation_lcd_ssid, GEM_READONLY);
GEMItem menuItemNetworkIp("IP:",workstation_lcd_ip,GEM_READONLY);
GEMItem menuItemNetworkIntRssi("Signal:", workstation_lcd_rssi,GEM_READONLY);


GEM_u8g2 menu(u8g2,GEM_POINTER_ROW, 7, 9, 8, 55);


void WorkstationLcdInit (void) {

  // Read Relay Status
  backlight_status = WorkstationRelaysReadRelay1Status();
  uv_light_switch_status = WorkstationUVReadUvLightStatus();

  // Turn on LCD
  pinMode(WORKSTATION_LCD_PIN_SWITCH, OUTPUT); 
  digitalWrite(WORKSTATION_LCD_PIN_SWITCH, HIGH);

  u8g2.begin();
  lcdMainState = LCD_STATE_IDLE;
  u8g2.clearBuffer();

  menu.setSplashDelay(SPLASH_SCREEN_DELAY);
  
  menu.init();
}

void WorkstationLcdMain(void){
  wifi_connection = WorkstationWifiGetConnectionStatus();
  uv_light_switch_status = WorkstationUVReadUvLightStatus();

    //If system already complete bootup, wait for key press
    if (menu.readyForKey()) {
      menu.registerKeyPress(joystick_status);
    }  
  switch(lcdMainState){
      case LCD_STATE_IDLE:
      {
        if(!system_booted){
          // If system has not boot up, run functions to beuild menus
          lcdMainState = LCD_STATE_BOOT;
        }
        else{   
        
        }
      }    
      break;
      case LCD_STATE_BOOT:    
        WorkstationLcdMainMenu();      
        WorkstationLcdAmbienceMenu();
        WorkstationLcdNetworkMenu();
        system_booted = true; //Boot up complete
        lcdMainState = LCD_STATE_MAIN_MENU;
      break;
      case LCD_STATE_MAIN_MENU:
        
        lcdMainState = LCD_STATE_IDLE;
      break;
      case LCD_STATE_NETWORK:
        WorkstationWifiGetIp(workstation_lcd_ip, GEM_STR_LEN);
        WorkstationWifiGetSsid(workstation_lcd_ssid,GEM_STR_LEN);
        WorkstationWifiGetRssi(&workstation_lcd_rssi);
        
        lcdMainState = LCD_STATE_IDLE;
      break;
       case LCD_STATE_AMBIENCE:
        backlight_status = WorkstationRelaysReadRelay1Status();

        if(!WorkstationUVReadTimerStatus()){
          uv_light_timer = -1;
        }

        lcdMainState = LCD_STATE_IDLE;
      break;
  }
  
}


void WorktstationLcdSetState(GEMCallbackData callbackData){
  lcdMainState= (workstation_lcd_state_t)callbackData.valInt;
}

void WorkstationLcdMainMenu() {
    menuPageMain.addMenuItem(menuItemAmbience);
   // menuPageMain.addMenuItem(menuItemContainer);
    //menuPageMain.addMenuItem(menuItemGarden);
    menuPageMain.addMenuItem(menuItemNetwork);
    //menuPageMain.addMenuItem(menuItemWeather);
    // Add menu page to menu and set it as current
    menu.setMenuPageCurrent(menuPageMain);
    menu.drawMenu();
}

/**
 * Builds the Ambience Menu
*/
void WorkstationLcdAmbienceMenu(void){
    menuPageAmbience.addMenuItem(menuItemAmbienceBacklight);
    menuPageAmbience.addMenuItem(menuItemAmbienceSpotLight);
    menuPageAmbience.addMenuItem(menuItemAmbienceUVLight); 
    menuPageAmbience.setParentMenuPage(menuPageMain);  

    
    
    menuPageAmbienceBacklight.addMenuItem(menuItemAmbienceBacklightSwitch);
    menuPageAmbienceBacklight.addMenuItem(menuItemAmbienceBacklightColorSelect);
    menuPageAmbienceBacklight.setParentMenuPage(menuPageAmbience);
    //Hide color select menu if backlight is off
    menuItemAmbienceBacklightColorSelect.hide(!backlight_status);

    menuPageAmbienceUVLight.addMenuItem(menuItemAmbienceUvLightSwitch);
    menuPageAmbienceUVLight.addMenuItem(menuItemAmbienceUvLightTimerSelect);
    menuPageAmbienceUVLight.setParentMenuPage(menuPageAmbience);
    menuItemAmbienceUvLightTimerSelect.hide(uv_light_switch_status);

  //menu.setMenuPageCurrent(menuPageAmbience);
  //menu.drawMenu();
}

/**
 * Builds the Network menu
*/
void WorkstationLcdNetworkMenu(void) {
    
    // Add menu items to menu page
    menuPageNetwork.addMenuItem(menuItemNetworkSsid);
    menuPageNetwork.addMenuItem(menuItemNetworkIp);
    menuPageNetwork.addMenuItem(menuItemNetworkIntRssi);

    menuPageNetwork.setParentMenuPage(menuPageMain); 

    // Add menu page to menu and set it as current
    //menu.setMenuPageCurrent(menuPageNetwork);
    //menu.drawMenu();
}

void WorkstationLcdDrawNetworkInfo(void){
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.setFontPosTop();
  if(wifi_connection){
      u8g2.drawGlyph(WORKSTATION_NETWORK_INFO_ICON_X_POS, WORKSTATION_NETWORK_INFO_ICON_Y_POS, 0x260E);
  }
  else{
      u8g2.drawGlyph(WORKSTATION_NETWORK_INFO_ICON_X_POS, WORKSTATION_NETWORK_INFO_ICON_Y_POS,0x2613);
  }
  
}

void WorsktationLcdMoveCursorUp(void){
  if(DEBUG_MODE){
    Serial.println("Menu cursor up.");
  }
  joystick_status = GEM_KEY_UP;
}

void WorsktationLcdMoveCursorDown(void){
   if(DEBUG_MODE){
    Serial.println("Menu cursor down.");
  }
  joystick_status = GEM_KEY_DOWN;
}

void WorsktationLcdEnterButtonPress(void){
   if(DEBUG_MODE){
    Serial.println("Menu ENTER button pressed.");
  }
  joystick_status = GEM_KEY_OK;
}

void WorsktationLcdResetButton(void){
  joystick_status = GEM_KEY_NONE;
}

void WorkstationLcdAmbienceBacklightSetColor(GEMCallbackData data){
  WorkstationRgbSetColor(current_rgb_color);
  if(DEBUG_MODE){
    Serial.print("Set current color:");
    Serial.println(current_rgb_color);  
  }
  
}

void WorkstationLcdAmbienceBacklightSetStatus(GEMCallbackData data){
  backlight_status != data.valBoolean;
  
  if(backlight_status){
    //Switch Relay 1 on
    WorkstationRelaysActivateRelay1();
    menuItemAmbienceBacklightColorSelect.show();   
  }
  else{
    //Deactivate Relay 1
    WorkstationRelaysDeactivateRelay1();
    menuItemAmbienceBacklightColorSelect.hide();
  }
  if(DEBUG_MODE){
    Serial.print("Set ambience Status:");
    Serial.println(backlight_status);  
  }
  
}

void WorkstationLcdAmbienceUvLightSetStatus(GEMCallbackData data){
  uv_light_switch_status != data.valBoolean;
  
  if(uv_light_switch_status){
    //Switch Relay 2 on
    WorkstationUvLightSwitchOn();
    menuItemAmbienceUvLightTimerSelect.show();   
  }
  else{
    //Deactivate Relay 1
    WorkstationUvLightSwitchOff();
    menuItemAmbienceUvLightTimerSelect.hide();
  }
  if(DEBUG_MODE){
    Serial.print("Set UV-Light Status:");
    Serial.println(uv_light_switch_status);  
  }
  
}

void WorkstationLcdAmbienceUvLightSetTimer(GEMCallbackData data){
    WorkstationUvSetTimer(uv_light_timer);
  if(DEBUG_MODE){
    Serial.print("Set UV Timer:");
    Serial.println(uv_light_timer);  
  }
}

