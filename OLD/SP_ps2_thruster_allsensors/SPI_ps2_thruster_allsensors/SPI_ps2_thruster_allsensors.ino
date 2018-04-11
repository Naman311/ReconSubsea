// slave
#include "I2Cdev.h"

#include <HDC1050.h>

#include <MPU6050_6Axis_MotionApps20.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;

HDC1050 hdcSensor;
#define OUTPUT_READABLE_YAWPITCHROLL

#define LED_PIN 13 
bool blinkState = false;

bool dmpReady = false;  
uint8_t mpuIntStatus;   
uint8_t devStatus;      
uint16_t packetSize;    
uint16_t fifoCount;     
uint8_t fifoBuffer[64]; 

Quaternion q;           
VectorInt16 aa;         
VectorInt16 aaReal;     
VectorInt16 aaWorld;    
VectorFloat gravity;    
float euler[3];         
float ypr[3];           

uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

volatile bool mpuInterrupt = false; 
void dmpDataReady() {
    mpuInterrupt = true;
}

float tc, tf, h;

#include <Servo.h>
Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;
#include <SPI.h>
#include "SPI_anything.h"
unsigned int a;

void setup ()
  {
// have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
   // end of setup

esc1.attach(7); //Specify the esc signal pin,Here as D8
//esc1.writeMicroseconds(1000); //initialize the signal to 1000

esc2.attach(6); //Specify the esc signal pin,Here as D8
//esc2.writeMicroseconds(1000); //initialize the signal to 1000

esc3.attach(5); //Specify the esc signal pin,Here as D8
//esc3.writeMicroseconds(1000); //initialize the signal to 1000

esc4.attach(4); //Specify the esc signal pin,Here as D8
//esc4.writeMicroseconds(1000); //initialize the signal to 1000

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

Serial.begin (9600);   // debugging
while (!Serial);

hdcSensor.turnOnHeater(true);
hdcSensor.setTemperatureRes(HDC1050::T_RES_14);
hdcSensor.setHumidityRes(HDC1050::H_RES_8);
hdcSensor.updateConfigRegister();

mpu.initialize();
mpu.testConnection();
devStatus = mpu.dmpInitialize();
mpu.setXGyroOffset(220);
mpu.setYGyroOffset(0);
mpu.setZGyroOffset(-85);
mpu.setZAccelOffset(1788);
if (devStatus == 0) {
       
  mpu.setDMPEnabled(true);
  
  attachInterrupt(0, dmpDataReady, RISING);
  mpuIntStatus = mpu.getIntStatus();
  
  dmpReady = true;
  
  packetSize = mpu.dmpGetFIFOPacketSize();
  } 
  else 
  {
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }

pinMode(LED_PIN, OUTPUT);
}
void loop () 
  {   
  SPI_readAnything (a);
  tf = hdcSensor.getTemperatureHumidity(tc, h);
  unsigned int x=0;  
  float val=0.0,sam=0.0,avgval=0.0,valf=0.0;
  for(int i=0;i<150;i++)
  {
  val=analogRead(A0);
  sam+=val;
  }
  avgval=sam/150.00;
  valf=(2.5-(avgval*(5.0/1024.0)))/0.185;   //current value 
  
  unsigned int v=0;  
  float val1=0.0,sam1=0.0,avgval1=0.0,valf1=0.0;
  for(int i=0;i<150;i++)
  {
  val1=analogRead(A1);
  sam1+=val1;
  }
  avgval1=sam1/150.00;
  valf1=(2.5-(avgval1*(5.0/1024.0)))/0.185;   //current value
  unsigned int z=0;  
  float val2=0.0,sam2=0.0,avgval2=0.0,valf2=0.0;
  for(int i=0;i<150;i++)
  {
    val2=analogRead(A2);
    sam2+=val2;
    //delay(3);
  }
  avgval2=sam2/150.00;
  valf2=(2.5-(avgval2*(5.0/1024.0)))/0.185;   //current value
  
  unsigned int y=0;  
  float val3=0.0,sam3=0.0,avgval3=0.0,valf3=0.0;
  for(int i=0;i<150;i++)
  {
    val3=analogRead(A3);
    sam3+=val3;
  }
  avgval3=sam3/150.00;
  valf3=(2.5-(avgval3*(5.0/1024.0)))/0.185;   //current value
  float sensorValue = (analogRead(A4))/100.0; //Do maths for calibration
  // if programming failed, don't try to do anything
  if (!dmpReady) return;
  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
  // other program behavior stuff here
  // .
  // .
  // .
  // if you are really paranoid you can frequently test in between other
  // stuff to see if mpuInterrupt is true, and if so, "break;" from the
  // while() loop to immediately process the MPU data
  // .
  // .
  // .
  }
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  
  // get current FIFO count
  fifoCount = mpu.getFIFOCount();
  
  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
  // reset so we can continue cleanly
  mpu.resetFIFO();
  // Serial.println(F("FIFO overflow!"));
  
  // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & 0x02) {
  // wait for correct available data length, should be a VERY short wait
  while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
  
  // read a packet from FIFO
  mpu.getFIFOBytes(fifoBuffer, packetSize);
  
  // track FIFO count here in case there is > 1 packet available
  // (this lets us immediately read more without waiting for an interrupt)
  fifoCount -= packetSize;
  
  #ifdef OUTPUT_READABLE_QUATERNION
    // display quaternion values in easy matrix form: w x y z
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    Serial.print("quat\t");
    Serial.print(q.w);
    Serial.print("\t");
    Serial.print(q.x);
    Serial.print("\t");
    Serial.print(q.y);
    Serial.print("\t");
    Serial.println(q.z);
