#include "Arduino.h"
#include "workstation_keypad.h"
#include "workstation_cfg.h"
#include "workstation_lcd.h"

//void IRAM_ATTR keypadHandler(void);
workstation_keypad_button_t readButtonValue(uint16_t value);

// Timer: Auxiliary variables
unsigned long keypadCurrentTimestamp = millis();
unsigned long lastButtonPress = 0;
uint16_t buttonValue=0;
workstation_keypad_button_t previousPressedButton;




void WorkstationKeypadInit(void){
    Serial.println("Initializing Keypad"); 
 
  // PIR Motion Sensor mode INPUT_PULLUP
  //pinMode(WORKSTATION_KEYPAD_PIN_IN,INPUT_PULLUP );
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  //attachInterrupt(digitalPinToInterrupt(WORKSTATION_KEYPAD_PIN_IN), keypadHandler, FALLING);

  previousPressedButton=IDLE_BUTTON;
}

void WorkstationKeypadMain(void){
    workstation_keypad_button_t currentPressedButton=readButtonValue(buttonValue);
    
    if(previousPressedButton!=currentPressedButton){
        switch (currentPressedButton)
        {
        case LEFT_BUTTON:
            Serial.println("Left Button pressed!!!");
            break;
        case TOP_BUTTON:
            Serial.println("Top Button pressed!!!");
            WorsktationLcdMoveCursorUp();
        break;
        case BOTTOM_BUTTON:
            Serial.println("Bottom Button pressed!!!");
            WorsktationLcdMoveCursorDown();
        break;
        case RIGHT_BUTTON:
            Serial.println("Right Button pressed!!!");
        break;
        case ENTER_BUTTON:
            Serial.println("Enter Button pressed!!!");
            WorsktationLcdEnterButtonPress();
        break;
        default:
            Serial.println("Idle Button pressed!!!");
            WorsktationLcdResetButton();
            currentPressedButton=IDLE_BUTTON;
        break;
        }
        previousPressedButton = currentPressedButton;
    }

    
}  

// Checks if motion was detected, sets LED HIGH and starts a timer
/*
void IRAM_ATTR keypadHandler() {
    keypadCurrentTimestamp = millis();
if ((keypadCurrentTimestamp - lastButtonPress) > WORKSTATION_KEYPAD_KEY_PRESS_WAIT)
{
        buttonValue=analogRead(WORKSTATION_KEYPAD_PIN_IN);
        currentPressedButton=readButtonValue(buttonValue);
    
       lastButtonPress = millis();
}

}
*/
workstation_keypad_button_t readButtonValue(uint16_t value){
    keypadCurrentTimestamp = millis();
     
    
    if ((keypadCurrentTimestamp - lastButtonPress) > WORKSTATION_KEYPAD_KEY_PRESS_WAIT)
    {
        buttonValue=analogRead(WORKSTATION_KEYPAD_PIN_IN);
      
        if(buttonValue==LEFT_BUTTON_HIGH){
                return LEFT_BUTTON;
            }
            else if((buttonValue >= TOP_BUTTON_LOW) && (buttonValue <= TOP_BUTTON_HIGH)){
                return TOP_BUTTON;
            }
            else if((buttonValue >= BOTTOM_BUTTON_LOW) && (buttonValue <= BOTTOM_BUTTON_HIGH)){
                return BOTTOM_BUTTON;
            }
            else if((buttonValue >= RIGHT_BUTTON_LOW) && (buttonValue <= RIGHT_BUTTON_HIGH)){
                return RIGHT_BUTTON;
            }
            else if((buttonValue >= ENTER_BUTTON_LOW) && (buttonValue <= ENTER_BUTTON_HIGH)){
                return ENTER_BUTTON;
            }
            else{
                //Serial.println(buttonValue);
                return IDLE_BUTTON;
                
            }
       lastButtonPress = millis();
    }   

    return IDLE_BUTTON;
}

