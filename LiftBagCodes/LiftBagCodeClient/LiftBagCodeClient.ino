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
/*int switchCase(int rw)
{
  switch(rw)
  {
    case 0: return 5;
    case 1: return 6;
    case 2: return 7;
    case 3: return 1;
    default: return 9; 
  }
}*/



void endConnection()
{
  client.stop();
  WiFi.disconnect();
  Serial.println("Reconnecting after 6 seconds");
  delay(6000);
  connectStatus=false;
  //connectHotspot2();
  connecthotspot();                         //again initialise the hotsopt
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  
  int data=0;                                                     //replace d with switch case
  
  if(data==0&&connectStatus==false)connecthotspot();
  String ss="";
  if(client.connect(host,80))
  {
    Serial.println("Connected");
    while(client.connected())
    {
      data=switchh();
      if(data==0)data=1;
      Serial.println("Sending "+String(data));
      String d=String(data);
      client.println(d);                                        //sends the integer
      delay(4);
      while(client.available())
      {
        String s3=client.readStringUntil('\r');                 //Reads the response
        ss=s3;
        
      }
      int sw=ss.toInt();
      
      
      Serial.print("Received:");
      Serial.println(ss+"\n");                                       //Just prints data for now
       //if(data==1){break;}
      delay(300);
    }
    Serial.println("COnnection disabled");
    endConnection();
  }
  else 
  {
    Serial.println("Connection failed");
    client.stop();
  }
  
}
