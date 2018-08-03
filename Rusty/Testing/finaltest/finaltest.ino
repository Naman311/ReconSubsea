// THRUSTER MANU ETHERNET RETURN RANDOM
//ll=3
//bl=4
//fl=5
//rr=6
//fr=7
//br=8
                   
/*//Motor A
const int inputPin1  = 10;    
const int inputPin2  = 11;    
//Motor B
const int inputPin3  = 12;   
const int inputPin4  = 13;
*/
#include <Servo.h>
#include <Ethernet.h> //Load Ethernet Library
#include <EthernetUdp.h> //Load UDP Library
#include <SPI.h> //Load the SPI Library
#define UDP_TX_PACKET_MAX_SIZE 70 //increase UDP size

int t1=0;int t2=-1;int t3=4;int t4=2;
int v1=0; long y=0;long p=0; long r=0;

long temp;
long humidity;
long internal_pressure;

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
  //for(int i=0;i<=nNos;i++){Serial.print(nos[i]);Serial.print(",");}
  //Serial.println();
}
byte ll= 3;
Servo left;

byte rr = 6;
Servo right;

byte fr = 7;
Servo frontRight;

byte fl = 5;
Servo frontLeft;

byte bl = 4;
Servo backLeft;

byte br = 8;
Servo backRight;

//Motor A

const int inputPin5  = 10;    
const int inputPin6  = 11; 

//Motor C
const int inputPin1  = 12;   
const int inputPin2  = 13;

void move_up()
{
  //int signal=1700;
  int signal=map(nos[1],-1,-10,1501,1800);
  left.writeMicroseconds(signal);
  right.writeMicroseconds(signal);
}

void move_down()
{
  //int signal=1300;
  int signal=map(nos[1],1,10,1200,1499);
  left.writeMicroseconds(signal);
  right.writeMicroseconds(signal);
}

void move_forward()
{ 
  //int signal=1700;
  int signal=map(nos[3],-1,-10,1501,1800);
  frontLeft.writeMicroseconds(signal);
  frontRight.writeMicroseconds(signal);
}
void move_backward()
{
  //int signal=1300;
  int signal=map(nos[3],1,10,1501,1800);
  backLeft.writeMicroseconds(signal);
  backRight.writeMicroseconds(signal);
}
void move_left()
{
  //int signal=1300;
  int signal=map(nos[2],-10,-1,1200,1499);
  frontLeft.writeMicroseconds(signal);
  backLeft.writeMicroseconds(signal);
}
void move_right()
{
  //int signal=1700;
  int signal=map(nos[2],1,10,1200,1499);
  frontRight.writeMicroseconds(signal);
  backRight.writeMicroseconds(signal);
}

void movement()
{//l r fl fr bl br
  if(nos[1]>0)
  {
    move_down();
  }
  else if(nos[1]<0)
  {
    move_up();
  }
  else
  {
    int signal=1500;
    left.writeMicroseconds(signal);
    right.writeMicroseconds(signal);
  }
  if(nos[3]>0)
  {
    move_backward();
  }
  else if(nos[3]<0)
  {
    move_forward();
  }
  else
  {
    int signal=1500;
    frontLeft.writeMicroseconds(signal);
    frontRight.writeMicroseconds(signal);
    backLeft.writeMicroseconds(signal);
    backRight.writeMicroseconds(signal);
  }
  if(nos[4]<0)
  {
    move_left();
  }
  else if(nos[4]>0)
  {
    move_right();
  }
  else
  {
    int signal=1500;
    frontLeft.writeMicroseconds(signal);
    backLeft.writeMicroseconds(signal);
    frontRight.writeMicroseconds(signal);
    backRight.writeMicroseconds(signal);
  }
}

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

void manu_move()
{
  if(nos[5]==1)
  {
    digitalWrite(inputPin2, LOW);
    digitalWrite(inputPin1, HIGH);
    delay(1500);
    //GALTI HO SAKTI HAI
  }
  else if(nos[6]==1)
  {
    digitalWrite(inputPin1, LOW);
    digitalWrite(inputPin2, HIGH);
    delay(1500);
  }
  else
  {
    digitalWrite(inputPin1, LOW);
    digitalWrite(inputPin2, LOW);
  }
  if(nos[7]==1)
  {
    digitalWrite(inputPin6, LOW);
    digitalWrite(inputPin5, HIGH);
  }
  else if(nos[8]==1)
  {
    digitalWrite(inputPin5, LOW);
    digitalWrite(inputPin6, HIGH);
  }
  else
  {
    digitalWrite(inputPin5, LOW);
    digitalWrite(inputPin6, LOW);
  }
}



void setup() {
  Serial.begin(115200); //Turn on Serial Port
  Ethernet.begin(mac, ip); //Initialize Ethernet
  Udp.begin(localPort); //Initialize Udp
  left.attach(ll);
  right.attach(rr);
  frontLeft.attach(fl);
  frontRight.attach(fr);
  backLeft.attach(bl);
  backRight.attach(br);
  left.writeMicroseconds(1500); // send "stop" signal to ESC.
  right.writeMicroseconds(1500); // send "stop" signal to ESC.
  frontLeft.writeMicroseconds(1500);
  frontRight.writeMicroseconds(1500);
  backLeft.writeMicroseconds(1500);
  backRight.writeMicroseconds(1500);
  pinMode(inputPin1, OUTPUT);
  pinMode(inputPin2, OUTPUT);

  pinMode(inputPin5, OUTPUT);
  pinMode(inputPin6, OUTPUT);
  digitalWrite(inputPin1, LOW);
    digitalWrite(inputPin2, LOW);
    digitalWrite(inputPin5, LOW);
    digitalWrite(inputPin6, LOW);
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
  { //Check to see if a request is present
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Reading the data request on the Udp
    datReq=packetBuffer; //Convert packetBuffer array to string datReq
    addNos(datReq);
    Serial.println(datReq);
    }
    movement();
    manu_move();
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
  Udp.print(output); //Send string back to client 
  Udp.endPacket(); //Packet has been sent
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
  } 
  // put your main code here, to run repeatedly:

