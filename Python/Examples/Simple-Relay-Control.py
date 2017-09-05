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
#the object requires that you pass it the bus object so that it can communicate and share the bus with other chips if necessary
#create the MCP23017 object from the MCP23017 library and pass it the GPIO output map and address defined above
#the object requires that you pass it the bus object so that it can communicate and share the bus with other chips if necessary
mcp23017 = mcp23017(bus, kwargs)
 
 
while True :
    #turn on Relay at GPIO 0 in bank 1 aka the first relay
    mcp23017.set_gpio_high(1, 0)
    time.sleep(.2)
    #turn off Relay at GPIO 0 in bank 1 aka the first relay
    mcp23017.set_gpio_low(1, 0)
    time.sleep(.2)
    #toggle Relay at GPIO 1 in bank 1 aka the second relay
    mcp23017.toggle_gpio(1, 1)
    time.sleep(.2)
    #toggle Relay at GPIO 2 in bank 2 aka the third relay
    mcp23017.toggle_gpio(2, 2)
    time.sleep(.2)
    #toggle Relay at GPIO 2 in bank 2 aka the third relay in bank 2
    mcp23017.toggle_gpio(2, 2)
    #print out the byte that indicates the status of the GPIOs.
    #255 means all GPIOs are high. 0 means all GPIOs are low.
    #if only GPIO 0 is high then the value will be 1, if only GPIO 3 is high the value will be 8
    #the bits in the byte would look like 00000001 (GPIO 0 is 1 aka high) and 00001000 (GPIO 0 is 1 aka high)
    #the argument passed is the bank you would like to get the status of.
    print mcp23017.get_gpio_bank_status(1)
    print mcp23017.get_gpio_bank_status(2)