
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
const float SensorOffset = 39;
int prev=0;
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

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;


#define OUTPUT_READABLE_YAWPITCHROLL




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
String converstion(int t1,int t2,int t3,int t4,int v1,float y,float p,float r,float temp,float humidity,float internal_pressure,int t6){
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
  result+=',';
  result+=t6;
  return (result);
  }
// function to covert string to int array
float nos[18];
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

  servo.writeMicroseconds(signal); // Send signal to ESC.
}
void move_down()
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
  int signal=map(nos[4],-10,0,1100,1500);
  fl.writeMicroseconds(signal);
  bl.writeMicroseconds(signal);
}
void move_right()
{
  int signal=map(nos[4],1,10,1501,1900);
  fr.writeMicroseconds(signal);
  br.writeMicroseconds(signal);
}
void roll_left()
{
  int signal=map(nos[0],-10,0,1100,1500);
  l.writeMicroseconds(signal);
  int signal=map(nos[0],-10,0,1900,1501);
  r.writeMicroseconds(signal);
}
void roll_right()
{
  int signal=map(nos[0],1,10,1100,1500);
  r.writeMicroseconds(signal);
  int signal=map(nos[0],1,10,1900,1501);
  l.writeMicroseconds(signal);
}
void pitch_anti()
{
  br.writeMicroseconds();
}
void pitch_cloc()
{
  
}
void mani_open()
{
  analogWrite(6,0);
  analogWrite(9,0);
  delay(5);
  analogWrite(7,1024);
  analogWrite(8,1024);
}
void mani_close()
{
  analogWrite(7,0);
  analogWrite(8,0);
  delay(5);
  analogWrite(6,1024);
  analogWrite(9,1024);
}
void obs_clock()
{
  digital
}
void obs_anti()
{
  
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
    roll_left();
  }
  else if(nos[0]>0)
  {
    roll_right();
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
<<<<<<< HEAD
}*/

/*
    GPIO    NodeMCU   Name  |   Uno
   ===================================
     15       D8       SS   |   D10
     13       D7      MOSI  |   D11
     12       D6      MISO  |   D12
     14       D5      SCK   |   D13
*/
#include <SPI.h>

class ESPMaster {
  private:
    uint8_t _ss_pin;

  public:
    ESPMaster(uint8_t pin): _ss_pin(pin) {}
    void begin() 
    {
      pinMode(_ss_pin, OUTPUT);
      digitalWrite(_ss_pin, HIGH);
    }
    void readData(uint8_t * data) 
    {
      digitalWrite(_ss_pin, LOW);
      SPI.transfer(0x03);
      SPI.transfer(0x00);
      for (uint8_t i = 0; i < 32; i++) 
      {
        data[i] = SPI.transfer(0);
      }
      digitalWrite(_ss_pin, HIGH);
    }

    void writeData(uint8_t * data, size_t len) 
    {
      uint8_t i = 0;
      digitalWrite(_ss_pin, LOW);
      SPI.transfer(0x02);
      SPI.transfer(0x00);
      while (len-- && i < 32)
      {
        SPI.transfer(data[i++]);
      }
      while (i++ < 32) 
      {
        SPI.transfer(0);
      }
      digitalWrite(_ss_pin, HIGH);
    }

    String readData() 
    {
      char data[33];
      data[32] = 0;
      readData((uint8_t *)data);
      return String(data);
    }

    void writeData(const char * data)
    {
      writeData((uint8_t *)data, strlen(data));
    }
};

ESPMaster esp(SS);

void send(const char * message) 
{
  esp.writeData(message);
  delay(10);
  Serial.print("Slave: ");
  Serial.println(esp.readData());
  Serial.println();
}

int incomingByte = 0;   // for incoming serial data

