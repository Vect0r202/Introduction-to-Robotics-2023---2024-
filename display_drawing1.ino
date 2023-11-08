 //#include <avr/wdt.h>

//pinii displayului
const int pinA = 12, pinB = 10, pinC = 9, pinD = 8, pinE = 7, pinF = 6, pinG = 5, pinDP = 4;

//pinii joystickului
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;

//vector pini + nr total
const int segmentPins[] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};
const int numSegments = 8;
 
int currentSegment = 7, previousSegment;

//pentru reset
volatile bool swPressed = false;
unsigned long swPressStartTime = 0;

//joystick switchstate
byte swState = LOW;
byte lastSwState = LOW;
byte ledState = LOW;
unsigned long swPressTime = 0;

unsigned long currentMillis;
unsigned long previousMillis = 0;
 
int display[8];
int displayIndex = 0;
 
int X;
int Y;
 
char position = 0;
char lastPosition = 0;

bool ok = true;
 
void setup() {
  for (int i = 0; i < numSegments; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(pinSW), swInterrupt, RISING);
  digitalWrite(pinDP, HIGH);
  Serial.begin(9600);
  //wdt_enable(WDTO_15MS);
}
 
void loop() {
  X = analogRead(pinX);
  Y = analogRead(pinY);
  swState = digitalRead(pinSW);
  position = 0;
 
  if (X > 650){
    position = "r";
  }
  else if (X < 350){
    position = "l";
  }
  else if (Y > 650){
    position = "u";
  }
  else if (Y < 350){
    position = "d";
  }
 
  if(position != lastPosition){
    movement();
  }
  lastPosition = position;
 
  lightON();
 
  currentMillis = millis();
  if(ok){
    if (currentMillis - previousMillis >= 500) {
      previousMillis = currentMillis;
 
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      digitalWrite(segmentPins[currentSegment], ledState);
    }
  }
  

  // incercare de reset cu interrupt esuata

  // if (swPressed) {
  //   unsigned long currentTime = millis();
  //   if (currentTime - swPressStartTime > 1500) {
  //     wdt_reset();
  //   }
  //   swPressed = false;
  // }

  if (swState != lastSwState) {
    
    if (swState == LOW) {
      swPressTime = millis();
    } else {
      unsigned long swReleaseTime = millis();
      if (swReleaseTime - swPressTime > 1500) { 
        // reset - s ar putea face usor (setez toate ledurile la low, dar oricum nu intra in acest if :(( )
      }
    }
  }
  lastSwState = swState;
}
 
void movement(){
  if (X > 650) {
    if(currentSegment == 0){
      currentSegment = 0;
      previousSegment = 0;
    }
    else if(currentSegment == 1){
      currentSegment = 0;
      previousSegment = 1;
    }
    else if(currentSegment == 2){
      currentSegment = 6;
      previousSegment = 2;
    }
    else if(currentSegment == 3){
      currentSegment = 6;
      previousSegment = 3;
    }
    else if(currentSegment == 4){
      currentSegment = 6;
      previousSegment = 4;
    }
    else if(currentSegment == 5){
      currentSegment = 0;
      previousSegment = 5;
    }
    else if(currentSegment == 6){
      if(previousSegment == 2){
        currentSegment = 1;
      }
      if(previousSegment == 4){
        currentSegment = 5;
      }
      if(previousSegment == 3){
        currentSegment = 0;
      }
      previousSegment = 6;
    }
  } 
  else if (X < 350) {
    if(currentSegment == 0){
      currentSegment = 6;
      previousSegment = 0;
    }
    else if(currentSegment == 1){
      currentSegment = 6;
      previousSegment = 1;
    }
    else if(currentSegment == 2){
      currentSegment = 3;
      previousSegment = 2;
    }
    else if(currentSegment == 3){
      currentSegment = 3;
      previousSegment = 3;
    }
    else if(currentSegment == 4){
      currentSegment = 3;
      previousSegment = 4;
    }
    else if(currentSegment == 5){
      currentSegment = 6;
      previousSegment = 5;
    }
    else if(currentSegment == 6){
      if(previousSegment == 1){
        currentSegment = 2;
      }
      if(previousSegment == 5){
        currentSegment = 4;
      }
      if(previousSegment == 0){
        currentSegment = 3;
      }
      previousSegment = 6;
    }
  } 
  else if (Y < 350) {
    if(currentSegment == 0){
      currentSegment = 5;
    }
    else if(currentSegment == 1){
      currentSegment = 5;
    }
    else if(currentSegment == 2){
      currentSegment = 4;
    }
    else if(currentSegment == 3){
      currentSegment = 4;
    }
    else if(currentSegment == 4){
      currentSegment = 4;
    }
    else if(currentSegment == 5){
      currentSegment = 5;
    }
    else if(currentSegment == 6){
      currentSegment = 5;
    }
    else if(currentSegment == 7){
      currentSegment = 2;
    }
  } 
  else if (Y > 650) {
    if(currentSegment == 0){
      currentSegment = 1;
    }
    else if(currentSegment == 1){
      currentSegment = 1;
    }
    else if(currentSegment == 2){
      currentSegment = 7;
    }
    else if(currentSegment == 3){
      currentSegment = 2;
    }
    else if(currentSegment == 4){
      currentSegment = 2;
    }
    else if(currentSegment == 5){
      currentSegment = 1;
    }
    else if(currentSegment == 6){
      currentSegment = 1;
    }
  }
  for(int i = 0; i < 8; i++){
    digitalWrite(segmentPins[i], LOW);
  }
}
 
void lightON(){
  // retinem ledurile pe care am apasat butonul in vectorul display
  swState = digitalRead(pinSW);
  if(swState != lastSwState) {
    if(swState == LOW) {
      display[displayIndex]=currentSegment;
      displayIndex++;
    }
  }
  lastSwState = swState;
  if(displayIndex == 8){
    ok = false;
    // ledul curent nu va mai clipoci
  }
 
  // pastram aprinse toate ledurile din vectorul display (acolo retinem ledurile pe care am apasat butonul)
  for(int j = 0; j < displayIndex; j++){
    if(display[j] != currentSegment){
      digitalWrite(segmentPins[display[j]], HIGH); 
    }
  }
}


// incercare de reset cu interrupt esuata

// void swInterrupt() {
//   if (digitalRead(pinSW) == HIGH) {
//     swPressStartTime = millis();
//     swPressed = true;
//   }
// }

