//TODO: FIX LOL
//this is current main driver I'm working on 

#include <Wire.h>
float RateRoll, RatePitch, RateYaw;
float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
int RateCalibrationNumber;
void gyro_signals(void) {
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission(); 
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x08);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission(); 
  Wire.requestFrom(0x68,6);
  int16_t GyroX=Wire.read()<<8 | Wire.read();
  int16_t GyroY=Wire.read()<<8 | Wire.read();
  int16_t GyroZ=Wire.read()<<8 | Wire.read();
  RateRoll=(float)GyroX/65.5;
  RatePitch=(float)GyroY/65.5;
  RateYaw=(float)GyroZ/65.5;
}
void setup() {
  //General 
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);

  //IMU
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  for (RateCalibrationNumber=0;
         RateCalibrationNumber<2000; 
         RateCalibrationNumber ++) {
    gyro_signals();
    RateCalibrationRoll+=RateRoll;
    RateCalibrationPitch+=RatePitch;
    RateCalibrationYaw+=RateYaw;
    delay(1);
  }
  RateCalibrationRoll/=2000;
  RateCalibrationPitch/=2000;
  RateCalibrationYaw/=2000;   

  //ESC - Left
  analogWriteFrequency(22, 250); 
  analogWriteResolution(12);
  delay(250);
  analogWrite(22, 0);
  delay(2000); 
  Serial.println("ESC armed and ready to ramp up throttle.");
}

void printHeadingsIMU() {
  Serial.print("Roll rate [°/s]= ");
  Serial.print(RateRoll); 
  Serial.print(" Pitch Rate [°/s]= ");
  Serial.print(RatePitch);
  Serial.print(" Yaw Rate [°/s]= ");
  Serial.println(RateYaw);
  delay(50);
}

void runLeftMotorLogic() {
  
}

void loop() {
  gyro_signals();
  RateRoll-=RateCalibrationRoll;
  RatePitch-=RateCalibrationPitch;
  RateYaw-=RateCalibrationYaw;
  printHeadingsIMU(); 
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
