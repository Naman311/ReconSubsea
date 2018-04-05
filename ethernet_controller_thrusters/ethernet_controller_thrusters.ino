
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
  thruster_movement()
  {
    
    // thruster code
    int signal = 1100; // Set signal value, which should be between 1100 and 1900
  
    servo.writeMicroseconds(signal); // Send signal to ESC.
  }

void setup() {
  // Ethernet setup
  Serial.begin(9600); //Turn on Serial Port
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
    thruster_movement();
  }
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
  Udp.print(output); //Send string back to client 
  Udp.endPacket(); //Packet has been sent
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
}
