#include <RF24.h>
#include <printf.h>
#include <SPI.h>

int CE = 9;
int CSN = 10;
RF24 radio(CE, CSN);
const byte data_pipe[6] = "54321";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  printf_begin();
  pinMode(CSN, OUTPUT);
  radio.begin();
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, data_pipe);
  radio.startListening();
}

void loop() {
  if (radio.available())
  {
    int payload_size = radio.getDynamicPayloadSize();
    if (payload_size > 1)
    {
      char* payload = new char[payload_size + 1];
      radio.read(payload, payload_size);
      payload[payload_size] = '\0';
      printf("Got Message: %s\r\n", payload);
    }
  }
}
