all:
	/usr/bin/arm-linux-gnueabihf-g++ --std=c++11 -o Serial SerialPort.hpp SerialPort.cpp Serial.cpp
