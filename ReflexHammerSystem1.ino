#include <MPU6050_tockn.h>
#include <Wire.h>
#include <LiquidCrystal.h>

MPU6050 mpu6050(Wire);

const int RS = 1,
          EN = 2,
          D4 = 4,
          D5 = 5,
          D6 = 6,
          D7 = 7;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

int forceSnsrPin = 15;
int forceSnsrReading;

void setup()
{
  lcd.begin(16, 2);
  
  Wire.begin();
  
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  //Serial.begin(9600);
}

void loop()
{
    mpu6050.update();

    getAngle();
    getImpactForce();

    delay(1000);
    lcd.clear();
}

void getAngle()
{
  //    Serial.println("=======================================================");
  //    Serial.print("accX : ");Serial.print(mpu6050.getAccX());
  //    Serial.print("  accY : ");Serial.print(mpu6050.getAccY());
  //    Serial.print("  accZ : ");Serial.println(mpu6050.getAccZ());
 
  //    Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
  //    Serial.print("  accAngleY : ");Serial.println(mpu6050.getAccAngleY());
  
  //    Serial.print("angleX : ");Serial.print(mpu6050.getAngleX());
  //    Serial.print("  angleY : ");Serial.print(mpu6050.getAngleY());
  //    Serial.print("  angleZ : ");Serial.println(mpu6050.getAngleZ());
  //    Serial.println("=======================================================");

      lcd.setCursor(0, 0);
      lcd.print("Y:");
      lcd.print(mpu6050.getAngleY());
  
      lcd.setCursor(8, 0);
      lcd.print("Z:");
      lcd.print(mpu6050.getAngleZ());
}

void getImpactForce()
{
  int temp  = analogRead(forceSnsrPin);
  forceSnsrReading = map(temp, 0, 1028, 0, 5000);
  Serial.print("Impact Voltage: ");
  Serial.println(forceSnsrReading);
  
  lcd.setCursor(0, 1);
  lcd.print("Impact: ");
  lcd.print(forceSnsrReading);
  lcd.print("mV");
  
}
