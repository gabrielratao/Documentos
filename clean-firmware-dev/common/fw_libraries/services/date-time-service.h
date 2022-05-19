#ifndef _DATE_TIME_SERVICE_H_
#define _DATE_TIME_SERVICE_H_

#include <Wire.h>
#include <RtcDS3231.h>
#include <Time.h>
#include "serial-internet-interface.h"

class ServiceControl
{
    #define MAXNUM_TRIES 5
    #define MAX_TIMEOUT_MS 60000UL
    protected:
        uint32_t    _t_lastcmd_ms = 0;
        uint8_t     _numtries = 0;    
        
    public:
        void set_lastcmd_ms()       {   _t_lastcmd_ms = millis();   }
        void reset_tries()          {   numtries = 0;                           }
        bool provider_timeout()     {   return ((millis() - _t_lastcmd_ms) >= MAX_TIMEOUT_MS);   }
        bool provider_triesout()    {   return (++_numtries >= MAXNUM_TRIES);    }
};

class SerialInternetTimeService : public ServiceControl
{
    protected:
        serialData      _serialData;

        void restart_provider();
    public:
        SerialInternetTimeService(SerialInternet* provider);
        
        bool request_internet_time();

        bool update_system_time();
}

class Timer
{
public:
    virtual void set_time(time_t t);

    #define LEAP_YEAR(Y)     ( ((1970+(Y))>0) && !((1970+(Y))%4) && ( ((1970+(Y))%100) || !((1970+(Y))%400) ) )

    static void breakTime(time_t timeInput, tmElements_t &tm)
    {
        // Copiada de TimeLib.h para evitar erro de memória
        
        uint8_t year;
        uint8_t month, monthLength;
        uint32_t time;
        unsigned long days;

        time = (uint32_t)timeInput;
        tm.Second = time % 60;
        time /= 60; // now it is minutes
        tm.Minute = time % 60;
        time /= 60; // now it is hours
        tm.Hour = time % 24;
        time /= 24; // now it is days
        tm.Wday = ((time + 4) % 7) + 1;  // Sunday is day 1 
        
        year = 0;  
        days = 0;
        while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
            year++;
        }
        tm.Year = year; // year is offset from 1970 
        
        days -= LEAP_YEAR(year) ? 366 : 365;
        time  -= days; // now it is days in this year, starting at 0
        
        days=0;
        month=0;
        monthLength=0;
        for (month=0; month<12; month++) {
            if (month==0)           monthLength=31;
            else if (month==1) 
            { // february 
                if (LEAP_YEAR(year))monthLength=29;
                else                monthLength=28;
            } 
            else if (month==2)      monthLength=31;
            else if (month==3)      monthLength=30;
            else if (month==4)      monthLength=31;
            else if (month==5)      monthLength=30;
            else if (month==6)      monthLength=31;
            else if (month==7)      monthLength=31;
            else if (month==8)      monthLength=30;
            else if (month==9)      monthLength=31;
            else if (month==10)     monthLength=30;
            else if (month==11)     monthLength=31;
            else                    month = 12;
            
            if (time >= monthLength)    time -= monthLength;
            else    break;
        }
        tm.Month = month + 1;  // jan is month 1  
        tm.Day = time + 1;     // day of month
    }

    
    static time_t makeTime(const tmElements_t &tm)
    {   
        // Copiada de TimeLib.h para evitar erro de memória
        
        int i;
        uint32_t seconds;

        // seconds from 1970 till 1 jan 00:00:00 of the given year
        seconds= tm.Year*(SECS_PER_DAY * 365);
        for (i = 0; i < tm.Year; i++) {
            if (LEAP_YEAR(i)) {
            seconds +=  SECS_PER_DAY;   // add extra days for leap years
            }
        }
        static const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; // API starts months from 1, this array starts from 0
        // add days for this year, months start from 1
        for (i = 1; i < tm.Month; i++) {
            if ( (i == 2) && LEAP_YEAR(tm.Year)) { 
            seconds += SECS_PER_DAY * 29;
            } else {
            seconds += SECS_PER_DAY * monthDays[i-1];  //monthDay array starts from 0
            }
        }
        seconds+= (tm.Day-1) * SECS_PER_DAY;
        seconds+= tm.Hour * SECS_PER_HOUR;
        seconds+= tm.Minute * SECS_PER_MIN;
        seconds+= tm.Second;
        return (time_t)seconds; 
    }

    static int second() {  return second(now());   }
    static int second(time_t t) 
    {  // the second for the given time
        tmElements_t tm;
        breakTime(t, tm);
        return tm.Second;
    }

    static int minute() {   return minute(now()); }
    static int minute(time_t t) 
    {
        tmElements_t tm;
        breakTime(t, tm);
        return tm.Minute;  
    }

    static int hour() { return hour(now()); }
    static int hour(time_t t) 
    { // the hour for the given time
        tmElements_t tm;
        breakTime(t, tm);
        return tm.Hour;  
    }

    static int hourFormat12() { return hourFormat12(now()); }
    static int hourFormat12(time_t t) 
    { // the hour for the given time in 12 hour format
        tmElements_t tm;
        breakTime(t, tm);
        if( tm.Hour == 0 )
            return 12; // 12 midnight
        else if( tm.Hour  > 12)
            return tm.Hour - 12 ;
        else
            return tm.Hour ;
    }

    static int day(){  return(day(now())); }
    static int day(time_t t) 
    { // the day for the given time (0-6)
        tmElements_t tm;
        breakTime(t, tm);
        return tm.Day;
    }

    static int weekday() { return  weekday(now()); }
    static int weekday(time_t t) 
    {
        tmElements_t tm;
        breakTime(t, tm);
        return tm.Wday;
    }
    
    static int month(){    return month(now());    }
    static int month(time_t t) 
    {  // the month for the given time
        tmElements_t tm;
        breakTime(t, tm);
        return tm.Month;
    }

    static int year() {    return year(now()); }
    static int year(time_t t) 
    { // the year for the given time
        tmElements_t tm;
        breakTime(t, tm);
        return tmYearToCalendar(tm.Year);
    }

};

#define I2C Wire

#define DS3231
#define SOFTWARE_WIRE_SDA SDA  // Or whatever other pin you use
#define SOFTWARE_WIRE_SCL SCL  // Or whatever other pin you use

#define MY_TIMEZONE -3
#define MY_TIMEZONE_IN_SECONDS (MY_TIMEZONE * SECS_PER_HOUR)

class RTCControl : public Timer
{
public:
    RTCControl() : Rtc(I2C)    {}
    RtcDS3231<TwoWire> Rtc;
    virtual void set_time(time_t t);
};

#endif