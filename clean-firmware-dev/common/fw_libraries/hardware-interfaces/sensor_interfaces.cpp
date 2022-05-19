#include "sensor_interfaces.h"
#include "debug-output.h"

const uint16_t  MAX_MSECS_WT = 3000;

void serial_sensor::write_cmd(char cmd, Stream* _mySerial)
{
    _mySerial->flush();
    delay(500);
    while (_mySerial->available()) _mySerial->read();   // Cleans buffer
    _mySerial->write(cmd);
}

uint8_t serial_sensor::serial_available_wt(uint16_t msecs_wt, Stream* _mySerial)
{
    while (!_mySerial->available()) {
        delay(1);
        if((--msecs_wt) <= 0)   return 0;
    }
    return 1;
}

uint8_t serial_sensor::clear_buffer(Stream* _mySerial)
{
    uint8_t ret_value = 0;
    
    while(_mySerial->available())   ret_value += _mySerial->read();

    return ret_value;
}

uint8_t specDGS_UART::getData(SerialParser* _sensor, Stream* _mySerial)
{
    clear_buffer(_mySerial);
    write_cmd('\r', _mySerial);
    if(!serial_available_wt(MAX_MSECS_WT, _mySerial))  return 0;
    
    String str = _mySerial->readStringUntil('\n');
    
    return _sensor->parse_dataString(str);
}

uint8_t specDGS_UART::set_continuous(SerialParser* _sensor, Stream* _mySerial, bool continuous)
{
    clear_buffer(_mySerial);
    write_cmd('c', _mySerial);
    if(serial_available_wt(6000, _mySerial) != continuous) return 0;
    
    String str = _mySerial->readStringUntil('\n');
    
    return _sensor->parse_dataString(str);
}

uint8_t specDGS_UART::set_to_sleep(SerialParser* _sensor, Stream* _mySerial)
{
    clear_buffer(_mySerial);
    write_cmd('r', _mySerial);
    
    if(!serial_available_wt(10000, _mySerial))  return 0;
        
    return 1;
}

uint8_t specDGS_UART::set_barcode(String barcode, Stream* _mySerial)
{
    // print_debug(F("[specDGS_UART] set_barcode():"));

    clear_buffer(_mySerial);
    write_cmd('B', _mySerial);
    
    if(!serial_available_wt(MAX_MSECS_WT, _mySerial))  return 0;
        
    String str = _mySerial->readString();
    // print_debug(F("[specDGS_UART] set_barcode() - str: %s"), str.c_str());
    delay(10);
    
    if(str != F("\r\nRemove Sensor and Scan: "))   return 0;
    for (uint32_t i = 0; i < barcode.length() + 1; i++) {
      _mySerial->write(barcode.charAt(i));
      delay(10);
    }
    _mySerial->write('\r');
    // print_debug(F("[specDGS_UART] set_barcode(): Barcode sent!"));

    // if(!serial_available_wt(MAX_MSECS_WT, _mySerial))  return 0;
    // str = _mySerial->readStringUntil('\r');
    // str.setCharAt(str.length() - 1, '\r');
    // print_debug("[specDGS_UART] set_barcode() - str: %s", str.c_str());
    
    if(!serial_available_wt(MAX_MSECS_WT, _mySerial))  return 0;
    str = _mySerial->readString();
    // print_debug(F("[specDGS_UART] set_barcode() - str: %s"), str.c_str());
    
    if(str != F("\r\nSetting OC...done\r\nSetting zero...done\r\n"))   return 0;
    delay(2000);
    print_debug(F("[specDGS_UART] set_barcode(): Success!"));

    return 1;
}

specDGS_RS485::specDGS_RS485(int dePin, int rePin) 
    : _dePin(dePin), _rePin(rePin)
{
    pinMode(_dePin, OUTPUT);
    pinMode(_rePin, OUTPUT);
    endTransmission();
    noReceive();
}
    
specDGS_RS485::~specDGS_RS485()
{
    endTransmission();
    noReceive();
}

