
/* controller keys
1.Left analog x axis
2.Left analog y axis
3.LT +ve RT -ve
4.Right y axis
5.Right x axis
6.A
7.B
8.X
9.Y
10.LB
11.RB
12.BACK
13.START
14.LEFT CLICK
15.RIGHT CLICK
*/

// Ethernet header
#include <Ethernet.h> //Load Ethernet Library
#include <EthernetUdp.h> //Load UDP Library
#include <SPI.h> //Load the SPI Library
#define UDP_TX_PACKET_MAX_SIZE 70 //increase UDP size

// thruster header
#include <Servo.h>

//  thruster init
byte servoPin = 9;
Servo servo;

////////////acs712(current sensor)/////////
double Voltage = 0;
double Current = 0;

///////////////////////////////////////////

/////////////////HDC1050///////////////////
#include <Wire.h>
#include <HDC1050.h>


// HDC1050 init
HDC1050 hdcSensor;
float tc, tf, h;
////////////////////////////////////////////

////////////////MPU6050////////////////////////////////////////////////////////////////////////////////////////////////
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;
//MPU6050 mpu(0x69); // <-- use for AD0 high

/* =========================================================================
   NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2.
 * ========================================================================= */

/* =========================================================================
   NOTE: Arduino v1.0.1 with the Leonardo board generates a compile error
   when using Serial.write(buf, len). The Teapot output uses this method.
   The solution requires a modification to the Arduino USBAPI.h file, which
   is fortunately simple, but annoying. This will be fixed in the next IDE
   release. For more info, see these links:

   http://arduino.cc/forum/index.php/topic,109987.0.html
   http://code.google.com/p/arduino/issues/detail?id=958
 * ========================================================================= */



// uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)
//#define OUTPUT_READABLE_QUATERNION

// uncomment "OUTPUT_READABLE_EULER" if you want to see Euler angles
// (in degrees) calculated from the quaternions coming from the FIFO.
// Note that Euler angles suffer from gimbal lock (for more info, see
// http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_EULER

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
#define OUTPUT_READABLE_YAWPITCHROLL

// uncomment "OUTPUT_READABLE_REALACCEL" if you want to see acceleration
// components with gravity removed. This acceleration reference frame is
// not compensated for orientation, so +X is always +X according to the
// sensor, just without the effects of gravity. If you want acceleration
// compensated for orientation, us OUTPUT_READABLE_WORLDACCEL instead.
//#define OUTPUT_READABLE_REALACCEL

// uncomment "OUTPUT_READABLE_WORLDACCEL" if you want to see acceleration
// components with gravity removed and adjusted for the world frame of
// reference (yaw is relative to initial orientation, since no magnetometer
// is present in this case). Could be quite handy in some cases.
//#define OUTPUT_READABLE_WORLDACCEL

// uncomment "OUTPUT_TEAPOT" if you want output that matches the
// format used for the InvenSense teapot demo
//#define OUTPUT_TEAPOT



#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize1;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

///////////////////////////////////////////////////////////////////////
int flag=0;
float sum1,sum2=0,sum3=0;
///////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////pressure sensor//////////////////////////////////////////////////////////////
#include <Wire.h>
#include <SPI.h>
const int spi_ss = 10;   // for slave select
uint8_t byte_0, byte_1, byte_2; // for taking the input in bits
uint32_t spi_bytes; // to add all the bits by bitshift method
float vo; //convert the bits in voltage
float vr = 5;
float pp=0; //for storing the value of presure
float vt=0;
float t=0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Etherner init
int t1=0;int t2=-1;int t3=4;int t4=2;
int v1=0; long y=0;long p=0; long r=0;

long temp;
long humidity;
long internal_pressure;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE}; //Assign a mac address
IPAddress ip(169, 254, 234, 100); //Assign my IP adress
unsigned int localPort = 5000; //Assign a Port to talk over
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
String datReq; //String for our data
int packetSize; //Size of Packet
EthernetUDP Udp; //Define UDP Object

// Function to convert sensors output to string
String converstion(int t1,int t2,int t3,int t4,int v1,long y,long p,long r,long temp,long humidity,long internal_pressure){
  String result;
  result=t1;
  result+=',';
  result+=t2;
  result+=',';
  result+=t3;
  result+=',';
  result+=t4;
  result+=',';
  result+=v1;
  result+=',';
  result+=y;
  result+=',';
  result+=p;
  result+=',';
  result+=r;
  result+=',';
  result+=temp;
  result+=',';
  result+=humidity;
  result+=',';
  result+=internal_pressure;
  return (result);
  }
// function to covert string to int array
float nos[17];
int nNos=-1;  

