#include <ESP8266WiFi.h>
#include <Wire.h>
#include <math.h>


/*
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ;
int num_samples = 10;    // take the average of multiple samples
float average1,average2,average3;
 
void adaFruit()
{
   Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  float sum1 = 0.0f,sum2 = 0.0f,sum3 = 0.0f;
  for (int i=0; i < num_samples; i++)
  {
    float z = AcZ;  // +z is up from the top of the pcb
    float y = AcY;  // +y is lengthwise to the right
    float x = AcX;
    
    float angle1 = atan2(x, y) * 180.0f / M_PI;  // angle will be between -360 and 360 degrees
    float angle2 = atan2(y, z) * 180.0f / M_PI;  // angle will be between -360 and 360 degrees
    float angle3 = atan2(z, x) * 180.0f / M_PI;  // angle will be between -360 and 360 degrees
    sum1 += angle1;
    sum2 += angle2;
    sum3 += angle3;
    delay(15);
  }
  average1 = round(sum1 / num_samples);
  average2 = round(sum2 / num_samples);
  average3 = round(sum3 / num_samples);
  
}*/

float average2=20.1,average3=30.9;
const char* ssid="OBS";
WiFiServer server(80);
void setup() {

//----------------------------------MPU Initialisations---------------------------
  /* Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
*/  
  Serial.begin(115200);
  WiFi.softAP(ssid);
  IPAddress myIP=WiFi.softAPIP();
  server.begin();
  Serial.println("Web Server Started,IP"+myIP.toString());
  
}
String prepareHtmlPage()
{
  String htmlPage=String("HTTP/1.1 200 OK\r\n")+"Content-Type: text/html\r\n"
                    +"Connection: close\r\n"
                    +"<!DOCTYPE HTML>"+
                    "<html>"+
                    "<h1>MATE 2018 SEISMOGRAPH STATUS</h1>"
                    +"<p>Voltage=4.995 Xangle="+String(average2)+" Yangle="+String(average3)
                    +" Count=0</p>"+"<p>DATA:0,4.4,-4.3,0,0,6.8,-7,0,0,1.6,-1.6,0,0,6.3,-6.4,0</p>"
                    +"</body></html>\r\n";
  return htmlPage;
}

void loop() {
      
      //adaFruit();
      WiFiClient client=server.available();
      Serial.println(client);
      Serial.println(WiFi.softAPIP());
      if(client)  {
          while(client.connected())
          {
            if(client.available())
            {
              String line=client.readStringUntil('\r');
              if(line.length()==1&&line[0]=='\n')
              {
                client.println(prepareHtmlPage());
                break;
              }
            }
          }
        
      }
      delay(20);
      client.stop();
}
