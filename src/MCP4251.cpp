#include "MCP4251.h"
#include <SPI.h>

// Instantiation of object
MCP4251::MCP4251(uint8_t slaveSelectPin, float pot0ResistanceRmax, float pot0ResistanceRmin, float pot1ResistanceRmax, float pot1ResistanceRmin)
{
    this->_slaveSelectPin = slaveSelectPin;

    this->_pot0ResistanceRmax = pot0ResistanceRmax;
    this->_pot0ResistanceRmin = pot0ResistanceRmin;
    this->_pot0ResistanceRAB = pot0ResistanceRmax - pot0ResistanceRmin;
    this->_pot0ResistanceRW = pot0ResistanceRmin;

    this->_pot1ResistanceRmax = pot1ResistanceRmax;
    this->_pot1ResistanceRmin = pot1ResistanceRmin;
    this->_pot1ResistanceRAB = pot1ResistanceRmax - pot1ResistanceRmin;
    this->_pot1ResistanceRW = pot1ResistanceRmin;
}

void MCP4251::begin()
{
    ::pinMode(_slaveSelectPin, OUTPUT);
    ::digitalWrite(_slaveSelectPin, HIGH);
    SPI.begin();
    this->DigitalPotInitTcon();
    this->DigitalPotSetWiperMin(0);
    this->DigitalPotSetWiperMin(1);
}

void MCP4251::DigitalPotWiperIncrement(bool potNum)
{
    byte cmdByte = B00000000;
    ::digitalWrite(_slaveSelectPin, LOW);
    if (potNum)
    {
        cmdByte = ADDRESS_WIPER_1 | COMMAND_INCREMENT;
        SPI.transfer(cmdByte);
    }
    else
    {
        cmdByte = ADDRESS_WIPER_0 | COMMAND_INCREMENT;
        SPI.transfer(cmdByte);
    }
    ::digitalWrite(_slaveSelectPin, HIGH);
}

void MCP4251::DigitalPotWiperDecrement(bool potNum)
{
    byte cmdByte = B00000000;
    ::digitalWrite(_slaveSelectPin, LOW);
    if (potNum)
    {
        cmdByte = ADDRESS_WIPER_1 | COMMAND_DECREMENT;
        SPI.transfer(cmdByte);
    }
    else
    {
        cmdByte = ADDRESS_WIPER_0 | COMMAND_DECREMENT;
        SPI.transfer(cmdByte);
    }
    ::digitalWrite(_slaveSelectPin, HIGH);
}

void MCP4251::DigitalPotSetWiperPosition(bool potNum, unsigned int value)
{
    byte cmdByte = B00000000;
    byte dataByte = B00000000;
    if (value > 255)
        cmdByte |= B00000001;
    else
        dataByte = (byte)(value & 0X00FF);
    ::digitalWrite(_slaveSelectPin, LOW);
    if (potNum)
    {
        cmdByte = cmdByte | ADDRESS_WIPER_1 | COMMAND_WRITE;
        SPI.transfer(cmdByte);
        SPI.transfer(dataByte);
    }
    else
    {
        cmdByte = cmdByte | ADDRESS_WIPER_0 | COMMAND_WRITE;
        SPI.transfer(cmdByte);
        SPI.transfer(dataByte);
    }
    ::digitalWrite(_slaveSelectPin, HIGH);
}

void MCP4251::DigitalPotSetWiperMin(bool potNum)
{
    if (potNum)
        DigitalPotSetWiperPosition(1, 0);
    else
        DigitalPotSetWiperPosition(0, 0);
}

void MCP4251::DigitalPotSetWiperMax(bool potNum)
{
    if (potNum)
        DigitalPotSetWiperPosition(1, 256);
    else
        DigitalPotSetWiperPosition(0, 256);
}

void MCP4251::DigitalPotSetWiperMid(bool potNum)
{
    if (potNum)
        DigitalPotSetWiperPosition(1, 128);
    else
        DigitalPotSetWiperPosition(0, 128);
}

uint16_t MCP4251::DigitalPotReadWiperPosition(bool potNum)
{
    byte cmdByte = B00000000;
    byte hByte = B00000000;
    byte lByte = B00000000;
    ::digitalWrite(_slaveSelectPin, LOW);
    if (potNum)
    {
        cmdByte = ADDRESS_WIPER_1 | COMMAND_READ;
        hByte = SPI.transfer(cmdByte);
        lByte = SPI.transfer(DUMMY_DATA);
    }
    else
    {
        cmdByte = ADDRESS_WIPER_0 | COMMAND_READ;
        hByte = SPI.transfer(cmdByte);
        lByte = SPI.transfer(DUMMY_DATA);
    }
    ::digitalWrite(_slaveSelectPin, HIGH);
    return ((uint16_t)hByte << 8 | (uint16_t)lByte) & BITMASK_READ_DATA_REGISTER;
}

