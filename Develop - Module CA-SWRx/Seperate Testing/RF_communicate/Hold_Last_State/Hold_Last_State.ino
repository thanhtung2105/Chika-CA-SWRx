#include <EEPROM.h>

const int buttonPin = 7;    
const int ledPin = 4;       

int ledState;                
int buttonState;             
int lastButtonState = LOW;   

long lastDebounceTime = 0;  
long debounceDelay = 50;    

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, ledState);
  Serial.begin (9600);
  checkLedState(); 
}

void loop() {
  int reading = digitalRead(buttonPin);

  if(reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if((millis() - lastDebounceTime) > debounceDelay) {
    
    if(reading != buttonState) {
      buttonState = reading;

      if(buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  digitalWrite(ledPin, ledState);
  EEPROM.update(0, ledState);
 
  lastButtonState = reading;
}


void checkLedState() {
   ledState = EEPROM.read(0);
   if(ledState == 1) {
    digitalWrite(ledPin, HIGH);
   } 
   if(ledState == 0) {
    digitalWrite(ledPin, LOW);
   }
}
