void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  establishContact();  // send a byte to establish contact until receiver responds
}

void loop() {
  while (Serial1.available() >= 0) {
    char receivedData = Serial1.read();   // read one byte from serial buffer and save to receivedData
    // Change the value according to the board you are programming
    // Receiver 1: '1' and '2'
    // Receiver 2: '3' and '4'
    // Receiver 3: '5' and '6'
    // Receiver 4: '7' and '8'
    if (receivedData == '1') {
      digitalWrite(LED_BUILTIN, HIGH); // switch LED On
    }
    else if (receivedData == '2') {
      digitalWrite(LED_BUILTIN, LOW);  // switch LED Off
    }
  }

}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}
