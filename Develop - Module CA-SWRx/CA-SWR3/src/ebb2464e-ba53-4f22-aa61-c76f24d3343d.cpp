/* In this product - the address (channel) to communicate is define as <the HC code> (3 degits)
+ <company code> (7 degits) + <product code> (3 degits). So we have the following list product code:      
CA-SWR: 1002502019001 (13)
CA-SWR2: 1002502019002 (13)
CA-SWR3: 1002502019003 (13)

	Product code: ebb2464e-ba53-4f22-aa61-c76f24d3343d
	RF channel (3 button): 1002502019003
	Button topic: 5faf98dd-9aa4-4a02-b0dc-344d5c6304fe
                9554cca1-0133-4682-81f9-acc8bcb40121
                7b777605-1ea2-4878-9194-1b1e72edcb98
*/

#include <Arduino.h>
#include <EEPROM.h>
#include <RF24.h>
#include <SPI.h>
using namespace std;

int CE = 9;
int CSN = 10;
RF24 radio(CE, CSN);                         //nRF24L01 (CE,CSN) connections PIN
const uint64_t address = 1002502019003;       //Changeable

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