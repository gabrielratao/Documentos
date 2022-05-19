#ifdef ESP8266

#include "esp-serial-iot.h"
#include "debug-output.h"
#include <ArduinoJson.h>

const size_t json_capacity  = JSON_OBJECT_SIZE  (2)
                            + JSON_OBJECT_SIZE  (5)
                            + 5*JSON_OBJECT_SIZE(1);

StaticJsonDocument<json_capacity> doc;


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
CommandTypes ESPSerialHandler::parse(const String str)
{
    print_debug(F("[ESPSerialHandler] parse: %s"), str.c_str());
    DeserializationError error = deserializeJson(doc, str.c_str());
    // Test if parsing succeeds.
    if (error)  return ERROR;
    uint8_t type = doc["type"];
    print_debug(F("[ESPSerialHandler] parse - type: %d"), type);
    if(type == DATA)    _dataStr = doc["body"].as<String>();
    _status = WT_RESPONSE;
    doc.clear();
    return (CommandTypes)type;
}

/*
    JSON from ESP
    {
        'type': [CODE], [TIME], [COORDINATES] -> [1], [2], [3]
        'body':
            [CODE]:
                {
                    'value': code
                }
            [TIME]:
                {
                    'code': time
                }
            [COORDINATES]:
                {
                    'latitude': latitude,
                    'longitude': longitude
                }
    }
*/

void ESPSerialHandler::send_http_code(int code, Stream& _serial)
{
    JsonObject object = doc.to<JsonObject>();
    object["type"] = (uint8_t)DATA;
    object["body"] = code;
    serializeJson(doc, _serial);
    doc.clear();
}

void ESPSerialHandler::send_time(time_t time, Stream& _serial)
{
    JsonObject object = doc.to<JsonObject>();
    object["type"] = (uint8_t)REQUESTTIME;
    object["body"] = time;
    serializeJson(doc, _serial);
    doc.clear();
}

void ESPSerialHandler::send_coordinates(String json_coord, Stream& _serial)
{
    JsonObject object = doc.to<JsonObject>();
    object["type"] = (uint8_t)REQUESTCOORDINATES;
    object["body"] = json_coord;
    serializeJson(doc, _serial);
    doc.clear();
}
ESPSerialHandler espSerial;

#endif