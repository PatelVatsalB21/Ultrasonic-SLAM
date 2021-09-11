#include <AFMotor.h>  
#include <Servo.h> 

AF_DCMotor motor1(1); 
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
Servo servo;

unsigned long prev_t = 0;
unsigned long curr_t = 0;

int readings[] = {0, 30, 0, 10, 40, 10, 20, 10, 10};
int mapsize, rows;
int **map_array;
int x,y;
int orientation;
// 0 forward x, 1 back x, 2 forward y, 3 back_y

byte unit_TH = 20000;
byte unit_TV = 15000;
byte full_rot_time = 30000;

void setup() {
  servo.attach(10);  
  initializeUS();
//  int **map_array = (int **)calloc(rows, sizeof(int *));
//  for (int j=0; j < rows; j++)
//    map_array[j] = (int *)calloc(3, sizeof(int));

  
  mapsize = sizeof(readings)/sizeof(int);
  rows = mapsize/3;
  if(mapsize%3 > 0){
    rows += 1;
  }
  
  map_array = new int*[rows];
  for (int i = 0; i < 3; i++) {
    map_array[i] = new int[3];
  }

  Serial.println("Clean Map");
  for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
         Serial.println(map_array[i][j]);
}

void loop() {
  make_map();
}

void cal_coordinates(int abs_x , int abs_y){
  int curr_row = abs_y;
  int curr_col = abs_x;
  int curr_row_length = map_array[curr_row][0] + map_array[curr_row][1] + map_array[curr_row][2];

  if(curr_row < rows && curr_col < curr_row_length){
    navigate(curr_col, curr_row);
  }
}

void navigate(int corr_x , int corr_y) {
  boolean inv_x, inv_y;
  int new_x = abs(corr_x - x);
  int new_y = abs(corr_y - y);
  int angle = cal_angle(corr_x, corr_y);
  
//  if(x > corr_x){
//    inv_x = true;
//  }
//  if(y > corr_y){
//    inv_y = true;
//  }

  if(x > corr_x && y > corr_y){
    angle += 180; 
  } else if(x > corr_x && y < corr_y){
    angle += 90; 
  } else if(x < corr_x && y > corr_y){
    angle = 360 - angle; 
  } 

  int hypo = sqrt(sq(corr_x)+sq(corr_y));
  orient_to_dir(angle);
  move_distance(hypo);
  orient_back(angle);
}

void orient_back(int angle){
  byte rot_time = (angle/360)*full_rot_time;
    curr_t = millis();
    while(millis() - curr_t < rot_time){
      turn_long_l();      
    }
    move_s();
}

void orient_to_dir(int angle){
  orient_origin();
  byte rot_time = (angle/360)*full_rot_time;
  if(angle < 180){
    curr_t = millis();
    while(millis() - curr_t < rot_time){
      turn_long_l();      
    }
    move_s();
  }else {
    curr_t = millis();
    while(millis() - curr_t < rot_time){
      turn_long_r();      
    }
    move_s();
  }
  
}

void orient_origin(){
  if(orientation == 1){
      turn_r();
      turn_r();
  }else if (orientation == 2){
      turn_r();
  }else if (orientation == 3){
      turn_l();
  }
  orientation = 0;
}

int cal_angle(int ang_x, int ang_y){
  const float DEG2RAD = PI / 180.0f;
  const float RAD2DEG = 180.0f / PI;
  float angle = atan2(ang_y, ang_x) * RAD2DEG;
  return (int)angle;
}


void move_distance(int distance){
  prev_t = millis();
  while(readUS > 15 && millis()-prev_t < distance*unit_TH){
    move_f();
  }
  curr_t = millis();
  if(curr_t-prev_t < distance*unit_TH){
    delta_obstacle();
    int mid_t = millis();
    while(millis() - mid_t < curr_t - prev_t){
      move_distance(curr_t - prev_t/unit_TH);
    }
  }
}

void delta_obstacle(){
  
}


void make_map(){
  int itr = 0;
//  int r = 0;
//  for(int j=0; j<mapsize; j++){
//    map_array[r][itr] = room_map[j];
//    if(itr == 3){
//      r++;
//      itr = 0;
//    }
//    itr++;
//  }
    for (int i = 0; i <  3; i++) {
      for (int j = 0; j < 3; j++){
         Serial.println("Readings");
         Serial.println(readings[itr]);
         map_array[i][j] = readings[itr];
         Serial.println(map_array[i][j]);
         itr++;  
      }
    }

  Serial.println("New Map");
  for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
         Serial.println(map_array[i][j]);
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

void move_b() {
  Serial.println("Backward");
  motor1.run(BACKWARD);      
  motor2.run(BACKWARD);
  motor3.run(BACKWARD); 
  motor4.run(BACKWARD);    
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

void turn_long_r() {
  Serial.println("Long Right");
  motor1.run(FORWARD);
  motor4.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);     
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


void turn_long_l() {
  Serial.println("Long Left");
  motor1.run(BACKWARD);     
  motor4.run(BACKWARD);  
  motor2.run(FORWARD);
  motor3.run(FORWARD);   
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

int look_long_r(){
  servo.write(10);
  int d = readUS();
  return d;
}

int look_long_l(){
  servo.write(170);
  int d = readUS();
  return d;
}

//void re_Readings(int i) {
//  int *q;
//  q = (int *) calloc (i* 2 , sizeof (int));
//  for (int j = 0; j < i; j++){
//    q[j] = readings[j];
//  }
//  free(readings);
//  readings = q;
//  q = NULL;
//  Serial.println("Rereadings");
//  for (int j = 0; j < 2*i; j++){
//    Serial.println(readings[j]);
//  }
//}

//void addReading(int i){
//  Serial.println("index:");
//  Serial.println(index);
//  if((mapsize-1) == index){
//    Serial.print(mapsize);
//    re_Readings(mapsize);
//    mapsize *= 2;
//  }
//  if(i % 2 == 0) {
//    if ((i & 3) == 0){
//      readings[index+1] = l;
//      index++;
//      Serial.println("IF 1");
//    } else {
//      readings[index+1] = r;
//      index++;
//      Serial.println("IF 2");
//    }
//  }else {
//    if(i == 1){
//      readings[0] = 0;
//      readings[1] = b;
//      readings[2] = 0;
//      base_size = readings[1];
//      index = 2;
//      Serial.println("IF 3");
//    } else {
//      readings[index+1] = b;
//      index++;
//      Serial.println("IF 4");
//    }
//  }
//  for(int j =0; j<=mapsize; j++) {
//    Serial.print(readings[j]); 
//    Serial.print(", "); 
//  }
//  Serial.println(" ");
//  Serial.println("Size");
//  Serial.println(mapsize);
//}


void printMap() {
  Serial.println("Map: ");
  for(int j =0; j<=mapsize; j++) {
    Serial.print(readings[j]); 
    Serial.print(", "); 
  }
}


void writeMap(){
  FILE *f = fopen("map.data", "wb");
  fwrite(map_array, sizeof(int), mapsize, f);
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
