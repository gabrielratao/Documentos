#include "variable.h"
#include "debug-output.h"

/*--------SensorData---------*/

size_t SensorData::toCSV(Print* p)
{
    print_debug(F("[SensorData] toCSV:"));
    size_t size_of_data = p->print(year());
    size_of_data += p->print(',');
    size_of_data += p->print(month());
    size_of_data += p->print(',');
    size_of_data += p->print(day());
    size_of_data += p->print(',');
    size_of_data += p->print(hour());
    size_of_data += p->print(',');
    size_of_data += p->print(minute());
    size_of_data += p->print(',');
    size_of_data += p->print(second());
    size_of_data += p->print(',');
    size_of_data += p->print(GPSDriver::get_latitude(), 6);
    size_of_data += p->print(',');
    size_of_data += p->print(GPSDriver::get_longitude(), 6);
    size_of_data += p->print(',');
    size_of_data += p->print(GPSDriver::get_altitude(), 6);
    size_of_data += p->print(',');
    size_of_data += p->print(Device::id);
    size_of_data += p->print(',');
    size_of_data += p->print(GPSDriver::get_gps_st()*2+_readSt);    // devstatus
    size_of_data += p->print(',');
    size_of_data += p->print(get_id());
    size_of_data += p->print(',');
    size_of_data += p->println(_value, 2);
    print_debug(F("[SensorData] toCSV - size_of_data: %d"), size_of_data);
    return size_of_data;
}

//---------------------------


/*--------Variable---------*/

void Variable::identify_sense(SensorData* data)
{
    data->set_id(_id);
    data->set_timestamp(now());
    data->set_coordinates(GPSDriver::get_latitude(), GPSDriver::get_longitude());
    data->setReadSt(_readSt);
}

//---------------------------


/*-------AnalogInput-------*/

void AnalogInput::sense(SensorData* data)
{
    print_debug(F("[AnalogInput] sense:"));
    if      (_unit == SI_AnalogV)   data->set_value(_var.getAve()/1000);
    else if (_unit == SI_AnalogmV)  data->set_value(_var.getAve());
    else    data->set_value(0.0);
    identify_sense(data);
}

//----------------------------


/*-----GasConcentration-----*/

void GasConcentration::sense(SensorData* data)
{
    print_debug(F("[GasConcentration] sense:"));
    if      (_unit == SI_CONC_ppb)  data->set_value(_var.getAve());
    else if (_unit == SI_CONC_ppm)  data->set_value(_var.getAve()/1000);
    else if (_unit == SI_CONC_ug)   data->set_value(_var.getAve());
    // else if (_unit == SI_CONC_ug)
    // {

    // }
    else    data->set_value(0.0);
    identify_sense(data);
}

//----------------------------


/*--------Temperature--------*/

void Temperature::sense(SensorData* data)
{
    print_debug(F("[Temperature] sense:"));
    if      (_unit == SI_TEMP_Farenheit)data->set_value(_var.getAve()* 9/5 + 32);
    else if (_unit == SI_TEMP_Celsius)  data->set_value(_var.getAve());
    else    data->set_value(0.0);
    identify_sense(data);
}

//----------------------------

/*----------Humidity----------*/

void Humidity::sense(SensorData* data)
{
    print_debug(F("[Humidity] sense:"));
    data->set_value(_var.getAve());
    identify_sense(data);
}

//----------------------------

/*----------Pressure----------*/

void Pressure::sense(SensorData* data)
{
    print_debug(F("[Pressure] sense:"));
    data->set_value(_var.getAve());
    identify_sense(data);
}

//----------------------------