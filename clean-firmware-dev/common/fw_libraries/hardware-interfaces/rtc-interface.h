#ifndef _RTC_INTERFACE_H_
#define _RTC_INTERFACE_H_

template <class T>
class RTCInterface
{
protected:
    T* _rtc;
    bool _up2Date = false;
    
public:
    RTCInterface(T* rtc) : _rtc(rtc)    {}
    virtual void set_time(unsigned long t) = 0;
    virtual unsigned long get_time()       = 0;
    void set_up_to_date(bool b) {   _up2Date = b;       }
    bool is_up_to_date()        {   return _up2Date;    }
};

#endif