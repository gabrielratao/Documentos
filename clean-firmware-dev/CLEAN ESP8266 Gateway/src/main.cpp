#include "debug-output.h"
#include "esp-iot.h"
#include "esp-serial-iot.h"
#include "iot-generic.h"

/*--- TO DO: ---*/
// Fazer configurável por software

#define NUM_WIFIS 3
bool DEBUG::debug = false;

const WiFiCredentials CRED_1("<ssid-enterprise-1>", "<password-1>", ENTERPRISE, 
                              "<username-1>");

const WiFiCredentials CRED_2("ssid-personal-1", "password-2", PERSONAL);

const WiFiCredentials CRED_3("ssid-personal-2", "password-3", PERSONAL);

const WiFiCredentials wifiCreds[NUM_WIFIS] = { CRED_1, CRED_2, CRED_3 };

#define TIMEZONE_SEC        -3*3600
#define DAYLIGTHOFFSET_SEC  0

void setup() 
{
  Serial.begin(9600UL);
  Serial.setDebugOutput(false);
  Serial.print(F("+STARTESP;"));
  
  setup_wifi_connection<NUM_WIFIS>(wifiCreds);
  print_debug(F("Timestamp: %lld"), get_time(TIMEZONE_SEC,DAYLIGTHOFFSET_SEC));
  espHTTP.set_available(true);
  espSerial.set_status(WT_REQUEST);
  Serial.print(F("+ESPREADY;"));
}

void loop() 
{
  static CommandTypes _cmdType = ERROR;

  if(Serial.available())
  { 
    String serial_Str = Serial.readStringUntil(';');
    if(espSerial.get_status() == WT_REQUEST)  _cmdType = espSerial.parse(serial_Str);
    Serial.flush();
  }
  if(espSerial.get_status() == WT_RESPONSE)
  {
    espSerial.set_status(WT_REQUEST);
    switch (_cmdType)
    {
      case DATA:
      {
        static uint8_t _numberOfPostTries = 0;
        #define MAX_NUM_TRIES 3
        int code = espHTTP.post(HOST, PORT, URL, espSerial.get_data());
        
        if(code <= 0 && WiFi.status() != WL_CONNECTED)
        {
          setup_wifi_connection<NUM_WIFIS>(wifiCreds);
          if(++_numberOfPostTries >= MAX_NUM_TRIES-1) ESP.restart();
        }
        else  espSerial.send_http_code(code, Serial);
        break;
      }

      case REQUESTTIME:
      {
        static uint8_t _numberOfTries = 0;
        #define MAX_NUM_TRIES 3
        time_t t = get_time(TIMEZONE_SEC,DAYLIGTHOFFSET_SEC);
        if(!t && WiFi.status() != WL_CONNECTED)
        {
          setup_wifi_connection<NUM_WIFIS>(wifiCreds);
          if(++_numberOfTries >= MAX_NUM_TRIES-1) ESP.restart();
        }
        else  espSerial.send_time(t, Serial);
        break;
      }
      
      /*--- TO DO: ---*/
      // Get coordinates from Google API
      case REQUESTCOORDINATES:
      {
        break;
      }
      
      default:
      {
        break;
      }
    }
  }
  
}