#ifdef ESP8266

#include <time.h>
#include "debug-output.h"
#include "esp-iot.h"

//----------------------------------------------------------------------------

ESPWifiConnector::ESPWifiConnector()
{
    #if defined(ESP8266)
        wifi_status = WL_DISCONNECTED;
    #endif
}

bool ESPWifiConnector::can_try_connection()
{
    if((_authType == PERSONAL && (!_ssid.length() || !_password.length())) 
        || (_authType == ENTERPRISE && (!_ssid.length() || !_password.length() || !_user.length())))
        return false;
    return true;
}

bool ESPWifiConnector::connect_to_wifi() 
{
    print_debug(F("[ESPWifiConnector] connect_to_wifi:"));
    
    #if defined(ESP8266)
    
    if(!can_try_connection())
        return false;
    print_debug(F("[ESPWifiConnector] connect_to_wifi: Ready for connection!"));

    while (disconnect_from_wifi() == false) { delay(10); }
    print_debug(F("[ESPWifiConnector] connect_to_wifi: Disconnected!"));
    
    if(_authType == PERSONAL) {
        WiFi.begin(_ssid, _password);
    }
    else if(_authType == ENTERPRISE) {
        wifi_station_disconnect();
        
        station_config wifi_config;
        memset(&wifi_config, 0, sizeof(wifi_config));
        strcpy((char*)wifi_config.ssid, _ssid.c_str());
        wifi_station_set_config(&wifi_config);
        
        wifi_station_clear_cert_key();
        wifi_station_clear_enterprise_ca_cert();
        wifi_station_set_wpa2_enterprise_auth(1);
        wifi_station_set_enterprise_identity((uint8*)_user.c_str(), _user.length());
        wifi_station_set_enterprise_username((uint8*)_user.c_str(), _user.length());
        wifi_station_set_enterprise_password((uint8*)_password.c_str(), _password.length());
        wifi_station_connect();
    }

    print_debug(F("[ESPWifiConnector] connect_to_wifi - _authType: %d"), _authType);
    print_debug(F("[ESPWifiConnector] connect_to_wifi - wifi status: %d"), wifi_station_get_connect_status());
        
    uint32_t init_timeout = millis() + DEFAULT_TIMEOUT;
    
    while(WiFi.status() != WL_CONNECTED) { 
        if (millis() > init_timeout)    return false;
        delay(1);
    }
    print_debug(F("[ESPWifiConnector] connect_to_wifi: Connected!"));
    
    #endif

    return true;
}

bool ESPWifiConnector::disconnect_from_wifi() 
{
    print_debug(F("[ESPWifiConnector] disconnect_from_wifi:"));
    bool discon = false;
    
    #if defined(ESP8266)

    discon = WiFi.disconnect();
    // Clean up to be sure no old data is still inside
    wifi_station_clear_cert_key();
    wifi_station_clear_enterprise_ca_cert();
    wifi_station_clear_enterprise_identity();
    wifi_station_clear_enterprise_username();
    wifi_station_clear_enterprise_password();
    wifi_station_clear_enterprise_new_password();
    
    print_debug(F("[ESPWifiConnector] disconnect_from_wifi: Disconnected!"));
    
    #endif
    
    return discon;
}

bool ESPWifiConnector::set_auth_method(const auth_t auth) 
{
    if (auth != PERSONAL && auth != ENTERPRISE)
        return false;
    _authType = auth;
    return true;
}

bool ESPWifiConnector::set_ssid(String newssid) 
{
    if(newssid.length() > SSID_MAX_SIZE || newssid == 0)
        return false;
    _ssid = newssid;
    return true;
}

bool ESPWifiConnector::set_username(String newusername) 
{
    if(newusername.length() > USERNAME_MAX_SIZE || newusername == 0)
        return false;
    _user = newusername;
    return true;
}

bool ESPWifiConnector::set_pass(String newpass) 
{
    if(newpass.length() > PASS_MAX_SIZE || newpass == 0)
        return false;
    _password = newpass;
    return true;
}

void ESPWifiConnector::config_wifi_connection(WiFiCredentials wifiCred)
{
    // Set Authentication method (Personal or Enterprise)
    espWifi.set_auth_method((auth_t)wifiCred.get_auth());
    // Set WiFi SSID
    espWifi.set_ssid(wifiCred.get_ssid());
    // Set WiFi USERNAME (only in case of Enterprise)
    if ((auth_t)wifiCred.get_auth() == ENTERPRISE)  espWifi.set_username(wifiCred.get_user());
    // Set WiFi PASSWORD
    espWifi.set_pass(wifiCred.get_password());    
}

bool ESPWifiConnector::connect()
{
    WiFi.mode(WIFI_STA);
    delay(5000);
    print_debug(F("[ESPWifiConnector] connect(): Connecting to %s"), _ssid.c_str());
    espWifi.disconnect_from_wifi();
    return espWifi.connect_to_wifi();
}

ESPWifiConnector espWifi;

// -----------------------------------------------------------------------

