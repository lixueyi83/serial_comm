/**
 * @file TecControl.hpp
 * 
 * @author: Xueyi Li
 * @copyright Copyright &copy; 10/05/22 BD Biosciences. All Rights Reserved.
 */

#ifndef TEC_CONTROL_H_
#define TEC_CONTROL_H_

#include <sstream>
#include <map>
#include <memory>
#include "SerialPort.hpp"

namespace TecControlProtocol {

enum
{
    Input1 = 1,
    DesiredControlValue = 2,
    PowerOutput = 3,
    AlarmStatus = 4,
    AlarmType = 5,
    ControlType = 6,
    ControlOutputPolarity = 7,
    PowerOnOff = 8,
    OutputShutdownIfAlarm = 9,
    FixedDesiredControlSetting = 10,
    ProportionalBandwidth = 11,
    IntegralGain = 12,
    DerivativeGain = 13,
    AlarmDeadband = 14,
    HighAlarmSetting = 15,
    LowAlarmSetting = 16,
    ControlDeadbandSetting = 17,
    Reference = 18,
    Input1Offset = 19,
    AlarmLatchEnable = 20,
    ControlTimebase = 21,
    AlarmLatchReset = 22,
    HeatMultlpier = 23,
    ChooseTempWorkingUnits = 24,
    TNA
};

struct TecControlTable_t
{
    int write;
    int read;
};

std::map<int, TecControlTable_t> TecControlTable
{
    { Input1,                      { TNA,    0x01 } }
    ,{ DesiredControlValue,         { TNA,    0x03 } }
    ,{ PowerOutput,                 { TNA,    0x04 } }
    ,{ AlarmStatus,                 { TNA,    0x05 } }
    ,{ AlarmType,                   { 0x28,   0x42 } }
    ,{ ControlType,                 { 0x2b,   0x44 } }
    ,{ ControlOutputPolarity,       { 0x2c,   0x45 } }
    ,{ PowerOnOff,                  { 0x2d,   0x46 } }
    ,{ OutputShutdownIfAlarm,       { 0x2e,   0x47 } }
    ,{ FixedDesiredControlSetting,  { 0x1e,   0x50 } }
    ,{ ProportionalBandwidth,       { 0x1d,   0x51 } }
    ,{ IntegralGain,                { 0x1e,   0x52 } }
    ,{ DerivativeGain,              { 0x1f,   0x52 } }
    ,{ AlarmDeadband,               { 0x22,   0x56 } }
    ,{ HighAlarmSetting,            { 0x23,   0x57 } }
    ,{ LowAlarmSetting,             { 0x24,   0x58 } }
    ,{ ControlDeadbandSetting,      { 0x25,   0x59 } }
    ,{ Reference,                   { 0x26,   0x5a } }
    ,{ Input1Offset,                { 0x27,   0x5b } }
    ,{ AlarmLatchEnable,            { 0x2f,   0x48 } }
    ,{ ControlTimebase,             { 0x30,   0x49 } }
    ,{ AlarmLatchReset,             { 0x33,   TNA  } }
    ,{ HeatMultlpier,               { 0x0c,   0x5c } }
    ,{ ChooseTempWorkingUnits,      { 0x32,   0x4b } }
};

using namespace CppLinuxSerial;

class TecControl
{
    public:
        TecControl()
            : m_dev("/dev/ttyS12")
            // , m_log("TecControlSerial.log")
        {
            try{
                m_serial = std::make_shared<SerialPort>(
                        m_dev,
                        BaudRate::B_9600,
                        NumDataBits::EIGHT,
                        Parity::NONE,
                        NumStopBits::ONE
                    );
                m_serial->SetTimeout(-1); // Block when reading until any data is received
                m_serial->Open();
                // m_serial->Read(m_response);
            } catch (...) {
                std::cout << "Open device \"" << m_dev << "\" failed" << std::endl;
            }
        }

        ~TecControl() { m_serial->Close(); }

        double ReadTec(int command)
        {
            std::cout << "cmd = " << command << std::endl;
            std::string cmd = ReadCommandStr(TecControlTable[IntegralGain].read);
            double ret = TecWriteRead(cmd);

            std::cout << "ret = " << ret << std::endl;
            switch(command)
            {
                case Input1:                    
                case DesiredControlValue:       
                case FixedDesiredControlSetting:
                case ProportionalBandwidth:     
                case IntegralGain:              
                case DerivativeGain:            
                case HighAlarmSetting:          
                case LowAlarmSetting:           
                case HeatMultlpier:             
                    ret /= 100.0;
                    break;

                case AlarmDeadband:             
                case PowerOutput:               
                case AlarmStatus:               
                case AlarmType:                 
                case ControlType:               
                case ControlOutputPolarity:     
                case PowerOnOff:                
                case OutputShutdownIfAlarm:     
                case ControlDeadbandSetting:    
                case Reference:                 
                case Input1Offset:              
                case AlarmLatchEnable:          
                case ControlTimebase:           
                case AlarmLatchReset:           
                case ChooseTempWorkingUnits:    
                    break;

                default:
                    break;
            }
            std::cout << "ret = " << ret << std::endl;

            return ret;
        }

