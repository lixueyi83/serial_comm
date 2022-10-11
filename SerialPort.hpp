/**
 * @file tec1_uart.hpp
 * 
 * @author: Xueyi Li
 * @copyright Copyright &copy; 10/10/22 BD Biosciences. All Rights Reserved.
 */

#ifndef MOSAIC_FIRMWARE_TEC1_UART_H
#define MOSAIC_FIRMWARE_TEC1_UART_H

#include <string>
#include <fstream> // For file I/O (reading/writing to COM port)
#include <sstream>

// #include <termios.h>        // POSIX terminal control definitions (struct termios)
// #include <asm/termios.h>    // Terminal control definitions (struct termios)

#include <vector>
#include <asm/ioctls.h>
#include <asm/termbits.h>

#include "Exception.hpp"

// SerialPort object is used to perform rx/tx serial communication.
class SerialPort
{
    public:
        enum class BaudRateType { STANDARD, CUSTOM, };
        // Specifies all the same baud rates as UNIX, as well as B_CUSTOM to specify your own.
        // See https://linux.die.net/man/3/cfsetispeed for list of supported UNIX baud rates.
        enum class BaudRate
        {
            B_0,    B_50,   B_75,   B_110,  B_134,   B_150,   B_200,   B_300,    B_600,    B_1200,
            B_1800, B_2400, B_4800, B_9600, B_19200, B_38400, B_57600, B_115200, B_230400, B_460800,
            B_CUSTOM, // Placeholder
        };
        enum class NumDataBits { FIVE, SIX, SEVEN, EIGHT, };
        enum class Parity { NONE, EVEN, ODD, };
        enum class NumStopBits { ONE, TWO, };
        enum class State { CLOSED, OPEN, }; // Represents the state of the serial port.

    public:
        SerialPort(); // Default constructor. You must specify at least the device before calling Open().
        SerialPort(const std::string &device, speed_t baudRate);
        SerialPort(const std::string &device, BaudRate baudRate);
        SerialPort(const std::string &device, BaudRate baudRate, NumDataBits numDataBits, Parity parity, NumStopBits numStopBits);
        virtual ~SerialPort();

        // Sets the device to use for serial port communications, and this method can be called when serial port is in any state.
        void SetDevice(const std::string &device);
        State GetState(); // Get the state of the serial port
        void SetBaudRate(BaudRate baudRate); // Call this to set a standard baud rate.
        void SetBaudRate(speed_t baudRate); // Call this to set a custom baud rate.
        void SetNumDataBits(NumDataBits numDataBits); // Call this to set the num. of data bits.
        void SetParity(Parity parity); // Call this to set the parity.
        void SetNumStopBits(NumStopBits numStopBits);

        // Sets the read timeout (in milliseconds)/blocking mode.
        // Only call when state != OPEN. This method manupulates VMIN and VTIME.
        // timeout_ms: Set to -1 to infinite timeout, 0 to return immediately with any data (non
        // blocking, or >0 to wait for data for a specified number of milliseconds). Timeout will be
        // rounded to the nearest 100ms (a Linux API restriction). Maximum value limited to 25500ms
        // (another Linux API restriction).
        void SetTimeout(int32_t timeout_ms);
        
        void SetEcho(bool value); // Enables/disables echo. Pass in true to enable echo, false to disable echo.
        void Open(); // Must call this before you can configure the COM port.
        void Close();
        void Write(const std::string& data);
        void WriteBinary(const std::vector<uint8_t>& data);
        void Read(std::string& data);
        void ReadBinary(std::vector<uint8_t>& data);
        int32_t Available(); // Get number of bytes available in receive buffer (ready to be read).

    private:
        void ConfigureTermios();    // Configures the tty device as a serial port. Device must be open (valid file descriptor) when this is called.
        // void SetTermios(termios myTermios);
        termios2 GetTermios2();     // Returns a populated termios2 structure for the serial port pointed to by the file descriptor.
        void SetTermios2(termios2 tty); // Assigns the provided tty settings to the serial port pointed to by the file descriptor.
        State state_;               // Keeps track of the serial port's state.
        std::string device_;        // The file path to the serial port device (e.g. "/dev/ttyUSB0").
        BaudRateType baudRateType_; // The type of baud rate that the user has specified.
        BaudRate baudRateStandard_; // The current baud rate if baudRateType_ == STANDARD.
        speed_t baudRateCustom_;    // The current baud rate if baudRateType_ == CUSTOM.
        NumDataBits numDataBits_ = NumDataBits::EIGHT; // The num. of data bits. Defaults to 8 (most common).
        Parity parity_ = Parity::NONE; // The parity. Defaults to none (most common).
        NumStopBits numStopBits_ = NumStopBits::ONE; // The num. of stop bits. Defaults to 1 (most common).
        int fileDesc_; // The file descriptor for the open file. This gets written to when Open() is called.
        bool echo_;
        int32_t timeout_ms_;
        std::vector<char> readBuffer_;
        unsigned char readBufferSize_B_;
        static constexpr BaudRate defaultBaudRate_ = BaudRate::B_57600;
        static constexpr int32_t defaultTimeout_ms_ = -1;
        static constexpr unsigned char defaultReadBufferSize_B_ = 255;
};

#endif //  MOSAIC_FIRMWARE_TEC1_UART_H