time_t get_time(int timezone_sec, int daylightOffset_sec)
{
    configTime(timezone_sec, daylightOffset_sec, 
        "pool.ntp.org", "br.pool.ntp.org", "time.nist.gov");
    
    uint32_t init_timeout = millis() + DEFAULT_TIMEOUT*10;
    time_t t = time(nullptr);
    while (t < 1622501035)
    {
        if (millis() > init_timeout)    return -1;
        delay(2);
        t = time(nullptr);
    }
    
    return (t+timezone_sec+daylightOffset_sec)-SECS_YR_2000;
}

// -----------------------------------------------------------------------

#include <WiFiClient.h>
int HTTPHandler::post(const String  host, const uint16_t port, const String  url, const String  message)
{
    print_debug(F("[HTTPHandler] post() - message: %s"), message.c_str());
    int _httpCode = 0;
    
    if(message.length() >= MAX_MESSAGE_SIZE && !check_host(host) 
        && !check_port(port) && !check_url(url))    return -1;
    
    if(!check_connection())                         return -1;
    
    WiFiClient wifiClient;
    if(!wifiClient.connect(host, port))             return -1;
    
    if(!begin(wifiClient, host, port, url))         return -1;
    
    addHeader("Content-Type", "application/json");
    _httpCode = POST(message);
    
    print_debug(F("[HTTPHandler] post() - httpCode: %d\n\r"), _httpCode);
    end();  
    
    return _httpCode;
}

int HTTPHandler::post(IPAddress ip, const uint16_t port, const String url, const String message)
{
    print_debug(F("[HTTPHandler] post() - message: %s"), message.c_str());
    int _httpCode = 0;
    
     if(message.length() >= MAX_MESSAGE_SIZE
        && !check_port(port) && !check_url(url))    return -1;
    
    if(!check_connection())                         return -1;
    
    WiFiClient wifiClient;
    if(!wifiClient.connect(ip, port))             return -1;

    if(!begin(wifiClient, ip.toString(), port, url))         return -1;
    
    addHeader("Content-Type", "application/json");
    _httpCode = POST(message);
    
    print_debug(F("[HTTPHandler] post() - httpCode: %d\n\r"), _httpCode);
    end();  
    
    return _httpCode;
}

int HTTPHandler::get(const String url, String &payload) 
{
    if(!check_url(url)) return -1;
    int _httpCode = 0;
    WiFiClient wifiClient;
    if(check_connection() && begin(wifiClient, url))
    {
        addHeader("Content-Type", "application/json");
        _httpCode = GET();
        payload = getString();
        end();    
    }
    else _httpAvailable = false;
    return _httpCode;
}

HTTPHandler espHTTP;

//------------------------------------------------------------------

/*--- TO DO: ---*/
// Get internet coordinates

// g_coordinates_st google_coordinates::last_conn_st = W_ERROR;
float google_coordinates::latitude  = 0;
float google_coordinates::longitude = 0;
float google_coordinates::accuracy  = 0;

// */
/*
// WiFiCredentials for Google Geolocation API
#define G_API_HOST  "www.googleapis.com"
#define G_API_PAGE  "/geolocation/v1/geolocate?key="
#define G_API_KEY   "AIzaSyC35uwLfWZQS10aT85PCvwIo4RUcyxP0_U"

// Data for Google GeoLocation API Configuration...
const String homeMobileCountryCode = "\"homeMobileCountryCode\": 724,";
const String homeMobileNetworkCode = "\"homeMobileNetworkCode\": 8,";
const String radioType = "\"radioType\": \"gsm\",";
const String carrier = "\"carrier\": \"TIM\",";
const String cellTowers = "\"cellTowers\": [";
const String wifiAccessPoints = "\"wifiAccessPoints\": [";

bool ESPWifiConnector::get_google_api_coord()
{
    bool state = false;
    // g_coordinates_st state = W_ERROR;
    #if defined(ESP8266)
    
    if(WiFi.mode(WIFI_STA) && disconnect_from_wifi())
    {
        // state = W_SCAN;
        int n = WiFi.scanNetworks();

        DynamicJsonDocument doc(1024);
        
        String jsonString = "{\n";
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

        if (client.connect(G_API_HOST, 443)) 
        {
            state = W_CONNECTED;
            client.println("POST " + (String)G_API_PAGE + (String)G_API_KEY + " HTTP/1.1");
            client.println("Host: "+ (String)G_API_HOST);
            client.println("Connection: close");
            client.println("Content-Type: application/json");
            client.println("User-Agent: Arduino/1.0");
            client.print("Content-Length: ");
            client.println(jsonString.length());    
            client.println();
            client.print(jsonString);  
            delay(500);
        }

        while (wifi_client->available()) 
        {
            String line = wifi_client->readStringUntil('\r');
            
            auto error = deserializeJson(doc, line);
            if(!error)
            {
                state = W_COORD;
                latitude = doc["location"]["lat"];
                longitude = doc["location"]["lng"]; 
                accuracy = doc["accuracy"];
            }
        }

        
    }
    
    // print_debug("[google_coordinates] state: %d", state);
    #endif
    return state;
}
// */

#endif