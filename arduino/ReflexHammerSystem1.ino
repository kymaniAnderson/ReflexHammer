#include <Wire.h>
#include "ds3231.h"

int displacement;
long forceSnsrForce;
float emgVoltage;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
 
  DS3231_init(DS3231_CONTROL_INTCN);

  tableHead();
 }

void loop()
{
  getImpactForce();
  getEMG();
  getDisplacement();
  tableBody();

  delay(350);
}

void tableHead()
{
  Serial.print("Time (HH/MM/SS)");
  Serial.print("\t\t");
  Serial.print("EMG Voltage (V)");
  Serial.print("\t\t");
  Serial.print("Impact (N)");
  Serial.print("\t");
  Serial.print("Displacement (cm)");
  Serial.println();
}

void tableBody()
{
  struct ts t; 
  DS3231_get(&t);
  
  //Time (HH/MM/SS)
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min); 
  Serial.print(":");
  Serial.print(t.sec);
  Serial.print("\t\t\t");
  
  //Voltage (V)
  Serial.print(emgVoltage);
  Serial.print("\t\t\t");
  
  //Impact (N)
  Serial.print(forceSnsrForce);
  Serial.print("\t\t");
   
  //Displacement (cm)
  Serial.print(displacement); //manually taken
  Serial.println();
}

void getImpactForce()
{
  int forceSnsrReading  = analogRead(A0);
  int forceSnsrVoltage = map(forceSnsrReading, 0, 1023, 0, 5000);
  
  unsigned long forceSnsrResistance = 5000 - forceSnsrVoltage;     
  forceSnsrResistance *= 10000;                
  forceSnsrResistance /= forceSnsrVoltage;

  unsigned long forceSnsrConductance = 1000000;           
  forceSnsrConductance /= forceSnsrResistance;

  if (forceSnsrConductance <= 1000) 
   {
      forceSnsrForce = forceSnsrConductance / 80;    
   }
  else 
   {
      forceSnsrForce = (forceSnsrConductance - 1000) / 30;          
   }
}

void getEMG()
{
  float emgReading = analogRead(A1);
  emgVoltage = emgReading * (5.0 / 1023.0);
}

void getDisplacement()
{
  int dispReading = analogRead(A2);
  displacement = map(dispReading,0,1023,5,35);
}
