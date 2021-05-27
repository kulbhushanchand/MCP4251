#ifndef MCP4251_h
#define MCP4251_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "Wprogram.h"
#endif

#define ADDRESS_WIPER_0 B00000000
#define ADDRESS_WIPER_1 B00010000
#define ADDRESS_TCON B01000000
#define ADDRESS_STATUS B01010000

#define COMMAND_WRITE B00000000
#define COMMAND_INCREMENT B00000100
#define COMMAND_DECREMENT B00001000
#define COMMAND_READ B00001100

#define DUMMY_DATA B11111111

#define BITMASK_READ_DATA_REGISTER 0X01FF //B0000000111111111

#define BITMASK_POT0_STARTUP B00001000
#define BITMASK_POT1_STARTUP B10000000
#define BITMASK_POT0_B_TERMINAL_CONNECT B00000001
#define BITMASK_POT1_B_TERMINAL_CONNECT B00010000
#define BITMASK_POT0_WIPER_TERMINAL_CONNECT B00000010
#define BITMASK_POT1_WIPER_TERMINAL_CONNECT B00100000
#define BITMASK_POT0_A_TERMINAL_CONNECT B00000100
#define BITMASK_POT1_A_TERMINAL_CONNECT B01000000



class MCP4251
{
public:
  // Constructor
  MCP4251(uint8_t slaveSelectPin, float pot0ResistanceRmax, float pot0ResistanceRmin, float pot1ResistanceRmax, float pot1ResistanceRmin);
  void begin();
  void DigitalPotWiperIncrement(bool potNum);
  void DigitalPotWiperDecrement(bool potNum);
  void DigitalPotSetWiperPosition(bool potNum, uint16_t value); // Not working yet
  void DigitalPotSetWiperMin(bool potNum);
  void DigitalPotSetWiperMax(bool potNum);
  void DigitalPotSetWiperMid(bool potNum);

  uint16_t DigitalPotReadWiperPosition(bool potNum);
  uint16_t DigitalPotReadTconRegister();
  uint16_t DigitalPotReadStatusRegister();

  void DigitalPotWriteTconRegister(uint16_t value);

  void DigitalPotStartup(bool potNum);
  void DigitalPotShutdown(bool potNum);

  void DigitalPotTerminalBConnect(bool potNum);
  void DigitalPotTerminalBDisconnect(bool potNum);

  void DigitalPotTerminalAConnect(bool potNum);
  void DigitalPotTerminalADisconnect(bool potNum);

  void DigitalPotWiperConnect(bool potNum);
  void DigitalPotWiperDisconnect(bool potNum);

  void DigitalPotInitTcon();

  uint16_t DigitalPotResistanceToPosition(bool potNum, float resistance);
  float DigitalPotPositionToResistance(bool potNum, uint16_t position);

private:
  uint8_t _slaveSelectPin;
  float _pot0ResistanceRmax, _pot0ResistanceRmin, _pot0ResistanceRAB, _pot0ResistanceRW, _pot1ResistanceRmax, _pot1ResistanceRmin, _pot1ResistanceRAB, _pot1ResistanceRW;

};

#endif