void setup() {
  // Ethernet setup
  Serial.begin(115200); //Turn on Serial Port
  SPI.begin();
  esp.begin();
=======
}
void setup() {
  // Ethernet setup
  pinMode(,OUTPUT);
  pinMode(,OUTPUT);
  analogWrite(,0);
  analogWrite(,0);
  Serial.begin(115200); //Turn on Serial Port
>>>>>>> 7327904a6a6abccf9e7a39f86c0bebf6ce6bdfbb

///////////////////////////HDC1050///////////////////////////////////
Wire.begin();

  hdcSensor.turnOnHeater(true);
  hdcSensor.setTemperatureRes(HDC1050::T_RES_14);
  hdcSensor.setHumidityRes(HDC1050::H_RES_8);
  hdcSensor.updateConfigRegister();
/////////////////////////////////////////////////////////////////////

///////////////////////////MPU6050/////////////////////////////////////////////////////////////
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif


    while (!Serial); // wait for Leonardo enumeration, others continue immediately
    mpu.initialize();
 mpu.testConnection();

    devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);
    mpu.setZAccelOffset(0); // 1688 factory default for my test chip

    if (devStatus == 0) {

        mpu.setDMPEnabled(true);

        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        dmpReady = true;

        packetSize1 = mpu.dmpGetFIFOPacketSize();
    } 
    else
    {
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    pinMode(LED_PIN, OUTPUT);
delay(30000);

///////////////////////////////////////////////////////////////////////////////////////////
  
  Ethernet.begin(mac, ip); //Initialize Ethernet
  Udp.begin(localPort); //Initialize Udp
  servo.attach(servoPin);

  servo.writeMicroseconds(1500); // send "stop" signal to ESC.
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

/////////////////////////////////////////////////////////////////////

////////////////////////////pressure sensor//////////////////////////////////
  float sensorValue = (analogRead(A0)-SensorOffset); //Do maths for calibration

  
/////////////////////////////////////////////////////////////////////////////
 


//////////////////////////MPU6050///////////////////////////////////////////////////////////////////
if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize1) {
<<<<<<< HEAD

=======
>>>>>>> 7327904a6a6abccf9e7a39f86c0bebf6ce6bdfbb
    }


    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    fifoCount = mpu.getFIFOCount();

    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();



    } else if (mpuIntStatus & 0x02) {

        while (fifoCount < packetSize1) fifoCount = mpu.getFIFOCount();


        mpu.getFIFOBytes(fifoBuffer, packetSize1);
        

        fifoCount -= packetSize1;


//////////////////////////////////////////////////////   
if(flag==0)     
{
  mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

            sum1=(ypr[0] * 180/M_PI);

            sum2=(ypr[1] * 180/M_PI);
            sum3=(ypr[2] * 180/M_PI);
flag=1;
}
///////////////////////////////////////////////////////            
        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            y=ypr[0] * 180/M_PI-sum1;
            p=ypr[1] * 180/M_PI-sum2;
            r=ypr[2] * 180/M_PI-sum3;
        #endif
////////////////////////////////////////////////////////////////////////////////////////////////
  
tf = hdcSensor.getTemperatureHumidity(tc, h);
  
  temp = tc;
  humidity = h;
<<<<<<< HEAD


  internal_pressure = sensorValue,2;

=======

  internal_pressure = sensorValue,2;
>>>>>>> 7327904a6a6abccf9e7a39f86c0bebf6ce6bdfbb
  int t5 = random(10,50);
  int t6=random(10,50);
  v1 = random(30,48);
    
  t1++;t2++;t3++;t4++;
<<<<<<< HEAD
   
=======
 
>>>>>>> 7327904a6a6abccf9e7a39f86c0bebf6ce6bdfbb
  String output= converstion(t1,t2,t3,t4,v1,y,p,r,temp,humidity,internal_pressure,t6);
  packetSize = Udp.parsePacket(); //Read theh packetSize
  
  if(packetSize>0)
  { 
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Reading the data request on the Udp
    datReq=packetBuffer; //Convert packetBuffer array to string datReq
    addNos(datReq);
    Serial.println(datReq);
    if(nos[17]==1 && prev!=nos[17])
    {
      send("1");
    }
    else if(nos[17]==2 && prev!=nos[17])
    {
      send("2"); 
    }
    else if(nos[18]==3 && prev!=nos[17])
    {
      send("3");
    }
    prev=nos[17];
    thruster_movement();
  }
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  //Initialize Packet send
  Udp.print(output); //Send string back to client 
  Udp.endPacket(); //Packet has been sent
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
<<<<<<< HEAD
}
=======
>>>>>>> 7327904a6a6abccf9e7a39f86c0bebf6ce6bdfbb
}
}
