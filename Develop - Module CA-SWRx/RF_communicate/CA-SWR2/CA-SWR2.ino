#include <RF24.h>
#include <SPI.h>

/* In this product - the address (channel) to communicate is define as SWR (convert to decimal) 
+ 'date make device' + 'product no.' ; In this case, SWR is known as '83 87 82' and add with the date 
making device for example today is Feb 17th and this is the first product in that day; then the address 
for this SWR is: const byte address[15] = "83878217022001"	( 83 87 82 | 17 02 20 | 01 )					*/

RF24 radio(9, 10);   //nRF24L01 (CE,CSN) connections PIN
const byte address[15] = "83878226022001";    //Changeable

const int button_1 = 5;
const int button_2 = 6;

const int control_1 = 2;
const int control_2 = 3;

boolean stateDEVICE_control_1 = false;
boolean stateDEVICE_control_2 = false;

boolean state_Button_send[2];
boolean state_Button_control[2];

void setup() {
  SPI.begin();
  Serial.begin(9600);
  Serial.println("\nCA-SWR say hello to your home <3 ! ");
  pinMode(button_1, INPUT);
  pinMode(button_2, INPUT);
  pinMode(control_1, OUTPUT);
  pinMode(control_2, OUTPUT);
  digitalWrite(control_1, LOW);
  digitalWrite(control_2, LOW);
  
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
   memset(&state_Button_control, ' ', sizeof(state_Button_control));
	 radio.read(&state_Button_control, sizeof(state_Button_control));
   digitalWrite(control_1, state_Button_control[0]);
   stateDEVICE_control_1 = state_Button_control[0];
   digitalWrite(control_2, state_Button_control[1]);
   stateDEVICE_control_2 = state_Button_control[1];
  }
  
  boolean check_Button_1 = isButton_Click(button_1);
  boolean check_Button_2 = isButton_Click(button_2);
  
  if (check_Button_1)
	{
		radio.stopListening();
		stateDEVICE_control_1 = !stateDEVICE_control_1;
		digitalWrite(control_1, stateDEVICE_control_1);		
		state_Button_send[0] = stateDEVICE_control_1;
		state_Button_send[1] = stateDEVICE_control_2;
		
		radio.openWritingPipe(address);
		radio.write(&state_Button_send, sizeof(state_Button_send));
	}
	
	if (check_Button_2)
	{
    radio.stopListening();
		stateDEVICE_control_2 = !stateDEVICE_control_2;
		digitalWrite(control_2, stateDEVICE_control_2);		
		state_Button_send[0] = stateDEVICE_control_1;
		state_Button_send[1] = stateDEVICE_control_2;
		
		radio.openWritingPipe(address);
		radio.write(&state_Button_send, sizeof(state_Button_send));
	}
 
  delay(100);
}

int isButton_Click(int GPIO_to_read)
{
    int out = 0;
    while (digitalRead(GPIO_to_read) == 1)
    {
        delay(20);
        out = 1;
    }
    return out;
}
