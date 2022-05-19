#ifndef _SENSOR_H
#define _SENSOR_H

#include <Stream.h>
#include "sensor_interfaces.h"
#include "measurement.h"
#include "drivers.h"

class Sensor
{
  public:
    virtual Measurement read() = 0;
};

template <class T>
class SensorInterface : public Sensor
{
  protected:
      T* _sensor;
      
  public:
      SensorInterface(T* sensor) : _sensor(sensor)    {}
      virtual Measurement read() = 0;
};

#define NAN_CONSTANT        -9999.99
class Smoother
{
  private:
    uint8_t     BUFFER_SIZE;
    uint8_t     readIndex = 0;
    double*     readings;
    uint8_t*    status;
    double      sum = 0;
    uint8_t     readSt = 0;
    bool        full = false;
    void        zero_buffer() { 
      for(uint8_t i = 0; i < BUFFER_SIZE; i++) {
        readings[i] = 0; 
        status[i] = 0;
      }
    }
    void        reset_buffer(){ zero_buffer(); sum = 0; readIndex = 0;  full = false;     }
    Measurement invalidate_buffer();
    uint8_t     filter_status(uint8_t new_status);
    
  public:
    Smoother(uint8_t buff);
    ~Smoother() { free(readings);  };
    
    double getAve() ;
    Measurement smooth(double new_value, uint8_t new_status);
};

#define DEFAULT_BAUDRATE    9600
#define MAX_STR_SIZE        100

// /*
enum en_rs485_pinout{  
        MASTERPIN = 10, 
        RE_S1PIN = 3, DE_S1PIN = 2,
        RE_S2PIN = 5, DE_S2PIN = 4,
        RE_S3PIN = 7, DE_S3PIN = 6,
        RE_S4PIN = 9, DE_S4PIN = 8
        };

class HeatSensor  // Represents sensors that require, for heating time
{
  protected:
    bool        started       = false ;
    uint32_t    _secs_started = 0     ;
    uint32_t    SECS_RDY      = 1800  ;
  public:
    HeatSensor(uint32_t secs = 1800) : SECS_RDY(secs) {}
    void          startHeat   () { _secs_started = millis()/1000; started = true;                         }
    bool          heatStarted () { return started;                                                        }
    ReadingStatus heatState   () { return ((millis()/1000 -_secs_started) > SECS_RDY) ? READY : NOT_READY;}
};

class specDGS_sensor : public HeatSensor, SerialParser
{
private:
    serial_sensor*  _serial                 ;
    Stream*         _mySerial               ;
    String          _barcode                ;
    uint32_t        _id                     ;
    long int        dataArray[11]           ; //Multipurpose array
    
public:
    specDGS_sensor(serial_sensor* serial, Stream* mySerial)
      : HeatSensor()
    {
        if (serial    != nullptr) _serial   = serial  ;
        if (mySerial  != nullptr) _mySerial = mySerial;
    }
    specDGS_sensor(serial_sensor* serial, Stream* mySerial, uint32_t id, uint32_t secs = 3600)
      : HeatSensor(secs), _id(id)
    {
        if (serial  != nullptr) _serial   = serial  ;
        if (mySerial!= nullptr) _mySerial = mySerial;
    }
    specDGS_sensor(serial_sensor* serial, Stream* mySerial, const char barcode[], uint32_t id, uint32_t secs = 3600)
      : HeatSensor(secs), _id(id)
    {
        if (serial  != nullptr) _serial   = serial  ;
        if (mySerial!= nullptr) _mySerial = mySerial;
        if (barcode != nullptr) {
          _barcode = String(barcode);
        }
    }
    
    virtual uint16_t    parse_dataString(String str );
    uint8_t     write_barcode   ()  { return _serial->set_barcode(_barcode, _mySerial); }
    uint8_t     sleep           ();
    uint32_t    readSensor      (Smoother& conc, Smoother& temp, Smoother& hum);
};

#endif