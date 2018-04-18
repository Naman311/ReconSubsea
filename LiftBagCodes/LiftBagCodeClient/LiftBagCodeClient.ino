#include <ESP8266WiFi.h>
#include <string>
const char* ssid="LiftBag";
const char* host="192.168.0.4";
WiFiClient client;
bool connectStatus=false;
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

void endConnection()
{
  client.stop();
  WiFi.disconnect();
  delay(3000);
  connectStatus=false;
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
      Serial.println("Sending "+String(data));
      String d=String(data);
      client.println(d);                                        //sends the integer
      delay(4);
      while(client.available())
      {
        String s3=client.readStringUntil('\r');                 //Reads the response
        ss+=s3;
        if(s3[0]!='\0')break;
      }
      Serial.println(ss);                                       //Just prints data for now
       if(data==1)break;                                        
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
