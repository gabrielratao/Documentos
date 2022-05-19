#include "hard-storage.h"
#include "debug-output.h"

#include "Arduino.h"
#include <SPI.h>
#include <SdFat.h>

// #define LOG_FILENAME    "fileslog.csv"
// #define ERR_FILENAME    "errorlog.csv"
// #define EXTENSION       ".csv"
// #define HEAD1_FILE_STR   "Year,Month,Day,Hour,Min,Sec,Latitude,Longitude,Altitude,Device,DeviceSt,CO(ppb),NO2(ppb),O3(ppb),SO2(ppb),Temperature(C),Humidity(%)"
// #define HEAD2_FILE_STR   F("Year,Month,Day,Hour,Min,Sec,Latitude,Longitude,Altitude,Device,DeviceSt, SensorID,Value") // add unidade

// /*
void dateTime(uint16_t* date, uint16_t* tim) 
{
    // time_t t = time(nullptr);
    tmElements_t curr_dateTime;
    breakTime(now(), curr_dateTime);
    
    // return date using FAT_DATE macro to format fields
    *date = FAT_DATE(curr_dateTime.Year+1900, curr_dateTime.Month+1, curr_dateTime.Day);
    
    // return time using FAT_TIME macro to format fields
    *tim = FAT_TIME(curr_dateTime.Hour, curr_dateTime.Minute, curr_dateTime.Second);
}
// /*
size_t open_file(const char* file_name)
{
    print_debug(F("[FileHandler] open_file():"));
    size_t size_of_data= 0;
    
    if(!SD.exists((char*)file_name))
    {
        print_debug(F("[FileHandler] open_file() - File %s doesn't exists creating new one\n"), file_name);
        
        SdFile::dateTimeCallback(dateTime);
        File curr_file = SD.open(file_name, FILE_WRITE);
        if(!curr_file)  return 0;
        String head1 = HEAD_FILE_STR;
        size_of_data = curr_file.println(head1);
        curr_file.close();
    }
    else
    {
        SdFile::dateTimeCallback(dateTime);
        File curr_file = SD.open(file_name, FILE_WRITE);
        if(!curr_file)  return 0;
        size_of_data = curr_file.size();
        curr_file.close();
    }
            
    print_debug(F("[FileHandler] open_file() - RETURN: %d\n"), (int)size_of_data);
    return size_of_data;
}
// /*

void delete_json_file()
{
  if(SD.exists((char*)json_file_name)) SD.remove((char*)json_file_name);
}

size_t read_json_line(String& json_str)
{
  static size_t line_index = 0;

  if(!SD.exists((char*)json_file_name)) return 0;

  File file = SD.open(json_file_name, FILE_READ);

  if(!file.available() || file.size() <= line_index || !file.seek(line_index)){
    file.close();
    return 0;
  }

  json_str = file.readStringUntil(';');
  file.close();
  size_t size_of_data = json_str.length()+3;
  line_index += size_of_data;
  
  return size_of_data;
}

size_t save_to_json_file(const String& json_str)
{
    print_debug(F("[FileHandler] save_to_json_file(): %s\n"), json_file_name);
    
    SdFile::dateTimeCallback(dateTime);
    File file = SD.open(json_file_name, FILE_WRITE);
    if(!file)  return 0;
    
    file.seek(file.size());
    size_t size_of_data = file.print(json_str);
    size_of_data += file.println(';');
    file.close();   
    
    print_debug(F("[FileHandler] save_to_json_file() - size: %d\n"), size_of_data);
    return size_of_data;
}

size_t save_to_file(DataContainer* data, const char* file_name)
{
    print_debug(F("[FileHandler] save_to_file(): %s\n"), file_name);
    
    time_t t = now();
    if(t <= 626295408) return 0;
    if(!SD.exists((char*)file_name)) return 0;
    
    File file = SD.open(file_name, FILE_WRITE);
    if(!file || !file.seek(file.size())) return 0;
    
    size_t size_of_data = data->toCSV(&file);
    file.close();
    
    print_debug(F("[FileHandler] save_to_file() - size: %d\n"), size_of_data);
    return size_of_data;
}

// */
/*
size_t HardStorage::save_to_errorlog (String datetimeString, String errorString)
{
    //info(debbie, "[HardStorage] save_to_errorlog():\n", 1);
    size_t size_of_data = 0;
    
    SdFile::dateTimeCallback(dateTime);
    error_log = SD.open(ERR_FILENAME, FILE_WRITE);
    if(error_log)
    {
        //debug(debbie, "[HardStorage] save_to_errorlog() - Open File successfully: %s\n", error_log.name());
        
        uint32_t file_size = error_log.size();
        //verbose(debbie, "[HardStorage] save_to_errorlog() - Error File size: %d\n", (int)file_size);
        error_log.seek(file_size);
        
        String str = datetimeString + ',' + errorString;
        size_of_data = error_log.println(str);
        error_log.close();   
        
        //debug(debbie, "[HardStorage] save_to_errorlog() - Saved to Error File successfully! Data: %s\n", str.c_str());
    }
    else 
    {
        //error(debbie, "[HardStorage] save_to_errorlog() - Could not open file: %s!!!", ERR_FILENAME);
    }

    //info(debbie, "[HardStorage] save_to_errorlog() - RETURN: %d\n", (int)size_of_data);
    sd_ok = !(size_of_data <= 0);
    return size_of_data;
}
// */
/*
size_t HardStorage::open_file(String dateString)
{
    print_debug("[HardStorage] open_file():");
    // curr_filename = dateString + EXTENSION;
    curr_filename = "datalog" + String(EXTENSION);
    size_t size_of_data = 0;
    
    if(!SD.exists(curr_filename.c_str()))
    {
        print_debug("[HardStorage] open_file() - File doesn't %s exists creating new one\n", curr_filename.c_str());
    
        SdFile::dateTimeCallback(dateTime);
        files_log = SD.open(LOG_FILENAME, FILE_WRITE);
        if(files_log)
        {
            print_debug("[HardStorage] open_file() - Open Log File successfully: %s\n", files_log.name());
            
            SdFile::dateTimeCallback(dateTime);
            File curr_file = SD.open(curr_filename.c_str(), FILE_WRITE);
            if(curr_file)
            {
                print_debug("[HardStorage] open_file() - Open File successfully: %s\n", curr_file.name());
            
                String head1 = HEAD1_FILE_STR;
                size_of_data = curr_file.println(head1);
                curr_file.close();
                
                uint32_t file_size = files_log.size();
                files_log.seek(file_size);
                files_log.println(curr_filename);
                print_debug("[HardStorage] open_file() - File name saved to Log File successfully! Data: %s\n", curr_filename.c_str());
            }
            else    print_debug("[HardStorage] open_file() - Could not open file: %s!!!", curr_filename.c_str());
        }
        else    print_debug("[HardStorage] open_file() - Could not open file: %s!!!", LOG_FILENAME);
        
        files_log.close(); 
    }
    else{
        File curr_file = SD.open(curr_filename.c_str(), FILE_WRITE);
        if(curr_file)
        {
            curr_filesize = curr_file.size();
            size_of_data = curr_filesize;
            curr_file.close();
        }
        else size_of_data = 1;
    }    
    print_debug("[HardStorage] open_file() - RETURN: %d\n", (int)size_of_data);
    
    sd_ok = !(size_of_data <= 0);
    return size_of_data;
}
// */
