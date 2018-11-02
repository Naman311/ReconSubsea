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

Servo servo;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
byte servoPin=3;
byte servoPin1=5;
byte servoPin2=6;
byte servoPin3=4;
byte servoPin4=7;
byte servoPin5=8 ;

void thruster_movement()
{Serial.println("scam tm");
  int signal=1500;
  if(nos[2]<0)
  {Serial.println("zzzzzzzzzzzz");
    signal=map(nos[2],-1,-10,1500,1600);
    servo.writeMicroseconds(signal);
    servo1.writeMicroseconds(signal);
    servo2.writeMicroseconds(signal);
    /*servo3.writeMicroseconds(signal);
    servo4.writeMicroseconds(signal);
    servo5.writeMicroseconds(signal);
  */}
  else if(nos[2]>0)
  {Serial.println("aaaaaaaaaaaa");
    signal=map(nos[2],1,10,1500,1400);
    /*servo.writeMicroseconds(signal);
    servo1.writeMicroseconds(signal);
    servo2.writeMicroseconds(signal);
    */servo3.writeMicroseconds(signal);
    servo4.writeMicroseconds(signal);
    servo5.writeMicroseconds(signal);
  }
  else{
    Serial.println("qqqqqqqqqqq");
    servo.writeMicroseconds(signal);
    servo1.writeMicroseconds(signal);
    servo2.writeMicroseconds(signal);
    servo3.writeMicroseconds(signal);
    servo4.writeMicroseconds(signal);
    servo5.writeMicroseconds(signal);
    }
  
}


void setup() {
  Serial.begin(9600); //Turn on Serial Port
  Ethernet.begin(mac, ip); //Initialize Ethernet
  Udp.begin(localPort); //Initialize Udp
  servo.attach(servoPin);
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);
  servo5.attach(servoPin5);
  servo.writeMicroseconds(1500);
  servo1.writeMicroseconds(1500);
  servo2.writeMicroseconds(1500);
  servo3.writeMicroseconds(1500);
  servo4.writeMicroseconds(1500);
  servo5.writeMicroseconds(1500);
  delay(1000);
  Serial.println("scam steup");
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
  if(nos[0]!=0 || nos[1]!=0)Serial.println("LS");
  else if(nos[2]>0)Serial.println("LT");
  else if(nos[2]<0)Serial.println("RT");
  if(nos[8]==1) 
  {
    Serial.println('Y');
    
  }
  thruster_movement();
  Serial.println("scam loop");
}
