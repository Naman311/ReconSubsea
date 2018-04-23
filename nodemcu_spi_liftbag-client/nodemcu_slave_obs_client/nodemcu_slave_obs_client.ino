/*
    GPIO    NodeMCU   Name  |   Uno
  ===================================
     15       D8       SS   |   D10
     13       D7      MOSI  |   D11
     12       D6      MISO  |   D12
     14       D5      SCK   |   D13
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 

#include "SPISlave.h"
String Str="NULL";

#include <string>
const char* ssid="LiftBag";
const char* host="192.168.0.4";

const char* ssid1 = "OBS";
const char* host1 = "192.168.4.1";

String html = "";
float arr[40]; int narr = -1;

WiFiClient client;
const int httpPort = 80;

int curr=0;
int stat=-1;
int flag=0;
char buff[100]={0};

// function to connect to server
void liftbag_cli()
{
  WiFi.begin(ssid);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println("WiFi connected:"+WiFi.localIP());
  Serial.print("connecting to ");
  Serial.println(host); 
}

void obs_cli()
{
  WiFi.begin(ssid1);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println("WiFi connected:"+WiFi.localIP());
  Serial.print("connecting to ");
  Serial.println(host1); 
}

void clear_buff()
{
  for(int ii=0;ii<100;ii++)
  {
    buff[ii]=0;
  }
}

////////////

void setup() {
//client setup
  Serial.begin(115200);
  delay(10);                     //Baud rate prper initialization
  pinMode(13,INPUT);             //Pin D7 on NodeMcu Lua. Button to switch on and off the solenoid.
  WiFi.mode(WIFI_STA);           //NodeMcu esp12E in station mode
  WiFi.persistent(false);
/////////////////////////


//slave setup
  Serial.setDebugOutput(true);

  // data has been received from the master. Beware that len is always 32
  // and the buffer is autofilled with zeroes if data is less than 32 bytes long
  // It's up to the user to implement protocol for handling data length
  SPISlave.onData([](uint8_t * data, size_t len) 
  {
    String message = String((char *)data);
    (void) len;
    if(message.equals("1"))
    {
      curr=1;
    }
    else if(message.equals("2"))
    {
      curr=2;
    }
    else if(message.equals("3"))
    {
      curr=3;
    }
    else
    {
      curr=0;
    }
    if (curr==1 && stat!=1) 
    {
      stat=1;
      Str.toCharArray(buff,100);
      SPISlave.setData(buff);
      clear_buff();
    } 
    else if (curr==2 && stat!=2) 
    {
      stat=2;
      Str.toCharArray(buff,100);
      SPISlave.setData(buff);
      clear_buff();
    } 
    else if(curr==3 && stat!=3) 
    {
      stat=3;
      Str.toCharArray(buff,100);
      SPISlave.setData(buff);
      clear_buff();
    }
    else if(curr==0)
    {
      Serial.printf("Question:%s||\n",(char *)data);
    }
  });

  // The master has read out outgoing data buffer
  // that buffer can be set with SPISlave.setData
  SPISlave.onDataSent([]() 
  {
    //Serial.println("Answer Sent");
  });

  // status has been received from the master.
  // The status register is a special register that bot the slave and the master can write to and read from.
  // Can be used to exchange small data or status information
  SPISlave.onStatus([](uint32_t data)
  {
    //Serial.printf("Status: %u\n", data);
    //SPISlave.setStatus(millis()); //set next status
  });

  // The master has read the status register
  SPISlave.onStatusSent([]()
  {
    //Serial.println("Status Sent");
  });

  // Setup SPI Slave registers and pins
  SPISlave.begin();

  // Set the status register (if the master reads it, it will read this value)
  SPISlave.setStatus(millis());

  // Sets the data registers. Limited to 32 bytes at a time.
  // SPISlave.setData(uint8_t * data, size_t len); is also available with the same limitation
  Str.toCharArray(buff,100);
  SPISlave.setData(buff);
  clear_buff();
///////////////////////
}
void intArray()
{ 
  int check = false;
  //Serial.println(html.length());
  for (int i = 0; i < html.length(); i++)
  { 
    //Serial.println(html.substring(i,i+6));
    if (html.substring(i, i + 8) == "Voltage=")
    { 
      //Serial.println("Voltage");
      i += 8;
      int j;
      String ss = "";
      for (j = i; html[j] != 'X'; j++)
        if (html[i] != ' ')
          ss += html[j];
        else break;
      i = j - 1;
      //Serial.println("Voltage:" + ss);
      arr[++narr] = ss.toFloat();
    }
    else if (html.substring(i, i + 7) == "Xangle=")
    {
      //Serial.println("Xangle");
      i += 7;
      int j;
      String ss = "";
      for (j = i; html[j] != 'Y'; j++)
        ss += html[j];
      i = j - 1;
      //Serial.println("Xangle:" + ss);
      arr[++narr] = ss.toFloat();
    }
    else if (html.substring(i, i + 7) == "Yangle=")
    {
      //Serial.println("Yangle");
      i += 7;
      int j;
      String ss = "";
      for (j = i; html[j] != 'C'; j++)
        ss += html[j];
      i = j - 1;
      //Serial.println("Yangle:" + ss);
      arr[++narr] = ss.toFloat();
    }
    else if (html.substring(i, i + 6) == "Count=")
    {
      //Serial.println("Yangle");
      i += 6;
      int j;
      String ss = "";
      for (j = i; html[j] != '<'; j++)
        ss += html[j];
      i = j;
      //Serial.println("Count:" + ss);
      arr[++narr] = ss.toFloat();
    }
    else if (html.substring(i, i + 5) == "DATA:")
    {
      //Serial.println("Data");
      i += 6;
      int j;
      int nn = 0;
      String ss = "";
      for (int j = i; j < html.length() && nn != 16; j++)
      {
        if (html[j] == ',')
        {
          arr[++narr] = ss.toFloat();
          ss = "";
          nn++;
        }
        else if (html[j] == '<')
        {
          arr[++narr] = ss.toFloat();
          ss = "";
          nn++;
        }
        else ss += html[j];
      }
     }
   }
  //Serial.println("19 points");
  for (int i = 0; i <= narr; i++) 
  {
    if(i!=0 || i==narr)
    {
      Str+=',';
    }
    Str+=String(arr[i]);
    //Serial.print("[" + String(i) + "]");
    //Serial.println(arr[i], 3);
  }
  //delay(500);
}

void loop() 
{
    if(stat==1 && flag!=1)
    {
      flag=1;
      liftbag_cli();
      while(stat==1)
      {
        String ss="";
        // Use WiFiClient class to create TCP connections
        if (!client.connect("192.168.4.1", httpPort)) 
        {
          Serial.println("connection failed");
          flag=0;
          return;
        }   
        //Request to server to activate the led
        String d=String(stat);
        client.println(d);      
        delay(10);
        //Read all the lines of the reply from server and print them to Serial Monitor etc
        while(client.available())
        {
          String line = client.readStringUntil('\r');
          Str=line;
          //Serial.print(line);
        }
      }           
    }
    else if(stat==2 && flag!=2)
    {
      flag=2;
      Str="NULL";
      client.stop();
      WiFi.disconnect();  // Erases SSID/password
    }
    else if(stat==3 && flag!=3)
    {
      flag=3;
      obs_cli();
      // Use WiFiClient class to create TCP connections
      while(stat==3)
      {
        for (int i = 0; i <= narr; i++)arr[i] = 0;
        narr = -1;
        html = "";
        if (!client.connect("192.168.4.1", httpPort)) 
        {
          Serial.println("connection failed");
          flag=0;
          return;
        }   
       //Request to server to activate the led
       client.print(String("Get /") + " HTTP/1.1\r\n" +
           "Host: " + String(host) + "\r\n" +
           "Connection: close\r\n" + "\r\n");      
        delay(10);
        //Read all the lines of the reply from server and print them to Serial Monitor etc
        while(client.available())
        {
          html += client.readStringUntil('\n');
        }
      //Serial.println(html);
      intArray();
      }     
    }
}

