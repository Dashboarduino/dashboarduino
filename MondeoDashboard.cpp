#include "MondeoDashboard.h"
#include "Arduino.h"


MondeoDashboard::MondeoDashboard(MondeoPins_t pins, MCP2515 *mcp2515)
{
  mcp2515->reset();
  mcp2515->setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515->setNormalMode();
  _mcp2515 = mcp2515;

  _ignitionPin = pins.ignitionPin;
  _leftIndicatorPin = pins.leftIndicatorPin;
  _rightIndicatorPin = pins.rightIndicatorPin;
  _headlightPin = pins.headlightPin;
  _fullbeamPin = pins.fullbeamPin;
  _handbrakePin = pins.handbrakePin;
  _oilPressurePin = pins.oilPressurePin;
  _debug = false;
}

void MondeoDashboard::setRpm(uint8_t rpm)
{
  _rpm = 20 * rpm;
}
void MondeoDashboard::setSpeed(uint8_t speed)
{
  _speed = speed;
}
void MondeoDashboard::setFuel(uint8_t fuel)
{
  _fuel = fuel;
}
void MondeoDashboard::setOilTemperature(uint8_t temperature)
{
  _oilTemperature = temperature;
}
void MondeoDashboard::setElectrics(bool on)
{
  _electrics = on;
  digitalWrite(_ignitionPin, !on);
}
void MondeoDashboard::setLights(bool on)
{
  _lights = on;
  digitalWrite(_headlightPin, !on);
}
void MondeoDashboard::setEngine(bool on)
{
  _engine = on;
}
void MondeoDashboard::setHandbrake(bool on)
{
  _handbrake = on;
  digitalWrite(_handbrakePin, on);
}
void MondeoDashboard::setFullbeam(bool on)
{
  _fullbeam = on;
  digitalWrite(_fullbeamPin, !on);
}
void MondeoDashboard::setOilFault(bool on)
{
  _oilFault = on;
}
void MondeoDashboard::setLeftIndicator(bool on)
{
  _leftIndicator = on;
  digitalWrite(_leftIndicatorPin, !on);
}
void MondeoDashboard::setRightIndicator(bool on)
{
  _rightIndicator = on;
  digitalWrite(_rightIndicatorPin, !on);
}
void MondeoDashboard::setEngineFault(bool on)
{
  _engineFault = on;
}
void MondeoDashboard::setEngineFail(bool on)
{
  _engineFail = on;
}
void MondeoDashboard::setBatteryWarning(bool on)
{
  _batteryWarning = on;
}
void MondeoDashboard::setOilPressureWarning(bool on)
{
  _oilPressureWarning = on;
  digitalWrite(_oilPressurePin, !on);
}

void MondeoDashboard::loop()
{
  // Status lights
  uint8_t warningLights = 0x00;
  if (_batteryWarning || (_electrics && !_engine))
  {
    // Show battery light
    warningLights += 0x02;
  }
  if (_engineFault && !_engineFail)
  {
    // Flash engine light
    warningLights += 0x80;
  }
  if (_engineFail)
  {
    // Solid engine light
    warningLights += 0x40;
  }

  _canMsg.can_id = 0x201;
  _canMsg.can_dlc = 8;
  _canMsg.data[0] = floor(0.015 * _rpm) + 1; // RPM
  _canMsg.data[2] = 0x00;
  _canMsg.data[3] = 0x00;
  _canMsg.data[4] = floor(_speed * 0.79); // Speed
  _canMsg.data[6] = 0x00;
  _canMsg.data[7] = 0x00;
  _mcp2515->sendMessage(&_canMsg);

  _canMsg.can_id = 0x420;
  _canMsg.can_dlc = 8;
  _canMsg.data[0] = (_oilTemperature + 40); // oil temperature - subtracts 40 degrees (130 -> 90 deg displayed)
  _canMsg.data[1] = 0x00; // Something to do with odometer - 3x past 0xfe and it increases by 0.1
  _canMsg.data[2] = 0x00;
  _canMsg.data[3] = 0x00;
  _canMsg.data[4] = warningLights; //  D0 - Flash engine light, D1 - Solid engine light, D2 - Unknown, D3 - unknown, D4 - unknown, D5 - Unknown, D6 - Battery Light, D7 - Unknown
  _canMsg.data[5] = 0x00;
  _canMsg.data[6] = 0x00;
  _canMsg.data[7] = 0x00;
  _mcp2515->sendMessage(&_canMsg);
}

void MondeoDashboard::processFaultsCommand(FaultsCommand_t cmd)
{
  this->setBatteryWarning(cmd.batteryWarning);
  this->setEngineFail(cmd.engineFail);
  this->setEngineFault(cmd.engineFault);
  this->setOilFault(cmd.oilFault);
  this->setOilPressureWarning(cmd.oilPressureWarning);
}

void MondeoDashboard::processRunningCommand(RunningCommand_t cmd)
{
  this->setRpm(cmd.rpm);
  this->setSpeed(cmd.speed);
  this->setFuel(cmd.fuel);
  this->setOilTemperature(cmd.oilTemperature);
  this->setElectrics(cmd.electrics);
  this->setLights(cmd.lights);
  this->setEngine(cmd.engine);
  this->setHandbrake(cmd.handbrake);
  this->setFullbeam(cmd.fullbeam);
  this->setLeftIndicator(cmd.leftIndicator);
  this->setRightIndicator(cmd.rightIndicator);
}