#ifndef _VARIABLE_H
#define _VARIABLE_H

#include "sensor_alphasense.h"
#include "drivers.h"

// READ_ST: GPS_ST | SENSOR_ST

class SensorData : public DataContainer
{
    private:
        uint8_t _readSt     = (uint8_t)NOT_READY ;
    public:
        virtual size_t toCSV(Print* p);
        void setReadSt(uint8_t st)  {   _readSt = st;   }
};

class Variable
{
protected:
    Smoother        _var;
    uint32_t        _id;
    variable_e      _unit;
    ReadingStatus  _readSt = NOT_READY;
    
    void identify_sense(SensorData* data);
    
public:
    Variable(uint32_t id, uint8_t unit, uint8_t buff_size) :
        _var(buff_size), _id(id), _unit((variable_e)unit)   {};
    ~Variable() {}
    uint32_t    getId()         {   return _id;             }
    uint8_t     getUnit()       {   return _unit;           }
    double      getValue()      {   return _var.getAve();   }
    Smoother&   getSmoother()   {   return _var;            }
    
    virtual void sense(SensorData* data) = 0;
    Measurement smooth(Measurement new_measurement) { 
        return _var.smooth(new_measurement.value, new_measurement.status); 
    }
};

// #define R_liters    0.000120272 // 1/(1000*8.314510)
// #define M_CO        28.01
// #define M_NO2       46.01
// #define M_O3        48.00
// #define M_SO2       64.06
// #define M_H2S       34.08

class AnalogInput : public Variable
{
private:
        
public:
    AnalogInput(uint32_t id, uint8_t unit, uint8_t buff_size) :
        Variable(id, unit, buff_size)    {}
    
    virtual void sense(SensorData* data);
};

class GasConcentration : public Variable
{
private:
    // double convertToug()
    // {

    // }
    
public:
    GasConcentration(uint32_t id, uint8_t unit, uint8_t buff_size) :
        Variable(id, unit, buff_size)    {}
    
    virtual void sense(SensorData* data);
};

class Temperature : public Variable
{
private:
    
public:
    Temperature(uint32_t id, uint8_t unit, uint8_t buff_size) :
        Variable(id, unit, buff_size)    {}
    
    virtual void sense(SensorData* data);
};

class Humidity : public Variable
{
private:
    
public:
    Humidity(uint32_t id, uint8_t unit, uint8_t buff_size) :
        Variable(id, unit, buff_size)    {}
    
    virtual void sense(SensorData* data);
};

class Pressure : public Variable
{
private:
    
public:
    Pressure(uint32_t id, uint8_t unit, uint8_t buff_size) :
        Variable(id, unit, buff_size)    {}
    
    virtual void sense(SensorData* data);
};

#endif