/**
 * @file main.cpp
 * 
 * @author: Xueyi Li
 * @copyright Copyright &copy; 10/04/22 BD Biosciences. All Rights Reserved.
 */


#include "SerialPort.hpp"
#include "TecControl.hpp"

using namespace CppLinuxSerial;

void test_construct_command()
{
    std::cout << __func__ << ":" << std::endl;
    std::cout << WriteCommandStr(0x1e, 220) << std::endl;
    std::cout << ReadCommandStr(0x52) << std::endl;
}

void test_checksum(void)
{
    std::cout << __func__ << ":" << std::endl;
    std::cout << checksum("001e000000dc") << std::endl; // dd
    std::cout << checksum("005200000000") << std::endl; // 47
    std::cout << checksum("1e000000dc") << std::endl;   // 7d
    std::cout << checksum("5200000000") << std::endl;   // e7
}

void test_int8ascii(void)
{
    std::cout << __func__ << ":" << std::endl;
    char ret[9] = "\0";
    int2ascii(220, ret);
    std::cout << std::string(ret) << std::endl;
}

void test_ascii2int()
{
    std::cout << __func__ << ":" << std::endl;
    std::cout << ascii2int("*000000dce7^") << std::endl; // 220
    std::cout << ascii2int("*000002268a^") << std::endl; // 550
}

int main_test()
{
    test_checksum();
    test_int8ascii();
    test_ascii2int();
    test_construct_command();
    return 0;

    // Use SerialPort serialPort("/dev/ttyS12", 13000); instead if you want to provide a custom baud rate
	SerialPort serialPort(
            "/dev/ttyS12",
            BaudRate::B_9600,
            NumDataBits::EIGHT,
            Parity::NONE,
            NumStopBits::ONE
    );

	serialPort.SetTimeout(-1); // Block when reading until any data is received
	serialPort.Open();

    std::string write_integral_gain = "*001e000000dcdd\r";   // 0x00dc = 220, 220/100 = 2.2
    std::string  read_integral_gain = "*00520000000047\r";   // return b'*000000dce7^'     
    std::string readData;

    write_integral_gain = "*001e000000dcdd\r";
	serialPort.Write(write_integral_gain);
	serialPort.Read(readData);

	serialPort.Write(read_integral_gain);
	serialPort.Read(readData);

	// Close the serial port
	serialPort.Close();
}

int main()
{
    TecControl uart; 

    std::cout << uart.ReadTec(TecControlTable[Input1].read) << std::endl;
    std::cout << uart.WriteTec(TecControlTable[Input1].write, 2.2) << std::endl;
}

