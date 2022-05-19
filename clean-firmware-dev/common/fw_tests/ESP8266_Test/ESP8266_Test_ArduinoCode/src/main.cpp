#include <Arduino.h>

#define _reset_pin 12UL

enum auth_t
{
  PERSONAL,
  ENTERPRISE
};

enum command_en
{
  SETAUTHMETHOD = 0,
  SETSSID = 1,
  SETPASSWORD = 2,
  SETUSERNAME = 3,
  SETHOST = 4,
  SETROUTE = 5,
  SETPORT,
  SETGMTOFFSET,
  GETAUTHMETHOD,
  GETSSID,
  GETUSERNAME,
  GETPASS,
  GETHOST,
  GETROUTE,
  GETPORT,
  GETGMTOFFSET,
  GETIP,
  RESPONSETIME,
  CONNECTWIFI,
  DISCONNECTWIFI,
  WIFISTATUS,
  GETTIMESTAMP,
  GETHEAPSIZE,
  HTTPPOST,
  HTTPGET,
  DEEPSLEEP,
  NONE
};
command_en cmd = NONE;

String ssid = "";
String username = "";
String password = "";
auth_t auth_type = ENTERPRISE;

enum wl_status  { WL_IDLE_STATUS  = 0, WL_NO_SSID_AVAIL  = 1, 
                  WL_CONNECTED    = 3, WL_CONNECT_FAILED = 4, 
                  WL_DISCONNECTED = 6                           };

int timestamp = 0;

void restart()
{
  digitalWrite(_reset_pin, LOW);
  delay(100);
  digitalWrite(_reset_pin, HIGH);
  Serial3.flush();
  cmd = SETAUTHMETHOD;
  Serial.println("Restarting ESP...");
}

bool act(String msg, String data)
{
  if (cmd == SETAUTHMETHOD)
  {
    if (msg.compareTo(F("AUTHMETHOD")) != 0 ||
        data.toInt() != auth_type)
    {
      restart();
      return false;
    }
    
    cmd = SETSSID;
    String command = "AT+SETSSID=" + ssid + '\r';
    Serial3.println(command);
    
    return true;
  }

  if (cmd == SETSSID)
  {
    if (msg.compareTo(F("SSID")) != 0 ||
        data.compareTo(ssid) != 0)
    {
      restart();
      return false;
    }

    cmd = SETUSERNAME;
    String command = "AT+SETUSERNAME=" + username + '\r';
    Serial3.println(command);
    return true;
  }

  if (cmd == SETUSERNAME)
  {
    if (msg.compareTo(F("USERNAME")) != 0 ||
        data.compareTo(username) != 0)
    {
      restart();
      return false;
    }

    cmd = SETPASSWORD;
    String command = "AT+SETPASSWORD=" + password + '\r';
    Serial3.println(command);
    return true;
  }

  if (cmd == SETPASSWORD)
  {
    if (msg.compareTo(F("PASSWORD")) != 0 ||
        data.compareTo(password) != 0)
    {
      restart();
      return false;
    }

    cmd = CONNECTWIFI;
    Serial3.println("AT+CONNECTWIFI\r");
    return true;
  }

  if (cmd == CONNECTWIFI)
  {
    if (msg.compareTo(F("CONNECTWIFI")) != 0)
    {
      restart();
      return false;
    }

    if (data.length() > 0 && data.compareTo(F("CONNECTED")) == 0)
    {
      cmd = WIFISTATUS;
      Serial3.println("AT+WIFISTATUS\r");
      return true;
    }
    else if (data.length() > 0 && data.compareTo(F("CONNECTED")) != 0)
    {
      restart();
      return false;
    }

    return true;
  }

  if (cmd == WIFISTATUS)
  {
    Serial.println("last cmd sent: WIFISTATUS");
    if (msg.compareTo(F("WIFISTATUS")) != 0 || data.length() <= 0)
    {
      restart();
      return false;
    }
    
    Serial.println("ESP responded with OK code");
    
    if ((wl_status)data.toInt() != WL_CONNECTED)
    {
      restart();
      return false;
    }
    
    Serial.println("ESP is connected and will request TIMESTAMP");
    cmd = GETTIMESTAMP;
    Serial3.println("AT+GETTIMESTAMP\r");
    return true;
  }

  if (cmd == GETTIMESTAMP)
  {
    Serial.println("last cmd sent: GETTIMESTAMP");
    if (msg.compareTo(F("TIMESTAMP")) != 0)
    {
      restart();
      return false;
    }

    Serial.println("ESP responded with OK code");
    if (data.length())
    {
      timestamp = data.toInt();
      cmd = SETAUTHMETHOD;
      Serial.print("ESP is connected and TIMESTAMP is:");
      Serial.println(timestamp);
    }
    
    return true;
  }

  return false;
}

bool act_on_rcv()
{
  String serial_msg = Serial3.readStringUntil('\n');
  Serial.print("String received from ESP8266: ");
  Serial.println(serial_msg);

  if (!serial_msg.length())
    return false;

  int8_t index = serial_msg.indexOf('+');
  int8_t index_end = serial_msg.indexOf('\r');

  if (index < 0 || index_end < 0) // messages always start with + and ends with \r
  {
    // restart();
    return false;
  }

  serial_msg = serial_msg.substring(index + 1, index_end);

  if (serial_msg.compareTo(F("ERR")) == 0)
  {
    restart();
    return false;
  }

  if (serial_msg.compareTo(F("INVALIDCOMMAND")) == 0)
  {
    restart();
    return false;
  }

  if (serial_msg.compareTo(F("STARTESP")) == 0)
  {
    String command = "AT+SETAUTHMETHOD=" + String(auth_type) + '\r'; 
    Serial3.println(command);
    return true;
  }

  index = serial_msg.indexOf(F("OK"));

  if (index < 0)
  {
    restart();
    return false;
  }

  serial_msg = serial_msg.substring(0, index);
  index = serial_msg.indexOf('=');

  String data = "";

  if (index > 0)
  {
    data = serial_msg.substring(index + 1);
    serial_msg = serial_msg.substring(0, index);
  }

  Serial.println("Parsed string:");
  Serial.print("serial_msg: ");
  Serial.println(serial_msg);
  Serial.print("data: ");
  Serial.println(data);

  return act(serial_msg, data);
}

void setup()
{
  Serial.begin(9600);
  Serial3.begin(9600);
  pinMode(_reset_pin, OUTPUT);
  Serial.println("Initializing test...");
  restart();
}

void loop()
{
  if(Serial3.available())
  { 
    act_on_rcv();
  }
}