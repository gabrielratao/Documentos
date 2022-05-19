#include "debug-output.h"
#include "drivers.h"

// Time
bool TimeDriver::_up2Date = false;

void TimeDriver::set_time(time_t t)
{
    if(t < now()-SECS_PER_HOUR || t > now()+SECS_PER_HOUR)    // One our difference
        setTime(t);
}

void TimeDriver::update_time_from_NTP(time_t t)
{
    print_debug(F("[TimeDriver] update_time_from_NTP: %ld"), t);
    if(!_up2Date)
    {
        time_t time = t <= SECS_YR_2000 ? t+SECS_YR_2000 : t;
        setTime(time);
        _up2Date = true;
        print_debug(F("[TimeDriver] update_time_from_NTP: UPDATED to %ld!"), time);
    }
}

//-------------------------------------------------------------

// Geo-location
double GPSDriver::_lat   = -99.99;
double GPSDriver::_lng   = -99.99;
double GPSDriver::_alt   = -99.99;
GPSSt_e GPSDriver::_gpsSt= OUTDATE;
