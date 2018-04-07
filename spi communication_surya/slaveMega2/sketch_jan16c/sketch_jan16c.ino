#include <stdlib.h>
#include <SPI.h>
long int count = 1;
uint8_t buffer[3];
char buf [100];
volatile byte pos;
volatile boolean process_it;
#define SCK_PIN  52
#define MISO_PIN 50
#define MOSI_PIN 51
#define SS_PIN   53 
void setup (void)
{
  Serial.begin (115200);  
  pinMode(MOSI_PIN, INPUT);
  pinMode(MISO_PIN, OUTPUT);
  pinMode(SCK_PIN, INPUT);
  pinMode(SS_PIN, INPUT);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
 
  DDRB=(1<<DDB4);
  SPCR=(1<<SPE); 
  SPCR |= _BV(SPE);
  SPCR != _BV(SPIE);
  pos = 0;
  process_it = false;
  SPI.attachInterrupt();

} 


ISR (SPI_STC_vect)
{
byte c = SPDR;
 
  // add to buffer if room
  if (pos < sizeof buf)
    {
    buf [pos++] = c;
    SPDR = c;
    if (c == '\n')
      process_it = true;
     
    } 
}
void loop (void)
{
  char c;
 
  if (process_it)
    {
    buf [pos] = 0; 
    Serial.println (buf);
    pos = 0;
    process_it = false;
   
    }
   
}  


