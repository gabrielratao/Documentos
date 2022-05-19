#include <Arduino.h>

#include <Wire.h>
#include "DFRobot_SHT20.h"


DFRobot_SHT20 sht20;



void setup(){
Serial.begin(9600);
Serial.println("Exemplo SHT20");
sht20.initSHT20();
}

void loop(){

  float umd = sht20.readHumidity();
  float temp = sht20.readTemperature();
  
  Serial.print(" temperatura:");
  Serial.print(temp, 1); // mostra a temperatura com 1 número após o ponto
  Serial.print("C");
  
  Serial.print(" umidade:");
  Serial.print(umd, 1); // mostra a umidade com 1 número após o ponto
  Serial.print("%");
  Serial.println();

  delay(1000);
}