#ifndef _GPS_INTERFACE_H_
#define _GPS_INTERFACE_H_

#include <Stream.h>
#include <TimeLib.h>

template <class T>
class GPSInterface
{
    protected:
        T* _gps;
        
    public:
        GPSInterface(T* gps) : _gps(gps)    {}
        
        time_t mk_timestamp(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
        {
            tmElements_t _gps_dateTime;
            _gps_dateTime.Year      = CalendarYrToTm(year);
            _gps_dateTime.Month     = month;
            _gps_dateTime.Day       = day;
            _gps_dateTime.Hour      = hour - 3;
            _gps_dateTime.Minute    = min;
            _gps_dateTime.Second    = sec;
            
            time_t t = makeTime(_gps_dateTime);
            return t;
        }

        virtual bool read_gps(int msecs_wt) = 0;
};

template <class T>
class GPSSerialInterface : public GPSInterface<T>
{
    protected:
        Stream* _serial;
        
    public:
        GPSSerialInterface(T* gps, Stream* serial) : 
            GPSInterface<T>(gps), _serial(serial)    {}
};

#endif