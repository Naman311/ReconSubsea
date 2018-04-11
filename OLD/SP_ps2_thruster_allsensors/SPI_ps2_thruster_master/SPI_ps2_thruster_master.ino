// master
#include <Psx.h>
#include <SPI.h>
#include "SPI_anything.h"
#define dataPin 4   //brown
#define cmndPin 5   //orange
#define attPin 7 //yellow
#define clockPin 6 //blue

Psx Psx;
void setup ()
  {
  Psx.setupPins(dataPin, cmndPin, attPin, clockPin, 10);
  SPI.begin ();
  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV16);
 }  // end of setup

void loop () 
  {
  delay (50); 
  unsigned int a = Psx.read(); 
  digitalWrite(SS, LOW);    // SS is pin 10
  SPI_writeAnything (a);
  digitalWrite(SS, HIGH);
    // for testing  
  }  // end of loop
