#include <ESP8266WiFi.h>
#include <string>
const char* ssid="LiftBag";
IPAddress ip(192,168,0,4);            //setting ip address
IPAddress gateway(192,168,1,1);       //setting gateway
IPAddress subnet(255,255,255,0);      //setting subnet
WiFiServer server(80);                //setting server
int pin=5;                            //for relay pin

String StrBuffer="";
int flag=0;

void setupServer()
{
      bool stats=WiFi.softAPConfig(ip,gateway,subnet);    //wifi function returns boolean true or false when we configure it with data
      WiFi.softAP(ssid);                //configuring soft access point
      Serial.print("IP address:");
      Serial.println(WiFi.softAPIP());  //Printing IP address
      server.begin();                   //server begins
      Serial.println("Starting server\n");
}
void endConnection()
{
  WiFi.disconnect();                               //Ends wiFi access point.
  delay(4000);
  setupServer();
}

int getStat()
{
  return 5;
}

void setup() 
{
    Serial.begin(115200);
    delay(200);
    pinMode(pin,OUTPUT);
    setupServer();                          //setting up server for wifi named TheDarkKnight
}

void loop() 
{

    WiFiClient client = server.available();
    StrBuffer="";
    String ss;
    if(client)           //waiting for client to connect
    {
      while(client.connected())
      {
        StrBuffer=client.readStringUntil('\r');             //reads string from client
        int buf=StrBuffer.toInt();
        Serial.print("Received:");
        Serial.print(buf);
        int data=buf*2;
        ss=String(data)+"\r";
        Serial.println("\nSending this to client:"+ss);
        client.print(ss);                                                     //Sending back status in string to client
        ss="";
        delay(300);
      }
    client.stop();
    Serial.println("Client disconnected :(");
    endConnection();                                                        //ending wifi connection for the nodeMCU for 5 sec
    }  
}

