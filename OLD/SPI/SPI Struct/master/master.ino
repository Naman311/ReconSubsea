// master

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
  SPI.begin ();
  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV8);

  foo.a = 42;
  foo.b = 32000;
  foo.c = 100000;
  }  // end of setup

void loop () 
  { 
  digitalWrite(SS, LOW);    // SS is pin 10
  SPI_writeAnything (foo);
  digitalWrite(SS, HIGH);
  delay (1000);  // for testing  
  
  foo.c++;
  }  // end of loop
