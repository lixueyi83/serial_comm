all: arm

arm:
	/usr/bin/arm-linux-gnueabihf-g++ --std=c++11 -o serial_arm SerialPort.hpp SerialPort.cpp main.cpp TecControl.hpp
	scp serial_arm axon:/mnt/emmc-fw/
	scp RW5R.py axon:/mnt/emmc-fw/

x64:
	g++ --std=c++11 -o main_x64 SerialPort.hpp SerialPort.cpp main.cpp TecControl.hpp
	
