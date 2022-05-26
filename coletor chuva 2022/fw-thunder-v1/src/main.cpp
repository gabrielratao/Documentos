#include <Arduino.h>

//sensor temperatura
#include <Adafruit_Sensor.h> //INCLUSÃO DE BIBLIOTECA
#include <Adafruit_BMP280.h> //INCLUSÃO DE BIBLIOTECA
 
Adafruit_BMP280 bmp; //OBJETO DO TIPO Adafruit_BMP280 (I2C)

//SHT20 
#include "DFRobot_SHT20.h"


DFRobot_SHT20 sht20;




//DHT
#include "DHT.h"

#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);


//RTC
#include <Wire.h> //INCLUSÃO DA BIBLIOTECA
#include "RTClib.h" //INCLUSÃO DA BIBLIOTECA
RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231


//cartão SD
#include <SD.h>
#include <SPI.h>

const int chipSelect = 7;
File dataFile;

//motor de passo
const int stepPin = 3;
const int dirPin = 4;
 

void setup () {
  
  Serial.begin(9600);
  Serial.println("Coletor de Chuva");
  Serial.println("Recipiente, Data, Horário, Umidade, Temperatura, Pressão");
  
  dht.begin();
  bmp.begin(0x76);
  rtc.begin();
  SD.begin(chipSelect);
  sht20.initSHT20();
  
  

  //motor
  pinMode (stepPin, OUTPUT);
  pinMode (dirPin, OUTPUT);

  digitalWrite(dirPin, LOW);
  
  //j numero de movimentos
  for(int j = 0; j < 25; j++){


    

    Serial.print(j + 1); //mostra o recipiente atual
    Serial.print(", ");
    
    dataFile = SD.open("chuva4.txt", FILE_WRITE);
    dataFile.print("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-");
    dataFile.print(j+1);
    dataFile.print(", ");
    
    
    //RTC
    DateTime now = rtc.now();
    Serial.print(now.day(), DEC); 
    Serial.print('/'); 
    Serial.print(now.month(), DEC); 
    Serial.print('/'); 
    Serial.print(now.year(), DEC); 
    
    Serial.print(", ");

    
    dataFile.print(now.day(), DEC); 
    dataFile.print('/');
    dataFile.print(now.month(), DEC);
    dataFile.print('/'); 
    dataFile.print(now.year(), DEC); 
    dataFile.print(", ");
    

    
    Serial.print(now.hour(), DEC); 
    Serial.print(':'); 
    Serial.print(now.minute(), DEC); 
    Serial.print(':'); 
    Serial.print(now.second(), DEC); 
    Serial.print(", ");

    
    dataFile.print(now.hour(), DEC); 
    dataFile.print(':');
    dataFile.print(now.minute(), DEC); 
    dataFile.print(':');
    dataFile.print(now.second(), DEC); 
    dataFile.print(", ");
    
    

    //sensores
    /*
    Serial.print(dht.readHumidity());
    Serial.print("%, ");
    Serial.print(bmp.readTemperature()); 
    Serial.print(" °C, ");

    Serial.print(bmp.readPressure()); 
    Serial.println(" Pa"); 

    
    dataFile.print(dht.readHumidity());
    dataFile.print("%, ");
    dataFile.print(bmp.readTemperature());
    dataFile.print(" °C, ");
    dataFile.print(bmp.readPressure());
    dataFile.println(" Pa"); 
    dataFile.close();
    */
     
    dataFile.print(sht20.readHumidity());
    dataFile.print("%, ");
    dataFile.print(sht20.readTemperature());
    dataFile.println("°C");
    Serial.print(sht20.readHumidity());
    Serial.print("%, ");
    Serial.print(sht20.readTemperature());
    Serial.println("°C");
    dataFile.close();
    
    //x tamanho do passo   (8 IDEAL)
    for(int x = 0; x < 8; x++) {
      digitalWrite(stepPin,HIGH);
      delayMicroseconds(5000);
      digitalWrite(stepPin,LOW);
      delayMicroseconds(5000);
      
    }
    

    delay(2000); // pausa entre os passos
  }
  
}

void loop() {

}