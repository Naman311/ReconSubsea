#include<Wire.h>
#include <math.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ;
void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
}
int num_samples = 10;    // take the average of multiple samples

void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  float sum1 = 0.0f,sum2 = 0.0f,sum3 = 0.0f;
  for (int i=0; i < num_samples; i++)
  {
    float z = AcZ;  // +z is up from the top of the pcb
    float y = AcY;  // +y is lengthwise to the right
    float x = AcX;
    
    float angle1 = atan2(x, y) * 180.0f / M_PI;  // angle will be between -360 and 360 degrees
    float angle2 = atan2(y, z) * 180.0f / M_PI;  // angle will be between -360 and 360 degrees
    float angle3 = atan2(z, x) * 180.0f / M_PI;  // angle will be between -360 and 360 degrees
    sum1 += angle1;
    sum2 += angle2;
    sum3 += angle3;
    delay(15);
  }
  float average1 = round(sum1 / num_samples);
  float average2 = round(sum2 / num_samples);
  float average3 = round(sum3 / num_samples);
 //Serial.println(average);
  Serial.print(average1);
  Serial.print("  ");
  Serial.print(average2);
  Serial.print("  ");
  Serial.print(average3);
  Serial.println("  ");
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
  /*Serial.print("  X: ");
  Serial.print(AcX);
  Serial.print("  Y: ");
  Serial.print(AcY);
  Serial.print("  Z: ");
  Serial.print(AcZ);
  Serial.println();*/
  //delay(500);
}
