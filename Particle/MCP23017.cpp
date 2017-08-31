#include "MCP23017.h"
#include "spark_wiring_usbserial.h"
#include "Particle.h"

//Comment line below out to turn off Serial logging
//#define LOGGING

//Constructor
MCP23017::MCP23017(){
}

void MCP23017::setAddress(int a){
    address = a;
}
void MCP23017::setAddress(int a0, int a1, int a2){
    address |= (a0*1+a1*2+a2*4);
}


void MCP23017::setRelays(int num){
    if(num > 8){
        outputMap[0] = 0;
        outputMap[1] = 256 - (1 << (num-8));
    }else{
        outputMap[0] = 256 - (1 << num);
    }
    relayCount = num;
}
void MCP23017::setInputs(int map1, int map2){
    inputMap[0] = map1;
    inputMap[1] = map2;
}
void MCP23017::setOutputs(int map1, int map2){
    outputMap[0] = map1;
    outputMap[1] = map2;
}
void MCP23017::setOutput(int num){
    if(num > 8){
        outputMap[1] ^= (1<<(num-1));
    }else{
        outputMap[0] ^= (1<<(num-1));
    }
    iosetCustom = true;
}
void MCP23017::init(){
    //Start I2C port
    Wire.begin();
    //Open connection to specified address
    sendCommand(0x00, outputMap, 2);
    sendCommand(0x0C, inputMap, 2);
    readStatus();
}


void MCP23017::turnOnRelay(int relay){
    relayOp(relay, 1);
}
void MCP23017::turnOffRelay(int relay){
    relayOp(relay, 2);
}
void MCP23017::toggleRelay(int relay){
    relayOp(relay, 3);
}

void MCP23017::relayOp(int relay, int op){
    if(relay > 8){
        byte rbit = (1<<(relay-9));
        if((outputMap[1] & rbit) > 0) return;
        setBankTwoStatus(bitop(bankTwoStatus, rbit, op));
    }else{
        byte rbit = (1<<(relay-1));
        if((outputMap[0] & rbit) > 0) return;
        setBankOneStatus(bitop(bankOneStatus, rbit, op));
    }
    
}
byte MCP23017::bitop(byte b1, byte b2, int op){
    switch(op){
        case 1:
            return b1 | b2;
        case 2:
            return b1 & ~b2;
        case 3:
            return b1 ^ b2;
    }
    return 0;
}

void MCP23017::turnOnAllRelays(){
    setBankOneStatus(bankOneStatus | (~outputMap[0] & 255));
    setBankTwoStatus(bankTwoStatus | (~outputMap[1] & 255));
}

void MCP23017::turnOffAllRelays(){
    setBankOneStatus(bankOneStatus & outputMap[0]);
    setBankTwoStatus(bankTwoStatus & outputMap[1]);
}



void MCP23017::setBankOneStatus(int status){
    sendCommand(18, status);
    readStatus();
}
void MCP23017::setBankTwoStatus(int status){
    sendCommand(19, status);
    readStatus();
}

int MCP23017::readRelayStatus(int relay){
    if(relay < 8){
        int rbit = (1<<(relay-9));
        if((outputMap[1] & rbit) > 0) return 256;
        return (bankTwoStatus & rbit) ? 1 : 0;
    }
    int rbit = (1<<(relay-1));
    if((outputMap[0] & rbit) > 0) return 256;
    return (bankOneStatus & (1<<(relay-1))) ? 1 : 0;
}
int lastReadBanks=0;
void MCP23017::readStatus(){
    bankOneStatus = sendCommand(18);
    bankTwoStatus = sendCommand(19);
}

