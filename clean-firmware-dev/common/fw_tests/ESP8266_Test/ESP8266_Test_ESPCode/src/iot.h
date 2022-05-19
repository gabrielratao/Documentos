#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

extern "C" {
  #include "user_interface.h"
  #include "c_types.h"
  #include "wpa2_enterprise.h"
}

#include <time.h>
#include "FS.h"

enum auth_t { PERSONAL, ENTERPRISE };

static const int        SSID_MAX_SIZE       = 64    ;
static const int        USERNAME_MAX_SIZE   = 64    ;
static const int        PASS_MAX_SIZE       = 64    ;
static const int        HOST_MAX_SIZE       = 128   ;
static const int        ROUTE_MAX_SIZE      = 128   ;
static const int        MAX_MESSAGE_SIZE    = 4096  ;
static const long int   DEFAULT_TIMEOUT     = 40 * 1000;

typedef struct google_coordinates{
    static float latitude, longitude, accuracy;
};

class IoTConnection
{
    
private:
    String ssid, username, pass, host, route;
    uint32_t port;
    
    // long gmtOffset_sec = -3*3600;
    // int daylightOffset_sec = -3*3600;
    long gmtOffset_sec = 0;
    int daylightOffset_sec = 0;

    // float latitude, longitude, accuracy;
    google_coordinates coordinates;
//    File certificate;
//    File key;

    enum state_t { CONNECTED, DISCONNECTED };
    state_t con_state;

    uint8_t auth_method;
    uint32_t init_timeout;

    HTTPClient client;
    
private:
    bool can_try_connection();
    void config_time();
    bool check_timeout();
    bool get_google_api_coord();
    bool is_all_setup_correctly();
    
public:
    IoTConnection();
    
    bool connect_to_wifi();
    bool disconnect_from_wifi();
    time_t now();

    int http_post   (String host, uint16_t port, String uri, String message);
    int http_post   (String message);
    int http_get    (String host, uint16_t port, String uri, String &payload);
    int http_get    (String &payload);
    
    bool set_auth_method(int auth_type      );
    bool set_ssid       (String newssid     );
    bool set_username   (String newusername );
    bool set_pass       (String newpass     );
    bool set_host       (String newhost     );
    bool set_route      (String newroute    );
    bool set_port       (uint32_t newport   );
    bool set_gmt_offset (long new_gmt_offset);
    
    int get_auth_method()   { return auth_method;       }
    String get_ssid()       { return ssid;              }
    int get_ssid_size()     { return ssid.length();     }
    String get_username()   { return username;          }
    int get_username_size() { return username.length(); }
    String get_pass()       { return pass;              }
    int get_pss_size()      { return pass.length();     }
    String get_host()       { return host;              }
    int get_host_size()     { return host.length();     }
    String get_route()      { return route;             }
    int get_route_size()    { return route.length();    }
    uint32_t get_port()     { return port;              }
    long get_gmt_offset()   { return gmtOffset_sec;     } 
    google_coordinates get_coordinates()
    {
        if(get_google_api_coord())  return coordinates;
    }
};