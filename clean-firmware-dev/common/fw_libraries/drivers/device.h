#ifndef _DEVICE_H
#define _DEVICE_H

enum devType_e { MOBDEV = 0 , FIXDEV };
struct Device
{
    static unsigned long id;
    static devType_e type;
    static unsigned int  st;
};

#endif