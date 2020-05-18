/*
  Product code: 2a0a6b88-769e-4a63-ac5d-1392a7199e88
  RF channel (1 button): 83878226022001
  Button topic: be47fa93-15df-44b6-bdba-c821a117cd41
*/

/* In this product - the address (channel) to communicate is define as <the HC code> (3 degits)
+ <company code> (7 degits) + <product code> (3 degits). So we have the following list product code:      
CA-SWR: 1002502019001 (13)
CA-SWR2: 1002502019002 (13)
CA-SWR3: 1002502019003 (13)
*/

#include <Arduino.h>
#include <EEPROM.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(9, 10);               //nRF24L01 (CE,CSN) connections PIN
const uint64_t address = 1002502019001;      //Changeable

const int button = 5;
const int control = 2;
const int led_state = A2;

int deviceState;

float state_Device_sendtoHC[1];
float state_Device_controlfromHC[1];

void checkDeviceState()
{
  deviceState = EEPROM.read(0);
  if (deviceState == 1)
  {
    digitalWrite(control, HIGH);
    digitalWrite(led_state, HIGH);
  }
  if (deviceState == 0)
  {
    digitalWrite(control, LOW);
    digitalWrite(led_state, LOW);
  }
}

int isButton_Click(int GPIO_to_read)
{
  int out = 0;
  while (digitalRead(GPIO_to_read) == 1)
  {
    delay(50);
    out = 1;
  }
  return out;
}

void setup()
{
  SPI.begin();
  Serial.begin(9600);
  Serial.println("\nCA-SWR say hello to your home <3 ! ");
  pinMode(button, INPUT);
  pinMode(control, OUTPUT);
  pinMode(led_state, OUTPUT);

  checkDeviceState();
  radio.begin();
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MAX);
}

void loop()
{
  radio.openReadingPipe(1, address);
  radio.startListening();

  if (radio.available())
  {
    memset(&state_Device_controlfromHC, ' ', sizeof(state_Device_controlfromHC));
    radio.read(&state_Device_controlfromHC, sizeof(state_Device_controlfromHC));
    Serial.println("Receive: ");
    Serial.println(state_Device_controlfromHC[0]);
    digitalWrite(control, (boolean)state_Device_controlfromHC[0]);
    digitalWrite(led_state, (boolean)state_Device_controlfromHC[0]);
    deviceState = (boolean)state_Device_controlfromHC[0];
  }

  boolean check_Button = isButton_Click(button);

  if (check_Button)
  {
    radio.stopListening();
    deviceState = !deviceState;
    digitalWrite(control, deviceState);
    digitalWrite(led_state, deviceState);
    state_Device_sendtoHC[0] = (boolean)deviceState;
    EEPROM.update(0, deviceState);

    radio.openWritingPipe(address);
    radio.write(&state_Device_sendtoHC[0], sizeof(state_Device_sendtoHC));
  }
}
