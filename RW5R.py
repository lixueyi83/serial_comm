#!/usr/bin/python3
# Oven Industries Temperature Controller
import serial

#pymon compliance
base = object
try:
    from genericDevice import ExternalDevice
    base = ExternalDevice
except:
    pass
def build_devices():
    return {"rw5r": [RW5R('/dev/ttyS12')]}


class RW5R(base):

    def __init__(self,serialPort,debug=1):
        super().__init__()
        self.debug = debug
        self.ser = serial.Serial(serialPort, 9600, timeout=1);
        self.ser.reset_input_buffer();

    def __del__(self):
        self.ser.close()

    def checkSum(self,s):
        return '{:02x}'.format(sum(list(s))%256).encode()

    def rw(self,s,i=0):
        command=b"00"+'{:02x}{:08x}'.format(s%256,i).encode()
        if self.debug: print(b"*"+command+self.checkSum(command)+b"\r")
        self.ser.write(b"*"+command+self.checkSum(command)+b"\r")
        response=self.ser.read_until(b'^')
        if self.debug: print(response)
        return int(response[1:9],16)

    @property
    def input1(self):
        return self.rw(0x01)/100

    @property
    def desired_control_value(self):
        return self.rw(0x03)/100

    @property
    def power_output(self):
        return self.rw(0x04)/255

    @property
    def alarm_status(self):
        return self.rw(0x05)

    @property
    def alarm_type(self):
        return self.rw(0x41)
        
    @alarm_type.setter
    def alarm_type(self,val):
        self.rw(0x28,val)

    @property
    def control_type(self):
        return self.rw(0x44)
        
    @control_type.setter
    def control_type(self,val):
        self.rw(0x2b,val)

    @property
    def control_output_polarity(self):
        return self.rw(0x45)
        
    @control_output_polarity.setter
    def control_output_polarity(self,val):
        self.rw(0x2c,val)

    @property
    def power_on(self):
        return self.rw(0x46)
        
    @power_on.setter
    def power_on(self,val):
        self.rw(0x2d,val)

    @property
    def output_shadow_if_alarm(self):
        return self.rw(0x47)
        
    @output_shadow_if_alarm.setter
    def output_shadow_if_alarm(self,val):
        self.rw(0x2e,val)

    @property
    def fixed_desired_control_setting(self):
        return self.rw(0x50)/100
        
    @fixed_desired_control_setting.setter
    def fixed_desired_control_setting(self,val):
        self.rw(0x1c,int(val*100))

    @property
    def proportional_bandwidth(self):
        return self.rw(0x51)/100
        
    @proportional_bandwidth.setter
    def proportional_bandwidth(self,val):
        self.rw(0x1d,int(val*100))

    @property
    def integral_gain(self):
        return self.rw(0x52)/100
        
    @integral_gain.setter
    def integral_gain(self,val):
        self.rw(0x1e,int(val*100))

    @property
    def derivative_gain(self):
        return self.rw(0x53)/100
        
    @derivative_gain.setter
    def derivative_gain(self,val):
        self.rw(0x1f,int(val*100))

    @property
    def alarm_deadband(self):
        return self.rw(0x56)
        
    @alarm_deadband.setter
    def alarm_deadband(self,val):
        self.rw(0x22,val)

    @property
    def high_alarm_setting(self):
        return self.rw(0x57)/100
        
    @high_alarm_setting.setter
    def high_alarm_setting(self,val):
        self.rw(0x23,int(val*100))

    @property
    def low_alarm_setting(self):
        return self.rw(0x58)/100
        
    @low_alarm_setting.setter
    def low_alarm_setting(self,val):
        self.rw(0x24,int(val*100))

    @property
    def control_deadband_setting(self):
        return self.rw(0x59)/100
        
    @control_deadband_setting.setter
    def control_deadband_setting(self,val):
        self.rw(0x25,int(val*100))

    @property
    def reference(self):
        return self.rw(0x5a)/100
        
    @reference.setter
    def reference(self,val):
        self.rw(0x26,int(val*100))

    @property
    def input1_offset(self):
        return self.rw(0x5b)
        
    @input1_offset.setter
    def input1_offset(self,val):
        self.rw(0x27,val)

    @property
    def alarm_latch_enable(self):
        return self.rw(0x48)
        
    @alarm_latch_enable.setter
    def alarm_latch_enable(self,val):
        self.rw(0x2f,val)

    @property
    def control_timebase(self):
        return self.rw(0x49)
        
    @control_timebase.setter
    def control_timebase(self,val):
        self.rw(0x30,val)

    @property
    def alarm_latch_reset(self):
        return 0
        
    @alarm_latch_reset.setter
    def alarm_latch_reset(self,val):
        self.rw(0x33,val)

    @property
    def heat_multiplier(self):
        return self.rw(0x5c)/100
        
    @heat_multiplier.setter
    def heat_multiplier(self,val):
        self.rw(0x0c,int(val*100))

    @property
    def temperature_working_units(self):
        return self.rw(0x4b)
        
    @temperature_working_units.setter
    def temperature_working_units(self,val):
        self.rw(0x32,val)



if __name__ == "__main__":

    controller = RW5R('/dev/ttyS12')

    # get four temp reads
    # for p in range (0,4):
        # print (controller.input1)

    # send the set temperature: 25.01
    # controller.fixed_desired_control_setting = 25.01
    # 25.01 verify
    # print (controller.fixed_desired_control_setting);

    integral_gain = controller.integral_gain
    print(integral_gain)
    controller.integral_gain = integral_gain + 3.1
    print(controller.integral_gain)
    controller.integral_gain = integral_gain
    print(controller.integral_gain)
    print("!!!!!!!!!");
