// slave

#include <SPI.h>
#include "SPI_anything.h"

// create a structure to store the different data values:
typedef struct myStruct
{
  byte a;
  int b;
  long c;
};

myStruct foo;

void setup ()
  {
  Serial.begin (115200);   // debugging

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  }  // end of setup

void loop () 
  { 
  SPI_readAnything (foo);
  Serial.println ((int) foo.a);
  Serial.println (foo.b);
  Serial.println (foo.c);
  Serial.println ();
  }  // end of loop
