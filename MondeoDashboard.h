#ifndef MONDEODASHBOARD_H
#define MONDEODASHBOARD_H
#include <stdint.h>
#include <mcp2515.h>
#include "Commands.h"

struct MondeoPins_t
{
  uint8_t ignitionPin;
  uint8_t leftIndicatorPin;
  uint8_t rightIndicatorPin;
  uint8_t headlightPin;
  uint8_t fullbeamPin;
  uint8_t handbrakePin;
  uint8_t oilPressurePin;
};


class MondeoDashboard
{
  public:
    MondeoDashboard(MondeoPins_t, MCP2515*);
    void setRpm(uint8_t);
    void setSpeed(uint8_t);
    void setFuel(uint8_t);
    void setOilTemperature(uint8_t);
    void setElectrics(bool);
    void setLights(bool);
    void setEngine(bool);
    void setHandbrake(bool);
    void setFullbeam(bool);
    void setOilFault(bool);
    void setLeftIndicator(bool);
    void setRightIndicator(bool);
    void setEngineFault(bool);
    void setEngineFail(bool);
    void setBatteryWarning(bool);
    void setOilPressureWarning(bool);

    void loop();
    void processRunningCommand(RunningCommand_t);
    void processFaultsCommand(FaultsCommand_t);

  private:
    MCP2515* _mcp2515;
    struct can_frame _canMsg;

    uint8_t _ignitionPin;
    uint8_t _leftIndicatorPin;
    uint8_t _rightIndicatorPin;
    uint8_t _headlightPin;
    uint8_t _fullbeamPin;
    uint8_t _handbrakePin;
    uint8_t _oilPressurePin;

    uint16_t _rpm;
    uint8_t _speed;
    uint8_t _fuel;
    uint8_t _oilTemperature;

    bool _electrics;
    bool _lights;
    bool _engine;
    bool _handbrake;
    bool _fullbeam;
    bool _oilFault;
    bool _leftIndicator;
    bool _rightIndicator;
    bool _engineFault;
    bool _engineFail;
    bool _batteryWarning;
    bool _oilPressureWarning;

    const uint8_t _batteryLight = 0x02;
    const uint8_t _engineFailLight = 0x40;
    const uint8_t _engineFaultLight = 0x80;


    bool _debug;
};

#endif