uint8_t specDGS_RS485::getData (SerialParser* _sensor, Stream* _stream)
{
    print_debug(F("[specDGS_RS485] getData():"));

    RS485Class* _myRS485 = static_cast<RS485Class*>(_stream);
    
    clear_buffer(_stream);
    receive();
    _myRS485->beginTransmission();
    delay(10);
    write_cmd('\r', _stream);
    delay(5);
    _myRS485->endTransmission();
    noReceive();
    
    _myRS485->receive();
    beginTransmission();
    uint8_t size = 0;
    String str;
    if(serial_available_wt(MAX_MSECS_WT, _stream))
    {
        str = _stream->readStringUntil('\n');
        size = _sensor->parse_dataString(str);
    }
    endTransmission();
    _myRS485->noReceive();
    
    print_debug(F("[specDGS_RS485] getData() - str: %s"), str.c_str());
    return size;
}

uint8_t specDGS_RS485::set_continuous (SerialParser* _sensor, Stream* _stream, bool continuous)
{
    RS485Class* _myRS485 = static_cast<RS485Class*>(_stream);
    
    clear_buffer(_stream);
    receive();
    _myRS485->beginTransmission();
    delay(10);
    write_cmd('c', _stream);
    delay(5);
    _myRS485->endTransmission();
    noReceive();

    _myRS485->receive();
    beginTransmission();
    String str;
    uint8_t size = 0;
    if(serial_available_wt(MAX_MSECS_WT, _stream) == continuous)
    {
        str = _stream->readStringUntil('\n');
        size = _sensor->parse_dataString(str);
    }
    
    endTransmission();
    _myRS485->noReceive();
    
    return size;
}

uint8_t specDGS_RS485::set_to_sleep (SerialParser* _sensor, Stream* _stream)
{
    RS485Class* _myRS485 = static_cast<RS485Class*>(_stream);
    
    clear_buffer(_stream);
    receive();
    _myRS485->beginTransmission();
    delay(5);
    write_cmd('r', _stream);
    delay(5);
    _myRS485->endTransmission();
    noReceive();

    _myRS485->receive();
    beginTransmission();
    delay(2);
    uint8_t size = 0;
    size = serial_available_wt(10000, _stream);
    endTransmission();
    _myRS485->noReceive();
    
    return size;
}

uint8_t specDGS_RS485::set_barcode(String barcode, Stream* _stream)
{
    // print_debug(F("[specDGS_RS485] set_barcode():"));

    uint8_t ret = 0;
    RS485Class* _myRS485 = static_cast<RS485Class*>(_stream);

    clear_buffer(_stream);
    receive();
    _myRS485->beginTransmission();
    delay(10);
    write_cmd('B', _stream);
    delay(5);
    _myRS485->endTransmission();
    noReceive();

    _myRS485->receive();
    beginTransmission();
    String str;
    if(serial_available_wt(MAX_MSECS_WT, _stream))
    {
        str = _stream->readString();
        // print_debug(F("[specDGS_RS485] set_barcode() - str: %s"), str.c_str());
        delay(10);
        
        if(str == F("\r\nRemove Sensor and Scan: "))
        {
            clear_buffer(_stream);
            receive();
            _myRS485->beginTransmission();
            delay(10);
            
            for (uint8_t i = 0; i < barcode.length() + 1; i++) {
                _stream->write(barcode.charAt(i));
                delay(10);
            }
            _stream->write('\r');
            delay(5);
            _myRS485->endTransmission();
            noReceive();
            
            _myRS485->receive();
            beginTransmission();
            // print_debug(F("[specDGS_RS485] set_barcode(): Barcode sent!"));

            if(serial_available_wt(MAX_MSECS_WT, _stream))
            {
                str = _stream->readStringUntil('\r');
                str.setCharAt(str.length() - 1, '\r');
                // print_debug(F("[specDGS_RS485] set_barcode() - str: %s"), str.c_str());
                
                if(serial_available_wt(MAX_MSECS_WT, _stream))
                {
                    str = _stream->readString();
                    // print_debug(F("[specDGS_RS485] set_barcode() - str: %s"), str.c_str());
                    
                    if(str != F("\r\nSetting OC...done\r\nSetting zero...done\r\n"))   ret = 1;
                    delay(2000);
                    // print_debug(F("[specDGS_RS485] set_barcode(): Success!"));
                }
            }
        }
    }
    endTransmission();
    _myRS485->noReceive();

    return ret;
}

#define AD_CONSTANT_MV 4.8828

double analog_sensor::read_mV()
{
    return AD_CONSTANT_MV*read_raw(); // 5/1024*1000 = 4.8828
}