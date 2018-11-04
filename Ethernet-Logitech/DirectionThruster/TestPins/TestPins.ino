#include <Servo.h>
#include <Ethernet.h>
#include <SPI.h>
#define UDP_TX_PACKET_MAX_SIZE 70
#include <Servo.h>

int t1=0;int t2=-1;int t3=4;int t4=2;
int v1=0; long y=0;long p=0; long r=0;
long temp; 
long humidity;
long internal_pressure;

String converstion(int t1,int t2,int t3,int t4,int v1,long y,long p,long r,long temp,long humidity,long internal_pressure){
  String result;
  result=t1;
  result+=',';
  result+=t2;
  result+=',';
  result+=t3;
  result+=',';
  result+=t4;
  result+=',';
  result+=v1;
  result+=',';
  result+=y;
  result+=',';
  result+=p;
  result+=',';
  result+=r;
  result+=',';
  result+=temp;
  result+=',';
  result+=humidity;
  result+=',';
  result+=internal_pressure;
  return (result);
}


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE}; //Assign a mac address
IPAddress ip(169, 254, 234, 100); //Assign my IP adress
unsigned int localPort = 5000; //Assign a Port to talk over
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
String datReq; //String for our data
int packetSize; //Size of Packet
EthernetUDP Udp; //Define UDP Object

float nos[17];
int nNos=-1;  
void addNos(String data)
{
  nNos=-1;
  String ss="";
  for(int i=0;i<data.length();i++)
  {
    if(data[i]==',')
    { nos[++nNos]=ss.toFloat();
      ss="";
    }
    else if(i==data.length()-1)
    { ss+=data[i];
      nos[++nNos]=ss.toFloat();
      ss="";
      break;
    }
    else 
    {
      ss+=data[i];
    }
  }  
}

/*
  Nomenclature goes this way. Back side is the side from where the tether goes into the cabinet.
  so this way, following is the nomenclature
  left thruster  facing upwards: LU
  right thruster facing upwards: RU
  front left thruster below upper sheet : FL
  front right thruster below upper sheet: FR
  back left thruster below upper sheet :  BL
  back right thruster below upper sheet:  BR
*/


Servo LU;
Servo RU;
Servo FL;
Servo FR;
Servo BL;
Servo BR;
byte LUpin=7;
byte RUpin=8;
byte FLpin=6;
byte FRpin=5;
byte BLpin=3;
byte BRpin=4;

byte pair=0;

void writeUD(int sig)
{
  LU.writeMicroseconds(sig);
  RU.writeMicroseconds(sig);
}
void writeFR(int sig)
{
  FL.writeMicroseconds(sig);
  FR.writeMicroseconds(sig);
}
void writeBR(int sig)
{
  BL.writeMicroseconds(sig);
  BR.writeMicroseconds(sig);
}
void writeLR(int sig)
{
  FR.writeMicroseconds(sig);
  BL.writeMicroseconds(sig);
}
void writeRR(int sig)
{
  FL.writeMicroseconds(sig);
  BR.writeMicroseconds(sig);
}

void thruster_movement()
{
  int sig=1700;
  if(nos[5]==1)
  {
    FL.writeMicroseconds(sig);
    pair=1;
  }  
  else if(nos[6]==1)
  {
    FR.writeMicroseconds(sig);
    pair=2;
  }
  else if(nos[7]==1)
  {
    BL.writeMicroseconds(sig);
    pair=3;
  }
  else if(nos[8]==1)
  {
    BR.writeMicroseconds(sig);
    pair=4;
  }
  else if(nos[9]==1)
  {
    LU.writeMicroseconds(sig);
    pair=5;
  }
  else if(nos[10]==1)
  {
    RU.writeMicroseconds(sig);
    pair=6;
  }
  else
  {
    switch(pair)
    {
      case 1:
            pair=0;
            FL.writeMicroseconds(1500);
            break;
      case 2:
            pair=0;
            FR.writeMicroseconds(1500);

            break;
      case 3:
            pair=0;
            BL.writeMicroseconds(1500);
            break;
      case 4:
            pair=0;
            BR.writeMicroseconds(1500);
            break;
      case 5:
            pair=0;
            LU.writeMicroseconds(1500);
            break;
      case 6:
            pair=0;
            RU.writeMicroseconds(1500);
            break;
                                
    }


    
  }
}


void setup() {
  Serial.begin(9600); //Turn on Serial Port
  Ethernet.begin(mac, ip); //Initialize Ethernet
  Udp.begin(localPort); //Initialize Udp
  LU.attach(LUpin);
  RU.attach(RUpin);
  FL.attach(FLpin);
  FR.attach(FRpin);
  BL.attach(BLpin);
  BR.attach(BRpin);
  LU.writeMicroseconds(1500);
  RU.writeMicroseconds(1500);
  FL.writeMicroseconds(1500);
  FR.writeMicroseconds(1500);
  BL.writeMicroseconds(1500);
  BR.writeMicroseconds(1500);
  delay(1000);
}

void loop() {
temp = random(15,30);
  humidity = random(10,300);
  internal_pressure = random(50,200);
  y = random(10,50);
  p = random(10,50);
  r = random(10,50);
  v1 = random(30,48);
  t1++;t2++;t3++;t4++;
  String output= converstion(t1,t2,t3,t4,v1,y,p,r,temp,humidity,internal_pressure);
  
  packetSize = Udp.parsePacket(); //Read theh packetSize
  if(packetSize>0)
  { 
    //Check to see if a request is present
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Reading the data request on the Udp
    datReq=packetBuffer; //Convert packetBuffer array to string datReq
    addNos(datReq);
    Serial.println(datReq);
  }
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
  Udp.print(output); //Send string back to client 
  Udp.endPacket(); //Packet has been sent
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
  thruster_movement();
}
