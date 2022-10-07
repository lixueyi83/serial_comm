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

    std::cout << uart.ReadTec(IntegralGain) << std::endl;
    std::cout << uart.WriteTec(IntegralGain, 2.2) << std::endl;
}

