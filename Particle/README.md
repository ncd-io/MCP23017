# Usage

This library provides a class that reads and controls the MCP23017. The MCP23017.ino file serves as an example use case.

Create an instance of the class
```cpp
MCP23017 RelayController;
```

Set the address if the address jumpers are used, this is only necessary if you are using more than one chip, either multiple 16 channel boards, or a 32 channel board.
The method accepts 3 arguments that are mapped to the address pins as printed on the board: A0, A1, A2
```cpp
RelayController.setAddress(1,0,0);
```

Set the number of relays on the board, this method will call the `setOutputs` method on the first n channels.
```cpp
RelayController.setRelays(16);
```

Set which inputs should be pulled up (if any)
This method expects two ints that are essentially bit maps, each bit represents a channel, the set bits indicate the channel should be pulled high.
```cpp
RelayController.setInputs(0, 128);
```

Initialize the controller
```cpp
RelayController.init();
```

Basic relay control functions
```cpp
RelayController.turnOnRelay(1);
RelayController.turnOffRelay(1);
RelayController.toggleRelay(2);
RelayController.turnOnAllRelays();
RelayController.turnOffAllRelays();
```

Read statuses. Bank statuses are, again, bit maps. Set bits of output banks indicate that a relay is active, set bits of an input indicate the input is high/connected
```cpp
relayController.readStatus();
int b1status = relayController.bankOneStatus;
int b2status = relayController.bankTwoStatus;
int inputStatus = relayController.readAllInputs();
```

This library also features a "relayTalk" method. It allows some basic "human readable" phrases to control relays when sent as a string.

```cpp
RelayController.relayTalk("turn on relay 1");
RelayController.relayTalk("turn off relay 1");
RelayController.relayTalk("toggle relay 1");
RelayController.relayTalk("turn on all relays");
RelayController.relayTalk("turn on off all relays");
```
