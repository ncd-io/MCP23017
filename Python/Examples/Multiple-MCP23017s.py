#import time to allow some processing sleep. Not Required.
import time
from mcp23017 import mcp23017
import smbus

# Get I2C bus, this is I2C Bus 1
bus = smbus.SMBus(1)

#define which GPIOs are to be used as outputs. By default all GPIOs are defined as inputs.
#because MCP23017 chips have two banks of 8 GPIOs we can optionally pass two sets to the object constructor
#the first set in the array will be for bank 0 and the second will be for bank 1.
#even if only bank 0 has input it still must be passed in an array i.e. [{0,1,2,3}]
#in each set the numbers passed will be the number of the GPIO that should be set as outputs i.e. 0 is the first GPIO 1 is the second
gpio_output_map =  [{0,1,2,3},{0,1,2,3}]
#kwargs is a Python set that contains the address of your device and the output map to be passed to the object for initialization.
kwargs = {'address': 0x20, 'gpio_output_map': gpio_output_map}
#create the MCP23017 object from the MCP23017 library and pass it the GPIO output map and address defined above
#the object requires that you pass it the bus object so that it can communicate and share the bus with other chips if necessary
mcp23017_1 = mcp23017(bus, kwargs)

#Now I will define a second MCP23017 chips so that I can send commands to it.
#The address of the second MCP23017 will automatically be one above the address configured for the first MCP23017
#This means that if you keep the first MCP23017 at the default address of 0x20 (byte value 32) then the address of the second MCP23017
#    will be 0x21 (byte value 33)



#This map sets all of bank 1 as outputs and leaves bank 2 in the default input state.
gpio_output_map =  [{0,1,2,3,4,5,6,7}, {0,1,2,3,4}]
kwargs = {'address': 0x21, 'gpio_output_map': gpio_output_map}
mcp23017_2 = mcp23017(bus, kwargs)

 
 
while True :
    #turn on Relay at GPIO 0 in bank 1 on chip 1
    mcp23017_1.set_gpio_high(1, 0)
    time.sleep(.2)
    #turn on gpio at index 11 on chip 1
    mcp23017_1.set_gpio_high_by_index(11)
    time.sleep(.2)
    #turn off gpio at index 11 on chip 1
    mcp23017_1.set_gpio_low_by_index(11)
    time.sleep(.2)
    turn off Relay at GPIO 0 in bank 1 on chip 1
    mcp23017_1.set_gpio_low(1, 0)
    time.sleep(.2)
    #toggle Relay at GPIO 1 in bank 2 on chip 1
    mcp23017_1.toggle_relay(2, 1)
    time.sleep(.2)
    #toggle Relay at GPIO 2 in bank 2 on chip 2
    mcp23017_2.toggle_relay(2, 2)
    time.sleep(.2)
    #toggle Relay at GPIO 2 in bank 2 on chip 2
    mcp23017_2.toggle_relay(2, 2)
    time.sleep(.2)
    #toggle Relay at GPIO 2 in bank 1 on chip 2
    mcp23017_2.toggle_relay(1, 2)
    time.sleep(.2)
    #toggle Relay at GPIO 2 in bank 1 on chip 2
    mcp23017_2.toggle_relay(1, 2)
    time.sleep(.2)
    
    #the commands below print out the byte that indicates the status of the GPIOs.
    #255 means all GPIOs are high. 0 means all GPIOs are low.
    #if only GPIO 0 is high then the value will be 1, if only GPIO 3 is high the value will be 8
    #the bits in the byte would look like 00000001 (GPIO 0 is 1 aka high) and 00001000 (GPIO 0 is 1 aka high)
    #the argument passed is the bank you would like to get the status of.
    #bank status of bank 1 on chip 1
    print mcp23017_1.get_gpio_bank_status(1)
    #bank status of bank 2 on chip 1
    print mcp23017_1.get_gpio_bank_status(2)
    #bank status of bank 1 on chip 2
    print mcp23017_2.get_gpio_bank_status(1)
    #bank status of bank 2 on chip 2
    print mcp23017_2.get_gpio_bank_status(2)
