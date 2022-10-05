all:
	/usr/bin/arm-linux-gnueabihf-g++ --std=c++11 -o serial_arm SerialPort.hpp SerialPort.cpp Serial.cpp TecControl.hpp
	g++ --std=c++11 -o serial_x64 SerialPort.hpp SerialPort.cpp Serial.cpp TecControl.hpp
