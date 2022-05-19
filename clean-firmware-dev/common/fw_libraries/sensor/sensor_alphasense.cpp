#include "sensor_alphasense.h"
#include "debug-output.h"

uint32_t AlphasenseOPC::readSensor(Smoother* pm10, Smoother* pm2_5, Smoother* pm1, Smoother* temp, Smoother* hum)
{
    print_debug("[AlphasenseOPC] readSensor():");

    HistogramData hist = _spiSensor->readHistogramData();

    print_debug(F("[AlphasenseOPC] readSensor(): %d"), hist.pm10);
    print_debug(F("[AlphasenseOPC] readSensor(): %d"), hist.pm2_5);
    print_debug(F("[AlphasenseOPC] readSensor(): %d"), hist.pm1);

    pm10->smooth (((hist.pm10 <=0) ? 0 : hist.pm10), READY);
    pm2_5->smooth (((hist.pm2_5 <=0) ? 0 : hist.pm2_5), READY);
    pm1->smooth (((hist.pm1 <=0) ? 0 : hist.pm1), READY);
    temp->smooth (((hist.getTempC() <=0) ? 0 : hist.getTempC()), READY);
    hum->smooth  (((hist.getHumidity() <=0) ? 0 : hist.getHumidity()), READY);
    print_debug(F("[AlphasenseOPC] readSensor() OK"));
    
    return _id; 
}

double AlphaSenseCompensator::readConc_Comp()  
{ 
    double temperature = _tempSensor->read().value;
    return config.sensitivity*comp_algorithm(config.type, 
    temperature, calc_WE(), calc_AE(), config.WEo, config.AEo); 
}

double AlphaOXCompensator::readConc_Comp()
{ 
    double temperature = _tempSensor->read().value;
    return config.sensitivity*comp_algorithm(config.type, temperature, 
        calc_WE()-read_NO2(), calc_AE(), config.WEo, config.AEo); 
}