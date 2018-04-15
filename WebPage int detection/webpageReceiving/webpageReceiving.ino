/*#include <ESP8266WiFi.h>


const char* ssid="OBS";
WiFiClient client;
const char* host="192.168.2.7";

*/
//String dataRec ="<!DOCTYPE><html><body><h1>MATE 2018 SEISMOGRAPH STATUS</h1><p>Voltage=4.995 Xangle=1.506 Yangle=3.067 Count=0</p><p>DATA 0,4.4,-4.3,0,0,6.8,-7,0,0,1.6,-1.6,0,0,6.3,-6.4,0</p></body></html>";

String html =  "<!DOCTYPE><html><body><h1>MATE 2018 SEISMOGRAPH STATUS</h1><p>Voltage=4.995 Xangle=1.506 Yangle=3.067 Count=0</p><p>DATA 0,4.4,-4.3,0,0,6.8,-7,0,0,1.6,-1.6,0,0,6.3,-6.4,0</p></body></html>";
float arr[40];int narr=-1;

String test="ThisIsToTestLength";


void intArray()
{ int check=false;
Serial.println(html.length());
  for(int i=0;i<html.length();i++)
  { //Serial.println(html.substring(i,i+6));
    if(html.substring(i,i+8)=="Voltage=")
    {   //Serial.println("Voltage");
        i+=8;
        int j;
        String ss="";
        for(j=i;html[j]!='X';j++)
        if(html[i]!=' ')
        ss+=html[j];
        else break;
        i=j-1;
        Serial.println("Voltage:"+ss);
        arr[++narr]=ss.toFloat();
        
    }
    else if(html.substring(i,i+7)=="Xangle=")
    {
        //Serial.println("Xangle");
        
        i+=7;
        int j;
        String ss="";
        for(j=i;html[j]!='Y';j++)
        ss+=html[j];
        i=j-1;
        Serial.println("Xangle:"+ss);
        arr[++narr]=ss.toFloat();
        
    }      
    else if(html.substring(i,i+7)=="Yangle=")
    {
        //Serial.println("Yangle");
        
        i+=7;
        int j;
        String ss="";
        for(j=i;html[j]!='C';j++)
        ss+=html[j];
        i=j-1;
        Serial.println("Yangle:"+ss);
        arr[++narr]=ss.toFloat();
        
    }
    
    else if(html.substring(i,i+6)=="Count=")
    {
        //Serial.println("Yangle");
        
        i+=6;
        int j;
        String ss="";
        for(j=i;html[j]!='<';j++)
        ss+=html[j];
        i=j;
        Serial.println("Count:"+ss);
        arr[++narr]=ss.toFloat();
        
    }
    else if(html.substring(i,i+5)=="DATA ")
    {
        //Serial.println("Data");
        
        i+=6;
        int j;
        int nn=0;
        String ss="";
        for(int j=i;j<html.length()&&nn!=16;j++)
        {
          if(html[j]==',')
          {
            arr[++narr]=ss.toFloat();
            ss="";
            nn++;
          }
          else if(html[j]=='<')
          {
            arr[++narr]=ss.toFloat();
            ss="";
            nn++;
          }
          else ss+=html[j];
        }        
        
       
    }
    
  }
  Serial.println("19 points");
  for(int i=0;i<=narr;i++){Serial.print("["+String(i)+"]");Serial.println(arr[i],3);}
}


void setup() 
{
  //dataRec=html;
  Serial.begin(9600);
  /*WiFi.begin(ssid);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.println(".");
    delay(100);
  }
  Serial.println("WiFi connected \nIP address:"+String(WiFi.localIP()));*/
}

void loop() 
{
  //html="";
  for(int i=0;i<=narr;i++)arr[i]=0;
  narr=-1;
  /*
  if(client.connect(host,80))
  {
    //String getStr=_getLink;

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    Serial.println("Now Reading Response");
    while(client.connected())
    {
      if(client.available())
      {
        String line=client.readStringUntil('\n');
        dataRec+=line;
        Serial.println(line);
      }
    }
    intArray();
    Serial.println("Disconnected");
    client.stop();
  }*/
  intArray();
  
  //Serial.println(test.length());
  delay(500);
}
