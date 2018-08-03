.,+
#include <Wire.h>
#include <SDL_Arduino_INA3221.h>

SDL_Arduino_INA3221 ina3221;

// the three channels of the INA3221 named for SunAirPlus Solar Power Controller channels (www.switchdoc.com)
#define CHANNEL 1

void setup(void) 
{
    
  Serial.begin(57600);
  Serial.println("SDA_Arduino_INA3221_Test");
  
  Serial.println("Measuring voltage and current with ina3221 ...");
  ina3221.begin();
}

void loop(void) 
{
  
  Serial.println("------------------------------");
  float shuntvoltage1 = 1;
  float busvoltage1 = 1;
  float current_mA1 = 1;
  float loadvoltage1 = 1;


  busvoltage1 = ina3221.getBusVoltage_V(CHANNEL);
  shuntvoltage1 = ina3221.getShuntVoltage_mV(CHANNEL);
  current_mA1 = -ina3221.getCurrent_mA(CHANNEL);  // minus is to get the "sense" right.   - means the battery is charging, + that it is discharging
  loadvoltage1 = busvoltage1 + (shuntvoltage1 / 1000);
  
 Serial.print("LIPO_Battery Bus Voltage:   ");
  Serial.print(busvoltage1); Serial.println(" V");
  Serial.print("LIPO_Battery Shunt Voltage: ");
  Serial.print(shuntvoltage1); Serial.println(" mV");
  Serial.print("LIPO_Battery Load Voltage:  ");
  Serial.print(loadvoltage1); Serial.println(" V");
  Serial.print("LIPO_Battery Current 1:       ");
  Serial.print(current_mA1); Serial.println(" mA");
  Serial.println("");
  delay(200);
}
