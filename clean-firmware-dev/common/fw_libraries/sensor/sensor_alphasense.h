#ifndef _SENSOR_ALPHASENSE_H
#define _SENSOR_ALPHASENSE_H

#include <Stream.h>
#include "alphasense.h"
#include "debug-output.h"
#include "sensor.h"
#include "OPCN3.h"

class AlphasenseOPC
{
private:
    OPCN3*    _spiSensor;
    uint32_t  _id       ;
    
public:
    AlphasenseOPC(OPCN3* _sensor) { if (_sensor != nullptr)  _spiSensor = _sensor; }
    AlphasenseOPC(OPCN3* _sensor, uint32_t id) : _id(id)
    { if (_sensor != nullptr)  _spiSensor = _sensor; }
    AlphasenseOPC(OPCN3* _sensor, const char barcode[], uint32_t id)
      : _id(id)
    { 
      if (_sensor != nullptr)  _spiSensor = _sensor;
    }
    
    uint8_t   sleep       ();
    uint32_t  readSensor  (Smoother* pm10, Smoother* pm2_5, Smoother* pm1, Smoother* temp, Smoother* hum);
};

class AlphaSenseOPCPM10Sensor : public SensorInterface<OPCN3>
{
  public:
    AlphaSenseOPCPM10Sensor(OPCN3* sensor) : 
      SensorInterface<OPCN3>(sensor) {}
    
    virtual Measurement read() {
      HistogramData hist = _sensor->readHistogramData();
      double value = hist.pm10;
      // print_debug("[AlphaSenseOPCPM10Sensor] read(): %lf", value);
      Serial.print("[AlphaSenseOPCPM10Sensor] read(): ");
      Serial.println(value);
      return Measurement(((hist.pm10 <=0) ? 0 : hist.pm10), READY);
    }
};

class AlphaSenseOPCPM2_5Sensor : public SensorInterface<OPCN3>
{
  public:
    AlphaSenseOPCPM2_5Sensor(OPCN3* sensor) : 
      SensorInterface<OPCN3>(sensor) {}
    
    virtual Measurement read() {
      HistogramData hist = _sensor->readHistogramData();
      double value = hist.pm2_5;
      // print_debug("[AlphaSenseOPCPM2_5Sensor] read(): %lf", value);
      Serial.print("[AlphaSenseOPCPM2_5Sensor] read(): ");
      Serial.println(value);
      return Measurement(((hist.pm2_5 <=0) ? 0 : hist.pm2_5), READY);
    }
};

class AlphaSenseOPCPM1Sensor : public SensorInterface<OPCN3>
{
  public:
    AlphaSenseOPCPM1Sensor(OPCN3* sensor) : 
      SensorInterface<OPCN3>(sensor) {}
    
    virtual Measurement read() {
      HistogramData hist = _sensor->readHistogramData();
      double value = hist.pm1;
      // print_debug("[AlphaSenseOPCPM1Sensor] read(): %lf", value);
      Serial.print("[AlphaSenseOPCPM1Sensor] read(): ");
      Serial.println(value);
      return Measurement(((hist.pm1 <=0) ? 0 : hist.pm1), READY);
    }
};

class AlphaSenseOPCTempSensor : public SensorInterface<OPCN3>
{
  public:
    AlphaSenseOPCTempSensor(OPCN3* sensor) : 
      SensorInterface<OPCN3>(sensor) {}
    
    virtual Measurement read() {
      HistogramData hist = _sensor->readHistogramData();
      double value = hist.getTempC();
      // print_debug("[AlphaSenseOPCTempSensor] read(): %lf", value);
      Serial.print("[AlphaSenseOPCTempSensor] read(): ");
      Serial.println(value);
      return Measurement(((hist.getTempC() <=0) ? 0 : hist.getTempC()), READY);
    }
};

class AlphaSenseOPCHumSensor : public SensorInterface<OPCN3>
{
  public:
    AlphaSenseOPCHumSensor(OPCN3* sensor) : 
      SensorInterface<OPCN3>(sensor) {}
    
    virtual Measurement read() {
      HistogramData hist = _sensor->readHistogramData();
      double value = hist.getHumidity();
      // print_debug("[AlphaSenseOPCHumSensor] read(): %lf", value);
      Serial.print("[AlphaSenseOPCHumSensor] read(): ");
      Serial.println(value);
      return Measurement(((hist.getHumidity() <=0) ? 0 : hist.getHumidity()), READY);
    }
};

