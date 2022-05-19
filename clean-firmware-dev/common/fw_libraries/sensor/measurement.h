#ifndef _MEASUREMENT_H
#define _MEASUREMENT_H

enum ReadingStatus { NOT_READY = 0, READY = 1  };

enum variable_e {   SI_TEMP_Farenheit   = 2,
                    SI_TEMP_Celsius     = 3,
                    SI_HUMD_Relative    = 4,
                    SI_CONC_ppm         = 5,
                    SI_CONC_ppb         = 6,
                    SI_CONC_ug          = 7,
                    SI_AnalogV          = 8,
                    SI_AnalogmV         = 9,
                    SI_PRES_Pascal      = 10,
                    SI_PRES_Atm         = 11
                };

struct Measurement
{
    double        value;
    ReadingStatus status;
    variable_e    si_unit;

    Measurement(double val, ReadingStatus st) 
    {
        value = val;
        status = st;
    }
};

#endif