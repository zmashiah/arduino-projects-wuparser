/* ===============================================================================
 * Simplae parser for JSON stream that maintains state internally for parsing
 * a JSON stream and fetching single value from it. Designed for Arduino that
 * has limited memory available.
 * ===============================================================================
 * By: Zakie Mashiah		zmashiah@gmail.com				June-15-2013
 * You may use, modify, publish this code for any type of use provided that
 * you keep a visible indication of the author
 * ===============================================================================
*/

#include <string.h>

#include "JsonStreamParser.h"

#define JSONSTREAMPARSER_MAX_JSON_LINE_LENT 200
#define JSONSTREAMPARSER_WAIT_FOR_RESPONSE	3000
#define JSONSTREAMPARSER_E_CONNECT			1




// Don't use constructor as Arduino environment does not work well with
// constructors. Rather use init function instead
void JsonStreamParser::init(const char *searchParts[])
{
	setSearch(searchParts);
	m_partPos = -1;
	m_bracketsCounter = 0;
	m_vectorCounter = 0;
	m_currtState = Start;
}

void JsonStreamParser::setSearch(const char *searchParts[])
{
	int i;

	for (i = 0; searchParts[i] != NULL && (i<JSONSTREAMPARSER_MAX_DEPTH); i++)
		m_searchParts[i] = searchParts[i];
	m_searchParts[i] = NULL;
}

const char *JsonStreamParser::parse(const char *streamLine)
{
	const char *lookingForTerm;
	int         partIdx;
	const char *ptr;
	const char *keyStart = NULL;
	const char *keyEnd = NULL;
	const char *valueStart = NULL;
	const char *valueEnd = NULL;

	partIdx = (m_partPos < 0) ? 0 : m_partPos;
	lookingForTerm = m_searchParts[partIdx];
	
	for (ptr = streamLine ; *ptr ; ptr++)
	{
		switch (m_currtState)
		{
		case Start:
			if ( (*ptr == ' ') || (*ptr =='\t') || (*ptr == '\r') || (*ptr == '\n') )
				continue;
			if (*ptr == '{')
			{
				m_currtState = WaitForKey;
				m_bracketsCounter++;
				continue;
			}
			return NULL;
		case AfterJsonClose:
			if ( (*ptr == ' ') || (*ptr =='\t') || (*ptr == '\r') || (*ptr == '\n') )
				continue;
			if ( *ptr == '}' )
			{
				m_bracketsCounter--;
				if (m_bracketsCounter == 0)
					m_currtState = Start;
				else
					m_currtState = AfterJsonClose;
				continue;
			}
			if ( *ptr == ',' )
			{
				m_currtState = WaitForKey;
				continue;
			}
			if (*ptr == '{')
			{
				m_currtState = WaitForKey;
				m_bracketsCounter++;
				continue;
			}
			return NULL;
		case WaitForKey:
			if (*ptr == '"')
			{
				m_currtState = InKey;
				keyStart = ptr + 1;
				continue;
			}
			if ( (*ptr == ' ') || (*ptr =='\t') || (*ptr == '\r') || (*ptr == '\n') )
				continue;
			if ( *ptr == ']')
			{
				m_vectorCounter--;
				continue;
			}
			if ( *ptr == ' }' )
			{
				m_bracketsCounter--;
				continue;
			}
			if (*ptr == ',')
				continue;
			break;
		case InKey:
			if (*ptr == '"')
			{
				m_currtState = WaitForColumns;
				keyEnd = ptr;
				if (strlen(lookingForTerm) == (keyEnd - keyStart))
					if (strncmp(lookingForTerm, keyStart, keyEnd - keyStart) == 0)
					{
						// We have the right token
						partIdx++;
						m_partPos = partIdx;
						lookingForTerm = m_searchParts[m_partPos];
					}
				break;
			}
			if ( ((*ptr >='a') && (*ptr <= 'z')) || ((*ptr >= 'A') && (*ptr <= 'Z')) || (*ptr == '_') || (*ptr == '-') || ( (*ptr >= '0') && (*ptr <= '9') )  )
				continue;
			return NULL;
		case WaitForColumns:
			if ( (*ptr == ' ') || (*ptr == '\t') )
				continue;
			if (*ptr == ':')
			{
				m_currtState = WaitForValue;
				continue;
			}
			return NULL;
		case WaitForValue:
			if ( (*ptr == '\r') || (*ptr == '\n') || (*ptr == ' ') || (*ptr == '\t') )
				continue;
			if ( *ptr == '[' )
			{
				m_vectorCounter++;
				continue;
			}
			if (*ptr == '{')
			{
				m_bracketsCounter++;
				m_currtState = WaitForKey;
				continue;
			}
			if ( (*ptr >= '0') && (*ptr <= '9') )
			{
				m_currtState = InValueNum;
				valueStart = ptr;
				continue;
			}
			if (*ptr == '"')
			{
				m_currtState = InValueStr;
				valueStart = ptr+1;
				continue;
			}
			return NULL;
		case InValueNum:
			if ( ((*ptr >= '0') && (*ptr <= '9')) || (*ptr == '.') )
				continue;
			if ((*ptr == ' ') || (*ptr == '\t') || (*ptr == '\r') || (*ptr == '\n') )
			{
				valueEnd = ptr;
				m_currtState = WaitForComma;
				if (lookingForTerm == NULL)
					return valueStart;
				continue;
			}
			if (*ptr == ',')
			{
				valueEnd = ptr;
				if (lookingForTerm == NULL)
					return valueStart;
				m_currtState = WaitForKey;
				continue;
			}
			return NULL;
		case InValueStr:
			// This parser does not support escaped " in values
			if (*ptr == '"')
			{
				m_currtState = WaitForComma;
				valueEnd = ptr - 1;
				if (lookingForTerm == NULL)
					return valueStart;
				continue;
			}
			if ( (*ptr == '\r') || (*ptr == '\n') ) // Ignore everything but EOL
				return NULL;
			continue;
		case WaitForComma:
			if (*ptr == ']')
			{
				m_vectorCounter--;
				if (m_vectorCounter == 0)
					m_currtState = AfterJsonClose;
				continue;
			}
			if (*ptr == '[')
			{
				m_vectorCounter++;
				m_currtState = AfterJsonClose;
				continue;
			}
			if ( *ptr == ',' )
			{
				m_currtState = WaitForKey;
				continue;
			}
			if ( *ptr == '}' )
			{
				m_bracketsCounter--;
				m_currtState = AfterJsonClose;
				continue;
			}
			if ( (*ptr == ' ') || (*ptr == '\t') || (*ptr == '\r') || (*ptr == '\n') )
				continue;
			return NULL;
		}
	}
	return NULL;
}