int MCP23017::readInputStatus(int input){
    if(!iosetCustom){
        input += relayCount;
    }
    int bankStatus = readAllInputs();
    return (bankStatus & (1<<(input-1))) ? 0 : 1;
}
int lastReadInputs=0;
int MCP23017::readAllInputs(){
    int bankStatus = sendCommand(18, 0, 0, 2);
    String report=String(bankStatus);
    
    #ifdef LOGGING
        if(bankStatus == 256){
            Serial.println("Read all input status command failed");
            return 256;
        }
    #endif
    int inverted = ~bankStatus;
    int shifted;
    if(!iosetCustom){
        shifted = inverted >> relayCount;
    }else{
        shifted = inverted;
    }
    if(millis()-lastReadInputs > 4000){
        report.concat(" - ");
        report.concat(String(shifted));
        Particle.publish("Input Status", report);
        lastReadInputs = millis();
    }
    return shifted;
}
int MCP23017::sendCommand(int reg){
    int cmds[1] = {0};
    return sendCommand(reg, cmds, 0, 1);
}
int MCP23017::sendCommand(int reg, int cmd){
    int cmds[1] = {cmd};
    return sendCommand(reg, cmds, 1, 1);
}
int MCP23017::sendCommand(int reg, int cmds[], int length){
    return sendCommand(reg, cmds, length, 1);
}
int MCP23017::sendCommand(int reg, int icmds[], int length, int rlen){
        byte cmds[length];
        for(int i=0;i<length;i++){
            cmds[i] = icmds[i];
        }
    int status = 256;
    while(retrys < 4 && status > 0){
        Wire.beginTransmission(address);
        Wire.write(reg);
        if(length > 0) Wire.write(cmds, length);
        status = Wire.endTransmission();
        retrys++;
    }
    initialized = false;
    int ret = 256;
    if(retrys < 4){
        initialized = true;
        Wire.requestFrom(address, rlen);
        int rbyte = 0;
        ret = 0;
        while(rbyte < rlen){
            int bread = Wire.read();
            ret = (ret << 8) + bread;
            rbyte++;
        }
    }
    retrys = 0;
    return ret;
}

int MCP23017::relayTalk(String command){
    int bank=0;
    int relay=0;
    int op=0;
    int p=command.indexOf(" ");
    bool all=false;
    bool relay_next=false;
    bool bank_next=false;
    String word;
    if(p > -1){
        while(command.length() > 0){
            if(p > -1){
                word = command.substring(0, p);
                command = command.substring(p+1);
                p=command.indexOf(" ");
            }else{
                word = command;
                command = "";
            }
            if(word.equalsIgnoreCase("on") || word.equalsIgnoreCase("activate")){
                if(op == 0 || bank > 0) op=1;
            }
            if(word.equalsIgnoreCase("off") || word.equalsIgnoreCase("deactivate")){
                op=2;
            }
            if(word.equalsIgnoreCase("toggle") || word.equalsIgnoreCase("flip")){
                op=3;
            }
            if(relay_next){
                int test = word.toInt();
                if(test > 0){
                    relay = test;
                    relay_next = false;
                }
            }
            if(bank_next){
                int test = word.toInt();
                if(test > 0){
                    bank = test;
                    bank_next = false;
                }
            }
            if(word.equalsIgnoreCase("relay") || word.equalsIgnoreCase("output")){
                relay_next = true;
            }
            if(word.equalsIgnoreCase("bank")){
                bank_next = true;
            }
            if(word.equalsIgnoreCase("all")){
                all = true;
            }
            
        }
    }
    if(all){
        if(bank < 2){
            int obyte;
            if(op == 1) obyte = bankOneStatus | (~outputMap[0] & 255);
            else if(op == 2) obyte = bankOneStatus & outputMap[0];
            else if(op == 3) obyte = ~bankOneStatus;
            setBankOneStatus(obyte);
        }
        if(bank != 1){
            int obyte;
            if(op == 1) obyte = bankTwoStatus | (~outputMap[0] & 255);
            else if(op == 2) obyte = bankTwoStatus & outputMap[0];
            else if(op == 3) obyte = ~bankTwoStatus;
            setBankTwoStatus(obyte);
        }
    }else{
        if(bank > 0) relay+=((bank-1)*8);
        relayOp(relay, op);
    }
    return 1;
}
