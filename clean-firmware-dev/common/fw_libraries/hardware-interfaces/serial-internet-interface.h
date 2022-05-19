#ifndef _SERIAL_INTERNET_INTERFACE_H_
#define _SERIAL_INTERNET_INTERFACE_H_

#include <Arduino.h>
#include "esp-serial-types.h"
#include "drivers.h"

class SerialInternetInterface
{
    protected:
        Stream*     _serial;
        bool        _serialFree = true;
    
    public:
        SerialInternetInterface(Stream* serial) : _serial(serial) {}
        bool is_free()           {   return _serialFree; }
        void set_busy(bool b)    {   _serialFree = !b;   }
        bool parse_response(String response);
};

class ESPSerialInterface : public SerialInternetInterface
{
    #define DEFAULT_ESP_RESET_PIN 12UL
    protected:
        uint8_t _reset_pin;
        enum ESPState { ESP_OFF = 0, ESP_STARTED, ESP_READY}
            _espState = ESP_OFF;

        #define DEFAULT_TIMEOUT_MS  240000UL
        #define HTTP_TIMEOUT_MS     300000UL
        #define BIG_TIMEOUT_MS      600000UL
        struct WatchDog
        {
            uint32_t _timeSinceLastOp_ms = DEFAULT_TIMEOUT_MS;
            uint32_t MAX_TIMEOUT_MS = DEFAULT_TIMEOUT_MS;
            bool timeout()  {   return ((millis() - _timeSinceLastOp_ms) >= MAX_TIMEOUT_MS);   }
            void set_time_last_op(uint32_t t_op) {   
                _timeSinceLastOp_ms = millis();                 
                MAX_TIMEOUT_MS = t_op;
            }
            void reset()    {   _timeSinceLastOp_ms = millis();   }
        }_watchDog;

    public:
        ESPSerialInterface(Stream* serial, uint8_t respin = DEFAULT_ESP_RESET_PIN) 
            : SerialInternetInterface(serial), _reset_pin(respin)  
        {   
            pinMode(_reset_pin, OUTPUT); 
            digitalWrite(_reset_pin, HIGH);
        }

        void restart()
        {
            digitalWrite(_reset_pin, LOW);
            delay(100);
            digitalWrite(_reset_pin, HIGH);
            _espState = ESP_OFF;
            _watchDog.set_time_last_op(DEFAULT_TIMEOUT_MS);
        }
        
        void watch_dog()    {   if(_watchDog.timeout()) restart();    }

        bool parse_esp_string(String str);
        bool request_time();
        bool request_coordinates();
        bool send_http_post(DataContainer* data);
        // bool get_time(time_t &t);
        // bool get_http_code(int &code);
        // bool get_coordinates(int &lat, int &long);
};

#endif