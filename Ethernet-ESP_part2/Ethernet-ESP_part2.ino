#include <ESP8266WiFi.h>
#include <Ethernet.h> //Load Ethernet Library
#include <EthernetUdp.h> //Load UDP Library
#include <SPI.h> //Load the SPI Library

//-------------------------------Global Variables(Ethernet)------------------------------------

#define UDP_TX_PACKET_MAX_SIZE 70 //increase UDP size
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE}; //Assign a mac address
IPAddress ip(169, 254, 234, 100); //Assign my IP adress
unsigned int localPort = 5000; //Assign a Port to talk over
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
String datReq; //String for our data
int packetSize; //Size of Packet
EthernetUDP Udp; //Define UDP Object

//-----------------------------Global Variables(WiFi)-------------------------------------------

const char* ssidLiftBag="LiftBag1";
const char* ssidOBS="OBSWiFi";
const char* hostLiftBag="192.168.0.4";
const char* hostOBS="192.168.1.10";

//--------------------------Other Variables--------------------------------------------------------

int t1=0;int t2=-1;int t3=4;int t4=2;
int v1=0; long y=0;long p=0; long r=0;
int nos[17]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};int nNos=-1;               //For Ethernet receiving
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
  result+=',';
  return (result);
  }

//------------------------------------Certain Functions--------------------------------------
void addNos(String data)
{
  nNos=-1;
  String ss="";
  for(int i=0;i<data.length();i++)
  {
    if(data[i]==','||data[i]!=data[data.length()-1])
    {
      nos[++nNos]==ss.toInt();
      ss="";
    }
    else if(data[i]==';')
    {
      nos[++nNos]=ss.toInt();
      ss="";
      break;
    }
    else ss+=data[i];
  }
  for(int i=0;i<=nNos;i++){Serial.print(data[i]);Serial.print(" ");}
  Serial.println();
}
//------------------------------variables for WiFi---------------------------------
bool conStatus=false;
WiFiClient client;
void liftBagConfigure()
{
  WiFi.begin(ssidLiftBag);
  while(WiFi.status()!=WL_CONNECTED){
    delay(300);
    Serial.print(".");
  }
}
void obsConfigure()
{
  WiFi.begin(ssidOBS);
  while(WiFi.status()!=WL_CONNECTED){
    delay(300);
    Serial.print(".");
    }
}

void setup() {
Serial.println("Initialized.........1");
Serial.begin(115200); //Turn on Serial Port
Serial.println("Initialized.........2");
Ethernet.begin(mac, ip); //Initialize Ethernet
Serial.println("Initialized.........3");
Udp.begin(localPort); //Initialize Udp
Serial.println("Initialized.........4");
delay(1500); //delay
Serial.println("Initialized.........5");
WiFi.mode(WIFI_STA);
}

void loop() {

//----------------------------Randomising Variables-----------------------------------------
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
    Serial.println("String Received"+datReq);
    addNos(datReq);
    switch(nos[9])
    { 
      case 0: Serial.println("Case 0");
            client.stop();
            //WiFi.end();
            break;
      case 1: if(conStatus!=true){
        liftBagConfigure();
        for(;client.connect(hostLiftBag,80)!=true;);
        conStatus=true;
        }
        if(client.connected())
        {
          client.print("1,1;\r\n");
          String line="";
          if(client.available()){
          line=client.readStringUntil('\r');
          Serial.println(line);
          }
        }
        else conStatus=false;
        break;
    case 2:
            if(conStatus!=true){liftBagConfigure();
            for(;client.connect(hostLiftBag,80)!=true;);
            conStatus=true;
            }
            if(client.connected())
            {
              client.print("0,0;\r\n");
              String line="";
              if(client.available()){
                line=client.readStringUntil('\r');
                Serial.println("line");
              }
            }
            else conStatus=false;
            client.stop();
            //WiFi.end();
            break;
    case 3:
            if(conStatus!=true){
              obsConfigure();
              for(;client.connect(hostOBS,80)!=true;);
            conStatus=true;
            }
            client.print(String("GET ") +"/Led"+" HTTP/1.1\r\n" + "Host: " + hostOBS + "\r\n" + "Connection: close\r\n\r\n");          
            if(client.available()){
                String line = client.readStringUntil('\r');
                Serial.print(line);
            }
            break;
    default: Serial.println("Different Data Received");
  }
  }
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
    Udp.print(output); //Send string back to client 
    Udp.endPacket(); //Packet has been sent
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
}
