#ifndef _ESP_IOT_H_
#define _ESP_IOT_H_

#ifdef ESP8266

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

extern "C" {
    #include "user_interface.h"
    #include "c_types.h"
    #include "wpa2_enterprise.h"
}

/*
typedef enum g_coordinates_st {W_ERROR = 0, W_SCAN, W_CONNECTED, W_COORD};
struct google_coordinates
{
    static float latitude, longitude, accuracy;
    static g_coordinates_st last_conn_st;
    
    //static uint8_t get_google_api_coord(ESP8266WiFiClass& wifi, String ssid, String pass);
};
// */

/* ---------------- WIFI CONFIGURATION ENTITIES ------------------*/
/* ---------------- TO DO: Software Configurable -----------------*/
//------------------------------------------------------------------
static const int        SSID_MAX_SIZE       = 64;
static const int        USERNAME_MAX_SIZE   = 64;
static const int        PASS_MAX_SIZE       = 64;
static const int        HOST_MAX_SIZE       = 128;
static const int        ROUTE_MAX_SIZE      = 128;
static const int        MAX_MESSAGE_SIZE    = 4096;
static const uint32_t   DEFAULT_TIMEOUT     = 30 * 1000;

enum auth_t { PERSONAL, ENTERPRISE };
// enum wl_status  { WL_IDLE_STATUS  = 0, WL_NO_SSID_AVAIL  = 1, 
//                   WL_CONNECTED    = 3, WL_CONNECT_FAILED = 4, 
//                   WL_DISCONNECTED = 6                           };
#define NUM_WIFI_MAX    20

class WiFiCredentials
{
public:
    WiFiCredentials(const String ssid, const String pass, const uint8_t auth) :
        _ssid(ssid), _password(pass), _auth(auth)   {}
    WiFiCredentials(const String s, const String p, const uint8_t a, 
        const String u) : WiFiCredentials(s,p,a)   
    {   if ((auth_t)a == ENTERPRISE)    _user = u;  }
    
    String get_ssid     ()  {   return _ssid;       }
    String get_user     ()  {   return _password;   }
    String get_password ()  {   return _password;   }
    uint8_t get_auth    ()  {   return _auth;       }
    
private:
    String _ssid;
    String _user;
    String _password;
    uint8_t _auth;
    
    void set_ssid    (const String s)   {   _ssid = s;      }
    void set_username(const String u)   {   _user = u;      }
    void set_password(const String p)   {   _password = p;  }
    void set_auth    (const uint8_t a)  {   _auth = a;      }
};

//------------------------------------------------------------------

struct google_coordinates{
    static float latitude, longitude, accuracy;
};

//------------------------------------------------------------------
/* ----------------------- ESPWifiConnector -------------------------*/
//------------------------------------------------------------------
class ESPWifiConnector
{
protected:
    auth_t      _authType;
    String      _ssid, _user, _password;
    uint32_t    _heapSize;
    
    // int daylightOffset_sec = 0;

    // float latitude, longitude, accuracy;
    wl_status_t wifi_status;
    google_coordinates coordinates;

    // unsigned long int last_time_recorded;
    // unsigned long int last_response_time;
    // unsigned long int init_timeout;
    
    bool get_google_api_coord();
    bool can_try_connection();
    
public:
    ESPWifiConnector();
    
    bool connect_to_wifi();
    bool disconnect_from_wifi();
    
    bool set_auth_method(const auth_t auth_type)   ;
    bool set_ssid       (const String newssid)     ;
    bool set_username   (const String newusername) ;
    bool set_pass       (const String newpass)     ;
    
    uint8_t     get_auth_method()   {   return _authType;   }
    String      get_ssid()          {   return _ssid;       }       
    String      get_username()      {   return _user;       }
    String      get_password()      {   return _password;   }
    
    google_coordinates get_coordinates()
    {
        if(get_google_api_coord())  return coordinates;
    }

    void config_wifi_connection(WiFiCredentials wifiCred);
    bool connect();
};
extern ESPWifiConnector espWifi;

//------------------------------------------------------------------

#define SECS_YR_2000 ((time_t)(946684800UL))
time_t get_time(int timezone_sec, int daylightOffset_sec);

//------------------------------------------------------------------

class HTTPHandler : private HTTPClient
{
private:
    bool        _httpAvailable = false;
    
    bool check_host(const String host) {
        return (host.length() > HOST_MAX_SIZE || host.length() <= 0) ? false : true ;
    }
    bool check_port(uint16_t port) {    return (port <= 0) ? false : true ; }
    bool check_url(const String url) {
        return (url.length() > HOST_MAX_SIZE || url.length() <= 0) ? false : true ;
    }
   
public:
    HTTPHandler() : HTTPClient() {}
    
    bool check_connection() { return (_httpAvailable) && (WiFi.status() == WL_CONNECTED); }
    
    void    set_available   (bool a)   {   _httpAvailable = a; }
    
    int get (const String url, String &payload);
    int post(const String  host, const uint16_t port, const String  url, const String  message);
    int post(IPAddress ip, const uint16_t port, const String url, const String message);
};

extern HTTPHandler espHTTP;

//------------------------------------------------------------------

template<size_t NUM_WIFIS>
void setup_wifi_connection(const WiFiCredentials wifiCreds[NUM_WIFIS])
{
    do{
        static uint8_t _wifiId = 0;
        espWifi.config_wifi_connection(wifiCreds[_wifiId]);
        _wifiId = _wifiId >= NUM_WIFIS-1 ? 0 : _wifiId+1;
        delay(5000);
    } while (!espWifi.connect());
    
    if (!get_time(0,0))  ESP.restart();
}

#endif
#endif