// comment this when settings are ready
#define __DEBUG 

// --- encoder 
int CLK = 5;  // PIN5
int DT = 4;   // PIN4
int position = 0; 
int rotation;  
int value;

void setupEncoder() {
  pinMode (CLK, INPUT);
  pinMode (DT, INPUT);
  rotation = digitalRead(CLK);   
  Serial.println("ready!");
}

int readEncoder() {
  int move = 0;
  value = digitalRead(CLK);
  if (value != rotation){ // we use the DT pin to find out which way we turning.
    move = 1;
    if (digitalRead(DT) != value) {  // Clockwise
      position ++;
    } else { 
      //Counterclockwise
      position--;
    }
  } 
  
  rotation = value;
  return move;
}

// --- sensor
int sensorsPin = 2; // PIN 2

void setupSensor(){
  pinMode(sensorsPin,INPUT);//set the state of D0 as INPUT
}

int readSensor() {
  return !digitalRead(sensorsPin);
}

// -- light

int lightValue = LOW;

int lightPin = LED_BUILTIN; // CHNAGE TO ANY, for example PIN6 

void setupLight() {
  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, lightValue);
}

void light(int v){
  lightValue = v;
  digitalWrite(lightPin, lightValue);
}

// ---
 
void setup() { 
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  setupEncoder();
  setupSensor();
  setupLight();
} 

int pause = 200; // cycle pause in ms
int cycle = 10;  // how long light is ON after encoder does not move; time = pause * cycle, 200 * 10 = 2000ms = sec
int running = cycle;
 
void loop() { 
  int sensor = readSensor();
  if (sensor) {
    light(LOW);
  }

  int move = readEncoder();
  if (move && !sensor) {
    light(HIGH);
    running = cycle;
  }

  if (!move) {
    if (!running){
      light(LOW);
    }
    else {
      running--;  
    }
  }
  
  delay(pause);

  #ifdef __DEBUG
  Serial.print("sensor: ");
  Serial.print(sensor);
  Serial.print("; running: ");
  Serial.print(running);
  Serial.println(" - tick");
  #endif
}
 
