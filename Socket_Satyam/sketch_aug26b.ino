
#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h> 
int packetsize;

byte mac[] = {
  0x7E, 0xAC, 0xB7, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

unsigned int localPort = 5000;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back
EthernetUDP Udp;  
String datReq;



void setup() {
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  Serial.begin(9600);
  // put your main code here, to run repeatedly:

}
void loop()
{
  packetsize=Udp.parsePacket();
  Udp.write("jasnkajd");
  if(packetsize)
  {
    Serial.write("rex");
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    datReq=packetBuffer;
    if (datReq=="red")
    {
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(ReplyBuffer);
      Udp.endPacket();
    }
  }
  memset(packetBuffer,0,UDP_TX_PACKET_MAX_SIZE);
  
  
}

