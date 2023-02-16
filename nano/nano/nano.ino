
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  establishContact();  // send a byte to establish contact until receiver responds
}

void loop() {
  int i=0;
  while (Serial.available() >= 0) {
    char receivedData = Serial.read();   // read one byte from serial buffer and save to receivedData
    // Change the value according to the board you are programming
    // Receiver 1: '1' and '2'
    // Receiver 2: '3' and '4'
    // Receiver 3: '5' and '6'
    // Receiver 4: '7' and '8'
    Serial.print(receivedData);
    if (receivedData == '1') {
      display.println(receivedData); 
      display.display();
      display.setCursor(0,i);
       if(i< 80){
       i++;
      }
    
      else{
        i=0;
      }
      digitalWrite(LED_BUILTIN, HIGH); // switch LED On
    }
    else if (receivedData == '2') {
      digitalWrite(LED_BUILTIN, LOW);  // switch LED Off
    }
   
  }

}

void establishContact() {
  
  while (Serial.available() <= 0) {
    display.setTextSize(1);    
    display.setCursor(0,0);
    display.setTextColor(WHITE);
    display.println("Connecting..."); 
    display.setTextSize(2);    
    display.display();
  }
}
