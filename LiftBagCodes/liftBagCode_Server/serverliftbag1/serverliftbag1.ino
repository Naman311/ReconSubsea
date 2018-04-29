#include <ESP8266WiFi.h>
#include <string>

int in = 16;

const char* ssid="LiftBag";
IPAddress ip(192,168,4,1);            //setting ip address
IPAddress gateway(192,168,1,1);       //setting gateway
IPAddress subnet(255,255,255,0);      //setting subnet
WiFiServer server(80);                //setting server

String StrBuffer="";

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
  setupServer();
}

void setup() {
      Serial.begin(115200);
      delay(200);
      setupServer();                          
      pinMode(in, OUTPUT);
      digitalWrite(in, HIGH);
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
          delay(2);
          StrBuffer=client.readStringUntil('\r');             //reads string from client
          Serial.println(StrBuffer);
          int buf=StrBuffer.toInt();
          if(buf==1)
          {
              ss="1\r";
              }
          else if (buf==2)
          {
              digitalWrite(in, LOW);
              ss="2\r";
           }
           else
           {
            ss="0/r";
           }
          client.print(ss);                                                     //Sending back status in string to client
          ss="";
       }
      client.stop();
      Serial.println("Client disconnected :(");
      endConnection();                                                        //ending wifi connection for the nodeMCU for 5 sec
     }
}

