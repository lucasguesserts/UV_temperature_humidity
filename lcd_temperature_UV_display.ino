#include <DHT.h>
#include <LiquidCrystal.h>

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define DHT_PIN A0
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

int UV_OUT = A4; //Output from the sensor
int UV_REF_3V3 = A5; //3.3V power on the Arduino board
int averageAnalogRead(int pinToRead);
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);

void setup() {
	/* Serial to debug */
	/*Serial.begin(9600);*/
	/* UV sensor setup */
	pinMode(UV_OUT, INPUT);
	pinMode(UV_REF_3V3, INPUT);
	/* LCD sensor setup */
	lcd.begin(16, 2);
	lcd.noCursor();
}

void loop() {
	/* DTH11 measurement */
	float humity = dht.readHumidity();
	float temperature = dht.readTemperature();

	/* UV measurement */
	int uvLevel = analogRead(UV_OUT);
	int refLevel = analogRead(UV_REF_3V3);
	float outputVoltage = 3.3 / refLevel * uvLevel; /* Use the 3.3V power pin as a reference to get a very accurate output value from sensor */
	float uvIntensity = mapfloat(outputVoltage, 0, 5, 0.0, 15.0); /* Convert the voltage to a UV intensity level */

	/* display */
	/* Temperature */
	lcd.setCursor(0, 0);
	lcd.print("T = "); lcd.print((int)temperature, 10); lcd.print("C");
	/* Humity */
	lcd.setCursor(8, 0);
	lcd.print("H = "); lcd.print((int)humity, 10); lcd.print("%");
	/* UV */
	lcd.setCursor(0, 1);
	lcd.print("UV = "); lcd.print(uvIntensity, 1); lcd.print(" ");
	if(uvIntensity < 0) lcd.print("ERR");
		else if(uvIntensity<3)  lcd.print("Low");
		else if(uvIntensity<6)  lcd.print("Med");
		else if(uvIntensity<8)  lcd.print("High");
		else if(uvIntensity<11) lcd.print("Very H");
		else if(uvIntensity>11) lcd.print("Extreme");

	/* Debug display */
	/*Serial.print("uvLevel: ");*/
	/*Serial.print(uvLevel);*/
	/*Serial.print("\n");*/
	/*Serial.print("refLevel: ");*/
	/*Serial.print(refLevel);*/
	/*Serial.print("\n");*/
	/*Serial.print("outputVoltage: ");*/
	/*Serial.print(outputVoltage);*/
	/*Serial.print("\n");*/
	/*Serial.print("uvIntensity: ");*/
	/*Serial.print(uvIntensity);*/
	/*Serial.print("\n");*/
	/*Serial.print("\n");*/

	delay(1500);
}

int averageAnalogRead(int pinToRead)
{
	byte numberOfReadings = 8;
	unsigned int runningValue = 0; 
	for(int x = 0 ; x < numberOfReadings ; x++)
		runningValue += analogRead(pinToRead);
	runningValue /= numberOfReadings;
	return runningValue;  
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
	return ((x - in_min) / (in_max - in_min)) * (out_max - out_min) + out_min;
}

