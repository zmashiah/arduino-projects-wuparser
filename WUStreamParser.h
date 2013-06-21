/* ===============================================================================
 *	Parser for Weather-Underground single entry of the JSON reply
 * ===============================================================================
 * By: Zakie Mashiah		zmashiah@gmail.com			June-15-2013
 * You may use, modify, publish this code for any type of use provided that
 * you keep a visible indication of the author
 * ===============================================================================
 * Usage:
 *		call init() with debug (0 = off, 1= on), location string, WU API Key and vector of char*
 *      indicating the depth of JSONs entry you are looking for.
 *      call get() 
 *      If found, the result will be non NULL, otherwise it will be NULL
 * ===============================================================================
*/

#ifndef __WUSTREAMPARSER_H
#define __WUSTREAMPARSER_H

#include <Ethernet.h>			// Arduino ethernet
#include "JsonStreamParser.h"

#define WUSTREAMPARSER_MAX_JSON_LINE_LENGTH 512
#define WUSTREAMPARSER_E_CONNECT -1
#define WUSTREAMPARSER_WAIT_FOR_RESPONSE 4000    // Time to wait (ms) after the GET request
#define WUSTREAMPARSER_WAIT_FOR_SHORTBUFF 300    // If buffer on ethernet is too small, wait perhaps more data will come. Increas on slow links

class WUStreamParser : public JsonStreamParser
{
private:
	const char		*m_location;
	const char		*m_apiKey;
	EthernetClient	 m_client;
	int				 m_netError;
	bool			 m_console; // True if debug on console
	// Debug print functions
	void print(const char *str);
        void print(int d);
	void println(const char *str);
public:
	void init(bool debugPrint, const char *location, const char *apiKey, const char *searchParts[]);
	const char *getServerName();
	int connect();
	const char *get();
};

#endif

