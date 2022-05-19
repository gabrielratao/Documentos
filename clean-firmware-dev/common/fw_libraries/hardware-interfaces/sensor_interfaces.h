#ifndef _SENSOR_INTERFACES_H
#define _SENSOR_INTERFACES_H

#include <Arduino.h>
#include "rs485-interface.h"

class SerialParser
{
  public:
    virtual uint16_t parse_dataString(String str ) = 0;
};

class serial_sensor
{
  public:
    static void     write_cmd           (char cmd, Stream* _serial)         ;
    static uint8_t  serial_available_wt (uint16_t msecs_wt, Stream* _serial);
    static uint8_t  clear_buffer        (Stream* _serial)                   ;

    virtual uint8_t getData             (SerialParser* _sensor, Stream* _mySerial)            = 0;
    virtual uint8_t set_continuous      (SerialParser* _sensor, Stream* _mySerial, bool con)  = 0;
    virtual uint8_t set_to_sleep        (SerialParser* _sensor, Stream* _mySerial)            = 0;
    virtual uint8_t set_barcode         (String barcode, Stream* _mySerial)                     = 0;
};

class specDGS_UART : public serial_sensor
{
  public:
    virtual uint8_t getData       (SerialParser* _sensor, Stream* _mySerial);
    virtual uint8_t set_continuous(SerialParser* _sensor, Stream* _mySerial, bool con);
    virtual uint8_t set_to_sleep  (SerialParser* _sensor, Stream* _mySerial);
    virtual uint8_t set_barcode   (String barcode, Stream* _mySerial);
};

class specDGS_RS485 : public serial_sensor
{
  private:
    int _dePin, _rePin;
    
    void beginTransmission()  { if (_dePin > -1)  digitalWrite(_dePin, HIGH); }
    void endTransmission  ()  { if (_dePin > -1)  digitalWrite(_dePin, LOW);  }
    void receive          ()  { if (_rePin > -1)  digitalWrite(_rePin, LOW);  }
    void noReceive        ()  { if (_rePin > -1)  digitalWrite(_rePin, HIGH); }
    
  public:
    
    specDGS_RS485(int dePin, int rePin);
    ~specDGS_RS485();

    virtual uint8_t getData       (SerialParser* _sensor, Stream* myRS485);
    virtual uint8_t set_continuous(SerialParser* _sensor, Stream* myRS485, bool continuous);
    virtual uint8_t set_to_sleep  (SerialParser* _sensor, Stream* myRS485);
    virtual uint8_t set_barcode   (String barcode, Stream* _mySerial);
};

class analog_sensor
{
protected:
    uint8_t inputPin;
    uint32_t read_raw() { return analogRead(inputPin);}
  
public:
    analog_sensor(uint8_t an_pin) : inputPin(an_pin)  {}
    double read_mV();
};

#endif