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
IPAddress ip(192,168,1,117); //Assign my IP adress
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



Servo FL;
Servo FR;
Servo BL;
Servo BR;
Servo LU;
Servo RU;
byte FLpin=3;
byte FRpin=5;
byte BLpin=8;
byte BRpin=6;
byte LUpin=7;
byte RUpin=4;
byte pair=0;

void writeUD(int sig)
{
  LU.writeMicroseconds(sig);
  sig=3000-sig;
  RU.writeMicroseconds(sig);
}
void writeFR(int sig)
{
  
  FL.writeMicroseconds(sig);
  sig=1500-(sig-1500);
  FR.writeMicroseconds(sig);
}
void writeBR(int sig)
{
 
  BR.writeMicroseconds(sig);
  sig=3000-sig;
  BL.writeMicroseconds(sig);
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
void writeLL(int sig)
{
  FL.writeMicroseconds(sig);
  sig=3000-sig;
  BL.writeMicroseconds(sig);
}
void writeRRR(int sig)
{
  FR.writeMicroseconds(sig);
  sig=3000-sig;
  BR.writeMicroseconds(sig);
}



void thruster_movement()
{
  int sig=1500;
  if(nos[3]!=0)
  {
    pair=1;
    if(nos[3]>0)
    {
      sig=map(nos[3],1,10,1500,1200);
      //sig=1550;
      writeUD(sig);
    }
    else
    {
      sig=map(nos[3],-1,-10,1500,1800);
      //sig=1550;
      writeUD(sig);
    }
  }
  else if(nos[1]!=0)
  {
    pair=2;
    if(nos[1]>0)
    {
      sig=map(nos[1],1,10,1500,1800);
      //sig=1550;
      writeBR(sig);
    }
    else
    {
      sig=map(nos[1],-1,-10,1500,1800);
      //sig=1550;
      writeFR(sig);
    }
  
    
  }
  else if(nos[0]!=0)
  {
    pair=3;
    if(nos[0]<0)
    {
      sig=map(nos[0],-1,-10,1500,1200);
      //sig=1550;
      writeLR(sig);
    }
    else
    {
      sig=map(nos[0],1,10,1500,1800);
      //sig=1550;
      writeRR(sig);
    }
  }
  else if(nos[4]!=0)
  {
    pair=4;
    if(nos[4]<0)
    {
      sig=map(nos[4],-1,-10,1500,1200);
      writeLL(sig);
    }
    else
    {
      sig=map(nos[4],-1,-10,1500,1200);
      writeRRR(sig);
    }
  }
  else
  {
    sig=1500;
    switch(pair)
    {
      case 1: 
            pair=0;
            writeUD(sig);
            break;
      case 2:
            pair=0;
            writeFR(sig);
            writeBR(sig);
            break;
      case 3:
            pair=0;
            writeLR(sig);
            writeRR(sig);
            break;
      case 4:
            pair=0;
            writeLL(sig);
            writeRRR(sig);
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
