#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include <Arduino.h>
// Time
#include <TimeLib.h>
#include "device.h"

class TimeDriver
{
private:
    static bool _up2Date;
public:
    static void set_time(time_t t);
    static void update_time_from_NTP(time_t t);
    static bool _already_up_to_date()   {   return _up2Date;    }
};

#include "rtc-interface.h"

template <class T>
class RTCDriver
{
public:
    static void update_rtc(RTCInterface<T>* rtc, time_t t)
    {
        if(!rtc->is_up_to_date() && TimeDriver::_already_up_to_date())  
        {
            print_debug(F("[RTCDriver] update_rtc: %ld"), t);
            rtc->set_time(t);
            rtc->set_up_to_date(true);
        }
    }

    static time_t sync_time_from_RTC(RTCInterface<T>* rtc)  {   return rtc->get_time(); }
};

//---------------------------------------------------------------------

// Geo-location

enum GPSSt_e { OUTDATE = 0, OK = 1  };

#include "gps-interface.h"
class GPSDriver
{
private:
    static double _lat, _lng, _alt;
    static GPSSt_e _gpsSt;
public:
    static void set_coordinates(bool valid, double lat, 
        double lng, double alt)
    {
        _lat = valid ? lat : -99.99;
        _lng = valid ? lng : -99.99;
        _alt = valid ? alt : -99.99;
    }
    static void set_gps_state(GPSSt_e st)   {   _gpsSt = st;    }
    
    static double get_latitude()    {   return _lat;    }
    static double get_longitude()   {   return _lng;    }
    static double get_altitude()    {   return _alt;    }
    static double get_gps_st()      {   return _gpsSt;  }
};

//---------------------------------------------------------------------

// DATA

class DataContainer
{
    protected:
        uint32_t    _id;
        double      _value;
        double      _lat, _long;
        time_t      _timestamp;

    public:
        void set_id(uint32_t id)            {   _id = id;               }
        void set_value(double value)        {   _value = value;         }    
        void set_coordinates(double lat, double lng)
        {   _lat = lat; _long = lng;   }
        void set_timestamp(time_t timestamp){   _timestamp = timestamp; }

        uint32_t    get_id()        {   return _id;         }
        double      get_value()     {   return _value;      }
        double      get_latitude()  {   return _lat;        }
        double      get_longitude() {   return _long;       }
        time_t      get_timestamp() {   return _timestamp;  }
        
        virtual size_t toCSV(Print* p) = 0;
};

#endif