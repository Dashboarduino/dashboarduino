#ifndef COMMANDS_H
#define COMMANDS_H
#include <stdint.h>

struct RunningCommand_t
{
  uint8_t length = 0x0d;
  uint8_t code = 0x01;
  uint8_t rpm = 0;
  uint8_t speed = 0;
  uint8_t fuel = 0;
  uint8_t oilTemperature = 0;
  bool electrics = false;
  bool engine = false;
  bool lights = false;
  bool handbrake = false;
  bool fullbeam = false;
  bool leftIndicator = false;
  bool rightIndicator = false;
};

struct FaultsCommand_t
{
  uint8_t length = 0x07;
  uint8_t code = 0x02;
  bool engineFault = false;
  bool engineFail = false;
  bool batteryWarning = false;
  bool oilPressureWarning = false;
  bool oilFault = false;
};
#endif