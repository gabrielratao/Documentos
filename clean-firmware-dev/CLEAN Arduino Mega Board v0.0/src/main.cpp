#include "debug-output.h"
#include "serial-internet-interface.h"
#include "drivers.h"
#include "hard-storage.h"
#include "variable.h"

bool DEBUG::debug = true;

unsigned long Device::id = 4;
devType_e Device::type = FIXDEV;

enum iotId_e    {   TEMPERATURE_ID      = 74,
                    OPC_TEMPERATURE_ID  = 76,
                    OPC_HUMIDITY_ID     = 77,
                    ALPHA_CO_ID         = 63, 
                    ALPHA_NO2_ID        = 64, 
                    ALPHA_OX_ID         = 65,
                    ALPHA_SO2_ID        = 66, 
                    ALPHA_H2S_ID        = 67, 
                    ALPHA_OX2_ID        = 68,
                    PRESSURE_ID         = 75,
                    PM10_ID             = 69, 
                    PM25_ID             = 70, 
                    PM01_ID             = 71,
                    EXT_TEMPERATURE_ID  = 72,
                    EXT_HUMIDITY_ID     = 73
                };

#include <Wire.h>
#include "DFRobot_SHT20.h"
DFRobot_SHT20 SHT20;
class SHT20TemperatureSensor: public SensorInterface<DFRobot_SHT20>
{
  public:
    SHT20TemperatureSensor(DFRobot_SHT20* sensor) : 
      SensorInterface<DFRobot_SHT20>(sensor) {}
    
    virtual Measurement read()
    {
      double value = _sensor->readTemperature();
      Serial.print("[SHT20TemperatureSensor] read(): ");
      Serial.println(value);
      return Measurement(_sensor->readTemperature(), READY);
    }
};
SHT20TemperatureSensor ExtTempSensor(&SHT20);

class SHT20HumiditySensor: public SensorInterface<DFRobot_SHT20>
{
  public:
    SHT20HumiditySensor(DFRobot_SHT20* sensor) : 
      SensorInterface<DFRobot_SHT20>(sensor) {}
    
    virtual Measurement read()
    {
      double value = _sensor->readHumidity();
      Serial.print("[SHT20HumiditySensor] read(): ");
      Serial.println(value);
      return Measurement(_sensor->readHumidity(), READY);
    }
};
SHT20HumiditySensor ExtHumSensor(&SHT20);

#include <SPI.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 BMP280; 

class BMP280_TemperatureSensor: public SensorInterface<Adafruit_BMP280>
{
  public:
    BMP280_TemperatureSensor(Adafruit_BMP280* sensor) : 
      SensorInterface<Adafruit_BMP280>(sensor) {}
    
    virtual Measurement read()
    {
      double value = _sensor->readTemperature();
      Serial.print("[BMP280_TemperatureSensor] read(): ");
      Serial.println(value);
      return Measurement(_sensor->readTemperature(), READY);
    }
};
BMP280_TemperatureSensor IntTempSensor(&BMP280);

class BMP280_PressureSensor: public SensorInterface<Adafruit_BMP280>
{
  public:
    BMP280_PressureSensor(Adafruit_BMP280* sensor) : 
      SensorInterface<Adafruit_BMP280>(sensor) {}
    
    virtual Measurement read()
    {
      float value = _sensor->readPressure();
      Serial.print("[BMP280_PressureSensor] read(): ");
      Serial.println(value);
      return Measurement(_sensor->readPressure(), READY);
    }
};
BMP280_PressureSensor IntPresSensor(&BMP280);

#define ALPHA_NO2_WPIN A1
#define ALPHA_SO2_WPIN A3
#define ALPHA_CO_WPIN  A5
#define ALPHA_OX_WPIN  A7
#define ALPHA_H2S_WPIN A9
#define ALPHA_OX2_WPIN A11
#define ALPHA_NO2_APIN A0
#define ALPHA_SO2_APIN A2
#define ALPHA_CO_APIN  A4
#define ALPHA_OX_APIN  A6
#define ALPHA_H2S_APIN A8
#define ALPHA_OX2_APIN A10

