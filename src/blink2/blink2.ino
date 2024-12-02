//better blink command with wiring
//will be integrated into later files for testing purposes 

#include <Wire.h>

void setup() {
  pinMode(13, OUTPUT); 
  Serial.begin(9600); 
}

void loop() {
  delay(1000); 
  for (int i = 100; i < 6100; i+= 1000) {
    digitalWrite(13, HIGH);
    delay(i); 
    digitalWrite(13, LOW);
    delay(i);
 }


}
