// This #include statement was automatically added by the Particle IDE.
#include "MCP23017.h"
MCP23017 relayController;
MCP23017 relayController2;

int b1status;
int b2status;
int b3status;
int b4status;


void setup() {
    
    relayController.setRelays(16);
    relayController.init();
    
    //If this is a 32 channel relay board, the A0 address jumper is ALWAYS set on the second chipset, so should never be set here on the first
    
    relayController2.setAddress(1,0,0);
    relayController2.setRelays(16);
    relayController2.init();
    
    Particle.function("controlC1", triggerRelay);
    Particle.function("controlC2", triggerRelay2);
    Particle.variable("Bank_1", b1status);
    Particle.variable("Bank_2", b2status);
    Particle.variable("Bank_3", b3status);
    Particle.variable("Bank_4", b4status);
    //Particle.variable("Input Status", b4status);
}

void loop() {
    relayController.readStatus();
    relayController2.readStatus();
    b1status = relayController.bankOneStatus;
    b2status = relayController.bankTwoStatus;
    b3status = relayController2.bankOneStatus;
    b3status = relayController2.bankTwoStatus;
    //b4status = String(relayController2.readAllInputs());
}

int triggerRelay(String command){
    relayController.relayTalk(command);
}
int triggerRelay2(String command){
    relayController2.relayTalk(command);
}
