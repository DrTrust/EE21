#define POTMETER_PIN 1
#define LDR_PIN 0
#define LED_PIN 3

void setup()
{

	pinMode(LED_PIN, OUTPUT);
	pinMode(POTMETER_PIN, INPUT);
	pinMode(LDR_PIN, INPUT);

  /* add setup code here */

	Serial.begin(9600);
	Serial.write("Communication started\n");
}

void loop()
{
	static int potmeter_previousValue;
	static int LDR_previousValue;

	int potmeter_Value = analogRead(POTMETER_PIN);
	int LDR_Value = analogRead(LDR_PIN);
	
	//calculate update
	if (abs(potmeter_Value-potmeter_previousValue)>5)
	{
		Serial.write("potmeter verandering (");
		
		char waarde[33];

		itoa(potmeter_Value, waarde, 10);

		Serial.write(waarde);
		Serial.write(")!\n");

		onPotmeterUpdate(potmeter_Value);

		potmeter_previousValue = potmeter_Value;
	}

	//calculate update
	if (abs(LDR_Value-LDR_previousValue)>7)
	{
		Serial.write("LDR verandering (");
		char waarde[33];

		itoa(LDR_Value, waarde, 10);

		Serial.write(waarde);
		Serial.write(")!\n");

		onLDRUpdate(LDR_Value);

		LDR_previousValue = LDR_Value;
	}
}

void onPotmeterUpdate(int value)
{

}

void onLDRUpdate(int value)
{

}
