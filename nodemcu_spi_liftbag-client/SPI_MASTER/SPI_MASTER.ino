/*
    GPIO    NodeMCU   Name  |   Uno
   ===================================
     15       D8       SS   |   D10
     13       D7      MOSI  |   D11
     12       D6      MISO  |   D12
     14       D5      SCK   |   D13
*/
#include <SPI.h>

class ESPMaster {
  private:
    uint8_t _ss_pin;

  public:
    ESPMaster(uint8_t pin): _ss_pin(pin) {}
    void begin() 
    {
      pinMode(_ss_pin, OUTPUT);
      digitalWrite(_ss_pin, HIGH);
    }
    void readData(uint8_t * data) 
    {
      digitalWrite(_ss_pin, LOW);
      SPI.transfer(0x03);
      SPI.transfer(0x00);
      for (uint8_t i = 0; i < 32; i++) 
      {
        data[i] = SPI.transfer(0);
      }
      digitalWrite(_ss_pin, HIGH);
    }

    void writeData(uint8_t * data, size_t len) 
    {
      uint8_t i = 0;
      digitalWrite(_ss_pin, LOW);
      SPI.transfer(0x02);
      SPI.transfer(0x00);
      while (len-- && i < 32)
      {
        SPI.transfer(data[i++]);
      }
      while (i++ < 32) 
      {
        SPI.transfer(0);
      }
      digitalWrite(_ss_pin, HIGH);
    }

    String readData() 
    {
      char data[33];
      data[32] = 0;
      readData((uint8_t *)data);
      return String(data);
    }

    void writeData(const char * data)
    {
      writeData((uint8_t *)data, strlen(data));
    }
};

ESPMaster esp(SS);

void send(const char * message) 
{
  esp.writeData(message);
  delay(10);
  Serial.print("Slave: ");
  Serial.println(esp.readData());
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  esp.begin();
  delay(1000);
}

void loop() {
  delay(2000);
  send("one");
}
