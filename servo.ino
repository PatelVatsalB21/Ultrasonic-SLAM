#include <Servo.h> 
Servo Servo1; 
void setup() { 
   // We need to attach the servo to the used pin number 
   Servo1.attach(10); 
   Servo1.write(170); 
}
void loop(){ 
//   // Make servo go to 0 degrees 
//   Servo1.write(10); 
//   delay(1000); 
//   // Make servo go to 90 degrees 
//   Servo1.write(90); 
//   delay(1000); 
//   // Make servo go to 180 degrees 
//   Servo1.write(170); 
//   delay(1000); 
}
