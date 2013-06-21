/* ===============================================================================
 * Sample code for Weather-Underground parser of single entry in the JSON
 * reply
 * ===============================================================================
 * By: Zakie Mashiah		zmashiah@gmail.com			June-15-2013
 * You may use, modify, publish this code for any type of use provided that
 * you keep a visible indication of the author
 * ===============================================================================
*/

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

#include "WUStreamParser.h"
#include "netinit.h"

WUStreamParser wuParser;
const char *wuDataNameVect[] = { "forecast", "simpleforecast", "forecastday", "qpf_allday", "in", NULL };
const char wuApiKey[] = "9d9ca34336e25c7c";


void (*restart)(void) = 0; // If we need to restart ourselves that's the way

void setup()
{
	Serial.begin(9600);
    Serial.println();
	Serial.println("Starting...");

	initNetwork();

    wuParser.init(0, "FL/Tampa", wuApiKey, wuDataNameVect); // Debug (0=off, 1=on), Tampa Florida, my API key and the search vector
}

void loop()
{
	static char counter = 0;

	Serial.println("In loop");
	Ethernet.maintain();
	const char *qpf = wuParser.get();
	if (qpf == NULL)
		Serial.println("\tError reading QPF");
	else
	{
		Serial.print("\tQPF=");
		Serial.println(qpf);
	}

	Serial.println("Sleeping");
	for (int i=0; i < 10; i++)
	{
		delay(1000 * 30);
                timeleft(9-i, 30);
                delay(1000 * 30);
                timeleft(9-i, 0);
	}
	Serial.println("Done sleeping");
}

void timeleft(int m, int s)
{
  Serial.print("\tWaking up in "); Serial.print(m);
  if (s)
  {
    Serial.print(':'); Serial.print(s);
  }
  else
    Serial.print(":00");
  Serial.println(" min");
}
/*
// Test program to use on PC, put the JSON output of WU into a file and this programm will parse it
int _tmain(int argc, _TCHAR* argv[])
{
	FILE *fp;
	JsonStreamParser jsp;
	const char *dataNameVect[] = { "forecast", "simpleforecast", "forecastday", "qpf_allday", "in", NULL };

	fopen_s(&fp, "D:\\Temp\\jsonsample.txt", "rt");
	jsp.init("", "", dataNameVect);
	if (fp != NULL)
	{
		char line[1024];
		char *ptr;
		size_t linelen;

		do
		{
			ptr = fgets(line, sizeof(line), fp);
			if (ptr != NULL)
			{
				const char *value = jsp.parse(ptr);
				if (value != NULL)
				{
					printf("Found value %s\n", value);
					return 0;
				}
			}
		} while (!feof(fp));
	return -1;
}
#endif
*/

