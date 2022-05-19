#ifndef _RS485_INTERFACE_H_
#define _RS485_INTERFACE_H_

#include <Arduino.h>

#ifdef PIN_SERIAL0_TX
#define RS485_DEFAULT_TX_PIN PIN_SERIAL0_TX
#else
#define RS485_DEFAULT_TX_PIN 1 
#endif

#ifdef PIN_SERIAL1_TX
#define RS485_DEFAULT_TX_PIN PIN_SERIAL1_TX
#else
#define RS485_DEFAULT_TX1_PIN 18
#endif

#ifdef PIN_SERIAL2_TX
#define RS485_DEFAULT_TX_PIN PIN_SERIAL2_TX
#else
#define RS485_DEFAULT_TX2_PIN 16
#endif

#ifdef PIN_SERIAL3_TX
#define RS485_DEFAULT_TX_PIN PIN_SERIAL3_TX
#else
#define RS485_DEFAULT_TX3_PIN 14 
#endif

#ifdef __AVR__
#define RS485_DEFAULT_DE_PIN 2
#define RS485_DEFAULT_RE_PIN -1
#elif ESP8266
#define RS485_DEFAULT_DE_PIN -1
#define RS485_DEFAULT_RE_PIN -1
#elif ESP32
#define RS485_DEFAULT_DE_PIN -1
#define RS485_DEFAULT_RE_PIN -1
#else
#define RS485_DEFAULT_DE_PIN A6
#define RS485_DEFAULT_RE_PIN A5
#endif

#if defined(__AVR__)
  typedef uint8_t SerialConfig;
#endif
class RS485Class : public Stream {
  public:
    RS485Class(HardwareSerial& hwSerial, int txPin, int dePin, int rePin);

    void begin(unsigned long baudrate);
    // void begin(unsigned long baudrate, uint8_t config);
    void begin(unsigned long baudrate, SerialConfig config);
    void end();
    int available();
    int peek();
    int read(void);
    void flush();
    size_t write(uint8_t b);
    using Print::write; // pull in write(str) and write(buf, size) from Print
    virtual operator bool();

    void beginTransmission();
    void endTransmission();
    void receive();
    void noReceive();

    void sendBreak(unsigned int duration);
    void sendBreakMicroseconds(unsigned int duration);

    void setPins(int txPin, int dePin, int rePin);

  private:
    HardwareSerial* _serial;
    int _txPin;
    int _dePin;
    int _rePin;

    bool _transmisionBegun;
    unsigned long _baudrate;
    SerialConfig _config;
};

extern RS485Class RS485     ;
#if defined(__AVR_ATmega2560__)
  extern RS485Class RS485_1   ;
  extern RS485Class RS485_2   ;
  extern RS485Class RS485_3   ;
#endif

#endif