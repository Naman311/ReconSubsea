char init_status[]="11111111";
int i=0;
int t1=0;int t2=-1;int t3=4;int t4=2;
int v1=0; long y=0;long p=0; long r=0;


long temp;
long humidity;
long internal_pressure;


void setup() {
  Serial.begin(9600);
  //Serial.println(init_status);
  //randomSeed(analogRead(A0));
  
}

void loop() {
  //randomSeed(analogRead(A0));


  //for(i=0;i<15;i++){
    temp = random(15,30);
    humidity = random(10,300);
   
    internal_pressure = random(50,200);
    y = random(10,50);
    p = random(10,50);
    r = random(10,50);
    
    v1 = random(30,48);
    

   
    Serial.print(v1);Serial.print(",");

    Serial.print(t1);Serial.print(",");
    Serial.print(t2);Serial.print(",");
    Serial.print(t3);Serial.print(",");
    Serial.print(t4);Serial.print(",");
   
    
    
    
    

    

    
    Serial.print(temp);Serial.print(",");
    Serial.print(humidity);Serial.print(",");
    
    Serial.print(internal_pressure);Serial.print(",");
    Serial.print(y);Serial.print(",");
    Serial.print(p);Serial.print(",");
    Serial.println(r);    
 
    t1++;t2++;t3++;t4++;
    //delay(500);
    
    
  }

/*while(true){

 
  for(i=0;i<6;i++){
   
    
    temp = random(15,30);
    humidity = random(10,300);
    depth = random(20,400);
    internal_pressure = random(50,200);

    v1 = random(30,48);
    v2 = random(0,24);
    v3 = random(0,12);
    v4 = random(0,24);

    pwm1 = random(0,24);
    pwm2 = random(0,24);
    pwm3 = random(0,24);
    pwm4 = random(0,24);
    pwm5 = random(0,24);
    pwm6 = random(0,24);
    pwm7 = random(0,24);
    pwm8 = random(0,24);

    
    Serial.print(t1);Serial.print(",");
    Serial.print(t2);Serial.print(",");
    Serial.print(t3);Serial.print(",");
    Serial.print(t4);Serial.print(",");
    Serial.print(t5);Serial.print(",");
    Serial.print(t6);Serial.print(",");
    Serial.print(t7);Serial.print(",");
    Serial.print(t8);Serial.print(",");

    Serial.print(pwm1);Serial.print(",");
    Serial.print(pwm2);Serial.print(",");
    Serial.print(pwm3);Serial.print(",");
    Serial.print(pwm4);Serial.print(",");
    Serial.print(pwm5);Serial.print(",");
    Serial.print(pwm6);Serial.print(",");
    Serial.print(pwm7);Serial.print(",");
    Serial.print(pwm8);Serial.print(",");

    Serial.print(v1);Serial.print(",");
    Serial.print(v2);Serial.print(",");
    Serial.print(v3);Serial.print(",");
    Serial.print(v4);Serial.print(",");

    Serial.print(temp);Serial.print(",");
    Serial.print(humidity);Serial.print(",");
    Serial.print(depth);Serial.print(",");
    Serial.println(internal_pressure);
    t1--;t2--;t3--;t4--;t5--;t6--;t7--;t8--;
    delay(500);
    
  }

  for(i=0;i<6;i++){

    v1 = random(30,48);
    v2 = random(0,24);
    v3 = random(0,12);
    v4 = random(0,24);
    
    Serial.print(t1);Serial.print(",");
    Serial.print(t2);Serial.print(",");
    Serial.print(t3);Serial.print(",");
    Serial.print(t4);Serial.print(",");
    Serial.print(t5);Serial.print(",");
    Serial.print(t6);Serial.print(",");
    Serial.print(t7);Serial.print(",");
    Serial.print(t8);Serial.print(",");


    Serial.print(pwm1);Serial.print(",");
    Serial.print(pwm2);Serial.print(",");
    Serial.print(pwm3);Serial.print(",");
    Serial.print(pwm4);Serial.print(",");
    Serial.print(pwm5);Serial.print(",");
    Serial.print(pwm6);Serial.print(",");
    Serial.print(pwm7);Serial.print(",");
    Serial.print(pwm8);Serial.print(",");

    Serial.print(v1);Serial.print(",");
    Serial.print(v2);Serial.print(",");
    Serial.print(v3);Serial.print(",");
    Serial.print(v4);Serial.print(",");

    Serial.print(temp);Serial.print(",");
    Serial.print(humidity);Serial.print(",");
    Serial.print(depth);Serial.print(",");
    Serial.println(internal_pressure);
   
    t1++;t2++;t3++;t4++;t5++;t6++;t7++;t8++;
    
    delay(700);
  
  }

}
}*/
