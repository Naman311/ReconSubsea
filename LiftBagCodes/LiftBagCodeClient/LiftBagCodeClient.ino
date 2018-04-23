#include <ESP8266WiFi.h>
#include <string>
const char* ssid="LiftBag";
const char* host="192.168.0.4";
WiFiClient client;
bool connectStatus=false;

//int dataLiftBag=0;
//int connectBit=2;

void connecthotspot()
{
  WiFi.begin(ssid);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println("WiFi connected:"+WiFi.localIP());
  connectStatus=true;
}

int switchh()
{
  char c=Serial.read();
  String s;
  s+=c;
  int i=s.toInt();
  return i;
}

/*void endConnection()
{
  client.stop();
  WiFi.disconnect();
  Serial.println("Reconnecting after 6 seconds");
  delay(6000);
  connectStatus=false;
  //connectHotspot2();
  connecthotspot();                         //again initialise the hotsopt
}*/

void setup() {
  Serial.begin(115200);
}

String ss="";

void loop()
{
  char c=Serial.read();
  String s;
  s+=c;
  int data=s.toInt();
  if(data==0)
  {
    connecthotspot();
  }
  else if(data==1)
  {
      if(!client.connect(host,80))
      {
        Serial.println("connection failed");
      }
      while(client.connected())
      {
        Serial.println("Sending "+String(data));
        String d=String(data);
        client.println(d);                                        //sends the integer
        //delay(4);
        while(client.available())
        {
          String s3=client.readStringUntil('\r');                 //Reads the response
          ss=s3;
        }
        int sw=ss.toInt();
        Serial.print("Received:");
        Serial.println(ss+"\n");                                       //Just prints data for now
      }
  }
  else if(data==2)
  {
    if(!client.connect(host,80))
    {
      Serial.println("connection failed");
    }
    while(client.connected())
    {
      Serial.println("Sending "+String(data));
      String d=String(data);
      client.println(d);                                        //sends the integer
      //delay(4);
      while(client.available())
      {
        String s3=client.readStringUntil('\r');                 //Reads the response
        ss=s3;
      }
      int sw=ss.toInt();
      Serial.print("Received:");
      Serial.println(ss+"\n");                                       //Just prints data for now
    }
  }
}