const uint8_t numAlpha = 6;
AlphaSenseISB Alpha_H2S(ALPHA_H2SB4_ID, H2SB, Sen_H2SB4_163480136, WEe_H2SB4_163480136, WAe_H2SB4_163480136, 
  WEo_H2SB4_163480136, WAo_H2SB4_163480136, ALPHA_H2S_WPIN, ALPHA_H2S_APIN);
AlphaSenseCompensator Alpha_NO2Comp((SensorInterface<DFRobot_SHT20>*)&ExtTempSensor, comp_algorithm_3, 
  ALPHA_NO2B43F_ID, NO2B, Sen_NO2B43F_202401512, WEe_NO2B43F_202401512, WAe_NO2B43F_202401512, 
  WEo_NO2B43F_202401512, WAo_NO2B43F_202401512, ALPHA_NO2_WPIN, ALPHA_NO2_APIN);
AlphaSenseCompensator Alpha_SO2Comp((SensorInterface<DFRobot_SHT20>*)&ExtTempSensor, comp_algorithm_2, 
      ALPHA_SO2B4_ID, SO2B, Sen_SO2B4_164702360, WEe_SO2B4_164702360, WAe_SO2B4_164702360,
      WEo_SO2B4_164702360, WAo_SO2B4_164702360, ALPHA_SO2_WPIN, ALPHA_SO2_APIN);
AlphaSenseCompensator Alpha_COComp((SensorInterface<DFRobot_SHT20>*)&ExtTempSensor, comp_algorithm_1, 
      ALPHA_COB4_ID, COB, Sen_COB4_162482658, WEe_COB4_162482658, WAe_COB4_162482658,
      WEo_COB4_162482658, WAo_COB4_162482658, ALPHA_CO_WPIN, ALPHA_CO_APIN);
AlphaSenseCompensator Alpha_O31Comp((SensorInterface<DFRobot_SHT20>*)&ExtTempSensor, comp_algorithm_3, 
      ALPHA_OXB431_ID1, OXB, Sen_OXB431_204660048, WEe_OXB431_204660048, WAe_OXB431_204660048, 
      WEo_OXB431_204660048, WAo_OXB431_204660048, ALPHA_OX_WPIN, ALPHA_OX_APIN);
AlphaSenseCompensator Alpha_O32Comp((SensorInterface<DFRobot_SHT20>*)&ExtTempSensor, comp_algorithm_1, 
      ALPHA_OXB431_ID2, OXB, Sen_OXB431_204660049, WEe_OXB431_204660049, WAe_OXB431_204660049, 
      WEo_OXB431_204660049, WAo_OXB431_204660049, ALPHA_OX2_WPIN, ALPHA_OX2_APIN);
// AlphaOXCompensator Alpha_O31Comp((SensorInterface<AlphaSenseCompensator>*)&Alpha_NO2Comp, 
//   (SensorInterface<DFRobot_SHT20>*)&ExtTempSensor, comp_algorithm_1, ALPHA_OXB431_ID1, OXB, 
//   Sen_OXB431_204660048, WEe_OXB431_204660048, WAe_OXB431_204660048, WEo_OXB431_204660048, 
//   WAo_OXB431_204660048, ALPHA_OX_WPIN, ALPHA_OX_APIN);
// AlphaOXCompensator Alpha_O32Comp((SensorInterface<AlphaSenseCompensator>*)&Alpha_NO2Comp, 
//   (SensorInterface<DFRobot_SHT20>*)&ExtTempSensor, comp_algorithm_1, ALPHA_OXB431_ID2, OXB, 
//   Sen_OXB431_204660049, WEe_OXB431_204660049, WAe_OXB431_204660049, WEo_OXB431_204660049, 
//   WAo_OXB431_204660049, ALPHA_OX2_WPIN, ALPHA_OX2_APIN);

#define CS_OPCN3_PIN 23
// OPCN3 alphaOPCN3(CS_OPCN3_PIN);
const uint8_t numAlphaOPCMP = 0;
const uint8_t numAlphaOPCTempRH = 0;

#define BUFFER_SIZE     6UL     // amostragem cada 5 segs, 5*12 = 60 segs, nova info cada 1 min, 
const uint8_t internalTempHumSensors = 2;
const uint8_t externalTempHumSensors = 2;
const uint8_t numSensors = internalTempHumSensors + externalTempHumSensors + 
  numAlpha + numAlphaOPCMP + numAlphaOPCTempRH;
