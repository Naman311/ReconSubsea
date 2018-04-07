#include <stdlib.h>
#include <SPI.h>
#define SCK_PIN  52
#define MISO_PIN 50
#define MOSI_PIN 51
#define SS_PIN   53
void setup() {
  // put your setup code here, to run once:
pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);
  SPI.begin();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

}
