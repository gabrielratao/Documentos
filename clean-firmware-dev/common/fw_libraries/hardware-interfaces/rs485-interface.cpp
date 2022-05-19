#include "rs485-interface.h"

RS485Class::RS485Class(HardwareSerial& hwSerial, int txPin, int dePin, int rePin) :
  _serial(&hwSerial),
  _txPin(txPin),
  _dePin(dePin),
  _rePin(rePin),
  _transmisionBegun(false)
{
}

void RS485Class::begin(unsigned long baudrate)
{
  begin(baudrate, SERIAL_8N1);
}

void RS485Class::begin(unsigned long baudrate, SerialConfig config)
{
  _baudrate = baudrate;
  _config = config;

  if (_dePin > -1) {
    pinMode(_dePin, OUTPUT);
    digitalWrite(_dePin, LOW);
  }

  if (_rePin > -1) {
    pinMode(_rePin, OUTPUT);
    digitalWrite(_rePin, HIGH);
  }

  _transmisionBegun = false;

  _serial->begin(baudrate, config);
}

void RS485Class::end()
{
  _serial->end();

  if (_rePin > -1) {
    digitalWrite(_rePin, LOW);
    pinMode(_dePin, INPUT);
  }
  
  if (_dePin > -1) {
    digitalWrite(_dePin, LOW);
    pinMode(_rePin, INPUT);
  }
}

int RS485Class::available()
{
  return _serial->available();
}

int RS485Class::peek()
{
  return _serial->peek();
}

int RS485Class::read(void)
{
  return _serial->read();
}

void RS485Class::flush()
{
  return _serial->flush();
}

size_t RS485Class::write(uint8_t b)
{
  if (!_transmisionBegun) {
    setWriteError();
    return 0;
  }

  return _serial->write(b);
}

RS485Class::operator bool()
{
  return true;
}

void RS485Class::beginTransmission()
{
  if (_dePin > -1) {
    digitalWrite(_dePin, HIGH);
    delayMicroseconds(50);
  }

  _transmisionBegun = true;
}

void RS485Class::endTransmission()
{
  _serial->flush();

  if (_dePin > -1) {
    delayMicroseconds(50);
    digitalWrite(_dePin, LOW);
  }

  _transmisionBegun = false;
}

void RS485Class::receive()
{
  if (_rePin > -1) {
    digitalWrite(_rePin, LOW);
  }
}

void RS485Class::noReceive()
{
  if (_rePin > -1) {
    digitalWrite(_rePin, HIGH);
  }
}

void RS485Class::sendBreak(unsigned int duration)
{
  _serial->flush();
  _serial->end();
  pinMode(_txPin, OUTPUT);
  digitalWrite(_txPin, LOW);
  delay(duration);
  _serial->begin(_baudrate, _config);
}

void RS485Class::sendBreakMicroseconds(unsigned int duration)
{
  _serial->flush();
  _serial->end();
  pinMode(_txPin, OUTPUT);
  digitalWrite(_txPin, LOW);
  delayMicroseconds(duration);
  _serial->begin(_baudrate, _config);
}

void RS485Class::setPins(int txPin, int dePin, int rePin)
{
  _txPin = txPin;
  _dePin = dePin;
  _rePin = rePin;
}

RS485Class RS485    (SERIAL_PORT_HARDWARE   , RS485_DEFAULT_TX_PIN, RS485_DEFAULT_DE_PIN, RS485_DEFAULT_RE_PIN);
#if defined(__AVR_ATmega2560__)
  RS485Class RS485_1  (SERIAL_PORT_HARDWARE1  , RS485_DEFAULT_TX1_PIN, RS485_DEFAULT_DE_PIN, RS485_DEFAULT_RE_PIN);
  RS485Class RS485_2  (SERIAL_PORT_HARDWARE2  , RS485_DEFAULT_TX2_PIN, RS485_DEFAULT_DE_PIN, RS485_DEFAULT_RE_PIN);
  RS485Class RS485_3  (SERIAL_PORT_HARDWARE3  , RS485_DEFAULT_TX3_PIN, RS485_DEFAULT_DE_PIN, RS485_DEFAULT_RE_PIN);
#endif