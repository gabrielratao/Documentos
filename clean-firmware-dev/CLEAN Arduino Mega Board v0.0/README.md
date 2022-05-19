# The CLEAN Arduino MEGA Board firmware
The firmware of the mobile prototype was developed for the [Microchip ATMega2560 microcontroller](http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega640-1280-1281-2560-2561-Datasheet-DS40002211A.pdf) embedded on an Arduino Mega platform. The code was developed in C/C++ programming language using the [Arduino Framework](https://www.arduino.cc/), available at the [PlatformIO IDE](https://docs.platformio.org/en/latest/home/index.html) for the [Microsoft Visual Studio Code editor](https://code.visualstudio.com/)(VSCode). For more details on programming the Arduino MEGA, please refer to [The Arduino MEGA Programming Guide](https://lcqar.ufsc.br/novo/index.php/the-arduino-mega-programming-guide/).

The code consists of two parts; one for setup and the other for the main loop. The current version of the firmware has three main functionalities which are in accordance with the [hardware of the monitor](https://lcqar.ufsc.br/novo/index.php/the-clean-fixed-monitor-prototype/), as follows:

- Reading the gas samples from the serial interface of the SPEC sensors and the analog output signal from the Alphasense sensors
- Storing the gas concentration information into an SD card
- Sending the gas concentration information to an HTTP server through the ESP8266 microcontroller

A detailed description of the hardware components of this prototype and the way they connect to the Arduino platform can be found at [The CLEAN Fixed Monitor Prototype](https://lcqar.ufsc.br/novo/index.php/the-clean-fixed-monitor-prototype/) and [The CLEAN Fixed Monitor Prototype Mounting Guide](https://lcqar.ufsc.br/novo/index.php/the-clean-fixed-monitor-mounting-guide/) For more information on the firmware strcuture refer to [The CLEAN Fixed Monitor Prototype Firmware](https://lcqar.ufsc.br/novo/index.php/the-clean-fixed-monitor-prototype-firmware/).
