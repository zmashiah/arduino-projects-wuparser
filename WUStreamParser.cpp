/* ===============================================================================
 *	Parser for Weather-Underground single entry of the JSON reply
 * ===============================================================================
 * By: Zakie Mashiah		zmashiah@gmail.com			June-15-2013
 * You may use, modify, publish this code for any type of use provided that
 * you keep a visible indication of the author
 * ===============================================================================
*/

#include <Arduino.h>
#include <Ethernet.h>

#include "JsonStreamParser.h"
#include "WUStreamParser.h"



void WUStreamParser::init(bool debugPrint, const char *location, const char *apiKey, const char *searchParts[])
{
	m_location = location;
	m_apiKey   = apiKey;
	m_console = debugPrint;
	JsonStreamParser::init(searchParts);
}


const char *WUStreamParser::getServerName()
{
	static const char server[] = "api.wunderground.com";
	return server;
}


void WUStreamParser::print(const char *str)
{
	if (m_console)
		Serial.print(str);
}

void WUStreamParser::print(int d)
{
  if (m_console)
    Serial.print(d);
}


void WUStreamParser::println(const char *str)
{
	if (m_console)
		Serial.println(str);
}

int WUStreamParser::connect()
{
	const char *server = getServerName();
	
	print("WUStreamParser: connecting to server: ");
	println(server);
	if (m_client.connect(server, 80))
		println("\tconnected");
	else
	{
		m_netError = WUSTREAMPARSER_E_CONNECT;
		println(" \tERROR, connection failed");
		return 0;
	}
	return 1;
	
}


const char *WUStreamParser::get()
{
	static const char query[] = "GET /api/%s/forecast/q/%s.json";
	char getCommandBuffer[WUSTREAMPARSER_MAX_JSON_LINE_LENGTH];
	int pos, bytesCount;
	const char *result = NULL;

	if (m_client.connected() == 0)
	{
		if ( connect() == 0)
			return NULL;
	}

	sprintf(getCommandBuffer, query, m_apiKey, m_location);
	m_client.println(getCommandBuffer);
        println(getCommandBuffer);
	sprintf(getCommandBuffer, "Host: %s", getServerName() );
	m_client.println(getCommandBuffer);
	m_client.println("Connection: close");
	m_client.println();
	delay(WUSTREAMPARSER_WAIT_FOR_RESPONSE);

	// Reuse the command buffer to read stream
	pos = 0;
        bytesCount = 0;
	memset(getCommandBuffer, 0, sizeof(getCommandBuffer) );
	while (m_client.available() && (pos < WUSTREAMPARSER_MAX_JSON_LINE_LENGTH) )
	{
		int inChar = m_client.read();
		if (inChar > 0)
		{
                        bytesCount++;
			if (inChar != 10 && inChar != 12)
				getCommandBuffer[pos++] = inChar;
			else
			{
                                print("\tprocessing line:"); println(getCommandBuffer);
				result = parse(getCommandBuffer);
				if (result != NULL)
					break;
                                pos = 0; // and reset to start from begining of read buffer
                                memset(getCommandBuffer, 0, sizeof(getCommandBuffer) );
                                int n = m_client.available();
                                if (n < WUSTREAMPARSER_MAX_JSON_LINE_LENGTH)
                                { // If buffer of unread data in ethernet card is smaller than our buffer, wait a bit for it to fill up
                                  print(" //buffer="); print(n);
                                  delay(WUSTREAMPARSER_WAIT_FOR_SHORTBUFF);
                                }
			}
		}
	}
        print("Processed: "); print(bytesCount); print(" bytes. ");
	println("Disconnecting");
	m_client.stop();
	return result;
}

