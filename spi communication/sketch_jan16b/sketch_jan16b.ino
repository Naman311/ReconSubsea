#include <stdlib.h>
#include <SPI.h>

long int count = 1;
byte config = B01010000;
#define SCK_PIN  52
#define MISO_PIN 50
#define MOSI_PIN 51
#define SS_PIN   53
uint8_t buffer[128];
char respBuffer[100];
volatile byte pos;
volatile boolean process_it;

void setup()
{
  byte tmp;
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);
  byte clr;
  clr=SPSR;
  clr=SPDR;
  Serial.begin(115200);
}
byte transferAndWait(const byte what)
{
  byte a = SPI.transfer(what);
  delayMicroseconds(20);
  return a;
} // end of transferAndWait
void loop()
{
char c;
byte r;
// enable Slave Select 
digitalWrite(SS_PIN, LOW);
pos=0;
for (const char *p="Hello, world!\n";c=*p;p++)
{
   //SPI.transfer(c);
   r=transferAndWait(c);
   respBuffer[pos++]=r;
   Serial.print(c);
   Serial.print(r);
}
Serial.println("RX:");
Serial.println(pos,DEC);
Serial.println(respBuffer);
pos=0;
delay(100);
digitalWrite(SS_PIN, HIGH);
delay(100);
   
}

