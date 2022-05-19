#ifndef ALPHASENSE_H
#define ALPHASENSE_H

#include <math.h>
// Zero background current temperature compensation factors

// NO-B4 factors
#define NOB4_NT_N30     2.9
#define NOB4_NT_N20     2.9
#define NOB4_NT_N10     2.2
#define NOB4_NT_0       1.8
#define NOB4_NT_10      1.7
#define NOB4_NT_20      1.6
#define NOB4_NT_30      1.5
#define NOB4_NT_40      1.4
#define NOB4_NT_50      1.3

#define NOB4_KT_N30     1.8
#define NOB4_KT_N20     1.8
#define NOB4_KT_N10     1.4
#define NOB4_KT_0       1.1
#define NOB4_KT_10      1.1
#define NOB4_KT_20      1.0
#define NOB4_KT_30      0.9
#define NOB4_KT_40      0.9
#define NOB4_KT_50      0.8

#define NOB4_K1T_N30    0.8
#define NOB4_K1T_N20    0.8
#define NOB4_K1T_N10    0.8
#define NOB4_K1T_0      0.8
#define NOB4_K1T_10     0.9
#define NOB4_K1T_20     1.0
#define NOB4_K1T_30     1.1
#define NOB4_K1T_40     1.2
#define NOB4_K1T_50     1.3

#define NOB4_K2T_N30   -25.0
#define NOB4_K2T_N20   -25.0
#define NOB4_K2T_N10   -25.0
#define NOB4_K2T_0     -25.0
#define NOB4_K2T_10    -16.0
#define NOB4_K2T_20     0.0
#define NOB4_K2T_30     56.0
#define NOB4_K2T_40     200.0
#define NOB4_K2T_50     615.0

// NO2-B43F factors
#define NO2B43F_NT_N30  1.3
#define NO2B43F_NT_N20  1.3
#define NO2B43F_NT_N10  1.3
#define NO2B43F_NT_0    1.3
#define NO2B43F_NT_10   1.0
#define NO2B43F_NT_20   0.6
#define NO2B43F_NT_30   0.4
#define NO2B43F_NT_40   0.2
#define NO2B43F_NT_50  -1.5

#define NO2B43F_KT_N30  2.2
#define NO2B43F_KT_N20  2.2
#define NO2B43F_KT_N10  2.2
#define NO2B43F_KT_0    2.2
#define NO2B43F_KT_10   1.7
#define NO2B43F_KT_20   1.0
#define NO2B43F_KT_30   0.7
#define NO2B43F_KT_40   0.3
#define NO2B43F_KT_50  -2.5

#define NO2B43F_K1T_N30 1.0
#define NO2B43F_K1T_N20 1.0
#define NO2B43F_K1T_N10 1.0
#define NO2B43F_K1T_0   1.0
#define NO2B43F_K1T_10  1.0
#define NO2B43F_K1T_20  1.0
#define NO2B43F_K1T_30  0.4
#define NO2B43F_K1T_40 -0.1
#define NO2B43F_K1T_50 -4.0

#define NO2B43F_K2T_N30 7.0
#define NO2B43F_K2T_N20 7.0
#define NO2B43F_K2T_N10 7.0
#define NO2B43F_K2T_0   7.0
#define NO2B43F_K2T_10  4.0
#define NO2B43F_K2T_20  0.0
#define NO2B43F_K2T_30  0.5
#define NO2B43F_K2T_40  5.0
#define NO2B43F_K2T_50  67.0

// OX-B431 factors
#define OXB431_NT_N30   0.9
#define OXB431_NT_N20   0.9
#define OXB431_NT_N10   1.0
#define OXB431_NT_0     1.3
#define OXB431_NT_10    1.5
#define OXB431_NT_20    1.7
#define OXB431_NT_30    2.0
#define OXB431_NT_40    2.5
#define OXB431_NT_50    3.7

#define OXB431_KT_N30   0.5
#define OXB431_KT_N20   0.5
#define OXB431_KT_N10   0.6
#define OXB431_KT_0     0.8
#define OXB431_KT_10    0.9
#define OXB431_KT_20    1.0
#define OXB431_KT_30    1.2
#define OXB431_KT_40    1.5
#define OXB431_KT_50    2.2

