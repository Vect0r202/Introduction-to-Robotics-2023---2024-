//buttons
const int button1 = 2, button2 = 3, button3 = 13;
volatile bool buttonPressed1 = false;

// pause variable
bool Pause = false;

// DisplayOn - variabila care daca are valoarea TRUE, atunci putem da reset, (si este in pauza)
bool DisplayOn = true;

// intervalul minim permis intre apasarea de butoane:
const int intervalll = 100;

unsigned long lastIncrement = 0, delayCount = 100, number = 0;

unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 1000;

const int latchPin = 11; // Connects to STCP of the shift register
const int clockPin = 10; // Connects to SHCP of the shift register
const int dataPin = 12;  // Connects to DS of the shift register

// Pin definitions for controlling the individual digits of the 7-segment display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

// Constant for the size of the shift register in bits (8 bits for a standard register)
const byte regSize = 8;
// for the decimal point at the 3rd display from left to right
byte decimalPoint = B00000001;

// Array to hold the pins that control the segments of each digit of the display
int displayDigits[] = {
  segD1, segD2, segD3, segD4
};
const int displayCount = 4; // Total number of digits in the display
const int encodingsNumber = 16; // Total number of encodings for the hexadecimal characters (0-F)

// Array holding binary encodings for numbers and letters on a 7-segment display
byte byteEncodings[encodingsNumber] = {
  // Encoding for segments A through G and the decimal point (DP)
  //A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};

void setup() {
  // Setup function is run once at the start of the program
  // Set the shift register pins as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button1), pause, FALLING);
  attachInterrupt(digitalPinToInterrupt(button2), reset, FALLING);
  // Initialize the digit control pins and set them to an off state
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  zeroState();
  Serial.begin(9600); // Begin serial communication, not used in this code snippet
}

void loop() {
  if(!Pause){
    if(millis() - lastIncrement > delayCount){
      number++;
      number %= 10000;
      lastIncrement = millis();
    }
  writeNumber(number);
  }else{
    writeNumber(number);
  }
  
}

void zeroState(){
  number = 0;
  writeNumber(number);
}

void writeReg(int encoding) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, encoding);
  digitalWrite(latchPin, HIGH);
}


void activateDisplay(int displayNumber) {
  for (int i = 0;i<displayCount;i++){
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int number){
  int currentNumber = number;
  int displayDigit = 3;
  int lastDigit = 0;
  while (currentNumber != 0){
    lastDigit = currentNumber%10;
    activateDisplay(displayDigit);
    if (displayDigit == 2) {
      writeReg(decimalPoint);
    }
    writeReg(byteEncodings[lastDigit]);
    delay(0);
    displayDigit--;
    currentNumber /= 10;
    writeReg(B00000000);
  }
}

void pause() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > intervalll) {
    buttonPressed1 = true;
    Pause = !Pause;
  }
  lastInterruptTime = interruptTime;
}

// RESET function

void reset(){
  if(Pause){
    zeroState();
  }
}
