/*
Author- Ankit Jha
*AJ*
*/
#include <ESP8266WiFi.h>
const char* ssid="TheDarkKnight";
IPAddress ip(192,168,0,4);            //setting ip address
IPAddress gateway(192,168,1,1);       //setting gateway
IPAddress subnet(255,255,255,0);      //setting subnet
WiFiServer server(80);                //setting server

String StrBuffer="";
int flag=0;
void setup() {
      Serial.begin(115200);
      Serial.println("Serial monitor started");
      delay(200);
      Serial.print("Configuring access point");
      bool stats=WiFi.softAPConfig(ip,gateway,subnet);    //wifi function returns boolean true or false when we configure it with data
      Serial.println(stats==false?"Failed!!":"Ready!!");
      Serial.println("Setting up access point:");
      WiFi.softAP(ssid);                //configuring soft access point
      Serial.print("IP address:");
      Serial.println(WiFi.softAPIP());  //Printing IP address
      server.begin();                   //server begins
      Serial.println("Starting server\n");
}

void loop() {
      WiFiClient client = server.available();
      if(flag==0)StrBuffer="";
      if(client)           //waiting for client to connect
      {
        Serial.println("Looks like he is connected\n");
        while(client.connected())
        {
          delay(2);
          StrBuffer=client.readStringUntil('\r');
          Serial.print(StrBuffer);
          
          
          flag=0;
          Serial.println();
          String s1="123";
          s1=s1+": assume this as status \r";
          client.print(s1);
          delay(2);
          //break;
        }
           
        
      
      //delay(2);
      client.stop();
      Serial.println("Client disconnected :(");
      }
  }

