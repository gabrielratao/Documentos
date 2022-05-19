#include "serial-internet-interface.h"
#include "debug-output.h"
#include <ArduinoJson.h>

const size_t json_capacity  = JSON_OBJECT_SIZE  (2)
                            + JSON_OBJECT_SIZE  (5)
                            + 5*JSON_OBJECT_SIZE(1);

StaticJsonDocument<json_capacity> doc;

/*
    JSON from ESP
    {
        'type': [ERROR], [DATA], [REQUESTTIME], [REQUESTCOORDINATES] -> [0], [1], [2], [3]
        'body':
            [CODE]:
                {
                    code
                }
            [TIME]:
                {
                    time
                }
            [COORDINATES]:
                {
                    'latitude': latitude,
                    'longitude': longitude
                }
    }
*/

bool SerialInternetInterface::parse_response(String response)
{
    print_debug(F("[ESPSerialInterface] parse_response: %s"), response.c_str());
    DeserializationError error = deserializeJson(doc, response.c_str());
    // Test if parsing succeeds.
    if (error)  return false;
    
    uint8_t type = doc["type"];
    switch ((CommandTypes)type)
    {
        case ERROR: return false;
        
        case DATA:
        {
            /*
            if(!_http_response._cmd_sent) return false;
            _http_response._cmd_sent = false;
            _http_response._responseOK = true;
            _http_response._code = doc["body"];
            */
            break;
        }
        
        case REQUESTTIME:
        {
            // if(!_time._cmd_sent) return false;
            // _time._cmd_sent = false;
            // _time._responseOK = true;
            // _time._time = doc["body"];
            time_t t = doc["body"];
            TimeDriver::update_time_from_NTP(t);
            break;
        }
        
        case REQUESTCOORDINATES:
        {
            // if(!_coordinates._cmd_sent) return false;
            // _coordinates._cmd_sent = false;
            // _coordinates._responseOK = true;
            // _coordinates._latitude = doc["body"]["latitude"];
            // _coordinates._longitude = doc["body"]["longitude"];
            break;
        }
        
        default:
            break;
    }
    set_busy(false);
    doc.clear();
    print_debug(F("[ESPSerialInterface] parse_response: OK"));
    
    return true;
}

/*
    JSON from Arduino
    {
        'type': [DATA], [REQUESTTIME], [REQUESTCOORDINATES] -> [1], [2], [3]
        'body':
            [DATA]:
                {
                    'sensorid': id,
                    'measuring': value,
                    'latitude': latitude,
                    'longitude': longitude,
                    'timestamp': timestamp
                }
    }
*/

bool ESPSerialInterface::parse_esp_string(String str)
{
    print_debug(F("[ESPSerialInterface] parse_esp_string: %s"), str.c_str());
    
    if(!str.length())   return false;
        
    if(_espState == ESP_OFF)
    {
        int8_t index = str.indexOf('+');
        if(index < 0)       return false;
        
        str = str.substring(index+1);
        
        if(str.compareTo(F("STARTESP")) != 0)   return false;
        
        _espState = ESP_STARTED;
        _watchDog.set_time_last_op(DEFAULT_TIMEOUT_MS);
        print_debug(F("[ESPSerialInterface] parse_esp_string: ESP STARTED!"));
        return true;
    }
    if(_espState == ESP_STARTED)
    {
        int8_t index = str.indexOf('+');
        if(index < 0)       return false;
        
        str = str.substring(index+1);
        
        if(str.compareTo(F("ESPREADY")) != 0)   return false;
        
        _espState = ESP_READY;
        _watchDog.set_time_last_op(BIG_TIMEOUT_MS);
        print_debug(F("[ESPSerialInterface] parse_esp_string: ESP READY!"));
        return true;
    }
    
    if(!parse_response(str))    return false;
    _watchDog.set_time_last_op(BIG_TIMEOUT_MS);
    return true;
}

bool ESPSerialInterface::request_time()
{
    if(_espState != ESP_READY || !is_free())  return false;
    
    JsonObject object = doc.to<JsonObject>();
    object["type"] = (uint8_t)REQUESTTIME;
    serializeJson(doc, *_serial);
    doc.clear();
    set_busy(true);
    _watchDog.set_time_last_op(DEFAULT_TIMEOUT_MS);
    
    print_debug(F("[ESPSerialInterface] get_time: OK"));
    return true;
}

bool ESPSerialInterface::request_coordinates()
{
    print_debug(F("[ESPSerialInterface] request_coordinates:"));
    if(_espState != ESP_READY || !is_free())  return false;
    
    JsonObject object = doc.to<JsonObject>();
    object["type"] = (uint8_t)REQUESTCOORDINATES;
    serializeJson(doc, *_serial);
    doc.clear();
    set_busy(true);
    _watchDog.set_time_last_op(DEFAULT_TIMEOUT_MS);

    print_debug(F("[ESPSerialInterface] request_coordinates: OK"));
    return true;
}

bool ESPSerialInterface::send_http_post(DataContainer* data)
{
    print_debug(F("[ESPSerialInterface] send_http_post:"));
    if(_espState != ESP_READY || !is_free())  return false;
    
    JsonObject object = doc.to<JsonObject>();
    object["type"] = (uint8_t)DATA;
    
    JsonObject body_obj = doc.createNestedObject("body");
    body_obj["sensorid" ] = data->get_id();
    body_obj["measuring"] = data->get_value();
    body_obj["latitude" ] = data->get_latitude();
    body_obj["longitude"] = data->get_longitude();
    body_obj["timestamp"] = data->get_timestamp();
    
    serializeJson(doc, *_serial);
    String json_str;
    serializeJson(doc, json_str);
    print_debug(F("[ESPSerialInterface] send_http_post - json_str: %s"), json_str.c_str());
    doc.clear();
    set_busy(true);
    _watchDog.set_time_last_op(HTTP_TIMEOUT_MS);

    print_debug(F("[ESPSerialInterface] send_http_post: OK"));
    return true;
}