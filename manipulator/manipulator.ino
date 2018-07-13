//Motor A
const int inputPin1  = 6;    
const int inputPin2  = 7;    
//Motor B
const int inputPin3  = 8;   
const int inputPin4  = 9;   
void setup() 
{ 
  Serial.begin(9600);  
    pinMode(inputPin1, OUTPUT);
    pinMode(inputPin2, OUTPUT);
    pinMode(inputPin3, OUTPUT);
    pinMode(inputPin4, OUTPUT);  
}

void loop()
{
    digitalWrite(inputPin1, HIGH);
    digitalWrite(inputPin2, LOW);
    digitalWrite(inputPin3, LOW);
    digitalWrite(inputPin4,HIGH);
    delay(1000); 
    digitalWrite(inputPin1, LOW);
    digitalWrite(inputPin2, HIGH);
    digitalWrite(inputPin3,HIGH);
    digitalWrite(inputPin4, LOW); 
    delay(1000);
}
   



