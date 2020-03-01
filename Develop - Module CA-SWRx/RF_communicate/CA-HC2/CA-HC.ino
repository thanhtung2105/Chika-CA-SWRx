#include <RF24.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/* In this product - the address (channel) to communicate is define as SWR (convert to decimal) 
+ 'date make device' + 'product no.' ; In this case, SWR is known as '83 87 82' and add with the date 
making device for example today is Feb 17th and this is the first product in that day; then the address 
for this SWR is: const byte address[15] = "83878217022001"  ( 83 87 82 | 17 02 20 | 01 )          */

RF24 radio(2, 15);    //nRF24L01 (CE,CSN) connections PIN
const byte address[15] = "83878226022001";

/*
const byte address[15] = "**************";
const byte address[15] = "**************";
const byte address[15] = "**************";
*/

boolean stateButton[1];
boolean stateButton_MQTT[1];

const char *ssid = "Phong_510";
const char *password = "phong510@hcmus";

//Topic: product_id/button_id
const char *CA_SWR = "2a0a6b88-769e-4a63-ac5d-1392a7199e88/be47fa93-15df-44b6-bdba-c821a117cd41";

//Config MQTT broker information:
const char *mqtt_server = "chika.gq";
const int mqtt_port = 2502;
const char *mqtt_user = "chika";
const char *mqtt_pass = "2502";

void setup_Wifi()
{
  delay(100);
  Serial.println();
  Serial.print("Connecting to ... ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }
    Serial.println("\n");
    Serial.println("WiFi connected!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

//Setup MQTT - Wifi ESP12F:
WiFiClient esp_12F;
PubSubClient client(esp_12F);

void setup()
{
    SPI.begin();
    Serial.begin(115200);

    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.mode(WIFI_STA);

    setup_Wifi();
  
    radio.begin();
    radio.setRetries(15, 15);
    radio.setPALevel(RF24_PA_MAX);

    Serial.println("WIFI CONNECTED");
    Serial.println(WiFi.SSID());
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    Serial.println("\nCA-HC say hello to your home <3 !");
  
    Serial.println("Trying connect MQTT ...");
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    if(!client.connected()) {
    reconnect_mqtt();
  }
  else
  client.loop();
  }
  
  radio.openReadingPipe(1, address);
  /*
  radio.openReadingPipe(1, address);
  radio.openReadingPipe(2, address_2);
  radio.openReadingPipe(3, address_3);
  */
  radio.startListening();
  
  if (radio.available())
    {
      memset(&stateButton, ' ', sizeof(stateButton));
      radio.read(&stateButton, sizeof(stateButton));
      stateButton_MQTT[0] = stateButton[0];
      stateButton_MQTT[1] = stateButton[1];

       if(stateButton[0])
            client.publish(CA_SWR,"1");
       else
            client.publish(CA_SWR,"0");
       
//       if(stateButton[1])
//            client.publish(CA_SWR_2,"1");
//       else
//            client.publish(CA_SWR_2,"0");
    }
  delay(100);
}

void reconnect_mqtt()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
        String clientId = "CA-SWR - ";
        clientId += String(random(0xffff), HEX);
        Serial.println(clientId);

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
        {
            Serial.println("Connected");
            client.subscribe(CA_SWR);
//            client.subscribe(CA_SWR_2);
        }
        else
        {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println("Try again in 3 seconds");
            delay(3000);
        }
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  //Topic list test is the value of variables: CA_SWR
  Serial.print("Topic [");
  Serial.print(topic);
  Serial.print("]: ");
  //Print message of button ID:
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  //Differenate the button ID: 1 - swr/01 & 2 - swr/02
  if ((char)topic[5] == 'f')
    switch ((char)payload[0])
    {
      case '1':
      radio.stopListening();
      stateButton_MQTT[0] = 1;
      
      radio.openWritingPipe(address);
      radio.write(&stateButton_MQTT, sizeof(stateButton_MQTT));
      break;
      case '0':
      radio.stopListening();
      stateButton_MQTT[0] = 0;
      
      radio.openWritingPipe(address);
      radio.write(&stateButton_MQTT, sizeof(stateButton_MQTT));
      break;
    }
   
//  else if ((char)topic[5] == '2')
//    switch ((char)payload[0])
//    {
//      case '1':
//      radio.stopListening();
//      stateButton_MQTT[1] = 1;
//      
//      radio.openWritingPipe(address);
//      radio.write(&stateButton_MQTT, sizeof(stateButton_MQTT));
//      break;
//      case '0':
//      radio.stopListening();
//      stateButton_MQTT[1] = 0;
//      
//      radio.openWritingPipe(address);
//      radio.write(&stateButton_MQTT, sizeof(stateButton_MQTT));
//      break;
//    } 
}