void addNos(String data)
{
  nNos=-1;
  String ss="";
  for(int i=0;i<data.length();i++)
  {
    if(data[i]==',')
    { nos[++nNos]=ss.toFloat();
      ss="";
    }
    else if(i==data.length()-1)
    { ss+=data[i];
      nos[++nNos]=ss.toFloat();
      ss="";
      break;
    }
    else 
    {
      ss+=data[i];
    }
  }  
  //for(int i=0;i<=nNos;i++){Serial.print(nos[i]);Serial.print(",");}
  //Serial.println();
}

// function to control thrusters
void thruster_movement()
{
  int signal;
  if(nos[1]<=0)
  {
     signal = map(nos[1],-10,0,1100,1500);
  }
  else if(nos[1]>0)
  {
    signal = map(nos[1],1,10,1501,1900);
  }
  // thruster code
  //int signal = 1100; // Set signal value, which should be between 1100 and 1900
  //Serial.println(nos[1]);
  //Serial.println(signal);
  servo.writeMicroseconds(signal); // Send signal to ESC.
}
/*void move_down()
{
  int signal=map(nos[1],1,10,1501,1900);
  l.writeMicroseconds(signal);
  r.writeMicroseconds(signal);
}

void move_up()
{
  int signal=map(nos[1],-10,0,1100,1500);
  l.writeMicroseconds(signal);
  r.writeMicroseconds(signal);
}
void move_forward()
{
  int signal=map(nos[3],-10,0,1100,1500);
  fl.writeMicroseconds(signal);
  fr.writeMicroseconds(signal);
}
void move_backward()
{
  int signal=map(nos[3],1,10,1501,1900);
  fl.writeMicroseconds(signal);
  fr.writeMicroseconds(signal);
}
void move_left()
{
  int signal=map(nos[2],-10,0,1100,1500);
  fl.writeMicroseconds(signal);
  bl.writeMicroseconds(signal);
}
void move_right()
{
  int signal=map(nos[2],1,10,1501,1900);
  fr.writeMicroseconds(signal);
  br.writeMicroseconds(signal);
}
void pitch_anti()
{//yet to complete
  int signal=map(nos[0],-10,0,1100,1500);
}
void pitch_cloc()
{//yet to complete
  int signal=map(nos[0],1,10,1100,1500);
}
void movement()
{//l r fl fr bl br
  int l_signal,r_signal,fl_signal,fr_signal,bl_signal,br_signal;
  if(nos[1]<=0)
  {
    move_down();
  }
  else if(nos[1]>0)
  {
    move_up();
  }
  if(nos[0]<=0)
  {
    pitch_anti();
  }
  else if(nos[0]>0)
  {
    pitch_cloc();
  }
  if(nos[3]<=0)
  {
    move_backward();
  }
  else if(nos[3]>0)
  {
    move_forward();
  }
  if(nos[4]<=0)
  {
    move_left();
  }
  else if(nos[4]>0)
  {
    move_right();
  }
}*/
void setup() {
  // Ethernet setup
  Serial.begin(9600); //Turn on Serial Port

///////////////////////////HDC1050///////////////////////////////////
Wire.begin();
  
  //Serial.println(hdcSensor.getManufacturerID(), HEX);
  //Serial.println(hdcSensor.getDeviceID(), HEX);
  //Serial.println(hdcSensor.getSerialID());
  //Serial.println(hdcSensor.batteryOK());

  hdcSensor.turnOnHeater(true);
  hdcSensor.setTemperatureRes(HDC1050::T_RES_14);
  hdcSensor.setHumidityRes(HDC1050::H_RES_8);
  hdcSensor.updateConfigRegister();
/////////////////////////////////////////////////////////////////////

///////////////////////////////pressure sensor///////////////////////////////////////
 Serial.begin(9600);          
  pinMode(spi_ss, OUTPUT);     
  digitalWrite(spi_ss, HIGH);  
  SPI.begin();    
/////////////////////////////////////////////////////////////////////////////////////

///////////////////////////MPU6050/////////////////////////////////////////////////////////////
 // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    Serial.begin(9600);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
    // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.

    // initialize device
  //  Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
  //  Serial.println(F("Testing device connections..."));
  //  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
 mpu.testConnection();
    // wait for ready
    //Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    //while (Serial.available() && Serial.read()); // empty buffer
    //while (!Serial.available());                 // wait for data
    //while (Serial.available() && Serial.read()); // empty buffer again

    // load and configure the DMP
    //Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    //mpu.setXGyroOffset(220);
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    //mpu.setZGyroOffset(-85);
    mpu.setZGyroOffset(0);
    mpu.setZAccelOffset(0); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
       // Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
       // Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        //Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize1 = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
//delay(30000);

///////////////////////////////////////////////////////////////////////////////////////////
  
  Ethernet.begin(mac, ip); //Initialize Ethernet
  Udp.begin(localPort); //Initialize Udp
  //delay(1500); //delay
  
  // thruster setup
  servo.attach(servoPin);

  servo.writeMicroseconds(1500); // send "stop" signal to ESC.
  //delay(1000); // delay to allow the ESC to recognize the stopped signal
}

