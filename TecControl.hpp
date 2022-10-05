/**
 * @file TecControl.hpp
 * 
 * @author: Xueyi Li
 * @copyright Copyright &copy; 10/05/22 BD Biosciences. All Rights Reserved.
 */

#ifndef TEC_CONTROL_H_
#define TEC_CONTROL_H_

#include <sstream>

namespace TecControl {

static std::string checksum(const std::string& data)
{
    int cksum = 0;
    for(unsigned int i = 0; i < data.size(); i++)
        cksum += data[i];
    cksum %= 256;
    std::stringstream sstream;
    sstream << std::hex << cksum;
    return sstream.str();
}

static std::string int2ascii(int data)
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

static void int2ascii(int lval, char *str)
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

static int ascii2int(const std::string& ascii)
{
    int ret;
    std::string data = ascii.substr(1,8);
    std::stringstream ss;
    ss << std::hex << data;
    ss >> ret;
    return ret;
}

static std::string construct_write_command(int command, int data)
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
    return stx + cc + dd + cs + etx;
}

static std::string construct_read_command(int command)
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
    return stx + cc + dd + cs + etx;
}

}

#endif
