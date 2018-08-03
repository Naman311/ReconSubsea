
#include <Servo.h>

//ll=3
//bl=4
//fl=5
//rr=6
//fr=7
byte servoPin = 8;
Servo servo;

/*byte servoPin2 = 12;
Servo servo2;

byte servoPin1 = 11;
Servo servo1;

byte servoPin3 = 3;
Servo servo3;

byte servoPin4 = 5;
Servo servo4;
*/
void setup() {
  //servo1.attach(servoPin1);
  servo.attach(servoPin);
  /*servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);
  */
  servo.writeMicroseconds(1500); // send "stop" signal to ESC.
  /*servo1.writeMicroseconds(1500); // send "stop" signal to ESC.
  servo2.writeMicroseconds(1500); // send "stop" signal to ESC.
  servo3.writeMicroseconds(1500); // send "stop" signal to ESC.
  servo4.writeMicroseconds(1500); // send "stop" signal to ESC.  */
  delay(1000); // delay to allow the ESC to recognize the stopped signal
}

void loop() 
{
  int signal= 1600;
  servo.writeMicroseconds(signal); // Send signal to ESC.
  /*servo1.writeMicroseconds(signal); // Send signal to ESC.
  servo2.writeMicroseconds(signal); // Send signal to ESC.
  servo3.writeMicroseconds(signal); // Send signal to ESC.
  servo4.writeMicroseconds(signal); // Send signal to ESC.*/
  delay(2000);
  signal=1500;
  servo.writeMicroseconds(signal); // Send signal to ESC.
  /*servo1.writeMicroseconds(signal); // Send signal to ESC.
  servo2.writeMicroseconds(signal); // Send signal to ESC.
  servo3.writeMicroseconds(signal); // Send signal to ESC.
  servo4.writeMicroseconds(signal); // Send signal to ESC.*/
  delay(2000);
}
