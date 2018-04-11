/*
Author - Ankit Jha
*AJ*
*/
#include <ESP8266WiFi.h>
const char* ssid="TheDarkKnight";
const char* host="192.168.0.4";

void setup() {
  Serial.begin(115200);
  Serial.println("Serial Monitor Started");
  delay(200);
  Serial.printf("Connecting to %s ",ssid);
  WiFi.begin(ssid);
  while (WiFi.status()!=WL_CONNECTED){
    delay(300);
    Serial.print(".");
  }
  Serial.printf("Connected to Access point %s\n",ssid);
}

void loop() {
  WiFiClient client;
  Serial.printf("connecting to [%s]\n",host);
  if(client.connect(host,80))
  {
    Serial.printf("Connected to %s",host);
    
   
    while(client.connected())
    {
    
        Serial.println("\nSending request to server");
        int chk=10;int pre=20;
        String s1=String(chk);String s2=String(pre);
        String s3=s1+" "+s2+"\r\n";
        client.print(s3);
        Serial.println(s3);
        delay(4);
        Serial.println("Receiving data..");
        String line="";
        while(client.available()){
        line=client.readStringUntil('\r');
        if(line[0]!='\0')break;
        }
        //Serial.println("\n");
        Serial.println(line);
     
    }
    Serial.println("fasdhibfasdkjsfb");
    client.stop();
    Serial.println("\n[Disconnected]");
  }
  else
  {
    Serial.println("connection failed!!!");
    client.stop();
  }
  delay(2000);
}
