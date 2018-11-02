
#include <Servo.h>

//ll=3
//bl=4
//fl=5
//rr=6
//fr=7
byte servoPin = 3;    //LU
Servo servo;
byte servoPin1 = 5;
Servo servo1;
byte servoPin2 = 6;     //LB
Servo servo2;
byte servoPin3 = 4;
Servo servo3;
byte servoPin4 = 7;
Servo servo4;
byte servoPin5 = 8;
Servo servo5;

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
  servo1.attach(servoPin1);
  servo.attach(servoPin);
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);
  servo5.attach(servoPin5);
  /*servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);
  */
  servo.writeMicroseconds(1500);
  servo1.writeMicroseconds(1500);
  servo2.writeMicroseconds(1500);
  servo3.writeMicroseconds(1500);
  servo4.writeMicroseconds(1500);
  servo5.writeMicroseconds(1500);// send "stop" signal to ESC.
  /*servo1.writeMicroseconds(1500); // send "stop" signal to ESC.
  servo2.writeMicroseconds(1500); // send "stop" signal to ESC.
  servo3.writeMicroseconds(1500); // send "stop" signal to ESC.
  servo4.writeMicroseconds(1500); // send "stop" signal to ESC.  */
  delay(1000); // delay to allow the ESC to recognize the stopped signal
}

void loop() 
{
  int signal= 1520;
  servo.writeMicroseconds(signal);
  servo1.writeMicroseconds(signal);
  servo2.writeMicroseconds(signal);
  servo3.writeMicroseconds(signal);
  servo4.writeMicroseconds(signal);
  servo5.writeMicroseconds(signal);// Send signal to ESC.
  /*servo1.writeMicroseconds(signal); // Send signal to ESC.
  servo2.writeMicroseconds(signal); // Send signal to ESC.
  servo3.writeMicroseconds(signal); // Send signal to ESC.
  servo4.writeMicroseconds(signal); // Send signal to ESC.*/
  delay(2000);
  signal=1500;
  servo.writeMicroseconds(signal);
  servo1.writeMicroseconds(signal);
  servo2.writeMicroseconds(signal);
  servo3.writeMicroseconds(signal);
  servo4.writeMicroseconds(signal);
  servo5.writeMicroseconds(signal);// Send signal to ESC.
  /*servo1.writeMicroseconds(signal); // Send signal to ESC.
  servo2.writeMicroseconds(signal); // Send signal to ESC.
  servo3.writeMicroseconds(signal); // Send signal to ESC.
  servo4.writeMicroseconds(signal); // Send signal to ESC.*/
  signal=1520;
  delay(2000);
  servo.writeMicroseconds(signal);
  servo1.writeMicroseconds(signal);
  servo2.writeMicroseconds(signal);
  delay(2000);
  signal=1500;
  servo.writeMicroseconds(signal);
  servo1.writeMicroseconds(signal);
  servo2.writeMicroseconds(signal);
  delay(2000);
}
