#include <EEPROM.h>

struct SensorReading {
    int ultraSoundValue;
    int lightValue;
};

SensorReading ultrasoundReadings[10];
SensorReading lightReadings[10];

int ultrasoundIndex = 0;
int lightIndex = 0;
// ========== GLOBAL VARIABLES ========== //
const int redLed = 5, greenLed = 6, blueLed = 7;
const int photocellPin = A0;
const int TrigPin = 9, EchoPin = 10;
long duration = 0;
int distance = 0;

//flag - it s for the messages - with this i only get the message once when the object gets in the range of the minimum threshold on the ultrasonic sensor chosen with the menu (or the light is too dim)
bool messagePrinted=false, nightMessagePrinted = false, dayMessagePrinted = false;

int ok=1, ok1=0, ok2=0, ok3=0, ok4=0;
int okSampling=0, okThreshold=0, okLight=0, okSensorReadings=0, okColor=0, okToggle=0;

int sampling, threshold = 0, lightthreshold = 0, photocellValue;

unsigned long previousMillis = 0;

// !! =========== !!

// sampling address for the EEPROM will be 1, for ultrasound threshold will be 3 and for the lightthreshold will be 5, i won t be using variables for theese addresses in order to use as little memory as possible

// !! =========== !!


// ========== SETUP ========== //

void setup(){
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  Serial.begin(9600);
  printMenu();
}

// ========== LOOP ========== //

void loop(){
  unsigned long currentMillis = millis();
  if (okSensorReadings) {
        if (currentMillis - previousMillis >= sampling * 1000) {
            previousMillis = currentMillis;
            int ultrasoundValue = distance;
            int lightValue = photocellValue;
            storeSensorReading(ultrasoundReadings, ultrasoundIndex, ultrasoundValue, 0);
            storeSensorReading(lightReadings, lightIndex, 0, lightValue);
            Serial.println("Ultrasound: " + String(ultrasoundValue) + ", Light: " + String(lightValue));
        }
        cancelPrintSensors(okSensorReadings);
    }

  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  duration = pulseIn(EchoPin, HIGH);
  distance = duration*0.034/2;

  photocellValue = analogRead(photocellPin);

  if(ok){
    if(Serial.available()){
      int choice = Serial.parseInt();
      if(choice!=0){
        ok=0;
      }
      chooseSubMenu(choice);
    }
  }
  
  if(ok1){
    if(Serial.available()){
      int choice1 = Serial.parseInt();
      chooseSubMenu1(choice1);
    }
  }
  if(ok2){
    if(Serial.available()){
      int choice2 = Serial.parseInt();
      chooseSubMenu2(choice2);
    }
  }
  if(ok3){
    if(Serial.available()){
      int choice3 = Serial.parseInt();
      chooseSubMenu3(choice3);
    }
  }
  if(ok4){
    if(Serial.available()){
      int choice4 = Serial.parseInt();
      chooseSubMenu4(choice4);
    }
  }
  if(okSampling){
    if(Serial.available()){
      sampling = Serial.parseInt();
      printSub1_1(sampling);
    }
  }
  if(okThreshold){
    printSub1_2();
    //ok = 1;
  }
  if(okLight){
    printSub1_3();
  }

  if (distance < threshold) {
    analogWrite(redLed, 255);
  }else if(distance >= threshold && threshold != 0){
    analogWrite(redLed, 0);
  }

  if (distance < threshold && !messagePrinted) {
    Serial.println("Too close!");
    messagePrinted = true;
  } else if (distance >= threshold) {
    messagePrinted = false;
  }

  if (photocellValue < lightthreshold) {
    analogWrite(blueLed, 255);
  }else if(photocellValue > lightthreshold && lightthreshold != 0){
    analogWrite(blueLed, 0);
  }

  if (photocellValue < lightthreshold && !nightMessagePrinted) {
    Serial.println("Night is coming!");
    nightMessagePrinted = true;
    dayMessagePrinted = false;
  } else if (photocellValue > lightthreshold && lightthreshold != 0 && !dayMessagePrinted) {
    Serial.println("Day is coming!");
    dayMessagePrinted = true;
    nightMessagePrinted = false;
  }

  // it is called non-stop and it turns the green light off when okToggle is 0
  toggleAuto(okToggle);
  
}

// ========== PRINT MENU AND SUBMENUS ========== //

void printMenu(){
  Serial.println("Select an option");
  Serial.println("1)Sensor Settings");
  Serial.println("2)Reset Logger Data");
  Serial.println("3)System Status");
  Serial.println("4)RGB LED Control");
}

void printSub1(){
  Serial.println("Select an option");
  Serial.println("1) Sensors Sampling Interval");
  Serial.println("2) Ultrasonic Alert Threshold");
  Serial.println("3) LDR Alert Threshold");
  Serial.println("4) Back");
  ok = 0;
}

void printSub2(){
  Serial.println("1) Yes");
  Serial.println("2) No");
  ok = 0;
}

void printSub3(){
  Serial.println("1) Current Sensor Readings");
  Serial.println("2) Current Sensor Settings");
  Serial.println("3) Display Logged Data");
  Serial.println("4) Back");
  ok = 0;
}

void printSub4(){
  Serial.println("1) Manual Color Control");
  Serial.println("2) LED: Toggle Automatic ON/OFF");
  Serial.println("3) Back");
  ok = 0;
}

// ========== CHOOSE MENUS OPTIONS ========== //

