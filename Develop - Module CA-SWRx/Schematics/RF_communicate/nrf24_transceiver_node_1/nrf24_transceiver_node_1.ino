#include <SPI.h>
#include <RF24.h>
#include <printf.h>

//Node 1 này mình dùng Arduino
const int CE = 9;
const int CSN = 10;
RF24 radio(CE, CSN);   //nRF24L01 (CE,CSN)
const byte address[9] = "29022020";

const int click_to_send_msg = 5;  //Bấm nút để gửi thông điệm, nút bấm kết nối vào PIN 5
const int control = A2;
char message[] = "Arduino: Hello node 2, I am node 1";

void setup() {
  SPI.begin();
  printf_begin();
  Serial.begin(9600);
  pinMode(click_to_send_msg, INPUT);
  pinMode(control, OUTPUT);
  
  radio.begin();
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MAX);
}

void loop()
{
  radio.openReadingPipe(1, address);
  radio.startListening();           // Ban đầu, để cho nrf ở chế độ luôn lắng nghe sự kiện

  if (radio.available())
  {
    int get_payloadSize = radio.getDynamicPayloadSize();
    if (get_payloadSize > 1)
    {
      char* msg_receive = new char[get_payloadSize + 1];
      radio.read(msg_receive, get_payloadSize);
      msg_receive[get_payloadSize] = '\0';
      printf("%s\n", msg_receive);
      digitalWrite(control,HIGH);
    }
  }
  else
  digitalWrite(control,LOW);

  bool check_button = isButton_Click(click_to_send_msg);
  if(check_button)
  {
    Serial.println("Sending ...");
    radio.stopListening();
    radio.openWritingPipe(address);
    radio.write(message, strlen(message));
  }
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
