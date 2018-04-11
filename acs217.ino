double Voltage = 0;
double Current = 0;

void setup(){
Serial.begin(9600);
}

void loop(){

// Voltage is Sensed 1000 Times for precision
for(int i = 0; i < 1000; i++) {
Voltage = (Voltage + (.0049 * analogRead(A0))); // (5 V / 1024 (Analog) = 0.0049) which converter Measured analog input voltage to 5 V Range
delay(1);
}
Voltage = Voltage /1000;

Current = (Voltage -2.5)/ 0.185; // Sensed voltage is converter to current

Serial.print(“\n Voltage Sensed (V) = “); // shows the measured voltage
Serial.print(Voltage,2); // the ‘2’ after voltage allows you to display 2 digits after decimal point
Serial.print(“\t Current (A) = “); // shows the voltage measured
Serial.print(Current,2); // the ‘2’ after voltage allows you to display 2 digits after decimal point

delay(1000);

}
