

# About

This Library is intended for use with any MCP23017 Relay Controller or GPIO board available from ncd.io

###Developer information
NCD has been designing and manufacturing computer control products since 1995.  We have specialized in hardware design and manufacturing of Relay controllers for 20 years.  We pride ourselves as being the industry leader of computer control relay products.  Our products are proven reliable and we are very excited to support Particle.  For more information on NCD please visit ncd.io

###Requirements
- The Python SMBus Module: https://pypi.python.org/pypi/smbus-cffi/
- An I2C connection to an MCP23017 Chip
- Knowledge base for developing and programming with Python.

### Version
1.0.0

### How to use this library

The libary must be imported into your application. Create an SMBus object that to utilize the I2C bus. Create an MCP23017 object and pass it the SMBus object to start to communicate to the chip. You can optionally pass in a kwarg to the object at instantiation that acts as an output map.
This output map is a set of GPIO numbers that you would like to set as outputs. For example {0,1,2,3,6} would set channels
0, 1, 2, 3, and 6 as outputs. Any channel with a relay on it should be configured as an output.

###Public accessible methods
```cpp
set_gpio_high(bank, target_gpio)
```
>This function sets a GPIO to high in a particular bank. If this GPIO is configured as an output and has a relay on it the
>Relay will be turned on. The bank argument passed is an integer, either 1 or 2. The target_gpio argument passed is an
>ingteger from 0-7 with 0 being the first GPIO on the chain.


```cpp
set_gpio_low(bank, target_gpio)
```
>This function sets a GPIO to low. If this GPIO is configured as an output and has a relay on it the
>Relay will be turned off.
>The bank argument passed is an integer, either 1 or 2.
>The target_gpio argument passed is an ingteger from 0-7 with 0 being the first GPIO on the chain.


```cpp
toggle_gpio(bank, target_gpio)
```
>This method will toggle the given GPIO. If the GPIO is high then it will be set low. If it is low then it will be set high.
>The bank argument passed is an integer, either 1 or 2.
>The target_gpio argument passed is an ingteger from 0-7 with 0 being the first GPIO on the chain.

```cpp
set_gpio_high_by_index(target_gpio)
```
>This method will set the given GPIO high.
>The target_gpio argument passed to this method is an integer from 0 to 15 that indicates the position of the GPIO from the
>index of zero.
>If you would like simple control of relays without worrying about banks, this is the recommended way to do it.

```cpp
set_gpio_low_by_index(target_gpio)
```
>This method will set the given GPIO low.
>The target_gpio argument passed to this method is an integer from 0 to 15 that indicates the position of the GPIO from the
>index of zero.
>If you would like simple control of relays without worrying about banks, this is the recommended way to do it.

```cpp
toggle_gpio_by_index(bank, target_gpio)
```
>This method will toggle the given GPIO. If the GPIO is high then it will be set low. If it is low then it will be set high.
>The target_gpio argument passed to this method is an integer from 0 to 15 that indicates the position of the GPIO from the
>index of zero.
>If you would like simple control of relays without worrying about banks, this is the recommended way to do it.


```cpp
get_single_gpio_status(bank, target_gpio)
```
>A call to this method will get the status of a singular gpio on a particular bank.
>The bank argument passed is an integer, either 1 or 2.
>The target_gpio argument passed is an ingteger from 0-7 with 0 being the first GPIO on the chain.


```cpp
get_gpio_bank_status(bank)
```
>A call to this method will read the status of all GPIOs in a given bank in byte form.
>255 means all GPIOs are high. 0 means all GPIOs are low.
>If only GPIO 0 is high then the value will be 1, if only GPIO 3 is high the value will be 8.
>The bits in the byte would look like 00000001 (GPIO 0 is 1 aka high) and 00001000 (GPIO 0 is 1 aka high)


```cpp
pull_up_gpio(target_gpio)
```
>This method pulls up the target GPIO. This is the most common state needed in GPIO input needs. This state will allow you
>to connect a simple switch or contact closure output sensor to your GPIO and reliably read it.
