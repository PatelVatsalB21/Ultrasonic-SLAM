#include <AFMotor.h>  
#include <Servo.h> 

AF_DCMotor motor1(1); 
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
Servo servo;

unsigned long prev_t = 0;
unsigned long curr_t = 0;

int readings[100];
byte mapsize = 100;
int i = 1;
byte unit_TH = 20000;
byte unit_TV = 15000;

byte l=0, b=0, r=0;

byte base_size = 0;

void setup() {
  servo.attach(10);  
  servo.write(80); 
  initializeUS();
}

void loop() {
  Serial.println("I is: ");
  Serial.print(i);
  l=0;
  b=0;
  r=0;
  if(readUS() > 15) {
      if(i%2 != 0){
        prev_t = millis();
        while(readUS() > 15){
          Serial.println("Inside 1");
          move_f();
        }
      } else {
        if (i % 4  != 0){
          turn_r();
          move_oneblock();
          if(look_l() > 20){
            turn_l();
            prev_t = millis();
            while(readUS() > 10){
              Serial.println("Inside 2");
              move_f();
            }
            curr_t = millis();
//            addReading(i);
            l = (curr_t - prev_t)/unit_TH;
            turn_r();
            Serial.println("Inside 3");
            move_f();
            delay(curr_t - prev_t);
          }
          turn_r();
          i++;
        } else {
          turn_l();
          move_oneblock();
          if(look_r()> 20){
            turn_r();
            prev_t = millis();
            while(readUS() > 10){
              Serial.println("Inside 4");
              move_f();
            }
            curr_t = millis();
//            addReading(i);
            r = (curr_t - prev_t)/unit_TH;
            turn_l();
            Serial.println("Inside 5");
            move_f();
            delay(curr_t - prev_t);
          }
          turn_l();
          i++;
        }
      }
    } else {
      move_s();
      curr_t = millis();
      b = (curr_t - prev_t)/unit_TH;
//      addReading(i);
      i++;
      if(look_r() > 20){
        turn_r();
      }
    }
}

void move_s() {
  Serial.println("Stop");
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
} 
  
void move_f() {
  Serial.println("Forward");
  motor1.setSpeed(250);
  motor2.setSpeed(250);
  motor3.setSpeed(250);
  motor4.setSpeed(250);   
  motor1.run(FORWARD);      
  motor2.run(FORWARD);
  motor3.run(FORWARD); 
  motor4.run(FORWARD);    
}

void move_oneblock() {
    Serial.println("OneBlock");
    motor1.run(FORWARD);      
    motor2.run(FORWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD);     
    delay(unit_TV);
    move_s();
}

void turn_r() {
  Serial.println("Right");
  motor1.run(FORWARD);
  motor4.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);     
  delay(2000);
//  motor1.run(FORWARD);      
//  motor2.run(FORWARD);
//  motor3.run(FORWARD);
//  motor4.run(FORWARD);      
} 

void turn_l() {
  Serial.println("Left");
  motor1.run(BACKWARD);     
  motor4.run(BACKWARD);  
  motor2.run(FORWARD);
  motor3.run(FORWARD);   
  delay(2000);
//  motor1.run(FORWARD);     
//  motor2.run(FORWARD);
//  motor3.run(FORWARD);
//  motor4.run(FORWARD);
}

void initializeUS(){
  pinMode(A0, OUTPUT); 
  pinMode(A1, INPUT);
  Serial.begin(9600);
  look_f();
}

int readUS() {
  digitalWrite(A0, LOW);
  delayMicroseconds(200);
  digitalWrite(A0, HIGH);
  delayMicroseconds(1000);
  digitalWrite(A0, LOW);
//  distance= duration*0.034/2;
  int distance = microsecondsToCentimeters(pulseIn(A1, HIGH));
  if(distance == 0)Serial.println(distance);
  return distance;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

void look_f(){
  servo.write(80);
}

int look_r(){
  servo.write(10);
  int d = readUS();
  delay(1000);
  look_f();
  return d;
}

int look_l(){
  servo.write(170);
  int d = readUS();
  delay(1000);
  look_f();
  return d;
}

void re_Readings() {
  byte i = sizeof(readings);
  int nreadings[2*i];
  while ( i-- ) *( nreadings + i ) = *( readings + i );
  int *readings = new int[2*i];
  readings = nreadings;
  mapsize = 2*mapsize;
}

void addReading(int i){
  int index = sizeof(readings)/sizeof(readings[0]) - 1;
  if(mapsize == index){
    re_Readings();
  }
  if(i % 2 == 0) {
    if ((i & 3) == 0){
      readings[index+1] = l;
    } else {
      readings[index+1] = r;
    }
  }else {
    if(i == 1){
      readings[0] = 0;
      readings[2] = 0;
      readings[1] = (curr_t - prev_t)/unit_TH;
      base_size = readings[1];
    } else {
      readings[index+1] = b;
    }
  }
}

void writeMap(){
  FILE *f = fopen("map.data", "wb");
  fwrite(readings, sizeof(int), sizeof(readings), f);
  fclose(f);
}

void readMap(){
//  #define fmap "C:\\tempExtract\\fmap.data"
//  FILE *fp;
//  int newMap[100];
//  fopen(fmap, "rb");
//  fread(newMap, sizeof(char), 1,fp);
//  fclose(fp);
}
