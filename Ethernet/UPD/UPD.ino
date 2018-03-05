#include <SPI.h>
#include <Ethernet.h>
int x=0;
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 
// Enter the IP address for Arduino, as mentioned we will use 192.168.0.16
// Be careful to use , insetead of . when you enter the address here
IPAddress ip(169,254,234,100);

// Initialize the Ethernet server library
EthernetServer server(80);

void setup() {
 
 // Serial.begin starts the serial connection between computer and Arduino
  Serial.begin(9600);
 
 // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  
  Serial.print("Arduino server IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
   EthernetClient client = server.available();  // Listen for incoming clients
 
  if (client) {
    x++;
   // When a client sends a request to a webserver, that request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
       // This line is used to send communication information between Arduino and your browser over Serial Monitor
        Serial.write(c);
        
       // When the request has ended send the client a reply
        if (c == '\n' && currentLineIsBlank) {
          
         // We send the HTTP response code to the client so it knows that we will send him HTML data
         // and to refresh the webpage every 5 seconds
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 1");
          client.println();
         // Here we write HTML data (for the page itself) which will be sent to the client.
         // The HTML includes Javascript which fills the data
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Arduino sensor data</title>");
          client.println("<script>");
          client.println("window.onload=function rfsh(){");
          client.println("document.getElementById('value').innerHTML =");
          client.print(x);
          client.println(";}");
          client.println("</script>");
          client.println("</head>");
          client.println("<body>");
          client.println("<br>");
          client.println("<h1>Light measured from the sensor is:</h1> ");
          client.println("<p id='value'></p>");
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // Check if a new line is started
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // If a new line was not strated
          currentLineIsBlank = false;
        }
      }
    }
    // Give the client some time to recieve the data (1ms)
    delay(100);
    // In the end close the connection
    client.stop();
  }
}
