#include "spark_wiring_i2c.h"
#include "spark_wiring_usbserial.h"
#include "spark_wiring_constants.h"

class MCP23017{
public:
    //Constructor
    MCP23017(void);
    //Set Address.  Indicate status of jumpers on board.  Send 0 for not installed, send 1 for installed
    void setAddress(int a0, int a1, int a2);
    void setAddress(int a);
    //Set number or outputs, for relay boards they will start at 0, if you need outputs at specific locations
    //use the setAsOutput method for each
    
    void setRelays(int num);
    void setOutputs(int map1, int map2);
    void setOutput(int num);
    
    void setInputs(int map1, int map2);
    
    void init();
    //Turn on Relay
    void turnOnRelay(int relay);
    //Turn off Relay
    void turnOffRelay(int relay);
    //Toggle Relay
    void toggleRelay(int relay);
    
    int relayTalk(String command);
    //Turn On all Relays
    void turnOnAllRelays();
    //Turn Off All Relays
    void turnOffAllRelays();
    //Set status of all relays in bank
    void setBankOneStatus(int status);
    void setBankTwoStatus(int status);
    //Read status of relay. Valid return 0 for off 1 for on.  256 returned if there is an error
    int readRelayStatus(int relay);
    //Read status of all relays in bank.  0-255 valid.  256 returned on error
    int readRelayBankStatus();
    //Read status of input
    int readInputStatus(int input);
    //Read status of all inputs
    int readAllInputs();
    int bankOneStatus;
    //Status of relays in bank 2
    int bankTwoStatus;
    //User Accessible Variables
    //Whether or not the controller is ready to accept commands
    bool initialized;
    
    void readStatus();
    
private:
    byte bitop(byte b1, byte b2, int op);
    
    void relayOp(int relay, int op);
    //internal use method for refreshing bank status variables

    int outputMap[2] = {255, 255};
    int inputMap[2] = {0, 0};
    bool iosetCustom = false;
    int relayCount = 0;

    int address = 0x20;
    int retrys = 0;
    byte outputRegister = 0x0A;
    int sendCommand(int reg, int cmd);
    int sendCommand(int reg);
    int sendCommand(int reg, int cmds[], int length);
    int sendCommand(int reg, int cmds[], int length, int rlen);
};
