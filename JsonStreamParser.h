/* ===============================================================================
 * Simplae parser for JSON stream that maintains state internally for parsing
 * a JSON stream and fetching single value from it. Designed for Arduino that
 * has limited memory available.
 * ===============================================================================
 * By: Zakie Mashiah		zmashiah@gmail.com			June-15-2013
 * You may use, modify, publish this code for any type of use provided that
 * you keep a visible indication of the author
 * ===============================================================================
 * Usage:
 *		call init() with vector of char* indicating the depth of JSONs entry
 *      you are looking for.
 *      call parse() with a line read from the JSON stream
 *      If found, the result will be non NULL, otherwise it will be NULL
 * ===============================================================================
*/
#ifndef __JSONSTREAMPARSER_H
#define __JSONSTREAMPARSER_H


#define JSONSTREAMPARSER_MAX_DEPTH			10		// Size of the serach vector.

class JsonStreamParser
{
private:
	enum PartStateMachine { Start, AfterJsonClose, WaitForKey, InKey, WaitForColumns, WaitForValue, InValueNum, InValueStr, WaitForComma, End };
	const char* m_searchParts[JSONSTREAMPARSER_MAX_DEPTH];
	int m_partPos;
	int m_bracketsCounter;
	int m_vectorCounter;
	PartStateMachine m_currtState;

public:
	void init(const char *searchParts[]);
	void setSearch(const char *searchParts[]);
	const char *parse(const char *streamLine);
};


#endif

