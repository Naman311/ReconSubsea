#include <Ethernet.h> //Load Ethernet Library
#include <EthernetUdp.h> //Load UDP Library
#include <SPI.h> //Load the SPI Library
#define UDP_TX_PACKET_MAX_SIZE 70 //increase UDP size
#include <Servo.h>

int t1=0;int t2=-1;int t3=4;int t4=2;
int v1=0; long y=0;long p=0; long r=0;

long temp;
long humidity;
long internal_pressure;

String converstion(int t1,int t2,int t3,int t4,int v1,long y,long p,long r,long temp,long humidity,long internal_pressure)
{
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
byte left = 3;
Servo ll;
byte right = 6;
Servo rr;
byte frontLeft = 5;
Servo fl;
byte frontRight = 7;
Servo fr;
byte backLeft = 4;
Servo bl;
byte backRight = 8;
Servo br;

void move_up()
{
  int signal=1600;
  //int signal=map(nos[1],-1,-10,1501,1800);
  ll.writeMicroseconds(signal);
  rr.writeMicroseconds(signal);
}

void move_down()
{
  int signal=1400;
  //int signal=map(nos[1],1,10,1200,1499);
  ll.writeMicroseconds(signal);
  rr.writeMicroseconds(signal);
}

void move_forward()
{ Serial.println("for");
  int signal=1600;
  //int signal=map(nos[3],-1,-10,1501,1800);
  fl.writeMicroseconds(signal);
  fr.writeMicroseconds(signal);
}
void move_backward()
{
  Serial.println("back");
  int signal=1600;
  //int signal=map(nos[3],1,10,1501,1800);
  bl.writeMicroseconds(signal);
  br.writeMicroseconds(signal);
}
void move_left()
{
  int signal=1600;
  //int signal=map(nos[2],-10,-1,1200,1499);
  fl.writeMicroseconds(signal);
  bl.writeMicroseconds(signal);
}
void move_right()
{
  int signal=1600;
  //int signal=map(nos[2],1,10,1200,1499);
  fr.writeMicroseconds(signal);
  br.writeMicroseconds(signal);
}

void movement()
{//l r fl fr bl br
  if(nos[1]>0.0f)
  {
    Serial.println("down");
    move_down();
  }
  else if(nos[1]<0.0f)
  {
    Serial.println("up");
    move_up();
  }
  else
  {//Serial.println("zero y");
    int signal=1500;
    ll.writeMicroseconds(signal);
    rr.writeMicroseconds(signal);
  }
  if(nos[2]>0.0f)
  {//Serial.println("back");
    move_backward();
  }
  else if(nos[2]<0.0f)
  {//Serial.println("forword");
    move_forward();
  }/*
  {//Serial.println("zero x");
    int signal=1500;
    fl.writeMicroseconds(signal);
    fr.writeMicroseconds(signal);
    bl.writeMicroseconds(signal);
    br.writeMicroseconds(signal);
  }*/
  /*if(nos[4]<0.0f)
  {Serial.println("left");
    move_left();
  }
  else if(nos[4]>0.0f)
  {Serial.println("right");
    move_right();
  }*/
  else
  {//Serial.println("zero z");
    int signal=1500;
    fl.writeMicroseconds(signal);
    bl.writeMicroseconds(signal);
    fr.writeMicroseconds(signal);
    br.writeMicroseconds(signal);
  }
}
void setup() 
{
  Serial.begin(9600); //Turn on Serial Port
  Ethernet.begin(mac, ip); //Initialize Ethernet
  Udp.begin(localPort); //Initialize Udp
  ll.attach(left);
  rr.attach(right);
  fl.attach(frontLeft);
  fr.attach(frontRight);
  bl.attach(backLeft);
  br.attach(backRight);
  ll.writeMicroseconds(1500); // send "stop" signal to ESC.
  rr.writeMicroseconds(1500); // send "stop" signal to ESC.
  fl.writeMicroseconds(1500);
  fr.writeMicroseconds(1500);
  bl.writeMicroseconds(1500);
  br.writeMicroseconds(1500);
  
  delay(1500); //delay
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
    //Serial.println(datReq);
    addNos(datReq);
  }
  movement();
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
  Udp.print(output); //Send string back to client 
  Udp.endPacket(); //Packet has been sent
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
}
