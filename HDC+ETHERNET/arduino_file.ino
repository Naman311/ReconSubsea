
/* controller keys
1.Left analog x axis
2.Left analog y axis
3.LT +ve RT -ve
4.Right y axis
5.Right x axis
6.A
7.B
8.X
9.Y
10.LB
11.RB
12.BACK
13.START
14.LEFT CLICK
15.RIGHT CLICK
*/

// Ethernet header
#include <Ethernet.h> //Load Ethernet Library
#include <EthernetUdp.h> //Load UDP Library
#include <SPI.h> //Load the SPI Library
#define UDP_TX_PACKET_MAX_SIZE 70 //increase UDP size

// thruster header
#include <Servo.h>

//  thruster init
byte servoPin = 9;
Servo servo;

/////////////////HDC1050///////////////////
#include <Wire.h>
#include <HDC1050.h>


// HDC1050 init
HDC1050 hdcSensor;
float tc, tf, h;
/////////////////////////////

// Etherner init
int t1=0;int t2=-1;int t3=4;int t4=2;
int v1=0; long y=0;long p=0; long r=0;

long temp;
long humidity;
long internal_pressure;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE}; //Assign a mac address
IPAddress ip(169, 254, 234, 100); //Assign my IP adress
unsigned int localPort = 5000; //Assign a Port to talk over
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
String datReq; //String for our data
int packetSize; //Size of Packet
EthernetUDP Udp; //Define UDP Object

// Function to convert sensors output to string
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
// function to covert string to int array
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

// function to control thrusters
void thruster_movement()
{
  int signal;
  if(nos[1]<=0)
  {
     signal = map(nos[1],-10,0,1100,1500);
  }
  else if(nos[1]>0)
  {
    signal = map(nos[1],1,10,1501,1900);
  }
  // thruster code
  //int signal = 1100; // Set signal value, which should be between 1100 and 1900
  //Serial.println(nos[1]);
  //Serial.println(signal);
  servo.writeMicroseconds(signal); // Send signal to ESC.
}
/*void move_down()
{
  int signal=map(nos[1],1,10,1501,1900);
  l.writeMicroseconds(signal);
  r.writeMicroseconds(signal);
}

void move_up()
{
  int signal=map(nos[1],-10,0,1100,1500);
  l.writeMicroseconds(signal);
  r.writeMicroseconds(signal);
}
void move_forward()
{
  int signal=map(nos[3],-10,0,1100,1500);
  fl.writeMicroseconds(signal);
  fr.writeMicroseconds(signal);
}
void move_backward()
{
  int signal=map(nos[3],1,10,1501,1900);
  fl.writeMicroseconds(signal);
  fr.writeMicroseconds(signal);
}
void move_left()
{
  int signal=map(nos[2],-10,0,1100,1500);
  fl.writeMicroseconds(signal);
  bl.writeMicroseconds(signal);
}
void move_right()
{
  int signal=map(nos[2],1,10,1501,1900);
  fr.writeMicroseconds(signal);
  br.writeMicroseconds(signal);
}
void pitch_anti()
{//yet to complete
  int signal=map(nos[0],-10,0,1100,1500);
}
void pitch_cloc()
{//yet to complete
  int signal=map(nos[0],1,10,1100,1500);
}
void movement()
{//l r fl fr bl br
  int l_signal,r_signal,fl_signal,fr_signal,bl_signal,br_signal;
  if(nos[1]<=0)
  {
    move_down();
  }
  else if(nos[1]>0)
  {
    move_up();
  }
  if(nos[0]<=0)
  {
    pitch_anti();
  }
  else if(nos[0]>0)
  {
    pitch_cloc();
  }
  if(nos[3]<=0)
  {
    move_backward();
  }
  else if(nos[3]>0)
  {
    move_forward();
  }
  if(nos[4]<=0)
  {
    move_left();
  }
  else if(nos[4]>0)
  {
    move_right();
  }
}*/
void setup() {
  // Ethernet setup
  Serial.begin(9600); //Turn on Serial Port
  
///////////////////////////HDC1050///////////////////////////////////
Wire.begin();
  
  //Serial.println(hdcSensor.getManufacturerID(), HEX);
  //Serial.println(hdcSensor.getDeviceID(), HEX);
  //Serial.println(hdcSensor.getSerialID());
  //Serial.println(hdcSensor.batteryOK());

  hdcSensor.turnOnHeater(true);
  hdcSensor.setTemperatureRes(HDC1050::T_RES_14);
  hdcSensor.setHumidityRes(HDC1050::H_RES_8);
  hdcSensor.updateConfigRegister();
/////////////////////////////////////////////////////////////////////
  
  Ethernet.begin(mac, ip); //Initialize Ethernet
  Udp.begin(localPort); //Initialize Udp
  delay(1500); //delay
  
  // thruster setup
  servo.attach(servoPin);

  servo.writeMicroseconds(1500); // send "stop" signal to ESC.
  delay(1000); // delay to allow the ESC to recognize the stopped signal
}

void loop() {
  //Ethernet code
tf = hdcSensor.getTemperatureHumidity(tc, h);
  
  temp = tc;
  humidity = h;
  //Serial.print(tc);
  //Serial.print("  ");
  //Serial.print(tf);
  //Serial.print("  ");
  //Serial.println(h);
 
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
    //Serial.println(datReq);

    thruster_movement();
  }
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
  Udp.print(output); //Send string back to client 
  Udp.endPacket(); //Packet has been sent
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
  //delay(100);
}
