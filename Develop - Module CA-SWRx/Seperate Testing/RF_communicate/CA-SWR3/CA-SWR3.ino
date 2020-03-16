/* In this product - the address (channel) to communicate is define as <the code of product> (2 degits)
+ <timestamp_of_production> (10 degits). With list product code:
- CA-SWR: 10;
- CA-SWR2: 20;
- CA-SWR3: 30;
And the timestamp when we create the product, so we have this list:        
CA-SWR: 101584324363 (12)
CA-SWR2: 201584324393 (12)
CA-SWR3: 301584324410 (12)

  Product code: 740a8d1e-c649-475e-a270-c5d9a44b40a8
  RF channel (3 button): 83878226022002
  Button topic: 774f2306-51ad-4bf1-ba9e-0ddee9bd2375
                5124ba3a-7a45-472b-8468-6f2a041733ac
                a0087ff7-3613-442f-b6c5-0d5d2f0f1a30
*/

#include <Arduino.h>
#include <EEPROM.h>
#include <RF24.h>
#include <SPI.h>
using namespace std;

int CE = 9;
int CSN = 10;
RF24 radio(CE, CSN);                         //nRF24L01 (CE,CSN) connections PIN
const uint64_t address = 301584324410;       //Changeable

const int button_1 = 5;
const int button_2 = 6;
const int button_3 = 7;
const int control_1 = 2;
const int control_2 = 3;
const int control_3 = 4;
const int led_state_1 = A3;
const int led_state_2 = A2;
const int led_state_3 = A1;

int deviceState_1, deviceState_2, deviceState_3;

boolean state_Device_sendtoHC[3];
boolean state_Device_controlfromHC[3];

void checkDevicesState()
{
  deviceState_1 = EEPROM.read(0);
  deviceState_2 = EEPROM.read(1);
  deviceState_3 = EEPROM.read(2);

  if (deviceState_1)
  {
    digitalWrite(control_1, HIGH);
    digitalWrite(led_state_1, HIGH);
  }
  else
  {
    digitalWrite(control_1, LOW);
    digitalWrite(led_state_1, LOW);
  }

  if (deviceState_2)
  {
    digitalWrite(control_2, HIGH);
    digitalWrite(led_state_2, HIGH);
  }
  else
  {
    digitalWrite(control_2, LOW);
    digitalWrite(led_state_2, LOW);
  }

  if (deviceState_3)
  {
    digitalWrite(control_3, HIGH);
    digitalWrite(led_state_3, HIGH);
  }
  else
  {
    digitalWrite(control_3, LOW);
    digitalWrite(led_state_3, LOW);
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
  Serial.println("\nCA-SWR3 say hello to your home <3 ! ");
  pinMode(button_1, INPUT);
  pinMode(button_2, INPUT);
  pinMode(button_3, INPUT);
  pinMode(control_1, OUTPUT);
  pinMode(control_2, OUTPUT);
  pinMode(control_3, OUTPUT);
  pinMode(led_state_1, OUTPUT);
  pinMode(led_state_2, OUTPUT);
  pinMode(led_state_3, OUTPUT);
  pinMode(CSN, OUTPUT);

  checkDevicesState();
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
  
    digitalWrite(control_1, state_Device_controlfromHC[0]);
    digitalWrite(led_state_1, state_Device_controlfromHC[0]);
    deviceState_1 = state_Device_controlfromHC[0];

    digitalWrite(control_2, state_Device_controlfromHC[1]);
    digitalWrite(led_state_2, state_Device_controlfromHC[1]);
    deviceState_2 = state_Device_controlfromHC[1];

    digitalWrite(control_3, state_Device_controlfromHC[2]);
    digitalWrite(led_state_3, state_Device_controlfromHC[2]);
    deviceState_3 = state_Device_controlfromHC[2];
  }

  boolean check_Button_1 = isButton_Click(button_1);
  boolean check_Button_2 = isButton_Click(button_2);
  boolean check_Button_3 = isButton_Click(button_3);

  if (check_Button_1)
  {
    radio.stopListening();
    deviceState_1 = !deviceState_1;
    digitalWrite(control_1, deviceState_1);
    digitalWrite(led_state_1, deviceState_1);
    state_Device_sendtoHC[0] = deviceState_1;
    state_Device_sendtoHC[1] = deviceState_2;
    state_Device_sendtoHC[2] = deviceState_3;
    EEPROM.update(0, deviceState_1);

    radio.openWritingPipe(address);
    radio.write(&state_Device_sendtoHC, sizeof(state_Device_sendtoHC));
  }

  if (check_Button_2)
  {
    radio.stopListening();
    deviceState_2 = !deviceState_2;
    digitalWrite(control_2, deviceState_2);
    digitalWrite(led_state_2, deviceState_2);
    state_Device_sendtoHC[0] = deviceState_1;
    state_Device_sendtoHC[1] = deviceState_2;
    state_Device_sendtoHC[2] = deviceState_3;
    EEPROM.update(1, deviceState_2);

    radio.openWritingPipe(address);
    radio.write(&state_Device_sendtoHC, sizeof(state_Device_sendtoHC));
  }

  if (check_Button_3)
  {
    radio.stopListening();
    deviceState_3 = !deviceState_3;
    digitalWrite(control_3, deviceState_3);
    digitalWrite(led_state_3, deviceState_3);
    state_Device_sendtoHC[0] = deviceState_1;
    state_Device_sendtoHC[1] = deviceState_2;
    state_Device_sendtoHC[2] = deviceState_3;
    EEPROM.update(2, deviceState_3);

    radio.openWritingPipe(address);
    radio.write(&state_Device_sendtoHC, sizeof(state_Device_sendtoHC));
  }
}
