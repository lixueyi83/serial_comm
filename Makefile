all:
	/usr/bin/arm-linux-gnueabihf-g++ --std=c++11 -o main_arm SerialPort.hpp SerialPort.cpp main.cpp TecControl.hpp
	g++ --std=c++11 -o main_x64 SerialPort.hpp SerialPort.cpp main.cpp TecControl.hpp
	scp serial_arm axon:/mnt/emmc-fw/
	
