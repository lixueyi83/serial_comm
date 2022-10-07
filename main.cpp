/**
 * @file main.cpp
 * 
 * @author: Xueyi Li
 * @copyright Copyright &copy; 10/04/22 BD Biosciences. All Rights Reserved.
 */


#include "SerialPort.hpp"
#include "TecControl.hpp"

using namespace CppLinuxSerial;

int main()
{
    TecControl uart; 

	// std::cout << uart.ReadTec(IntegralGain) << std::endl;
	// std::cout << uart.WriteTec(IntegralGain, 2.2) << std::endl;
}

void SerialPortTest()
{
    std::string m_dev = "/dev/ttyS12";
    SerialPort serialPort(
            m_dev,
            BaudRate::B_9600,
            NumDataBits::EIGHT,
            Parity::NONE,
            NumStopBits::ONE
    );
    serialPort.SetTimeout(-1); // Block when reading until any data is received
    serialPort.Open();
}
