#include <Servo.h>

byte servoPin = 9;
Servo servo;

byte servoPin1 = 10;
Servo servo1;

byte servoPin2 = 11;
Servo servo2;

void setup() {
  servo.attach(servoPin);
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  
  servo.writeMicroseconds(1500); // send "stop" signal to ESC.
  servo1.writeMicroseconds(1500); // send "stop" signal to ESC.
  servo2.writeMicroseconds(1500);
  delay(1000); // delay to allow the ESC to recognize the stopped signal
}

void loop() 
{
  int signal = 1700; // Set signal value, which should be between 1100 and 1900

  servo.writeMicroseconds(signal); // Send signal to ESC.
  servo1.writeMicroseconds(signal);
  servo2.writeMicroseconds(signal);
}
