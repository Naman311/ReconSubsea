/*
Author- Ankit Jha
*AJ*
*/
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
      //Serial.print("Configuring access point");
      bool stats=WiFi.softAPConfig(ip,gateway,subnet);    //wifi function returns boolean true or false when we configure it with data
      //Serial.println(stats==false?" Failed!!":" Ready!!");
      //Serial.println("Setting up access point:");
      WiFi.softAP(ssid);                //configuring soft access point
      Serial.print("IP address:");
      Serial.println(WiFi.softAPIP());  //Printing IP address
      server.begin();                   //server begins
      Serial.println("Starting server\n");
}
void endConnection()
{
  WiFi.disconnect();                               //Ends wiFi access point.
  //delay(4000);
  setupServer();
}

/*int getData()
{
  char c=Serial.read();
  String s;
  s+=c;
  int i=s.toInt();
  Serial.println("Sending"+String(i));
  return i;
}*/

int getStat()
{
  return 5;
}

void setup() {
      Serial.begin(115200);
      //Serial.println("Serial monitor started");
      delay(200);
      pinMode(pin,OUTPUT);
      setupServer();                          //setting up server for wifi named TheDarkKnight
}

void loop() {

      //Serial.println(WiFi.softAPIP());
      WiFiClient client = server.available();
      StrBuffer="";
      String ss;
      if(client)           //waiting for client to connect
      {
        //Serial.println("Looks like he is connected\n");
        while(client.connected())
        {
          delay(2);
          StrBuffer=client.readStringUntil('\r');             //reads string from client
          //Serial.print(StrBuffer);
//----------------------------------------------          
          //int b[]={0,0},bn=0,las=0;
          int buf=StrBuffer.toInt();
          /*for(int i=0;i<StrBuffer.length();i++)
          {
            if(StrBuffer[i]==',')
            {
              b[bn++]=(ss.substring(0)).toInt();            //converts a number from string to int
              //Serial.println(b[bn-1]);
              las=i+1;
              ss="";
            }
            else if(StrBuffer[i]==';')
            {
              b[bn++]=(ss.substring(0)).toInt();
              //Serial.println(b[bn-1]);
              ss="";
              break;
            }
            else ss=ss+StrBuffer[i];
          }*/
          Serial.print("Received:");
          Serial.println(buf);
          if(buf==1){
              //endConnection();
              break;
              }
          else {
              int dataa=getStat();
              ss=String(dataa)+"\r";
           }
          /*if(b[1]==0){ss+="Ron";}else {ss+="Roff";}
          */ 
          //-----------------------------------------------------------------          
          //flag=0;
          Serial.println("\nSending this to client:"+ss);
          client.print(ss);                                                     //Sending back status in string to client
          ss="";
          //if(flag==1)break;
          delay(2);
          //break;
          delay(300);
        }
           
        
      
      //delay(2);
      client.stop();
      Serial.println("Client disconnected :(");
      endConnection();                                                        //ending wifi connection for the nodeMCU for 5 sec
      }
      
  }