const uint8_t numSensors_iot = numSensors;

Sensor* sensors[numSensors] = 
  {
    (SensorInterface<DFRobot_SHT20>*)&ExtTempSensor,
    (SensorInterface<DFRobot_SHT20>*)&ExtHumSensor,
    (SensorInterface<Adafruit_BMP280>*)&IntTempSensor,
    (SensorInterface<Adafruit_BMP280>*)&IntPresSensor,
    (SensorInterface<AlphaSenseISB>*)(new AlphaSenseISBSensor(&Alpha_H2S)),
    (SensorInterface<AlphaSenseCompensator>*)(new AlphaSenseCompensatorSensor(&Alpha_NO2Comp)),
    (SensorInterface<AlphaSenseCompensator>*)(new AlphaSenseCompensatorSensor(&Alpha_SO2Comp)),
    (SensorInterface<AlphaSenseCompensator>*)(new AlphaSenseCompensatorSensor(&Alpha_COComp)),
    (SensorInterface<AlphaSenseCompensator>*)(new AlphaSenseCompensatorSensor(&Alpha_O31Comp)),
    (SensorInterface<AlphaSenseCompensator>*)(new AlphaSenseCompensatorSensor(&Alpha_O32Comp))//,
    // (SensorInterface<OPCN3>*)(new AlphaSenseOPCPM10Sensor(&alphaOPCN3)),
    // (SensorInterface<OPCN3>*)(new AlphaSenseOPCPM2_5Sensor(&alphaOPCN3)),
    // (SensorInterface<OPCN3>*)(new AlphaSenseOPCPM1Sensor(&alphaOPCN3)),
    // (SensorInterface<OPCN3>*)(new AlphaSenseOPCTempSensor(&alphaOPCN3)),
    // (SensorInterface<OPCN3>*)(new AlphaSenseOPCHumSensor(&alphaOPCN3))
  };

Variable* Vars[numSensors] = 
  { 
    new Temperature     (EXT_TEMPERATURE_ID, SI_TEMP_Celsius, BUFFER_SIZE),
    new Humidity        (EXT_HUMIDITY_ID, SI_HUMD_Relative, BUFFER_SIZE),
    new Temperature     (TEMPERATURE_ID, SI_TEMP_Celsius, BUFFER_SIZE),
    new Pressure        (PRESSURE_ID, SI_PRES_Pascal, BUFFER_SIZE),
    new GasConcentration(ALPHA_H2S_ID, SI_CONC_ppb, BUFFER_SIZE),
    new GasConcentration(ALPHA_NO2_ID, SI_CONC_ppb, BUFFER_SIZE),
    new GasConcentration(ALPHA_SO2_ID, SI_CONC_ppb, BUFFER_SIZE),
    new GasConcentration(ALPHA_CO_ID,  SI_CONC_ppb, BUFFER_SIZE),
    new GasConcentration(ALPHA_OX_ID,  SI_CONC_ppb, BUFFER_SIZE),
    new GasConcentration(ALPHA_OX2_ID, SI_CONC_ppb, BUFFER_SIZE)//,
    // new GasConcentration(PM10_ID, SI_CONC_ug, BUFFER_SIZE),
    // new GasConcentration(PM25_ID, SI_CONC_ug, BUFFER_SIZE),
    // new GasConcentration(PM01_ID, SI_CONC_ug, BUFFER_SIZE),
    // new Temperature(TEMPERATURE_ID, SI_TEMP_Celsius, BUFFER_SIZE),
    // new Humidity(HUMIDITY_ID, SI_HUMD_Relative, BUFFER_SIZE)
  };

// TO-DO: Add OPC MP and GPS
SensorData data;

