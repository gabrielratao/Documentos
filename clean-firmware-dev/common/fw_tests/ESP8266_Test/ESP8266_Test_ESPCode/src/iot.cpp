#include "iot.h"
#include <ArduinoJson.h>
#include <WiFiClientSecureBearSSL.h>

bool IoTConnection::can_try_connection()
{
    if((auth_method == PERSONAL && (ssid.length() == 0 || pass.length() == 0)) || (auth_method == ENTERPRISE && (ssid.length() == 0 || pass.length() == 0 || username.length() == 0)))
        return false;
    return true;
}

void IoTConnection::config_time()
{
    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");
}

bool IoTConnection::check_timeout() 
{
    if (millis() > init_timeout) {
        return true;
    }
    return false;
}

bool IoTConnection::is_all_setup_correctly()
{
    if(host.length() <= 0 || route.length() <= 0 || port <= 0)
        return false;
    return true;
}

IoTConnection::IoTConnection()
{
    con_state   = DISCONNECTED;
    auth_method = PERSONAL;
}

bool IoTConnection::connect_to_wifi()
{
    if(!can_try_connection())
        return false;
    if(WiFi.status() == WL_CONNECTED)   return true;

    if(auth_method == PERSONAL) 
        WiFi.begin(ssid.c_str(), pass.c_str());
    else if(auth_method == ENTERPRISE) 
    {
        wifi_station_set_wpa2_enterprise_auth(1);
        wifi_station_set_enterprise_identity((uint8*)username.c_str(), username.length());
        wifi_station_set_enterprise_username((uint8*)username.c_str(), username.length());
        wifi_station_set_enterprise_password((uint8*)pass.c_str(), pass.length());
        wifi_station_set_enterprise_new_password((uint8*)pass.c_str(), pass.length()); 
        WiFi.begin(ssid.c_str(), 0);
    }

    init_timeout = millis() + DEFAULT_TIMEOUT;
    while(WiFi.status() != WL_CONNECTED) 
    { 
        if(check_timeout()) ESP.restart();
        yield();
    }

    if(WiFi.status() != WL_CONNECTED)
        return false;
    else
        return true;
}

bool IoTConnection::disconnect_from_wifi() 
{
    bool discon = WiFi.disconnect();
    // Clean up to be sure no old data is still inside
    wifi_station_clear_cert_key();
    wifi_station_clear_enterprise_ca_cert();
    wifi_station_clear_enterprise_identity();
    wifi_station_clear_enterprise_username();
    wifi_station_clear_enterprise_password();
    wifi_station_clear_enterprise_new_password();
    
    return discon;
}

int IoTConnection::http_post(String host, uint16_t port, String uri, String message)
{
    if(host.length() == 0 || uri.length() == 0 || port == 0)  return -1;
    if(message.length() >= MAX_MESSAGE_SIZE)    return -1;
    client.begin(host.c_str(),(uint16_t)port, uri.c_str());
    // client.begin(IOTHOST,(uint16_t)IOTPORT);
    // client.begin(host.c_str(),port);

    int httpCode = client.POST((uint8_t *)message.c_str(),(size_t)message.length());

    client.end();

    return httpCode;
}

int IoTConnection::http_post(String message) 
{
    if(!is_all_setup_correctly() || message.length() >= MAX_MESSAGE_SIZE)
        return -1;

    client.begin(host,(uint16_t)port,route);

    int httpCode = client.POST((uint8_t *)message.c_str(),(size_t)message.length());

    client.end();

    return httpCode;
}
int IoTConnection::http_get(String host, uint16_t port, String uri, String &payload) 
{
    if(host.length() == 0 || uri.length() == 0 || port == 0)  return -1;

    // for(int i = 0; i < host.length(); i++) s_host += host[i];
    // for(int i = 0; i < route.length(); i++) s_route += route[i];
    
    client.begin(host.c_str(),(uint16_t)port,uri.c_str());
    // client.begin(s_host.c_str(),(uint16_t)port);
    // client.begin(host.c_str(),(uint16_t)port);

    int httpCode = client.GET();

    payload = client.getString();

    client.end();

    return httpCode;
}

int IoTConnection::http_get(String &payload) 
{
    if(!is_all_setup_correctly())   return -1;
        
    client.begin(host,(uint16_t)port,route);

    int httpCode = client.GET();

    payload = client.getString();

    client.end();

    return httpCode;
}

time_t IoTConnection::now() 
{
    config_time();
    time_t t = time(nullptr);
    while (t < 1514764800) 
    { // Just to avoid wrong time value
        delay(2);
        t = time(nullptr);
    }
    
    return t;
}

