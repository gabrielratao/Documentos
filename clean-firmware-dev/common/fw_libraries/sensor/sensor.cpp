#include "sensor.h"
#include "debug-output.h"

Smoother::Smoother(uint8_t buff)
{ 
    BUFFER_SIZE = buff;
    readings = (double*)malloc(BUFFER_SIZE*sizeof(double));
    status = (uint8_t*)malloc(BUFFER_SIZE*sizeof(uint8_t));
    reset_buffer(); 
}

Measurement Smoother::invalidate_buffer() 
{ 
    zero_buffer(); 
    sum = BUFFER_SIZE*NAN_CONSTANT; 
    readIndex = 0; 
    full = true; 
    readSt = NOT_READY;
    return Measurement(NAN_CONSTANT, NOT_READY);
}

double Smoother::getAve() 
{   
    if(!full && !readIndex)  return 0.0; // Avoids 0.0/0.0 = nan
    return sum /((full) ? BUFFER_SIZE : readIndex); 
}

uint8_t Smoother::filter_status(uint8_t new_status)
{
    uint8_t stSum = 0;
    for(uint8_t i = 0; i < BUFFER_SIZE; i++, stSum += status[i]);
    readSt = (new_status*BUFFER_SIZE == stSum) ? new_status : readSt;
    return readSt;
}

Measurement Smoother::smooth(double new_value, uint8_t new_status)
{
    if (new_value <= NAN_CONSTANT)  { return invalidate_buffer(); }
    if (sum <= NAN_CONSTANT)        sum = 0.0;
    sum -= readings[readIndex];
    readings[readIndex] = new_value;
    status[readIndex] = new_status;
    sum += readings[readIndex];
    if(++readIndex >= BUFFER_SIZE)    { full = true; readIndex = 0; }
    
    return Measurement(getAve(), 
        (ReadingStatus)filter_status(new_value));
}

uint8_t specDGS_sensor::sleep ()
{
    if(!_serial->set_to_sleep(this, _mySerial)) return 0;

    return 1;
}

uint16_t  specDGS_sensor::parse_dataString (String str )
{
    uint16_t size = str.length();
    if(size >= MAX_STR_SIZE)    return 0;
    
    // _last_str = str;
    String tempString = str;
    dataArray[0] = tempString.substring(7, tempString.indexOf(',')).toInt();
    
    tempString = tempString.substring(tempString.indexOf(',') + 2);
    
    for (int i = 1; i < 11; i++) 
    {
        String subS = tempString.substring(0, tempString.indexOf(','));
        if (subS.length() <= 0) return size;
        dataArray[i] = subS.toInt();
        tempString = tempString.substring(tempString.indexOf(',') + 2);
    }

    size = sizeof(dataArray);
    
    return size;
}

uint32_t specDGS_sensor::readSensor(Smoother& conc, Smoother& temp, Smoother& hum)
{
    print_debug("[specDGS_sensor] readSensor():");

    dataArray[0] = 0;
    double conc_val = NAN_CONSTANT;
    if (_serial->getData(this, _mySerial) && (uint32_t)dataArray[0] == _id)
    {
        if(!heatStarted())  startHeat();
        
        conc_val = dataArray[1]; 
        temp.smooth (dataArray[2], heatState());
        hum.smooth  (dataArray[3], heatState());
    }
    conc.smooth (conc_val, heatState());
    
    print_debug(F("[specDGS_sensor] readSensor() OK"));
    
    return dataArray[0]; 
}