const char ISB_NO2  [] PROGMEM  = "ISB_NO2.csv\0" ;
const char ISB_SO2  [] PROGMEM  = "ISB_SO2.csv\0" ;
const char ISB_CO   [] PROGMEM  = "ISB_CO.csv\0"  ;
const char ISB_O3   [] PROGMEM  = "ISB_O3.csv\0"  ;
const char ISB_O32  [] PROGMEM  = "ISB_O32.csv\0" ;
const char ISB_H2S  [] PROGMEM  = "ISB_H2S.csv\0" ;
const char INT_TMP  [] PROGMEM  = "INT_TMP.csv\0" ;
const char INT_PRES [] PROGMEM  = "INT_PRES.csv\0";
const char EXT_TMP  [] PROGMEM  = "EXT_TMP.csv\0" ;
const char EXT_HUM  [] PROGMEM  = "EXT_HUM.csv\0" ;
const char OPC_PM10 [] PROGMEM  = "OPC_PM10.csv\0" ;
const char OPC_PM2_5[] PROGMEM  = "OPC_PM2_5.csv\0" ;
const char OPC_PM1  [] PROGMEM  = "OPC_PM1.csv\0" ;
const char OPCN3_TMP[] PROGMEM  = "OPCN3_TMP.csv\0" ;
const char OPCN3_HUM[] PROGMEM  = "OPCN3_HUM.csv\0" ;

const char* filenames[numSensors] = { EXT_TMP, EXT_HUM, INT_TMP, INT_PRES,
                                      ISB_H2S, ISB_NO2, ISB_SO2, ISB_CO, 
                                      ISB_O3, ISB_O32/*, OPC_PM10, OPC_PM2_5,
                                      OPC_PM1, OPCN3_TMP, OPCN3_HUM*/
                                    };

/*
            | 6       | 5-4 | 3-0 | 
device_St = | DEV_ON  | GPS | SPEC|
*/
#define DEV_ON_MSK  0X40
#define GPS_BIT_MSK 4UL

ESPSerialInterface espIoT(&Serial3);

const uint8_t IOT_TIME_SEC    = 1UL   ;
const uint8_t SAMPLE_TIME_SEC = 2UL   ;
const uint32_t uSD_TIME_MSEC  = BUFFER_SIZE*numSensors*1000UL ;
const uint32_t  HTTP_TIME_SEC = BUFFER_SIZE*numSensors        ; 

#include <RtcDS3231.h>
class RTCDS3231Interface : public RTCInterface<RtcDS3231<TwoWire>>
{
public:
  RTCDS3231Interface(RtcDS3231<TwoWire>* rtc) : 
    RTCInterface<RtcDS3231<TwoWire>>(rtc) {}
  
  virtual void set_time(unsigned long t)
  {
    RtcDateTime dt;
    dt.InitWithEpoch32Time(t);
    _rtc->SetDateTime(dt);
  }

  virtual unsigned long get_time() { return _rtc->GetDateTime().Epoch32Time(); }
};
#define I2C Wire
// RtcDS3231<TwoWire> Rtc(I2C);
// RTCDS3231Interface My_RTCInterface(&Rtc);
// time_t sync_time()  { return RTCDriver<RtcDS3231<TwoWire>>::sync_time_from_RTC(&My_RTCInterface); }


#include <TinyGPS++.h>
#define MSECS_GPSOUTDATE    7000UL
#define GPSBaud             9600UL

class TinyGPSSerialInterface : public GPSSerialInterface<TinyGPSPlus>
{
public:
    TinyGPSSerialInterface(TinyGPSPlus* gps, Stream* serial) : 
        GPSSerialInterface<TinyGPSPlus>(gps, serial) {}
  
    virtual bool read_gps(int msecs_wt)
    {
      while(!_serial->available())
      {
          delay(1);
          if(--msecs_wt <= 0) {   GPSDriver::set_gps_state(OUTDATE); return false; }
      }
      while (_serial->available())
      {
          delay(1);
          msecs_wt = (--msecs_wt <= 0) ? 0 : msecs_wt;
          _gps->encode(_serial->read());
      }
      if(msecs_wt <= 0 && _gps->charsProcessed() < 10)
      {   GPSDriver::set_gps_state(OUTDATE); return false; }
          
      if(!_gps->location.isValid())
      {
          if (msecs_wt <= 0)  {   GPSDriver::set_gps_state(OUTDATE); return false; }
          return read_gps(msecs_wt);
      }
      print_debug("[GPSControl] readGPS(): valid data!");
          
      if (_gps->location.age() > MSECS_GPSOUTDATE || _gps->date.age() > MSECS_GPSOUTDATE)
              GPSDriver::set_gps_state(OUTDATE);
      else    GPSDriver::set_gps_state(OK);
      
      GPSDriver::set_coordinates(true, _gps->location.lat(), _gps->location.lng(), _gps->altitude.meters());
      time_t t = mk_timestamp(_gps->date.year(), _gps->date.month(), _gps->date.day(), 
          _gps->time.hour(), _gps->time.minute(), _gps->time.second());
      TimeDriver::set_time(t);
      
      return true;
    }
};
// TinyGPSSerialInterface gps(new TinyGPSPlus(), &Serial1);


