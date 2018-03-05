

 // MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Xmax=-1111,Xmin=1111,Ymax=-1111,Ymin=1111,Zmax=-1111,Zmin=1111;
void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
}
void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  /*if(AcX<Xmin)
  {
    Xmin=AcX;
  }
  if(AcX>Xmax)
  {
    Xmax=AcX;
  }
  if(AcY<Ymin)
  {
    Ymin=AcY;
  }
  if(AcY>Ymax)
  {
    Ymax=AcY;
  }
  if(AcZ<Zmin)
  {
    Zmin=AcZ;
  }
  if(AcZ>Zmax)
  {
    Zmax=AcZ;
  }
  Serial.print("Xmin: ");
  Serial.print(Xmin);
  Serial.print("  Xmax: ");
  Serial.print(Xmax);
  Serial.print("  Ymin: ");
  Serial.print(Ymin);
  Serial.print("  Ymax: ");
  Serial.print(Ymax);
  Serial.print("  Zmin: ");
  Serial.print(Zmin);
  Serial.print("  Zmax: ");
  Serial.print(Zmax);*/
  Serial.print("  X: ");
  Serial.print(AcX);
  Serial.print("  Y: ");
  Serial.print(AcY);
  Serial.print("  Z: ");
  Serial.print(AcZ);
  Serial.println();
  delay(500);
}
