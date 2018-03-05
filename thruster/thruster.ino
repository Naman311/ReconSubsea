

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = 0;  // analog pin used to connect the potentiometer
int val=0;    // variable to read the value from the analog pin

void setup() {
Serial.begin(9600);
 myservo.attach(9);  // attaches the servo on pin 9 to the servo object

Serial.println("100%");
myservo.write(180);
delay(1000);
Serial.println("0%");
myservo.write(50);
delay(1000);
Serial.println("Intialized!");

delay(1000);

}

void loop() {
   delay(500);
   if(Serial.available())
   {
  val=Serial.parseInt();
  myservo.write(val);                  // sets the servo position according to the scaled value
                             // waits for the servo to get there
Serial.println(val);
   }
   
}