bool IoTConnection::set_auth_method(int auth_type) 
{
    if (auth_type != PERSONAL && auth_type != ENTERPRISE)
        return false;
        
    auth_method = auth_type;
    return true;
}

bool IoTConnection::set_ssid(String newssid) 
{
    if(newssid.length() > SSID_MAX_SIZE || newssid == 0)
        return false;
    ssid = newssid;
    //memcpy(ssid, newssid, ssidsize);
    return true;
}

bool IoTConnection::set_username(String newusername) 
{
    if(newusername.length() > USERNAME_MAX_SIZE || newusername == 0)
        return false;
    username = newusername;
    //memcpy(username, newusername, usernamesize);
    return true;
}

bool IoTConnection::set_pass(String newpass) 
{
    if(newpass.length() > PASS_MAX_SIZE || newpass == 0)
        return false;
    pass = newpass;
    //memcpy(pass, newpass, passsize);
    return true;
}
// /*
bool IoTConnection::set_host(String newhost) 
{
    if(newhost.length() > HOST_MAX_SIZE || newhost == 0)
        return false;
    host = newhost;
    //memcpy(host, newhost, hostsize);
    return true;
}
// */
// /*
bool IoTConnection::set_route(String newroute) 
{
    if(newroute.length() > ROUTE_MAX_SIZE || newroute == 0)
        return false;
    route = newroute;
    //memcpy(route, newroute, routesize);
    return true;
}
// */
bool IoTConnection::set_port(uint32_t newport) 
{
    port = newport;
    return true;
}
//*/
// /*
bool IoTConnection::set_gmt_offset(long new_gmt_offset) 
{
    gmtOffset_sec = new_gmt_offset;
    return true;
}

/*
// g_coordinates_st google_coordinates::last_conn_st = W_ERROR;
float google_coordinates::latitude  = 0;
float google_coordinates::longitude = 0;
float google_coordinates::accuracy  = 0;
// */

/*
// Credentials for Google Geolocation API
const String G_API_HOST = "www.googleapis.com"                      ;
const String G_API_PAGE = "/geolocation/v1/geolocate?key="          ;
const String G_API_KEY  = "AIzaSyC35uwLfWZQS10aT85PCvwIo4RUcyxP0_U" ;

// Data for Google GeoLocation API Configuration...
const String homeMobileCountryCode = "\"homeMobileCountryCode\": 724,"  ;
const String homeMobileNetworkCode = "\"homeMobileNetworkCode\": 8,"    ;
const String radioType             = "\"radioType\": \"gsm\","          ;
const String carrier               = "\"carrier\": \"TIM\","            ;
const String cellTowers            = "\"cellTowers\": ["                ;
const String wifiAccessPoints      = "\"wifiAccessPoints\": ["          ;
// */

/*
bool IoTConnection::get_google_api_coord()
{
    bool state = false;
    // g_coordinates_st state = W_ERROR;

    if(WiFi.mode(WIFI_STA) && disconnect_from_wifi())
    {
        // state = W_SCAN;
        int n = WiFi.scanNetworks();

        DynamicJsonDocument doc(1024);
        
        String jsonString = F("{\n");
        jsonString += homeMobileCountryCode;  // this is a real UK MCC
        jsonString += homeMobileNetworkCode;   // and a real UK MNC
        jsonString += radioType;          // for gsm
        jsonString += carrier;       // associated with Vodafone 
        jsonString += wifiAccessPoints;
        for (int j = 0; j < n; ++j)
        {
            jsonString += "{\n";
            jsonString += "\"macAddress\" : \"";
            jsonString += (WiFi.BSSIDstr(j));
            jsonString += "\",\n";
            jsonString += "\"signalStrength\": ";
            jsonString += WiFi.RSSI(j);
            jsonString += "\n";
            (j < n-1) ? (jsonString += "},\n") : (jsonString += "}\n");
        }
        jsonString += ("]\n");
        jsonString += ("}\n");

        // WiFiClientSecure client;
        
        if(!connect_to_wifi())  return false;
        
        std::unique_ptr<BearSSL::WiFiClientSecure>wifi_client(new BearSSL::WiFiClientSecure);
        if(!client.begin(*wifi_client, G_API_HOST, 443, (String)G_API_PAGE + (String)G_API_KEY, true))  return false;
        int httpCode = client.POST(jsonString);
        
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
        {
            String payload = client.getString();
            
            auto error = deserializeJson(doc, payload);
            if(!error)
            {
                // state = W_COORD;
                coordinates.latitude = doc["location"]["lat"];
                coordinates.longitude = doc["location"]["lng"]; 
                coordinates.accuracy = doc["accuracy"];
            }
        }
    }
    return state;
}
// */