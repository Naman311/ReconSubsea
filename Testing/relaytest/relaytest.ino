int relayInput = 14; // the input to the relay pin
void setup() {

  Serial.begin(115200);
  pinMode(relayInput, OUTPUT); // initialize pin as OUTPUT

}
void loop() {

  digitalWrite(relayInput, HIGH); // turn relay on
  Serial.println("HIGH");
  delay(1000);

  digitalWrite(relayInput, LOW); // turn relay off
  Serial.println("LOW");
  delay(1000);

}
