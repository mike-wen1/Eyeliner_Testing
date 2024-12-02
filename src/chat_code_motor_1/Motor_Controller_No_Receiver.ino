#include <Wire.h>

void setup() {
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);

  analogWriteFrequency(22, 250);  // Start with 250 Hz, adjust as needed
  analogWriteResolution(12);

  // Provide some delay to allow everything to initialize
  delay(250);

  analogWrite(22, 0);
  delay(2000);  // Wait 2 seconds to ensure ESC arms at minimum throttle

  Serial.println("ESC armed and ready to ramp up throttle.");
}

void loop() {
  // Send low PWM signal to arm the ESC
  analogWrite(22, 0);
  delay(2000);  // Wait 2 seconds

  // Slowly ramp up throttle
  for (int pwm = 0; pwm <= 4095; pwm += 500) {
    analogWrite(22, pwm);
    Serial.print("Throttle: ");
    Serial.println(pwm);
    delay(500);
  }

  // Hold throttle for a bit
  analogWrite(22, 4095);
  delay(2000);

  //Reduce throttle again
  //TODO: make this work! 
  for (int pwm = 4095; pwm >= 0; pwm -= 500) {
    analogWrite(22, pwm);
    Serial.print("Throttle: ");
    Serial.println(pwm);
    delay(500);
  }

  // Return to zero throttle
  analogWrite(22, 0);
  delay(2000);

}
