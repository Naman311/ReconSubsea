// slave

#include <SPI.h>
#include "SPI_anything.h"
unsigned int a;

void setup ()
  {
  Serial.begin (9600);   // debugging

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  }  // end of setup

void loop () 
  {   
  SPI_readAnything (a);
  Serial.println (a);
  }  // end of loop