#define OXB431_K1T_N30  0.5
#define OXB431_K1T_N20  0.5
#define OXB431_K1T_N10  0.5
#define OXB431_K1T_0    0.6
#define OXB431_K1T_10   0.6
#define OXB431_K1T_20   1.0
#define OXB431_K1T_30   2.8
#define OXB431_K1T_40   5.0
#define OXB431_K1T_50   5.3

#define OXB431_K2T_N30  1.0
#define OXB431_K2T_N20  1.0
#define OXB431_K2T_N10  1.0
#define OXB431_K2T_0    1.0
#define OXB431_K2T_10   1.0
#define OXB431_K2T_20   1.0
#define OXB431_K2T_30   8.5
#define OXB431_K2T_40   23.0
#define OXB431_K2T_50   103.0

// CO-B4 factors
#define COB4_NT_N30     0.7
#define COB4_NT_N20     0.7
#define COB4_NT_N10     0.7
#define COB4_NT_0       0.7
#define COB4_NT_10      1.0
#define COB4_NT_20      3.0
#define COB4_NT_30      3.5
#define COB4_NT_40      4.0
#define COB4_NT_50      4.5

#define COB4_KT_N30     0.2
#define COB4_KT_N20     0.2
#define COB4_KT_N10     0.2
#define COB4_KT_0       0.2
#define COB4_KT_10      0.3
#define COB4_KT_20      1.0
#define COB4_KT_30      1.2
#define COB4_KT_40      1.3
#define COB4_KT_50      1.5

#define COB4_K1T_N30   -1.0
#define COB4_K1T_N20   -0.5
#define COB4_K1T_N10    0.0
#define COB4_K1T_0      0.0
#define COB4_K1T_10     0.0
#define COB4_K1T_20     1.0
#define COB4_K1T_30     1.0
#define COB4_K1T_40     1.0
#define COB4_K1T_50     1.0

#define COB4_K2T_N30    55.0
#define COB4_K2T_N20    55.0
#define COB4_K2T_N10    55.0
#define COB4_K2T_0      50.0
#define COB4_K2T_10     31.0
#define COB4_K2T_20     0.0
#define COB4_K2T_30    -50.0
#define COB4_K2T_40    -150.0
#define COB4_K2T_50    -250.0

// H2S-B4 factors
#define H2SB4_NT_N30     1.0
#define H2SB4_NT_N20     1.0
#define H2SB4_NT_N10     1.0
#define H2SB4_NT_0       1.0
#define H2SB4_NT_10      1.0
#define H2SB4_NT_20      1.0
#define H2SB4_NT_30      1.0
#define H2SB4_NT_40      1.0
#define H2SB4_NT_50      1.0

#define H2SB4_KT_N30     1.0
#define H2SB4_KT_N20     1.0
#define H2SB4_KT_N10     1.0
#define H2SB4_KT_0       1.0
#define H2SB4_KT_10      1.0
#define H2SB4_KT_20      1.0
#define H2SB4_KT_30      1.0
#define H2SB4_KT_40      1.0
#define H2SB4_KT_50      1.0

#define H2SB4_K1T_N30    1.0
#define H2SB4_K1T_N20    1.0
#define H2SB4_K1T_N10    1.0
#define H2SB4_K1T_0      1.0
#define H2SB4_K1T_10     1.0
#define H2SB4_K1T_20     1.0
#define H2SB4_K1T_30     1.0
#define H2SB4_K1T_40     1.0
#define H2SB4_K1T_50     1.0

#define H2SB4_K2T_N30    0.00
#define H2SB4_K2T_N20    0.00
#define H2SB4_K2T_N10    0.00
#define H2SB4_K2T_0      0.00
#define H2SB4_K2T_10     0.00
#define H2SB4_K2T_20     0.00
#define H2SB4_K2T_30     0.00
#define H2SB4_K2T_40     0.00
#define H2SB4_K2T_50     0.00

// SO2-B4 factors
#define SO2B4_NT_N30    1.6
#define SO2B4_NT_N20    1.6
#define SO2B4_NT_N10    1.6
#define SO2B4_NT_0      1.6
#define SO2B4_NT_10     1.6
#define SO2B4_NT_20     1.6
#define SO2B4_NT_30     1.9
#define SO2B4_NT_40     3.0
#define SO2B4_NT_50     5.8