void chooseSubMenu(int option){
  switch(option){
    case 1:
      printSub1();
      ok1=1;
      break;
    case 2:
      Serial.println("Are you shure you want to delete all sensor settings?");
      printSub2();
      ok2=1;
      break;
    case 3:
      printSub3();
      ok3=1;
      break;
    case 4:
      printSub4();
      ok4=1;
      break;
    default:
      Serial.println("Invalid option. Please try again.");
      printMenu();
      ok = 1;
      break;
  }
}

void chooseSubMenu1(int option1){
  switch(option1){
    case 1:
      Serial.println("Choose a sampling rate for the sensors (1 - 10) : ");
      okSampling = 1;
      ok1=0;
      break;
    case 2:
      Serial.println("Minimum threshold to distance: ");
      okThreshold = 1;
      ok1=0;
      break;
    case 3:
      Serial.println("Minimum threshold for the light: ");
      okLight = 1;
      ok1=0;
      break;
    case 4:
      ok1=0;
      ok=1;
      printMenu();
      break;
    default:
      Serial.println("Invalid option. Please try again.");
      printSub1();
  }
}

void chooseSubMenu2(int option2){
  switch(option2){
    case 1:
      sampling = 0;
      threshold= 0;
      lightthreshold = 0;
      EEPROM.update(1, 0);
      EEPROM.update(3, 0);
      EEPROM.update(5, 0);
      ok2=0;
      ok=1;
      printMenu();
      break;
    case 2:
      ok2=0;
      ok=1;
      printMenu();
      break;
    default:
      Serial.println("Invalid option. Please try again.");
      printSub2();
  }
}

void chooseSubMenu3(int option3){
  switch(option3){
    case 1:
      Serial.println("Input '5' to serial to cancel sensor readings.");
      okSensorReadings=1;
      ok3=0;
      break;
    case 2:
      printSub3_2();
      ok3=0;
      break;
    case 3:
      Serial.println("Displaying logged data for Ultrasound Readings:");
      displayLast10Readings(ultrasoundReadings);
      Serial.println("Displaying logged data for Light Readings:");
      displayLast10Readings(lightReadings);
      ok3 = 0;
      ok=1;
      printMenu();
      break;
    case 4:
      ok3=0;
      ok=1;
      printMenu();
      break;
    default:
      Serial.println("Invalid option. Please try again.");
      printSub3();
  }
}

void chooseSubMenu4(int option4){
  switch(option4){
    case 1:
      printSub4_1();
      ok4=0;
      break;
    case 2:
      okToggle=!okToggle;
      toggleAuto(okToggle);
      ok4=0;
      ok=1;
      printMenu();
      break;
    case 3:
      ok4=0;
      ok=1;
      printMenu();
      break;
    default:
      Serial.println("Invalid option. Please try again.");
      printSub4();
  }
}

// ========== FUNCTIONS ========== //

void printSub1_1(int sample){
  if(sample< 0 || sample > 10){
    Serial.println("Invalid input. Choose a sampling rate for the sensors (1 - 10) : ");
  }else{
    EEPROM.update(1,sample);
    okSampling=0;
    ok = 1;
    printMenu();
  }
}

void printSub1_2(){
  if(Serial.available()){
    threshold = Serial.parseInt();
    EEPROM.update(3,threshold);
    okThreshold=0;
    ok = 1;
    printMenu();
  }
  
}

void printSub1_3(){
  if(Serial.available()){
    lightthreshold = Serial.parseInt();
    EEPROM.update(5,lightthreshold);
    okLight=0;
    ok = 1;
    printMenu();
  }
}


// parameter is basically the okSensorReadings that enters the condition in the loop to print at the sampling interval the sensor values
void cancelPrintSensors(int &parameter){
  int choiceCancel=0;
  if(Serial.available()){
    choiceCancel = Serial.parseInt();
  }
  if(choiceCancel ==5){
    parameter = 0;
    Serial.println("Sensor readings canceled.");
    ok=1;
    printMenu();
  }
}

void printSub3_2(){
  Serial.print("Sampling rate is: ");
  Serial.println(EEPROM.read(1));
  Serial.print("Ultrasonic sensor threshold is: ");
  Serial.println(EEPROM.read(3));
  Serial.print("LDR threshold is: ");
  Serial.println(EEPROM.read(5));
  delay(1000);
  ok=1;
  printMenu();
}

void printSub3_3(){
  
}

void storeSensorReading(SensorReading readings[], int& index, int ultraSoundValue, int lightValue) {
    readings[index].ultraSoundValue = ultraSoundValue;
    readings[index].lightValue = lightValue;

    // Increment the index, and wrap around if it exceeds the array size
    index = (index + 1) % 10;
}


void displayLast10Readings(const SensorReading readings[]) {
    Serial.println("Last 10 Sensor Readings:");

    for (int i = 0; i < 10; ++i) {
        int currentIndex = (ultrasoundIndex + i) % 10;
        Serial.print("Ultrasound Value: ");
        Serial.print(readings[currentIndex].ultraSoundValue);
        Serial.print(", Light Value: ");
        Serial.println(readings[currentIndex].lightValue);
    }
    Serial.println();
    ok = 1;
}

void printSub4_1(){
  Serial.println("Manual color control.");
  //not working
  ok=1;
  printMenu();
}

int toggleAuto(int okToggle){
  if(okToggle){
    analogWrite(greenLed, 255);
  }else{
    analogWrite(greenLed, 0);
  }

}