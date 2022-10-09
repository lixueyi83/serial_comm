/**
 * @file main.cpp
 * 
 * @author: Xueyi Li
 * @copyright Copyright &copy; 10/04/22 BD Biosciences. All Rights Reserved.
 */

#include "SerialPort.hpp"
#include "TecControl.hpp"

using namespace CppLinuxSerial;

std::string write_integral_gain = "*001e000000dcdd\r";   // returns b'*000000dce7^', 0x00dc = 220, 220/100 = 2.2
std::string  read_integral_gain = "*00520000000047\r";   // return b'*000000dce7^'     
void tec_test()
{
	// Create serial port object and open serial port at B9600 buad,
    // 8 data bits, no parity bit, and one stop bit (8n1)
	SerialPort serialPort(
            "/dev/ttyS12",
            BaudRate::B_9600,
            NumDataBits::EIGHT,
            Parity::NONE,
            NumStopBits::ONE
    );

	// Use SerialPort serialPort("/dev/ttyS12", 13000); instead if you want to provide a custom baud rate

	serialPort.SetTimeout(-1); // Block when reading until any data is received
	serialPort.Open();

    std::string readData;

    write_integral_gain = "*001e000000dcdd\r"; 
	serialPort.Write(write_integral_gain);
	serialPort.Read(readData);
    std::cout << "read: " << readData << std::endl;

    serialPort.Write(read_integral_gain);
    serialPort.Read(readData);
    std::cout << "read: " << readData << std::endl;

	// Close the serial port
	serialPort.Close();
}

int main()
{
    // tec_test(); return 0;

    TecControlProtocol::TecControl uart; 
	std::cout << uart.ReadTec(TecControlProtocol::IntegralGain) << std::endl;
	std::cout << uart.WriteTec(TecControlProtocol::IntegralGain, 2.2) << std::endl;

    return 0;
}

