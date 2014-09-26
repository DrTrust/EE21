#include "Bounce2\Bounce2.h"


#define BUTTON_PIN1 8
#define BUTTON_PIN2 9
#define LED_PIN 3



// Instantiate a Bounce object
Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();

static String input = "";
int operator_type = 1;

int previousstate = 0;

void setup() {
	// Setup the button
	pinMode(BUTTON_PIN1, INPUT);
	// Activate internal pull-up
	digitalWrite(BUTTON_PIN1, HIGH);

	pinMode(BUTTON_PIN2, INPUT);
	// Activate internal pull-up
	digitalWrite(BUTTON_PIN2, HIGH);

	// After setting up the button, setup debouncer
	debouncer1.attach(BUTTON_PIN1);
	debouncer1.interval(5);

	debouncer2.attach(BUTTON_PIN2);
	debouncer2.interval(5);

	//Setup the LED
	pinMode(LED_PIN, OUTPUT);

	Serial.begin(9600);

	Serial.write("Serial communication started!\n");

}

void loop() {
	// On update bouncer value!!
	GetSerialInput();


	//Check for update on any button
	//Only on every 'true/high' state change should trigger an inversal of LED signal
	if (debouncer1.update() || debouncer2.update())
	{
		static boolean oldreadstate_1 = true;
		static boolean oldreadstate_2 = true;

		//write new state to LED, INVERT state!

		boolean readstate_1 = !debouncer1.read();
		boolean readstate_2 = !debouncer2.read();

		switch (operator_type)
		{
			case 1://OR
			{
					   if ((readstate_1 == true && (!oldreadstate_1)) || (readstate_2 == true && (!oldreadstate_2)))
						   previousstate = !previousstate;
					   break;
			}
			case 2://XOR
			{
					   if ((readstate_1 == true && (!oldreadstate_1)) || (readstate_2 == true && (!oldreadstate_2))&&!(readstate_1 && readstate_2))
						   previousstate = !previousstate;
					   break;
			}
			case 3://AND
			{
					   if (readstate_1 && readstate_2)
						   previousstate = !previousstate;
					   break;
			}
		}

		oldreadstate_1 = readstate_1;
		oldreadstate_2 = readstate_2;

		// update the LED
		digitalWrite(LED_PIN, previousstate);
	}
}

void GetSerialInput()
{
	if (Serial.available())
	{
		char inputbyte = (char) Serial.read();

		//If not-static
		if ((int)inputbyte > 31)
		{
			static bool mayRead = false;

			//add to input
			switch (inputbyte)
			{
				case '{':
				{
						mayRead = true;
						break;
				}
				case '}':
				{
						mayRead = false;
						break;
				}
				default:
				{
					if (mayRead)
					{
						validateCommand(inputbyte);

						//input += inputbyte;

						//validateCommand(inputbyte);
					}
				}
			}
		}
	}
}

void validateCommand(char inputString)
{
	switch (inputString)
	{
		case 'o':
		{
					Serial.write("OR operator selected\n");
				operator_type = 1;
				break;
		}
		case 'x':
		{
					Serial.write("XOR operator selected\n");
				operator_type = 2;
				break;
		}
		case 'a':
		{
					Serial.write("AND operator selected\n");
				operator_type = 3;
				break;
		}
	}
}