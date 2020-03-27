#include <MPU6050_tockn.h>
#include <Wire.h>
#include <LiquidCrystal.h>
 
MPU6050 mpu6050(Wire);

const int RS = 2,
          EN = 3,
          D4 = 4,
          D5 = 5,
          D6 = 6,
          D7 = 7;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

double displacement; 

int forceSnsrPin = 15;
int forceSnsrReading;

unsigned long forceSnsrResistance;
unsigned long forceSnsrConductance;

long forceSnsrForce;

void setup()
{
  lcd.begin(16, 2);
 
  Wire.begin();
 
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  Serial.begin(9600);
}

void loop()
{
    mpu6050.update();
    
    getEMG();
    //getAngle();
    //getDisplacement();
    getImpactForce();

    delay(50);
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
      lcd.print(mpu6050.getAccY());
  
      lcd.setCursor(8, 0);
      lcd.print("Z:");
      lcd.print(mpu6050.getAccZ());
}

void getImpactForce()
{
  forceSnsrReading  = analogRead(forceSnsrPin);
  int forceSnsrVoltage = map(forceSnsrReading, 0, 1023, 0, 5000);
  
  forceSnsrResistance = 5000 - forceSnsrVoltage;     
  forceSnsrResistance *= 10000;                
  forceSnsrResistance /= forceSnsrVoltage;

  forceSnsrConductance = 1000000;           
  forceSnsrConductance /= forceSnsrResistance;

  if (forceSnsrConductance <= 1000) 
   {
      forceSnsrForce = forceSnsrConductance / 80;    
   }
  else 
   {
      forceSnsrForce = (forceSnsrConductance - 1000) / 30;          
   }
    
  Serial.print("Force: ");
  Serial.println(forceSnsrForce);
  Serial.println("N");
  
  lcd.setCursor(0, 1);
  lcd.print("Impact: ");
  lcd.print(forceSnsrForce);
  lcd.print("N");
}

void getDisplacement()
{
   double initY = mpu6050.getAngleY();
   double initZ = mpu6050.getAngleZ();

   double tempY = initY + 2;
   double tempZ = initZ + 2;

   double finY; 
   double finZ; 
    
   while (initY > tempY && initZ > tempZ)//never enters this loop.. why?
   {//while movement detected:
     Serial.println("==================================");
     Serial.println("MOVEMENT!");
     Serial.println("==================================");
     finY = mpu6050.getAngleY();
     finZ = mpu6050.getAngleZ();
    }
    
   double temp = sq(finY - initY) + sq(finZ - initY);
   displacement = sqrt(temp);
   Serial.print("Displacement: ");
   Serial.println(displacement);
   Serial.println("----------------------------");
   lcd.setCursor(0, 1);
   lcd.print("Dis: ");
   lcd.print(displacement);
   lcd.print("m");
}

void getEMG()
{
  float emgReading = analogRead(A1);
  float voltage = emgReading * (5.0 / 1023.0);

  Serial.println("==================================");
  Serial.print("EMG Voltage: ");
  Serial.println(voltage);
  Serial.println("==================================");
}
