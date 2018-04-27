
int pinButton1 = 10;
int pinButton2 = 11;                        
int stateButton1=0;
int stateButton2=0;
//Motor A
const int inputPin1  = 8;    
const int inputPin2  = 9;    
//Motor B
const int inputPin3  = 2;   
const int inputPin4  = 3;   
void setup() 
{ Serial.begin(9600);
  pinMode(pinButton1, INPUT); 
  pinMode(pinButton2, INPUT);  
    pinMode(inputPin1, OUTPUT);
    pinMode(inputPin2, OUTPUT);
    pinMode(inputPin3, OUTPUT);
    pinMode(inputPin4, OUTPUT);  
}

void loop()
{
 if(digitalRead(pinButton1)==HIGH)
{
    digitalWrite(inputPin1, HIGH);
    digitalWrite(inputPin2, LOW);
    digitalWrite(inputPin3, LOW);
    digitalWrite(inputPin4,HIGH);
} 
 else
   if(digitalRead(pinButton2)==HIGH)
   { digitalWrite(inputPin1, LOW);
    digitalWrite(inputPin2, HIGH);
    digitalWrite(inputPin3,HIGH);
    digitalWrite(inputPin4, LOW); 
   } 
   else
   {digitalWrite(inputPin1, LOW);
    digitalWrite(inputPin2, LOW);
    digitalWrite(inputPin3, LOW);
   digitalWrite(inputPin4, LOW); 
   } 
   }
   



