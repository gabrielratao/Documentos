#include <Arduino.h>
#include <Wire.h>

//BMP280 Libraries
#include <SPI.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp; // I2C



//SCHT20 Libraries and defines
#include "DFRobot_SHT20.h"
DFRobot_SHT20 sht20;



void setup(){
  Serial.begin(9600);
  Serial.println("Testing MBP280 and SHT20\n");

  sht20.initSHT20();

  bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
}

void loop(){
  //SCH 20
  float umd = sht20.readHumidity();
  float temp = sht20.readTemperature();
  
  Serial.print("SHT20 data:\n");
  Serial.print("temperatura:");
  Serial.print(temp, 1); // mostra a temperatura com 1 número após o ponto
  Serial.print("C\n");
  
  Serial.print("umidade:");
  Serial.print(umd, 1); // mostra a umidade com 1 número após o ponto
  Serial.print("%");
  Serial.println();

  //BMP280
  Serial.print("\nBMP280 data\n");
  Serial.print(F("Temperature = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
  Serial.println(" m");

  Serial.println();
  delay(2000);



}