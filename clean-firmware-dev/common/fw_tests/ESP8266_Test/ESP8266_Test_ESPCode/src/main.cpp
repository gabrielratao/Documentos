#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "iot.h"

IoTConnection iot;
const size_t json_capacity  = JSON_OBJECT_SIZE  (2)
                            + JSON_OBJECT_SIZE  (10) + 5
                            + JSON_OBJECT_SIZE  (3) + 5*3
                            + JSON_ARRAY_SIZE   (10);

int8_t command_pos = 0, data_pos = 0;

bool is_command = true; //alternates between command or data

void act(String command, String data = "")
{
  if(command.charAt(0) != 'A' || command.charAt(1) != 'T' || command.charAt(2) != '+')
  {
    Serial.print(F("+ERR\r\n"));
    return;
  }

  String action = command.substring(command.indexOf('+')+1);
  if(!action.length())
  {
    Serial.print(F("+ERR\r\n"));
    return;
  }

  if(action.compareTo(F("SETAUTHMETHOD")) == 0)
  {
    if(!data.length())  
    {
      Serial.print(F("+ERR\r\n"));
      return;
    }

    if(iot.set_auth_method(data.toInt()))
    {
      Serial.print("+AUTHMETHOD=");
      Serial.print(iot.get_auth_method());
      Serial.print(F("OK"));
    }
    else  Serial.print(F("+ERR"));
  }

  else if(action.compareTo(F("SETSSID")) == 0)
  {
    if(!data.length())  
    {
      Serial.print(F("+ERR\r\n"));
      return;
    }

    if(iot.set_ssid(data))
    {
      Serial.print("+SSID=");
      Serial.print(iot.get_ssid());
      Serial.print(F("OK"));
    }
    else  Serial.print(F("+ERR"));

  }

  else if(action.compareTo(F("SETPASSWORD")) == 0)
  {
    if(!data.length())  
    {
      Serial.print(F("+ERR\r\n"));
      return;
    }

    if(iot.set_pass(data))
    {
      Serial.print("+PASSWORD=");
      Serial.print(iot.get_pass());
      Serial.print(F("OK"));
    }
    else  Serial.print(F("+ERR"));
  }

  else if(action.compareTo(F("SETUSERNAME")) == 0)
  {
    if(!data.length())  
    {
      Serial.print(F("+ERR\r\n"));
      return;
    }

    if(iot.set_username(data))
    {
      Serial.print("+USERNAME=");
      Serial.print(iot.get_username());
      Serial.print(F("OK"));
    }
    else  Serial.print(F("+ERR"));
  }

  else if(action.compareTo(F("SETHOST")) == 0)
  {
    if(!data.length())  
    {
      Serial.print(F("+ERR\r\n"));
      return;
    }

    if(iot.set_host(data))
    {
      Serial.print("+HOST=");
      Serial.print(iot.get_host());
      Serial.print(F("OK"));
    }
    else  Serial.print(F("+ERR"));

  }

  else if(action.compareTo(F("SETROUTE")) == 0)
  {
    if(!data.length())  
    {
      Serial.print(F("+ERR\r\n"));
      return;
    }

    if(iot.set_route(data))
    {
      Serial.print("+ROUTE=");
      Serial.print(iot.get_route());
      Serial.print(F("OK"));
    }
    else  Serial.print(F("+ERR"));
  } 

  else if(action.compareTo(F("SETPORT")) == 0)
  {
    if(!data.length())  
    {
      Serial.print(F("+ERR\r\n"));
      return;
    }

    if(iot.set_port(data.toInt()))
    {
      Serial.print("+PORT=");
      Serial.print(iot.get_port());
      Serial.print(F("OK"));
    }
    else  Serial.print(F("+ERR"));
  }

  else if(action.compareTo(F("SETGMTOFFSET")) == 0)
  {
    if(!data.length())  
    {
      Serial.print(F("+ERR\r\n"));
      return;
    }

    if(iot.set_gmt_offset(data.toInt()))  
    {
      Serial.print("+GMTOFFSET=");
      Serial.print(iot.get_gmt_offset());
      Serial.print(F("OK"));
    }
    else  Serial.print(F("+ERR"));
  }

  else if(action.compareTo(F("GETAUTHMETHOD")) == 0)
  {
    Serial.print("+AUTHMETHOD=");
    Serial.print(iot.get_auth_method());
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("GETSSID")) == 0)
  {
    Serial.print("+SSID=");
    Serial.print(iot.get_ssid());
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("GETUSERNAME")) == 0)
  {
    Serial.print("+USERNAME=");
    Serial.print(iot.get_username());
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("GETPASS")) == 0)
  {
    Serial.print("+PASSWORD=");
    Serial.print(iot.get_pass());
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("GETHOST")) == 0)
  {
    Serial.print("+HOST=");
    Serial.print(iot.get_host());
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("GETROUTE")) == 0)
  {
    Serial.print("+ROUTE=");
    Serial.print(iot.get_route());
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("GETPORT")) == 0)
  {
    Serial.print("+PORT=");
    Serial.print(iot.get_port());
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("GETGMTOFFSET")) == 0)
  {
    Serial.print("+GMTOFFSET=");
    Serial.print(iot.get_gmt_offset());
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("GETIP")) == 0)
  {
    Serial.print(F("+IP="));
    Serial.print(WiFi.localIP());
    Serial.print(F("OK"));
  } 
  
  else if(action.compareTo(F("RESPONSETIME")) == 0)
  {
    Serial.print(F("+RESPONSETIME"));
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("CONNECTWIFI")) == 0)
  {
    Serial.print(F("+CONNECTWIFI"));
    Serial.print(F("OK\r\n"));
    if(iot.connect_to_wifi()) Serial.print(F("+CONNECTWIFI=CONNECTEDOK"));
    else                      Serial.print(F("+ERR"));
  } 
  
  else if(action.compareTo(F("DISCONNECTWIFI")) == 0)
  {
    Serial.print(F("+DISCONNECTWIFI"));
    Serial.print(F("OK\r\n"));
    if(iot.disconnect_from_wifi())  Serial.print(F("+DISCONNECTWIFI=DISCONNECTEDOK"));
    else                            Serial.print(F("+ERR"));
  } 
  
  else if(action.compareTo(F("WIFISTATUS")) == 0)
  {
    Serial.print(F("+WIFISTATUS="));
    Serial.print(WiFi.status());
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("GETTIMESTAMP")) == 0)
  {
    if(WiFi.status() != WL_CONNECTED) 
    {
      Serial.print(F("+ERR\r\n"));
      return;
    }
    
    Serial.print(F("+TIMESTAMPOK\r\n"));
    time_t t = iot.now();
    Serial.print(F("+TIMESTAMP="));
    Serial.print(t);
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("GETHEAPSIZE")) == 0)
  {
    Serial.print(F("+HEAPSIZE="));
    Serial.print(ESP.getFreeHeap());
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("HTTPPOST")) == 0)
  {
    if(!data.length())  
    {
      Serial.print(F("+ERR\r\n"));
      return;
    }
    
    Serial.print(F("+HTTPPOST"));
    Serial.print(F("OK\r\n"));
    int code = iot.http_post(data);
    Serial.print(F("+HTTPCODE="));
    Serial.print(code);
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("HTTPGET")) == 0)
  {
    Serial.print(F("+HTTPGET"));
    Serial.print(F("OK\r\n"));
    String payload;
    int code = iot.http_get(payload);
    Serial.print(F("+HTTPCODE="));
    Serial.print(code);
    Serial.print(F(":"));
    Serial.print(payload);
    Serial.print(F("OK"));
  }

  else if(action.compareTo(F("DEEPSLEEP")) == 0)
  {
    Serial.print(F("+DEEPSLEEP"));
    Serial.print(F("+OK\r\n"));
    ESP.deepSleep(0); //put the ESP in deep sleep mode until the RST pin goes to LOW
  } 
  
  else  Serial.print("+INVALIDCOMMAND");

  Serial.print('\r');
  Serial.print('\n');
}

void setup() 
{
  Serial.begin(9600UL);
  Serial.setDebugOutput(false);
  
  WiFi.mode(WIFI_STA);
  delay(5000);
  Serial.print(F("+STARTESP\r\n"));
}

void loop() 
{
  if(Serial.available())
  { //if received a command or data
    String serial_Str = Serial.readStringUntil('\n');
    int8_t index_data = serial_Str.indexOf('=');
    
    if(index_data < 0)  // no data
      act(serial_Str.substring(0, serial_Str.indexOf('\r')));
    else
      act(serial_Str.substring(0, index_data), serial_Str.substring(index_data+1, serial_Str.indexOf('\r')));
  }
}