        double WriteTec(int command, double data)
        {
            std::cout << "data = " << data << std::endl;
            switch(command)
            {
                case Input1:                    
                case DesiredControlValue:       
                case FixedDesiredControlSetting:
                case ProportionalBandwidth:     
                case IntegralGain:              
                case DerivativeGain:            
                case HighAlarmSetting:          
                case LowAlarmSetting:           
                case HeatMultlpier:             
                    data *= 100;
                    break;

                case AlarmDeadband:             
                case PowerOutput:               
                case AlarmStatus:               
                case AlarmType:                 
                case ControlType:               
                case ControlOutputPolarity:     
                case PowerOnOff:                
                case OutputShutdownIfAlarm:     
                case ControlDeadbandSetting:    
                case Reference:                 
                case Input1Offset:              
                case AlarmLatchEnable:          
                case ControlTimebase:           
                case AlarmLatchReset:           
                case ChooseTempWorkingUnits:    
                    break;

                default:
                    break;
            }

            std::cout << "data = " << data << std::endl;
            std::string cmd = WriteCommandStr(TecControlTable[command].write, data);
            std::cout << "datastr = " << cmd << std::endl;
            double ret = TecWriteRead(cmd);
            return ret;
        }

    private:
        std::string checksum(const std::string& data)
        {
            int cksum = 0;
            for(unsigned int i = 0; i < data.size(); i++)
                cksum += data[i];
            cksum %= 256;
            std::stringstream sstream;
            sstream << std::hex << cksum;
            return sstream.str();
        }

        std::string int2ascii(int data)
        {
            std::string ret = "00000000";
            std::stringstream ss;
            std::string dd;
            ss << std::hex << data;
            ss >> dd;

            int i = 8, j = dd.length();
            while(j-- && i--) ret[i] = dd[j];
            return ret;
        }

        void int2ascii(int lval, char *str)
        {
            unsigned int ans, leftover, tempb, tempd;

            ans = lval;
            while(*str) str++;
            tempb = 0x10000000;

            for (int i = 0; i<8; i++)
            {
                leftover = ans % tempb;
                tempd = (ans / tempb);
                if (tempd<10)
                    *str = tempd + '0';
                else
                    *str = (tempd - 10) + 'a';
                ans = leftover;
                tempb = tempb / 16; // shift right 4-bits
                str++;
            }
            *str = 0;
        }

        int ascii2int(const std::string& ascii)
        {
            int ret;
            try
            {
                std::string data = ascii.substr(1,8);
                std::stringstream ss;
                ss << std::hex << data;
                ss >> ret;
            }
            catch(...)
            {
                std::cout << "response ascii is invalid." << std::endl;
            }
            return ret;
        }

        std::string WriteCommandStr(int command, int data)
        {
            const std::string stx = "*";
            const std::string etx = "\r";
            std::string cc;
            std::stringstream ss;
            ss << std::hex << command;
            ss >> cc;
            cc = "00" + cc;
            auto dd = int2ascii(data);
            auto cs = checksum(cc + dd);
            std::string ret = stx + cc + dd + cs + etx;
            return ret;
        }

        std::string ReadCommandStr(int command)
        {
            const std::string stx = "*";
            const std::string etx = "\r";
            std::stringstream ss;
            std::string cc;
            ss << std::hex << command;
            ss >> cc;
            cc = "00" + cc;
            auto dd = int2ascii(0);
            auto cs = checksum(cc + dd);
            std::string ret = stx + cc + dd + cs + etx;
            return ret;
        }

    private:
        std::string m_dev;
        std::shared_ptr<SerialPort> m_serial;
        std::ofstream m_log;
        std::string m_response;

        int TecWriteRead(const std::string& cmd)
        {
            std::string datastr="";
            try{
                m_serial->Write(cmd);
                std::cout << __func__ << " write:" << cmd << std::endl;
                // std::cout << "Hex: "; for(auto e:cmd) std::cout << std::hex << int(e) << " "; std::cout << std::endl;

                m_serial->Read(datastr);
                std::cout << __func__ << "  read:" << datastr << std::endl;
                // std::cout << "Hex: "; for(auto e:datastr) std::cout << std::hex << int(e) << " "; std::cout << std::endl;
            } catch (...) {
                std::cout << "Write/Read device \" " << m_dev << "\" failed" << std::endl; 
            }
            int ret = ascii2int(datastr);
            std::cout << __func__ << ": result:   " << ret << std::endl;
            return ret;
        }
}; // class TecControl

} // namespace TecControlProtocol

#endif // TEC_CONTROL_H_
