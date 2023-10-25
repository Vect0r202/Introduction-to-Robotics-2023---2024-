const int pin1 = A0, pin2 = A1, pin3 = A2;
const int led1 = 9, led2 = 10, led3 = 11;
float voltage1 = 0.0, voltage2 = 0.0, voltage3 = 0.0;
const float source = 5.0, um = 1023.0;
void setup() {
  Serial.begin(9600);
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {
  int value1 = analogRead(pin1)/source;
  int value2 = analogRead(pin2)/source;
  int value3 = analogRead(pin3)/source;
  // voltage1 = value1 * (source / um);
  // voltage2 = value2 * (source / um);
  // voltage3 = value3 * (source / um);
  // Serial.println(voltage1);
  // Serial.println(voltage2);
  // Serial.println(voltage3);
  analogWrite(led1, value1);
  analogWrite(led2, value2);
  analogWrite(led3, value3);
}
