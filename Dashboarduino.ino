#include <mcp2515.h> //Library for using CAN Communication
#include "MondeoDashboard.h"
#include "Commands.h"


struct can_frame canMsg;
MCP2515 mcp2515(10);
MondeoDashboard *dashboard;


void setup()
{
  SPI.begin();
  Serial.begin(115200);
  for (int i=2; i< 10; i++) 
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  delay(10);

  MondeoPins_t pins;
  pins.ignitionPin = 2;
  pins.leftIndicatorPin = 3;
  pins.rightIndicatorPin = 4;
  pins.headlightPin = 6;
  pins.fullbeamPin = 5;
  pins.handbrakePin = 9;
  pins.oilPressurePin = 8;
  dashboard = new MondeoDashboard(pins, & mcp2515);

  dashboard->setRpm(0);
  dashboard->setSpeed(0);
  dashboard->setFuel(0);
  dashboard->setOilTemperature(0);

  dashboard->setElectrics(false);
  dashboard->setLights(false);
  dashboard->setEngine(false);
  dashboard->setHandbrake(false);
  dashboard->setFullbeam(false);
  dashboard->setOilFault(false);
  dashboard->setLeftIndicator(false);
  dashboard->setRightIndicator(false);
  dashboard->setEngineFault(false);
  dashboard->setEngineFail(false);
  dashboard->setBatteryWarning(false);
  dashboard->setOilPressureWarning(false);

  delay(5);
}

void loop()
{
  if (Serial.available() > 0)
  {
    uint8_t cmd;
    uint8_t len;
    uint8_t read;
    uint8_t buffer[255];
    RunningCommand_t runningCommand;
    FaultsCommand_t faultCommand;

    len = Serial.peek();
    if (len > 0)
    {
    
      read = Serial.readBytes(buffer, len);

      if (buffer[1] == 0x01)
      {
        // Running Command
        memcpy(&runningCommand, &buffer, sizeof(RunningCommand_t));
        dashboard->processRunningCommand(runningCommand);
      }
      if (buffer[1] == 0x02)
      {
        // Running Command
        memcpy(&faultCommand, &buffer, sizeof(FaultsCommand_t));
        dashboard->processFaultsCommand(faultCommand);
      }
      if (buffer[1] == 0xff)
      {
        Serial.println("Dashboardino v0.1");
      }
    }
  }
  dashboard->loop();
  delay(20);
}
