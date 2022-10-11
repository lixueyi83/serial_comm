/**
 * @file main.cpp
 * 
 * @author: Xueyi Li
 * @copyright Copyright &copy; 10/04/22 BD Biosciences. All Rights Reserved.
 */

#include "SerialPort.hpp"
#include "TecControl.hpp"
#include <thread>

const int g_delay = 25;

int main()
{
    TecControl uart; 
    double data; 
    for(int i=0; i<50; i++)
    {
        data += 1.1;
        std::cout << "++++++ integral gain: " << data << " -> ";
        std::cout << uart.WriteTec(TecControl::IntegralGain, data) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(g_delay));

        std::cout << "------ integral gain: " << data << " -> ";
        std::cout << uart.ReadTec(TecControl::IntegralGain) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(g_delay));
    }

    return 0;
}

#if 0
void tec_test()
{
    std::string write_integral_gain = "*001e000000dcdd\r";   // returns b'*000000dce7^', 0x00dc = 220, 220/100 = 2.2
    std::string  read_integral_gain = "*00520000000047\r";   // return b'*000000dce7^'     

	// Create serial port object and open serial port at B9600 buad,
    // 8 data bits, no parity bit, and one stop bit (8n1)
	SerialPort serialPort(
            "/dev/ttyS12",
            SerialPort::BaudRate::B_9600,
            SerialPort::NumDataBits::EIGHT,
            SerialPort::Parity::NONE,
            SerialPort::NumStopBits::ONE
    );

	// Use SerialPort serialPort("/dev/ttyS12", 13000); instead if you want to provide a custom baud rate

	serialPort.SetTimeout(-1); // Block when reading until any data is received
	serialPort.Open();

    std::string readData;

    for(int i=0; i<10; i++)
    {
        // std::cout << "Write integral_gain: ";
        write_integral_gain = "*001e000000dcdd\r"; 
        serialPort.Write(write_integral_gain);
        std::this_thread::sleep_for(std::chrono::milliseconds(g_delay));
        serialPort.Read(readData);
        // std::cout << " write: " << write_integral_gain << ", read: " << readData << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(g_delay));

        // std::cout << "Read integral_gain:  ";
        read_integral_gain = "*00520000000047\r";
        serialPort.Write(read_integral_gain);
        std::this_thread::sleep_for(std::chrono::milliseconds(g_delay));
        serialPort.Read(readData);
        // std::cout << " write: " << read_integral_gain << ", read: " << readData << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(g_delay));
    }

#if 0
    write_integral_gain = "*001e000002127b\r"; 
	serialPort.Write(write_integral_gain);

    read_integral_gain = "*00520000000047\r";
    serialPort.Write(read_integral_gain);
    serialPort.Read(readData);
    std::cout << "read: " << readData << std::endl;

#endif
	// Close the serial port
	serialPort.Close();
}
#endif 
