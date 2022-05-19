#ifndef HARDSTORAGE_H
#define HARDSTORAGE_H

#include "drivers.h"
#include <SD.h>

#define CHIPSEL_PIN 53
#define json_file_name "read.txt"
#define HEAD_FILE_STR   F("Year,Month,Day,Hour,Minute,Second,Latitude,Longitude,Altitude,Device,DeviceSt, SensorID,Value") // add unidade

void dateTime(uint16_t* date, uint16_t* tim);

size_t open_file(const char* file_name);

void delete_json_file();

size_t read_json_line(String& json_str);

size_t save_to_json_file(const String& json_str);

size_t save_to_file(DataContainer* data, const char* file_name);

#endif