void loop() {
  //Ethernet code

/////////////////////ACS712(CURRENT SENSOR)//////////////////////////
// Voltage is Sensed 1000 Times for precision
for(int i = 0; i < 1000; i++) {
Voltage = (Voltage + (.0049 * analogRead(A0))); // (5 V / 1024 (Analog) = 0.0049) which converter Measured analog input voltage to 5 V Range
//delay(1);
}
Voltage = Voltage /1000;

Current = (Voltage -2.5)/ 0.185; // Sensed voltage is converter to current

//Serial.print(“\n Voltage Sensed (V) = “); // shows the measured voltage
Serial.print(Voltage,2); // the ‘2’ after voltage allows you to display 2 digits after decimal point
//Serial.print(“\t Current (A) = “); // shows the voltage measured
Serial.print(Current,2); // the ‘2’ after voltage allows you to display 2 digits after decimal point

//delay(1000);

/////////////////////////////////////////////////////////////////////

////////////////////////////pressure sensor//////////////////////////////////
  SPI.beginTransaction(SPISettings(1000, MSBFIRST, SPI_MODE2)); 
//set speed bit format and clock/data polarity while starting SPI transaction
  digitalWrite(spi_ss, LOW);
  byte_0 = SPI.transfer(0); // to take the first 8 bits
  byte_1 = SPI.transfer(0); //to take the second 8 bits
  byte_2 = SPI.transfer(0);   //to take the third 8 bits
  
    

  digitalWrite(spi_ss, HIGH);
     
  SPI.endTransaction();
  spi_bytes=(byte_0*1000000+byte_1*1000+byte_2);
  Serial.println(spi_bytes );
  vo = vr* (float(spi_bytes) / 16777216.0); // converting to output voltage
  Serial.println(vo);
  float kpa = (vo-0.2)/4.5*700.0;
  Serial.println(kpa+31.11); 
  delay(1000);
/////////////////////////////////////////////////////////////////////////////
 


//////////////////////////MPU6050///////////////////////////////////////////////////////////////////
if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize1) {
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

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        //Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize1) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize1);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize1;


//////////////////////////////////////////////////////   
if(flag==0)     
{mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            //Serial.print("ypr\t");
            sum1=(ypr[0] * 180/M_PI);
            //Serial.print("\t");
            //Serial.print(",");
            sum2=(ypr[1] * 180/M_PI);
            //Serial.print("\t");
            //Serial.print(",");
            sum3=(ypr[2] * 180/M_PI);
flag=1;}
///////////////////////////////////////////////////////            
        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            //Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI-sum1);
            //Serial.print("\t");
            Serial.print(",");
            Serial.print(ypr[1] * 180/M_PI-sum2);
            //Serial.print("\t");
            Serial.print(",");
            Serial.println(ypr[2] * 180/M_PI-sum3);
            y=ypr[0] * 180/M_PI-sum1;
            p=ypr[1] * 180/M_PI-sum2;
            r=ypr[2] * 180/M_PI-sum3;
        #endif
////////////////////////////////////////////////////////////////////////////////////////////////
  
tf = hdcSensor.getTemperatureHumidity(tc, h);
  
  temp = tc;
  humidity = h;
  //Serial.print(tc);
  //Serial.print("  ");
  //Serial.print(tf);
  //Serial.print("  ");
  //Serial.println(h);


  internal_pressure = kpa+31.11;
  //y = random(10,50);
  //p = random(10,50);
  //r = random(10,50);
  
  v1 = random(30,48);
    
  t1++;t2++;t3++;t4++;
  String output= converstion(t1,t2,t3,t4,v1,y,p,r,temp,humidity,internal_pressure);
  packetSize = Udp.parsePacket(); //Read theh packetSize
  
  if(packetSize>0)
  { //Check to see if a request is present
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Reading the data request on the Udp
    datReq=packetBuffer; //Convert packetBuffer array to string datReq
    addNos(datReq);
    Serial.println(datReq);

    thruster_movement();
  }
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
  Udp.print(output); //Send string back to client 
  Udp.endPacket(); //Packet has been sent
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
  //delay(100);
}
}void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
