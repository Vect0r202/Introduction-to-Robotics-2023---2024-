const int buttonPin1 = 2, buttonPin2 = 3, buttonPin3 = 4;
const int ledPin1 = 13, ledPin2 = 12, ledPin3 = 11, elPin = 10;

byte buttonState1 = LOW, buttonState2 = LOW, buttonState3 = LOW, ok = LOW, ok1 = LOW, ok2 = LOW, ok3 = LOW;
byte ledState1 = HIGH, ledState2 = LOW, ledState3 = LOW, elState = LOW;

byte reading1 = LOW, reading2 = LOW, reading3 = LOW;
// byte lastReading1 = LOW, lastReading2 = LOW, lastReading3 = LOW;

unsigned int lastDebounceTime;
unsigned int debounceDelay = 1000;

const unsigned long interval = 200;
unsigned long previousMillis = 0, currentDebouce = 0;

void setup() {
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(elPin, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis(), currentDebounce = millis();
  reading1 = digitalRead(buttonPin1);
  reading2 = digitalRead(buttonPin2);
  reading3 = digitalRead(buttonPin3);
  
  // 1 - 3

  if(!ok){
    digitalWrite(ledPin1, ledState1);
  }

  if(reading3){
    ok = HIGH;
  }
  if(ok){
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      elState = (elState == LOW) ? HIGH : LOW;
      digitalWrite(elPin, elState);
    }
  }
  if(ok){
    if(ledState1){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        lastDebounceTime = currentDebounce;
      }
    }
    digitalWrite(ledPin1, ledState1);
    delay(10);
    if(ledState1){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        lastDebounceTime = currentDebounce;
        ledState1 = !ledState1;
      }
    }
    digitalWrite(ledPin1, ledState1);
    delay(10);
    if(!ledState1){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        lastDebounceTime = currentDebounce;
        ledState2 = !ledState2;
      }
    }
    digitalWrite(ledPin2, ledState2);
    delay(10);
    if(!ledState2){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        ledState3 = !ledState3;
      }
    }
    digitalWrite(ledPin3, ledState3);
    if(ledState3){
      ok = LOW;
      elState = LOW;
      digitalWrite(elPin, elState);
    }
  }

  // 3 - 1

  if(!ok1){
    digitalWrite(ledPin3, ledState3);
  }

  if(reading1){
    ok1 = HIGH;
  }
  if(ok1){
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      elState = (elState == LOW) ? HIGH : LOW;
      digitalWrite(elPin, elState);
    }
  }
  if(ok1){
    if(ledState3){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        lastDebounceTime = currentDebounce;
      }
    }
    digitalWrite(ledPin3, ledState3);
    delay(10);
    if(ledState3){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        lastDebounceTime = currentDebounce;
        ledState3 = !ledState3;
      }
    }
    digitalWrite(ledPin3, ledState3);
    delay(10);
    if(!ledState3){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        lastDebounceTime = currentDebounce;
        ledState2 = !ledState2;
      }
    }
    digitalWrite(ledPin2, ledState2);
    delay(10);
    if(!ledState2){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        ledState1 = !ledState1;
      }
    }
    digitalWrite(ledPin1, ledState1);
    if(ledState1){
      ok1 = LOW;
      elState = LOW;
      digitalWrite(elPin, elState);
    }
  }

  // 1 - 2

  if(reading2 && ledState1){
    ok2 = HIGH;
  }

  if(reading2 && ledState3){
    ok3 = HIGH;
  }

  if(ok2){
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      elState = (elState == LOW) ? HIGH : LOW;
      digitalWrite(elPin, elState);
    }
  }
  if(ok2){
    if(ledState1){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        lastDebounceTime = currentDebounce;
      }
    }
    digitalWrite(ledPin1, ledState1);
    delay(10);
    if(ledState1){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        lastDebounceTime = currentDebounce;
        ledState1 = !ledState1;
      }
    }
    digitalWrite(ledPin1, ledState1);
    delay(10);
    if(!ledState1){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        lastDebounceTime = currentDebounce;
        ledState2 = !ledState2;
      }
    }
    digitalWrite(ledPin2, ledState2);
  }
  if(ok3){
    if(ledState3){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        lastDebounceTime = currentDebounce;
      }
    }
    digitalWrite(ledPin3, ledState3);
    delay(10);
    if(ledState3){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        lastDebounceTime = currentDebounce;
        ledState3 = !ledState3;
      }
    }
    digitalWrite(ledPin3, ledState3);
    delay(10);
    if(!ledState3){
      currentDebounce = millis();
      if((currentDebounce - lastDebounceTime) >= debounceDelay){
        lastDebounceTime = currentDebounce;
        ledState2 = !ledState2;
      }
    }
    digitalWrite(ledPin2, ledState2);
  }
  
}