uint16_t MCP4251::DigitalPotReadStatusRegister()
{
    byte cmdByte = B00000000;
    byte hByte = B00000000;
    byte lByte = B00000000;
    ::digitalWrite(_slaveSelectPin, LOW);
    cmdByte = ADDRESS_STATUS | COMMAND_READ;
    hByte = SPI.transfer(cmdByte);
    lByte = SPI.transfer(DUMMY_DATA);
    ::digitalWrite(_slaveSelectPin, HIGH);
    return ((uint16_t)hByte << 8 | (uint16_t)lByte) & BITMASK_READ_DATA_REGISTER;
}

uint16_t MCP4251::DigitalPotReadTconRegister()
{
    byte cmdByte = B00000000;
    byte hByte = B00000000;
    byte lByte = B00000000;
    ::digitalWrite(_slaveSelectPin, LOW);
    cmdByte = ADDRESS_TCON | COMMAND_READ;
    hByte = SPI.transfer(cmdByte);
    lByte = SPI.transfer(DUMMY_DATA);
    ::digitalWrite(_slaveSelectPin, HIGH);
    return ((uint16_t)hByte << 8 | (uint16_t)lByte) & BITMASK_READ_DATA_REGISTER;
}

void MCP4251::DigitalPotWriteTconRegister(uint16_t value)
{
    byte cmdByte = B00000000;
    byte dataByte = B00000000;
    if (value > 255)
        cmdByte |= B00000001;
    else
        dataByte = (byte)(value & 0X00FF);
    ::digitalWrite(_slaveSelectPin, LOW);
    cmdByte = cmdByte | ADDRESS_TCON | COMMAND_WRITE;
    SPI.transfer(cmdByte);
    SPI.transfer(dataByte);
    ::digitalWrite(_slaveSelectPin, HIGH);
}

void MCP4251::DigitalPotStartup(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if (potNum)
        lByte = lByte | BITMASK_POT1_STARTUP;
    else
        lByte = lByte | BITMASK_POT0_STARTUP;

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

void MCP4251::DigitalPotShutdown(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if (potNum)
        lByte = lByte & ~BITMASK_POT1_STARTUP;
    else
        lByte = lByte & ~BITMASK_POT0_STARTUP;

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

void MCP4251::DigitalPotTerminalBConnect(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if (potNum)
        lByte = lByte | BITMASK_POT1_B_TERMINAL_CONNECT;
    else
        lByte = lByte | BITMASK_POT0_B_TERMINAL_CONNECT;

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

void MCP4251::DigitalPotTerminalBDisconnect(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if (potNum)
        lByte = lByte & ~BITMASK_POT1_B_TERMINAL_CONNECT;
    else
        lByte = lByte & ~BITMASK_POT0_B_TERMINAL_CONNECT;

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

void MCP4251::DigitalPotTerminalAConnect(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if (potNum)
        lByte = lByte | BITMASK_POT1_A_TERMINAL_CONNECT;
    else
        lByte = lByte | BITMASK_POT0_A_TERMINAL_CONNECT;

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

void MCP4251::DigitalPotTerminalADisconnect(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if (potNum)
        lByte = lByte & ~BITMASK_POT1_A_TERMINAL_CONNECT;
    else
        lByte = lByte & ~BITMASK_POT0_A_TERMINAL_CONNECT;

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

void MCP4251::DigitalPotWiperConnect(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if (potNum)
        lByte = lByte | BITMASK_POT1_WIPER_TERMINAL_CONNECT;
    else
        lByte = lByte | BITMASK_POT0_WIPER_TERMINAL_CONNECT;

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

void MCP4251::DigitalPotWiperDisconnect(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if (potNum)
        lByte = lByte & ~BITMASK_POT1_WIPER_TERMINAL_CONNECT;
    else
        lByte = lByte & ~BITMASK_POT0_WIPER_TERMINAL_CONNECT;

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

void MCP4251::DigitalPotInitTcon()
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    lByte = lByte | DUMMY_DATA;

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

uint16_t MCP4251::DigitalPotResistanceToPosition(bool potNum, float resistance)
{
    if (potNum)
    {
        if (resistance <= this->_pot1ResistanceRmin)
            return 0;
        else if (resistance > this->_pot1ResistanceRmax)
            return 256;

        return (uint16_t)((((resistance - this->_pot1ResistanceRW) / (this->_pot1ResistanceRAB)) * (float)256) + 0.5);
    }
    else
    {
        if (resistance <= this->_pot0ResistanceRmin)
            return 0;
        else if (resistance > this->_pot0ResistanceRmax)
            return 256;

        return (uint16_t)((((resistance - this->_pot0ResistanceRW) / (this->_pot0ResistanceRAB)) * (float)256) + 0.5);
    }
}

float MCP4251::DigitalPotPositionToResistance(bool potNum, uint16_t position)
{
    if (potNum)
    {

        if (position < 0)
            return this->_pot1ResistanceRmin;
        else if (position > 256)
            return this->_pot1ResistanceRmax;

        return ((this->_pot1ResistanceRAB / 256) * (float)position) + this->_pot1ResistanceRW;
    }
    else
    {
        if (position < 0)
            return this->_pot0ResistanceRmin;
        else if (position > 256)
            return this->_pot0ResistanceRmax;

        return ((this->_pot0ResistanceRAB / 256) * (float)position) + this->_pot0ResistanceRW;
    }
}