#define SO2B4_KT_N30    1.0
#define SO2B4_KT_N20    1.0
#define SO2B4_KT_N10    1.0
#define SO2B4_KT_0      1.0
#define SO2B4_KT_10     1.0
#define SO2B4_KT_20     1.0
#define SO2B4_KT_30     1.2
#define SO2B4_KT_40     1.9
#define SO2B4_KT_50     3.6

#define SO2B4_K1T_N30   1.0
#define SO2B4_K1T_N20   1.0
#define SO2B4_K1T_N10   1.0
#define SO2B4_K1T_0     1.0
#define SO2B4_K1T_10    1.0
#define SO2B4_K1T_20    1.0
#define SO2B4_K1T_30    2.0
#define SO2B4_K1T_40    3.5
#define SO2B4_K1T_50    7.0

#define SO2B4_K2T_N30  -4.0
#define SO2B4_K2T_N20  -4.0
#define SO2B4_K2T_N10  -4.0
#define SO2B4_K2T_0    -4.0
#define SO2B4_K2T_10   -4.0
#define SO2B4_K2T_20    0.0
#define SO2B4_K2T_30    20.0
#define SO2B4_K2T_40    140.0
#define SO2B4_K2T_50    450.0

// Sensor numbers
#define ALPHA_NO2B43F_ID    202401512UL
#define ALPHA_SO2B4_ID      164702360UL
#define ALPHA_COB4_ID       162482658UL
#define ALPHA_OXB431_ID1    204660048UL
#define ALPHA_H2SB4_ID      163480136UL
#define ALPHA_OXB431_ID2    204660049UL

// Sensors constants
#define WEe_COB4_162482657  349.0
#define WAe_COB4_162482657  344.0
#define WEo_COB4_162482657  363.0
#define WAo_COB4_162482657  339.0
#define Sen_COB4_162482657  2.2779  //  1/0.439

#define WEe_COB4_162482658  369.0
#define WAe_COB4_162482658  347.0
#define WEo_COB4_162482658  385.0
#define WAo_COB4_162482658  342.0
#define Sen_COB4_162482658  2.551  //  1/0.392

#define WEe_H2SB4_163480136 349.0
#define WAe_H2SB4_163480136 344.0
#define WEo_H2SB4_163480136 361.0
#define WAo_H2SB4_163480136 342.0
#define Sen_H2SB4_163480136 0.6109  //  1/1.637

#define WEe_H2SB4_163480135 340.0
#define WAe_H2SB4_163480135 348.0
#define WEo_H2SB4_163480135 351.0
#define WAo_H2SB4_163480135 345.0
#define Sen_H2SB4_163480135 0.661  //  1/1.513

#define WEe_NO2B43F_202401513 215.0
#define WAe_NO2B43F_202401513 220.0
#define WEo_NO2B43F_202401513 207.0
#define WAo_NO2B43F_202401513 216.0
#define Sen_NO2B43F_202401513 3.1348  //  1/0.319

#define WEe_NO2B43F_202401512 234.0
#define WAe_NO2B43F_202401512 233.0
#define WEo_NO2B43F_202401512 231.0
#define WAo_NO2B43F_202401512 230.0
#define Sen_NO2B43F_202401512 3.096  //  1/0.323

#define WEe_OXB431_204660048 207.0
#define WAe_OXB431_204660048 234.0
#define WEo_OXB431_204660048 203.0
#define WAo_OXB431_204660048 231.0
#define Sen_OXB431_204660048 3.2051  //  1/0.312

#define WEe_OXB431_204660049 221.0
#define WAe_OXB431_204660049 231.0
#define WEo_OXB431_204660049 217.0
#define WAo_OXB431_204660049 227.0
#define Sen_OXB431_204660049 2.6525  //  1/0.377

#define WEe_SO2B4_164702361 328.0
#define WAe_SO2B4_164702361 343.0
#define WEo_SO2B4_164702361 362.0
#define WAo_SO2B4_164702361 345.0
#define Sen_SO2B4_164702361 2.7174  //  1/0.368

#define WEe_SO2B4_164702360 349.0
#define WAe_SO2B4_164702360 359.0
#define WEo_SO2B4_164702360 380.0
#define WAo_SO2B4_164702360 363.0
#define Sen_SO2B4_164702360 2.8902  //  1/0.346