void setup()
{  
  Serial.begin(9600);
  Serial1.begin(GPSBaud);
  Serial3.begin(9600UL);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  espIoT.restart();

  SD.begin(CHIPSEL_PIN);
  
  // Rtc.Begin();
  // if (!Rtc.GetIsRunning())
  // {
  //   print_debug(F("WARNING: RTC was not actively running, starting it now."));
  //   Rtc.SetIsRunning(true);
  // }
  // #ifdef DS3231
  //   // Reset the DS3231 RTC status in case it was wrongly configured
  //   Rtc.Enable32kHzPin(false);
  //   Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
  // #endif
  // time_t now = Rtc.GetDateTime().Epoch32Time();
  // print_debug(F("Time is: %lu\n"), now);
  // setSyncProvider(sync_time);
  // setSyncInterval(5*SECS_PER_MIN);
  print_debug(F("timestamp: %lu"), now);
  SHT20.initSHT20();
  BMP280.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  // alphaOPCN3.initialize();
}

void loop()
{
  static uint32_t mLastTime = 0, mLastTimeAlpha = 0, mLastTimeGPS = 0;
  static uint32_t mLastTimeuSD = 0, mLastTimeHTTP = 0;
  static bool sd_ok = false;

  espIoT.watch_dog();
  
  if((!TimeDriver::_already_up_to_date()))  espIoT.request_time();
  // if((!My_RTCInterface.is_up_to_date()))    
  //   RTCDriver<RtcDS3231<TwoWire>>::update_rtc(&My_RTCInterface, now());
  
  // /*
  if ((millis() - mLastTime) >= SAMPLE_TIME_SEC*1000)
  {
    mLastTime = millis();
    static uint8_t index  = 0;
    print_debug("[MAIN] SAMPLE No.: %d", index);
    Vars[index]->smooth(sensors[index]->read());
    index = (index >= numSensors - 1) ? 0 : index + 1;
  }
  // */
  
  // /*
  if((millis() - mLastTimeuSD) >= uSD_TIME_MSEC)  // Counts for sample time
  {
    print_debug("[MAIN] SD");
    mLastTimeuSD = millis();
    // blinkSD = true;
    static uint8_t data_index_uSD = 0;
    Vars[data_index_uSD]->sense(&data);
    
    char* filename = (char*)malloc(strlen_P(filenames[data_index_uSD])+1);
    strcpy_P(filename, filenames[data_index_uSD]);
    if(open_file(filename)) 
      sd_ok = !save_to_file(&data, filename);
    else  SD.begin(CHIPSEL_PIN);
    free(filename);
  
    data_index_uSD = (data_index_uSD >= numSensors-1) ? 0 : data_index_uSD + 1;
  }
  // */
  
  // /*
  if((millis() - mLastTimeHTTP) >= HTTP_TIME_SEC*1000)  // Counts for sample time
  {
    print_debug("[MAIN] HTTP");
    mLastTimeHTTP = millis();

    static uint8_t data_index_iot = 0;
    if(!sd_ok)
    {
      // String s;
      // data[data_index_iot]->toJSON(s);
      Vars[data_index_iot]->sense(&data);
      if(!espIoT.send_http_post(&data))  print_debug("Couldn't post!");
      data_index_iot = (data_index_iot >= numSensors-1) ? 0 : data_index_iot + 1;
    }
  }
  // */
  
  /*
  if((millis() - mLastTimeGPS) >= MSECS_GPSOUTDATE)  // Counts for sample time
  {
    print_debug("[MAIN] GPS");
    mLastTimeGPS = millis();
    gps.read_gps(MSECS_GPSOUTDATE/2);
  }
  // */
}

void serialEvent3()
{
  if(Serial3.available())
  {
    print_debug("[MAIN] ESP Serial received");
    espIoT.parse_esp_string(Serial3.readStringUntil(';'));
  }
}