/**
 * @file main.cpp
 * 
 * @author: Xueyi Li
 * @copyright Copyright &copy; 10/04/22 BD Biosciences. All Rights Reserved.
 */


#include "SerialPort.hpp"

using namespace CppLinuxSerial;

static char write_integral_gain[18] = "*001e000000dcdd\r";   // 0x00dc = 220, 220/100 = 2.2
static char  read_integral_gain[18] = "*00520000000047\r";   // return b'*000000dce7^'     


int main()
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
    // Read data (will block until at least 1 byte is received due to the SetTimeout(-1) call above)
	serialPort.Write(write_integral_gain);
	serialPort.Read(readData);

	serialPort.Write(read_integral_gain);
	serialPort.Read(readData);

	// Close the serial port
	serialPort.Close();
}