const double NTFactors[6][9] = 
    {   
        {   COB4_NT_N30, COB4_NT_N20, COB4_NT_N10,
            COB4_NT_0,   COB4_NT_10,  COB4_NT_20,
            COB4_NT_30,  COB4_NT_40,  COB4_NT_50            },

        {   H2SB4_NT_N30, H2SB4_NT_N20, H2SB4_NT_N10,
            H2SB4_NT_0,   H2SB4_NT_10,  H2SB4_NT_20,
            H2SB4_NT_30,  H2SB4_NT_40,  H2SB4_NT_50         },

        {   NOB4_NT_N30, NOB4_NT_N20, NOB4_NT_N10,
            NOB4_NT_0,   NOB4_NT_10,  NOB4_NT_20,
            NOB4_NT_30,  NOB4_NT_40,  NOB4_NT_50            },

        {   NO2B43F_NT_N30, NO2B43F_NT_N20, NO2B43F_NT_N10,
            NO2B43F_NT_0,   NO2B43F_NT_10,  NO2B43F_NT_20,
            NO2B43F_NT_30,  NO2B43F_NT_40,  NO2B43F_NT_50   },

        {   OXB431_NT_N30, OXB431_NT_N20, OXB431_NT_N10,
            OXB431_NT_0,   OXB431_NT_10,  OXB431_NT_20,
            OXB431_NT_30,  OXB431_NT_40,  OXB431_NT_50      },

        {   SO2B4_NT_N30, SO2B4_NT_N20, SO2B4_NT_N10,
            SO2B4_NT_0,   SO2B4_NT_10,  SO2B4_NT_20,
            SO2B4_NT_30,  SO2B4_NT_40,  SO2B4_NT_50         }
    };

const double KTFactors[6][9] = 
    {   
        {   COB4_KT_N30, COB4_KT_N20, COB4_KT_N10,
            COB4_KT_0,   COB4_KT_10,  COB4_KT_20,
            COB4_KT_30,  COB4_KT_40,  COB4_KT_50            },

        {   H2SB4_KT_N30, H2SB4_KT_N20, H2SB4_KT_N10,
            H2SB4_KT_0,   H2SB4_KT_10,  H2SB4_KT_20,
            H2SB4_KT_30,  H2SB4_KT_40,  H2SB4_KT_50         },

        {   NOB4_KT_N30, NOB4_KT_N20, NOB4_KT_N10,
            NOB4_KT_0,   NOB4_KT_10,  NOB4_KT_20,
            NOB4_KT_30,  NOB4_KT_40,  NOB4_KT_50            },

        {   NO2B43F_KT_N30, NO2B43F_KT_N20, NO2B43F_KT_N10,
            NO2B43F_KT_0,   NO2B43F_KT_10,  NO2B43F_KT_20,
            NO2B43F_KT_30,  NO2B43F_KT_40,  NO2B43F_KT_50   },

        {   OXB431_KT_N30, OXB431_KT_N20, OXB431_KT_N10,
            OXB431_KT_0,   OXB431_KT_10,  OXB431_KT_20,
            OXB431_KT_30,  OXB431_KT_40,  OXB431_KT_50      },

        {   SO2B4_KT_N30, SO2B4_KT_N20, SO2B4_KT_N10,
            SO2B4_KT_0,   SO2B4_KT_10,  SO2B4_KT_20,
            SO2B4_KT_30,  SO2B4_KT_40,  SO2B4_KT_50         }
    };

