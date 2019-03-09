#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
 
byte mac[] = { 0x7E, 0xAC, 0xBE, 0xEF, 0x2E, 0xEB}; //Assign a mac address
IPAddress ip(192,168,1,177);
unsigned int localPort = 5002;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
String datReq; 
int packetSize; 
EthernetUDP Udp; //Define UDP Object
 
void setup() {
  
Serial.begin(9600); 
Ethernet.begin(mac, ip);
Udp.begin(localPort);
//delay(1500);
}
 
void loop() {  
  packetSize = Udp.parsePacket(); //Read theh packetSize  
  if(packetSize>0)
  {   
      Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); 
      datReq=packetBuffer;   
      Serial.println(datReq);
      if(datReq =="orange")
      {   
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  
        Udp.print("You are Asking for Orange");  
        Udp.endPacket(); 
      }
   }
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
}

