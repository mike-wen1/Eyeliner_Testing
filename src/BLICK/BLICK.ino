//default blink command 

void setup() {
  pinMode(13, OUTPUT); 
}

void loop() {
  digitalWrite(13, HIGH);
  delay(50); 
  digitalWrite(13, LOW);
  delay(50); 

}