const double K1TFactors[6][9] = 
    {   
        {   COB4_K1T_N30, COB4_K1T_N20, COB4_K1T_N10,
            COB4_K1T_0,   COB4_K1T_10,  COB4_K1T_20,
            COB4_K1T_30,  COB4_K1T_40,  COB4_K1T_50            },

        {   H2SB4_K1T_N30, H2SB4_K1T_N20, H2SB4_K1T_N10,
            H2SB4_K1T_0,   H2SB4_K1T_10,  H2SB4_K1T_20,
            H2SB4_K1T_30,  H2SB4_K1T_40,  H2SB4_K1T_50         },

        {   NOB4_K1T_N30, NOB4_K1T_N20, NOB4_K1T_N10,
            NOB4_K1T_0,   NOB4_K1T_10,  NOB4_K1T_20,
            NOB4_K1T_30,  NOB4_K1T_40,  NOB4_K1T_50            },

        {   NO2B43F_K1T_N30, NO2B43F_K1T_N20, NO2B43F_K1T_N10,
            NO2B43F_K1T_0,   NO2B43F_K1T_10,  NO2B43F_K1T_20,
            NO2B43F_K1T_30,  NO2B43F_K1T_40,  NO2B43F_K1T_50   },

        {   OXB431_K1T_N30, OXB431_K1T_N20, OXB431_K1T_N10,
            OXB431_K1T_0,   OXB431_K1T_10,  OXB431_K1T_20,
            OXB431_K1T_30,  OXB431_K1T_40,  OXB431_K1T_50      },

        {   SO2B4_K1T_N30, SO2B4_K1T_N20, SO2B4_K1T_N10,
            SO2B4_K1T_0,   SO2B4_K1T_10,  SO2B4_K1T_20,
            SO2B4_K1T_30,  SO2B4_K1T_40,  SO2B4_K1T_50         }
    };

const double K2TFactors[6][9] = 
    {   
        {   COB4_K2T_N30, COB4_K2T_N20, COB4_K2T_N10,
            COB4_K2T_0,   COB4_K2T_10,  COB4_K2T_20,
            COB4_K2T_30,  COB4_K2T_40,  COB4_K2T_50            },

        {   H2SB4_K2T_N30, H2SB4_K2T_N20, H2SB4_K2T_N10,
            H2SB4_K2T_0,   H2SB4_K2T_10,  H2SB4_K2T_20,
            H2SB4_K2T_30,  H2SB4_K2T_40,  H2SB4_K2T_50         },

        {   NOB4_K2T_N30, NOB4_K2T_N20, NOB4_K2T_N10,
            NOB4_K2T_0,   NOB4_K2T_10,  NOB4_K2T_20,
            NOB4_K2T_30,  NOB4_K2T_40,  NOB4_K2T_50            },

        {   NO2B43F_K2T_N30, NO2B43F_K2T_N20, NO2B43F_K2T_N10,
            NO2B43F_K2T_0,   NO2B43F_K2T_10,  NO2B43F_K2T_20,
            NO2B43F_K2T_30,  NO2B43F_K2T_40,  NO2B43F_K2T_50   },

        {   OXB431_K2T_N30, OXB431_K2T_N20, OXB431_K2T_N10,
            OXB431_K2T_0,   OXB431_K2T_10,  OXB431_K2T_20,
            OXB431_K2T_30,  OXB431_K2T_40,  OXB431_K2T_50      },

        {   SO2B4_K2T_N30, SO2B4_K2T_N20, SO2B4_K2T_N10,
            SO2B4_K2T_0,   SO2B4_K2T_10,  SO2B4_K2T_20,
            SO2B4_K2T_30,  SO2B4_K2T_40,  SO2B4_K2T_50         }
    };

static double get_comp_factors(uint8_t type, double T, const double factors[6][9])
{
    if(T <= -30.0)  T = -30.0;
    else if(T >= 50.0)  T = 50.0;
    unsigned int index = round((30+T)/10);
    return factors[type][index];
}

static double comp_algorithm_1(uint8_t type, double T, double WE_term, double AE_term, double WEo, double AEo)
{
    // print_debug(F("comp_algorithm 1()"));
    double nT = get_comp_factors(type, T, NTFactors);
    return ( WE_term - nT*AE_term );
}

static double comp_algorithm_2(uint8_t type, double T, double WE_term, double AE_term, double WEo, double AEo)
{
    // print_debug(F("comp_algorithm 2()"));
    double kT = get_comp_factors(type, T, KTFactors);
    return ( WE_term - kT*(WEo/AEo)*AE_term );
}

static double comp_algorithm_3(uint8_t type, double T, double WE_term, double AE_term, double WEo, double AEo)
{
    // print_debug(F("comp_algorithm 3()"));
    double k1T = get_comp_factors(type, T, K1TFactors);
    return ( WE_term - (WEo-AEo) - k1T*AE_term );
}


static double comp_algorithm_4(uint8_t type, double T, double WE_term, double AE_term, double WEo, double AEo)
{
    // print_debug(F("comp_algorithm 4()"));
    double k2T = get_comp_factors(type, T, K2TFactors);
    return ( WE_term - WEo - k2T );
}

#endif