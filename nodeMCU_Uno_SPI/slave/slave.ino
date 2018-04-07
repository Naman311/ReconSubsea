#include <SPI.h>

char buf [100];
char checkBuf[4] = ":#$";
volatile byte pos;
volatile boolean process_it;                                   //Flag for checking if the data is recieved from Master i.e. ESP8266
//byte a;                                                        //Byte to store the processed data
void setup(void)
{
  Serial.begin(115200);                                     //Set UART baug rate to 115200
  SPCR |= bit(SPE);                                         //Configure ATMEGA328P/Arduino in slave mode
  SPCR |= bit(SPIE);
  pinMode(MISO, OUTPUT);                                    //Configure MISO as output, SlaveOut
  pos = 0;   // buffer empty
  process_it = false;                                       //Initialize flag to FALSE
  SPI.attachInterrupt();                                    //Enable interrupts for SPI--> You can do that like this too /*SPCR |= bit (SPIE)*/
}

// SPI interrupt routine
ISR(SPI_STC_vect)
{
  byte c = SPDR;                                           //Grab the data byte from the SPI Data Register (SPDR)
  if (pos < sizeof buf)
  {
    buf [pos++] = c;

    // example: newline means time to process buffer
    if (c == '\n')

      process_it = true;

  }  // end of room available
  //a = c;                                                   //Put the byte into a temporary variable for processing
  SPDR = 5;                                            //process the data byte and put it back into the SPDR for the Master to read it
  //process_it = true;                                       //Set the Flag as TRUE*/
}

void loop(void)
{
  if (process_it)                                         //Check if the data has been processed
  {
    int  j1 = 0, chkSum = 0;
    for(int i=0;i<pos-2;i++)Serial.print(buf[i]);
    Serial.println();
    Serial.println(buf[pos-2]);
    for (int j = pos - 5; j <pos - 2; j++)if (buf[j] != checkBuf[j1++]) {
        chkSum = 1;
        break;
      }
    if (chkSum != 0)Serial.println("Not Received");
    else Serial.println("Received");
    buf [pos] = 0;
    //Serial.print("::")    Serial.print (buf);Serial.println(";;");
    pos = 0;
    process_it = false;  // end of flag set
    /*Serial.println("Recieved\r\n");                     //UART - Notify if recived a byte from master
      process_it = false;                                 //Set the Flag to False*/
  }
}
