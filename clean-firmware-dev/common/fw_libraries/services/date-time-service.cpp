#include <ArduinoJson.h>
#include "debug-output.h"
#include "date-time-service.h"

const size_t json_capacity  = JSON_OBJECT_SIZE  (1)
                            + JSON_OBJECT_SIZE  (5)
                            + 5*JSON_OBJECT_SIZE(1);

StaticJsonDocument<json_capacity> doc;

SerialInternetTimeService::SerialInternetTimeService(SerialInternet* provider) 
    : _timeProvider(provider) 
{
    _serialData._type = REQUESTTIME;
    _serialData._responseOK = false;
}

void SerialInternetTimeService::restart_provider()
{
    reset_tries();
    _timeProvider->restart();
}

bool SerialInternetTimeService::request_internet_time() 
{   
    set_lastcmd_ms();
    print_debug(F("[SerialInternetTimeService] request_internet_time:"));
    
    JsonObject object = doc.to<JsonObject>();
    object["type"] = REQUESTTIME;
    String message = "";
    serializeJson(doc, message);
    doc.clear();
    
    return send_serial_msg(message);
}

bool SerialInternetTimeService::update_system_time()
{
    if(!_serialData._responseOK)
    {
        if(provider_timeout())  restart_provider();
        return false;
    }
    DeserializationError error = deserializeJson(doc, _serialData._response.c_str());
    if (error)  return false;
    
    time_t t = 
    _serialData->type = doc["type"];
    if(_serialData->type == ERROR)  return false;
    
    _serialData->_response = doc["body"];
    _serialData->_responseOK = true;
    _serialFree = true;
    doc.clear();
    print_debug(F("[ESPSerialInterface] parse_response: OK"));
    
    return true;
}