#include <Ethernet.h> //Load Ethernet Library
#include <EthernetUdp.h> //Load UDP Library
#include <SPI.h> //Load the SPI Library
//#include <ESP8266WiFi.h>
//#include <WiFiClient.h>

int sendSPI=0;
byte a;
volatile char buf[100];
volatile int pos;
volatile bool received=false;
String dataSPI;
/*const char* ssid="My ASUS";
const char* password="password";
void EspConnect()
{
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println("WiFi Connected");
  Serial.println("IP"+WiFi.localIP());
}*/



 
#define UDP_TX_PACKET_MAX_SIZE 70 //increase UDP size

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

int nos[17];int nNos=-1;  

void addNos(String data)
{
  nNos=-1;
  String ss="";
  for(int i=0;i<data.length();i++)
  {
    if(data[i]==',')
    { nos[++nNos]=ss.toInt();
      ss="";
    }
    else if(i==data.length()-1)
    { ss+=data[i];
      nos[++nNos]=ss.toInt();
      ss="";
      break;
    }
    else 
    {
      ss+=data[i];
    }
  }
  
  for(int i=0;i<=nNos;i++){Serial.print(nos[i]);Serial.print(",");}
  Serial.println();
}

void setup() {
  
  //Serial.println("Starting Serial");
Serial.begin(9600); //Turn on Serial Port

 // ESP.reset();
 //ESP.wdtEnable(2000);

  SPCR |=bit(SPE) ;    //Configure Arduino in slave mode
  //SPCR |=bit(SPIE);
  pinMode(MISO,OUTPUT);
  SPI.attachInterrupt();
  
  delay(5);
  Serial.println("Starting Serial");
  Serial.println("Start");
  Ethernet.begin(mac, ip); 
  Serial.println("UDP");
  Udp.begin(localPort); //Initialize Udp

  Ethernet.begin(mac, ip); //Initialize Ethernet
//delay(10); //delay
  Serial.println("Start");
}

void loop() {
  Serial.println("Hello World");
  //if(onCheck==true)EspConnect();
          
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
  
  if(packetSize>0){ //Check to see if a request is present
  Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Reading the data request on the Udp
  datReq=packetBuffer; //Convert packetBuffer array to string datReq
  //Serial.println(datReq);
  addNos(datReq);
  }
  sendSPI=nos[9];
  if(received==true)
  { 
   for(int i=0;i<sizeof(buf);i++)dataSPI+=buf[i];
   Serial.println("String SPI:"+dataSPI);
  }
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
    Udp.print(output); //Send string back to client 
    Udp.endPacket(); //Packet has been sent
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
}

//Not Checked SPI Communication
ISR(SPI_STC_vect)
{
  SPDR=sendSPI;
  delayMicroseconds(5);
  byte c=SPDR;
  pos=0;
  if(pos<sizeof(buf));
  {
  buf[pos++]=c;
  if(c=='\n')received=true;
  }
}