enum alphaKind  {   COB = 0, H2SB, NOB, NO2B, OXB, SO2B };
// /*
struct alphasenseISB_config
{
    alphaKind type;
    double sensitivity;    // Actually 1/Sensitivity
    double WEe, AEe, WEo, AEo;
    
    alphasenseISB_config(uint8_t typ, double s, 
        double we, double ae, double w0, double a0)
        : type((alphaKind)typ), sensitivity(s),
        WEe(we), AEe(ae), WEo(w0), AEo(a0) {}
};
// */

class AlphaSenseISB : public HeatSensor
{
protected:
    uint32_t _sensorId;
    alphasenseISB_config config;
    analog_sensor WE;
    double calc_WE()  { return WE.read_mV() - config.WEe; }
    
public:
    AlphaSenseISB(uint32_t id, alphaKind type, double s, double we, 
        double ae, double w0, double a0, uint8_t wPin, uint32_t secs = 3600)
      : HeatSensor(secs), _sensorId(id), config(type, s, we, ae, w0, a0), WE(wPin)  {
        startHeat();
      }
    double readConc() { return calc_WE()*config.sensitivity; }
    uint32_t getId() { return _sensorId; }
    uint8_t getType() { return (uint8_t)config.type; }
};

class AlphaSenseISBSensor : public SensorInterface<AlphaSenseISB>
{
  public:
    AlphaSenseISBSensor(AlphaSenseISB* sensor) : 
      SensorInterface<AlphaSenseISB>(sensor) {}
    
    virtual Measurement read() {
      double value = _sensor->readConc();
      // print_debug("[AlphaSenseISBSensor] read(): %lf", value);
      Serial.print("[AlphaSenseISBSensor] read(): ");
      Serial.println(value);
      return Measurement(_sensor->readConc(), _sensor->heatState());
    }  
};

class AlphaSenseCompensator : public AlphaSenseISB
{
protected:
    analog_sensor AE;
    Sensor* _tempSensor;
    double calc_AE()  { return AE.read_mV() - config.AEe; }
    
    double (*comp_algorithm)(uint8_t type, double T, double WE_term, double AE_term, double WEo, double AEo);
    
public:
    AlphaSenseCompensator(Sensor* tempSensor, double(*comp_alg)(uint8_t, double, double, double, double, double), 
        uint32_t id, alphaKind type, double s, double we, double ae, 
        double w0, double a0, uint8_t wPin, uint8_t aPin, uint32_t secs = 3600)
      : AlphaSenseISB(id, type, s, we, ae, w0, a0, wPin, secs), 
        AE(aPin), _tempSensor(tempSensor)
      { comp_algorithm = comp_alg;  }
    
    virtual double readConc_Comp();
};

class AlphaSenseCompensatorSensor : 
  public SensorInterface<AlphaSenseCompensator>
{
  public:
    AlphaSenseCompensatorSensor(AlphaSenseCompensator* sensor) : 
      SensorInterface<AlphaSenseCompensator>(sensor) {}
    
    virtual Measurement read() {
      double value = _sensor->readConc_Comp();
      // print_debug("[AlphaSenseCompensator] read(): %lf", value);
      Serial.print("[AlphaSenseCompensator] read(): ");
      Serial.println(value);
      return Measurement(_sensor->readConc_Comp(), _sensor->heatState());
    }  
};

class AlphaOXCompensator : public AlphaSenseCompensator
{
protected:
    Sensor* _no2Sensor;
    double read_NO2() { return _no2Sensor->read().value;  }

public:
    AlphaOXCompensator(Sensor* no2Sensor, Sensor* tempSensor,
        double(*comp_alg)(uint8_t, double, double, double, double, double), 
        uint32_t id, alphaKind type, double s, double we, double ae, 
        double w0, double a0, uint8_t wPin, uint8_t aPin, uint32_t secs = 3600)
        : AlphaSenseCompensator(tempSensor, comp_alg, id, type, s, we, ae, 
          w0, a0, wPin, aPin, secs), _no2Sensor(no2Sensor) {}

  virtual double readConc_Comp();
};

class AlphaOXCompensatorSensor : 
  public SensorInterface<AlphaOXCompensator>
{
  public:
    AlphaOXCompensatorSensor(AlphaSenseCompensator* sensor) : 
      SensorInterface<AlphaOXCompensator>(sensor) {}
    
    virtual Measurement read() {
      double value = _sensor->readConc_Comp();
      // print_debug("[AlphaOXCompensator] read(): %lf", value);
      Serial.print("[AlphaOXCompensator] read(): ");
      Serial.println(value);
      return Measurement(_sensor->readConc_Comp(), _sensor->heatState());
    }  
};

#endif