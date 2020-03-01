#include <RF24.h>
#include <SPI.h>

RF24 radio(2, 15);
const byte data_pipe[15] = "83878226022001";
boolean stateButton_MQTT[1];

void setup() {
  Serial.begin(115200);
  SPI.begin();
  radio.begin();
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(data_pipe);
}

void loop() {
  while(true)
{
  stateButton_MQTT[0] = 1;
  radio.write(&stateButton_MQTT, sizeof(stateButton_MQTT));

  delay(2000);

  stateButton_MQTT[0] = 0;
  radio.write(&stateButton_MQTT, sizeof(stateButton_MQTT));
}
}
