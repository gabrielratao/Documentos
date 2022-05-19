# The CLEAN ESP8266 Gateway firmware

The firmware for the [ESP8266](https://www.espressif.com/en/products/socs/esp8266) microcontroller was developed in C/C++ programming language using the [Arduino Framework](https://www.arduino.cc/). The code was programmed on the [PlatformIO IDE](https://docs.platformio.org/en/latest/home/index.html) for the [Microsoft Visual Studio Code editor](https://code.visualstudio.com/) (VSCode). For more details on programming the ESP8266 mounted on an ESP-01 module, its pinout, and how to connect the module to the Arduino MEGA, please refer to [The ESP-01 Module Programming Guide](https://lcqar.ufsc.br/novo/index.php/the-esp-01-programming-guide/).

The current version of the ESP8266’s firmware has three main functionalities, which are:

- Establishing a Wi-Fi connection to the Internet for the Arduino MEGA microcontroller or another master connected to the serial port.
- Sending the data collected by the master microcontroller to the RENOVAr web server as [HTTP](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol) posts.
- Getting the time from an [NTP Server](https://en.wikipedia.org/wiki/Network_Time_Protocol).

For more information on the firmware structure refer to [The ESP8266 Firmware](https://lcqar.ufsc.br/novo/index.php/the-esp8266-firmware/).
