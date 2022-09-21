#ifndef MCP4251_h
#define MCP4251_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "Wprogram.h"
#endif

#define ADDRESS_WIPER_0   B00000000
#define ADDRESS_WIPER_1   B00010000
#define ADDRESS_TCON0     B01000000

#define ADDRESS_WIPER_2   B01100000
#define ADDRESS_WIPER_3   B01110000
#define ADDRESS_TCON1     B10100000

#define ADDRESS_STATUS    B01010000

#define COMMAND_WRITE     B00000000
#define COMMAND_INCREMENT B00000100
#define COMMAND_DECREMENT B00001000
#define COMMAND_READ      B00001100

#define DUMMY_DATA B11111111

#define BITMASK_READ_DATA_REGISTER 0X01FF //B0000000111111111

#define BITMASK_POT0_STARTUP B00001000
#define BITMASK_POT1_STARTUP B10000000
#define BITMASK_POT2_STARTUP B00001000
#define BITMASK_POT3_STARTUP B10000000

#define BITMASK_POT0_B_TERMINAL_CONNECT B00000001
#define BITMASK_POT1_B_TERMINAL_CONNECT B00010000
#define BITMASK_POT2_B_TERMINAL_CONNECT B00000001
#define BITMASK_POT3_B_TERMINAL_CONNECT B00010000

#define BITMASK_POT0_WIPER_TERMINAL_CONNECT B00000010
#define BITMASK_POT1_WIPER_TERMINAL_CONNECT B00100000
#define BITMASK_POT2_WIPER_TERMINAL_CONNECT B00000010
#define BITMASK_POT3_WIPER_TERMINAL_CONNECT B00100000

#define BITMASK_POT0_A_TERMINAL_CONNECT B00000100
#define BITMASK_POT1_A_TERMINAL_CONNECT B01000000
#define BITMASK_POT2_A_TERMINAL_CONNECT B00000100
#define BITMASK_POT3_A_TERMINAL_CONNECT B01000000


class MCP4251
{
public:
  // Constructor
  MCP4251(uint8_t slaveSelectPin, float pot0ResistanceRmax, float pot0ResistanceRmin, float pot1ResistanceRmax, float pot1ResistanceRmin, float pot2ResistanceRmax, float pot2ResistanceRmin, float pot3ResistanceRmax, float pot3ResistanceRmin);
  void begin();
  void DigitalPotWiperIncrement(uint8_t potNum);
  void DigitalPotWiperDecrement(uint8_t potNum);
  void DigitalPotSetWiperPosition(uint8_t potNum, uint16_t value); // Need confirmation if it is working
  void DigitalPotSetWiperMin(uint8_t potNum);
  void DigitalPotSetWiperMax(uint8_t potNum);
  void DigitalPotSetWiperMid(uint8_t potNum);

  uint16_t DigitalPotReadWiperPosition(uint8_t potNum);
  uint16_t DigitalPotReadTconRegister();
  uint16_t DigitalPotReadStatusRegister();

  void DigitalPotWriteTcon0Register(uint16_t value);
  void DigitalPotWriteTcon1Register(uint16_t value);

  void DigitalPotStartup(uint8_t potNum);
  void DigitalPotShutdown(uint8_t potNum);

  void DigitalPotTerminalBConnect(uint8_t potNum);
  void DigitalPotTerminalBDisconnect(uint8_t potNum);

  void DigitalPotTerminalAConnect(uint8_t potNum);
  void DigitalPotTerminalADisconnect(uint8_t potNum);

  void DigitalPotWiperConnect(uint8_t potNum);
  void DigitalPotWiperDisconnect(uint8_t potNum);

  void DigitalPotInitTcon();

  uint16_t DigitalPotResistanceToPosition(uint8_t potNum, float resistance);
  float DigitalPotPositionToResistance(uint8_t potNum, uint16_t position);

private:
  uint8_t _slaveSelectPin;
  float _pot0ResistanceRmax, _pot0ResistanceRmin, _pot0ResistanceRAB, _pot0ResistanceRW, _pot1ResistanceRmax, _pot1ResistanceRmin, _pot1ResistanceRAB, _pot1ResistanceRW, \
        _pot2ResistanceRmax, _pot2ResistanceRmin, _pot2ResistanceRAB, _pot2ResistanceRW, _pot3ResistanceRmax, _pot3ResistanceRmin, _pot3ResistanceRAB, _pot3ResistanceRW;

};

#endif