#endif
#ifdef OUTPUT_READABLE_EULER
  // display Euler angles in degrees
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetEuler(euler, &q);
  Serial.print("euler\t");
  Serial.print(euler[0] * 180/M_PI);
  Serial.print("\t");
  Serial.print(euler[1] * 180/M_PI);
  Serial.print("\t");
  Serial.println(euler[2] * 180/M_PI);
#endif

#ifdef OUTPUT_READABLE_YAWPITCHROLL
  // display Euler angles in degrees
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  //Serial.print("ypr\t");
  Serial.print(ypr[0] * 180/M_PI);
  //Serial.print("\t");
  Serial.print(",");
  Serial.print(ypr[1] * 180/M_PI);
  //Serial.print("\t");
  Serial.print(",");
  Serial.print(ypr[2] * 180/M_PI);
  Serial.print(",");
  Serial.print(tc);
  Serial.print(",");
  Serial.print(tf);
  Serial.print(",");
  Serial.print(h);
  Serial.print(",");
  Serial.print(sensorValue,2);
  Serial.print(",");
  Serial.print(avgval3*(5.0/1024.0));      //voltage value
  Serial.print(",");
  Serial.print(valf3);
  Serial.print(",");
  Serial.print(avgval2*(5.0/1024.0));      //voltage value
  Serial.print(",");
  Serial.print(valf2);
  Serial.print(",");
  Serial.print(avgval1*(5.0/1024.0));      //voltage value
  Serial.print(",");
  Serial.print(valf1);
  Serial.print(",");
  Serial.print(avgval*(5.0/1024.0));      //voltage value
  Serial.print(",");
  Serial.print(valf);
  Serial.println("  ");

  
#endif

#ifdef OUTPUT_READABLE_REALACCEL
  // display real acceleration, adjusted to remove gravity
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetAccel(&aa, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
  Serial.print("areal\t");
  Serial.print(aaReal.x);
  Serial.print("\t");
  Serial.print(aaReal.y);
  Serial.print("\t");
  Serial.println(aaReal.z);
#endif

#ifdef OUTPUT_READABLE_WORLDACCEL
  // display initial world-frame acceleration, adjusted to remove gravity
  // and rotated based on known orientation from quaternion
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetAccel(&aa, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
  mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
  Serial.print("aworld\t");
  Serial.print(aaWorld.x);
  Serial.print("\t");
  Serial.print(aaWorld.y);
  Serial.print("\t");
  Serial.println(aaWorld.z);
#endif

#ifdef OUTPUT_TEAPOT
  // display quaternion values in InvenSense Teapot demo format:
  teapotPacket[2] = fifoBuffer[0];
  teapotPacket[3] = fifoBuffer[1];
  teapotPacket[4] = fifoBuffer[4];
  teapotPacket[5] = fifoBuffer[5];
  teapotPacket[6] = fifoBuffer[8];
  teapotPacket[7] = fifoBuffer[9];
  teapotPacket[8] = fifoBuffer[12];
  teapotPacket[9] = fifoBuffer[13];
  Serial.write(teapotPacket, 14);
  teapotPacket[11]++; // packetCount, loops at 0xFF on purpose
#endif

// blink LED to indicate activity
blinkState = !blinkState;
digitalWrite(LED_PIN, blinkState);
  }
  //Serial.println (a);
  switch (a){
    case 2048:                  // first 8 seperate
      esc1.writeMicroseconds(1150);
      break;
    case 18432:
      esc1.writeMicroseconds(1200);
      break;
    case 1024:
      esc2.writeMicroseconds(1350);
      break;
    case 17408:            
      esc2.writeMicroseconds(1400);
      break;
    case 512:
      esc3.writeMicroseconds(1240);
      break;
    case 16896:
      esc3.writeMicroseconds(1300);
      break;
    case 256:            
      esc4.writeMicroseconds(1260);
      break;
    case 16640:
      esc4.writeMicroseconds(1300);
      break;
    case 4096:            //last 4 combination
      esc1.writeMicroseconds(1140);
      esc2.writeMicroseconds(1320);
      break;
    case 20480:
      esc1.writeMicroseconds(1200);
      esc2.writeMicroseconds(1400);
      break;
    case 8192:
      esc3.writeMicroseconds(1240);
      esc4.writeMicroseconds(1260);
      break;
    case 24576:
      esc3.writeMicroseconds(1300);
      esc4.writeMicroseconds(1300);
      break;    
    case 0:
      esc1.writeMicroseconds(0);
      esc2.writeMicroseconds(0);
      esc3.writeMicroseconds(0);
      esc4.writeMicroseconds(0);
      break;
  }
  delay(25);
  }  // end of loop
