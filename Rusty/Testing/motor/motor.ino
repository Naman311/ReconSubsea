int incomingByte = 0;   // for incoming serial data

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        pinMode(7,OUTPUT);
        pinMode(6,OUTPUT);
        analogWrite(7,0);
        analogWrite(6,0);
}

int prev=0;
void loop() {

       if (Serial.available() > 0) 
      {
          // read the incoming byte:
          incomingByte = Serial.read();
        
          // say what you got:
          //Serial.print("I received: ");
          Serial.println(incomingByte, DEC);
      }
      if(incomingByte==49)
      {
        analogWrite(6,0);
        delay(5);
        analogWrite(7,1024);
        prev=49;
             
      }
      else if(incomingByte==50)
      {
        analogWrite(7,0);
        delay(5);
        analogWrite(6,1024);
        prev=50;         
      }
      else if(incomingByte==51)
      {
        if(prev==50)
        {
          analogWrite(7,500);
          delay(1);
          analogWrite(7,0);
        }
        else if(prev==49)
        {
          analogWrite(6,500);
          delay(1);
          analogWrite(6,0);
        }
        prev=0;
      }
}
