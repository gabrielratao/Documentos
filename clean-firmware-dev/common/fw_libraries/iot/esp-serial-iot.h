#ifndef _ESP_SERIAL_IOT_H_
#define _ESP_SERIAL_IOT_H_

#ifdef ESP8266

#include <Arduino.h>
#include "esp-serial-types.h"

//------------------------------------------------------------------
/* SerialHandler */
//------------------------------------------------------------------
enum ESPSerialStatus {  ESP_ERROR = 0, WT_REQUEST = 1, WT_RESPONSE = 2 };
class ESPSerialHandler
{
    protected:
        ESPSerialStatus _status;
        String          _dataStr;
        
    public:
        CommandTypes parse(const String str);
        String get_data()                   {   return _dataStr;    }
        ESPSerialStatus get_status()        {   return _status;     }
        void set_status(ESPSerialStatus st) {   _status = st;       }
        void send_http_code(int code, Stream& _serial);
        void send_time(time_t time, Stream& _serial);
        void send_coordinates(String json_coord, Stream& _serial);
};
extern ESPSerialHandler espSerial;

#endif
#endif