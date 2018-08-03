const float SensorOffset = 39;
void setup() {
  Serial.begin(9600);
}
 
// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  float sensorValue = (analogRead(A0)-SensorOffset); //Do maths for calibration
  // print out the value you read:
  Serial.print("Air Pressure: ");  
  Serial.print(sensorValue,2);
  Serial.println(" kPa");
  
  delay(1000);        // delay in between reads for stability
}
