int out = 7;
void setup()
{
	pinMode(out, OUTPUT);
}
void loop()
{
	digitalWrite(out,HIGH);
	delay(1000);
	digitalWrite(out,LOW);
	delay(1000);
}
