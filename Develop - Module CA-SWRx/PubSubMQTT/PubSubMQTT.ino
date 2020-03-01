#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "Phong_510";
const char *password = "phong510@hcmus";

const char *topic = "swr/02";

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
		delay(500);
		Serial.print(".");
	}
	  Serial.println("");
    Serial.println("WiFi connected!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

WiFiClient esp_12F;
PubSubClient client(esp_12F);

void setup()
{
	Serial.begin(115200);
	Serial.println("\n\n_ CA-SW2 say hello to your home _");

  setup_Wifi();
  
	Serial.println("WIFI CONNECTED");
	Serial.println(WiFi.SSID());
	Serial.print("IP: ");
	Serial.println(WiFi.localIP());
 	
	Serial.println("Trying connect MQTT ...");
	client.setServer(mqtt_server, mqtt_port);
	client.setCallback(callback);
}

//------------- MAIN LOOP -------------
void loop()
{ 
	if (WiFi.status() == WL_CONNECTED)
	{
	  if(!client.connected())	{
		reconnect_mqtt();
	}
	else
	client.loop();
	}

	client.publish(topic, "{\"protocol\":\"SONY\",\"bit\":12,\"size\":103,\"value\":2704,\"state\":[144,10,0,0,0,0,0,0,1,0,0,0,21,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12]}");
			
	delay(2100);
}

//------------- OTHER FUNCTIONS -------------

//Callback:
void callback(char *topic, byte *payload, unsigned int length)
{
	//Topic list test is the value of variables: CA_SW2_1 and CA_SW2_2
	Serial.print("Topic [");
	Serial.print(topic);
	Serial.print("]");
	//Print message of button ID:
	for (int i = 0; i < length; i++)
	{
		Serial.print((char)payload[i]);
	}
	Serial.println();
}

void reconnect_mqtt()
{
	boolean check_Button_1;
	boolean check_Button_2;
	while (!client.connected())
	{
		Serial.print("Attempting MQTT connection...");
        String clientId = "CA-SW2 - ";
        clientId += String(random(0xffff), HEX);
        Serial.println(clientId);

		if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
        {
            Serial.println("Connected");
            client.subscribe(topic);
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
