#include <Wire.h>
#include <SPI.h>
const int spi_ss = 10;   // for slave select
uint8_t byte_0, byte_1, byte_2; // for taking the input in bits
uint32_t spi_bytes; // to add all the bits by bitshift method
float vo; //convert the bits in voltage
float vr = 5;
float p=0; //for storing the value of presure
float vt=0;
float t=0;

void setup() {
    // put your setup code here, to run once:
  Serial.begin(9600);          
  pinMode(spi_ss, OUTPUT);     
  digitalWrite(spi_ss, HIGH);  
  SPI.begin();                
}

void loop() {
//put your main code here, to run repeatedly:
  SPI.beginTransaction(SPISettings(1000, MSBFIRST, SPI_MODE2)); 
//set speed bit format and clock/data polarity while starting SPI transaction
  digitalWrite(spi_ss, LOW);
  byte_0 = SPI.transfer(0); // to take the first 8 bits
  byte_1 = SPI.transfer(0); //to take the second 8 bits
  byte_2 = SPI.transfer(0);   //to take the third 8 bits
  
    

  digitalWrite(spi_ss, HIGH);
     
  SPI.endTransaction();
  spi_bytes=(byte_0*1000000+byte_1*1000+byte_2);
  Serial.println(spi_bytes );
  vo = vr* (float(spi_bytes) / 16777216.0); // converting to output voltage
  Serial.println(vo);
  float kpa = (vo-0.2)/4.5*700.0;
  Serial.println(kpa+31.11); 
  delay(1000);
}
