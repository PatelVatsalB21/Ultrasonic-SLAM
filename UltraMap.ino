unsigned long prev_t = 0;
unsigned long curr_t = 0;

int *readings;
int index = 0;
int mapsize = 100;
byte unit_TH = 20000;
byte unit_TV = 15000;

byte l=0, b=0, r=0;

byte base_size = 0;

void setup() {
  readings = (int *) calloc (5, sizeof(int));
  Serial.begin(9600);
}

void loop() {
  l=10;
  b=30;
  r=10;  
  for(int k=1; k<100; k++){
    Serial.println("k is: ");
    Serial.println(k);
    l++;
    b++;
    r++;
    addReading(k);
    delay(1000);
    Serial.println("LastReading");
    Serial.println(readings[index]);
    delay(1000);
  }
}

void re_Readings(int i) {
  int *q;
  q = (int *) calloc (i* 2 , sizeof (int));
  for (int j = 0; j < i; j++){
    q[j] = readings[j];
  }
  free(readings);
  readings = q;
  q = NULL;
  Serial.println("Rereadings");
  for (int j = 0; j < 2*i; j++){
    Serial.println(readings[j]);
  }
}

void addReading(int i){
  Serial.println("index:");
  Serial.println(index);
  if((mapsize-1) == index){
    Serial.print(mapsize);
    re_Readings(mapsize);
    mapsize *= 2;
  }
  if(i % 2 == 0) {
    if ((i & 3) == 0){
      readings[index+1] = l;
      index++;
      Serial.println("IF 1");
    } else {
      readings[index+1] = r;
      index++;
      Serial.println("IF 2");
    }
  }else {
    if(i == 1){
      readings[0] = 0;
      readings[1] = b;
      readings[2] = 0;
      base_size = readings[1];
      index = 2;
      Serial.println("IF 3");
    } else {
      readings[index+1] = b;
      index++;
      Serial.println("IF 4");
    }
  }
  for(int j =0; j<=mapsize; j++) {
    Serial.print(readings[j]); 
    Serial.print(", "); 
  }
  Serial.println(" ");
  Serial.println("Size");
  Serial.println(mapsize);
}

void writeMap(){
  FILE *f = fopen("map.data", "wb");
  fwrite(readings, sizeof(int), mapsize, f);
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
