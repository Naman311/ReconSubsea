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

const char* host = "192.168.4.1"; //Ip of the Host(Our Case esp8266-01 as server. Its the ip of the esp8266-01 as Access point)
const char* host2 = "";

WiFiClient client;
const int httpPort = 80;

int stat=0;
int flag=0;
String prev_data="Zero";
// function to connect to server
void conn_cli()
{
  WiFi.begin("ESP_D54736");      //Connect to this SSID. In our case esp-01 SSID.  
  while (WiFi.status() != WL_CONNECTED) {      //Wait for getting IP assigned by Access Point/ DHCP. 
                                              //Our case  esp-01 as Access point will assign IP to nodemcu esp12E.
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());             //Check out the Ip assigned by the esp12E
  Serial.print("connecting to ");
      Serial.println(host);
      // Use WiFiClient class to create TCP connections
      if (!client.connect("192.168.4.1", httpPort)) 
      {
        Serial.println("connection failed");
        return;
      }    
}

void conn_obs()
{
  WiFi.begin("");      //Connect to this SSID. In our case esp-01 SSID.  
  while (WiFi.status() != WL_CONNECTED) {      //Wait for getting IP assigned by Access Point/ DHCP. 
                                              //Our case  esp-01 as Access point will assign IP to nodemcu esp12E.
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());             //Check out the Ip assigned by the esp12E
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
    if (message.equals("one") && message!=prev_data) 
    {
      stat=1;
      // code to get data from server
      
      SPISlave.setData("one initiated");
      prev_data=message;
    } 
    else if (message.equals("two") && message!=prev_data) 
    {
      stat=2;
      SPISlave.setData("two initiated");
      prev_data=message;
    } 
    else if(message.equals("three") && message!=prev_data) 
    {
      stat=3;
      SPISlave.setData("three initiated");
      prev_data=message;
    }
    else if(!message.equals("one") || !message.equals("two") || !message.equals("three"))
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
    Serial.printf("Status: %u\n", data);
    SPISlave.setStatus(millis()); //set next status
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
  SPISlave.setData("NULL");
///////////////////////
}

void loop() 
{
    if(stat==1 && flag!=1)
    {
      flag=1;
      conn_cli();
      //Request to server to activate the led
      int writew=client.print(String("GET ") +"/Led"+" HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");         
      Serial.print("Written");
      Serial.println(writew);
      
      delay(10);
      //Read all the lines of the reply from server and print them to Serial Monitor etc
      while(client.available())
      {
        Serial.print("MA ki chut ");
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
      //Close the Connection. Automatically
      //Serial.println();
      //Serial.println("closing connection");             
    }//End if
    else if(stat==2 && flag!=2)
    {
      flag=2;
      client.stop();
      WiFi.disconnect();  // Erases SSID/password
    }
    else if(stat==3 && flag!=3)
    {
      flag=3;
      conn_cli();
      Serial.print("connecting to ");
      Serial.println(host);
      // Use WiFiClient class to create TCP connections
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect("192.168.4.1", httpPort)) 
      {
        Serial.println("connection failed");
        return;
      }    
      //Request to server to activate the led
      client.print(String("GET ") +"/Led"+" HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      delay(10);
      // Read all the lines of the reply from server and print them to Serial Monitor etc
      while(client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
      //Close the Connection. Automatically
      //Serial.println();
      //Serial.println("closing connection");
    }
    
}

