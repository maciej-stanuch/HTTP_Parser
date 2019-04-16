#pragma once
#include <map>
#include <string>

typedef enum {
	START,
	REQUEST,
	WHITESPACE0,
	QUERY,
	WHITESPACE1,
	VER,
	CRLF0,
	FIELDS,
	FIELD,
	WHITESPACE2,
	VALUE,
	CRLF1,
	CRLF2,

	ERROR
}__state_t;

class HTTP_Parser
{
private:
	static bool __iscontrol(char c);
	static bool __isheader(char c);

public:
	HTTP_Parser();
	
	static std::map<std::string, std::string> Parse(std::string request);
	
	~HTTP_Parser();
};


