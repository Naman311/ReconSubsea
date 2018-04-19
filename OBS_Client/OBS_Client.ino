#include <ESP8266WiFi.h>

const char* ssid = "OBS";
const char* host = "192.168.4.1";

//------------------------------variables for int data files------------------------
String html = "";
float arr[40]; int narr = -1;

void intArray()
{ int check = false;
  Serial.println(html.length());
  for (int i = 0; i < html.length(); i++)
  { //Serial.println(html.substring(i,i+6));
    if (html.substring(i, i + 8) == "Voltage=")
    { //Serial.println("Voltage");
      i += 8;
      int j;
      String ss = "";
      for (j = i; html[j] != 'X'; j++)
        if (html[i] != ' ')
          ss += html[j];
        else break;
      i = j - 1;
      Serial.println("Voltage:" + ss);
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
      Serial.println("Xangle:" + ss);
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
      Serial.println("Yangle:" + ss);
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
      Serial.println("Count:" + ss);
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
  Serial.println("19 points");
  for (int i = 0; i <= narr; i++) {
    Serial.print("[" + String(i) + "]");
    Serial.println(arr[i], 3);
  }
  delay(500);
}
//----------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to " + String(ssid));
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to host");
}

void loop() {
  WiFiClient client;
  for (int i = 0; i <= narr; i++)arr[i] = 0;
  narr = -1;
  Serial.println("Connecting to " + String(host));
  if (client.connect(host, 80))
  {
    Serial.println("Sending Request");
    client.print(String("Get /") + " HTTP/1.1\r\n" +
                 "Host: " + String(host) + "\r\n" +
                 "Connection: close\r\n" + "\r\n");
    html = "";
    while (client.connected())
    {
      if (client.available())
      {
        html += client.readStringUntil('\n');
      }
    }
    Serial.println(html);
    intArray();
    client.stop();
  }
  else
  {
    //client.stop();
    Serial.println("Disconnected");
  }
}
