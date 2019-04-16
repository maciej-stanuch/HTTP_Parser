#include "HTTP_Parser.h"
#define iscontrol(X) HTTP_Parser::__iscontrol((X))
#define isheader(X) HTTP_Parser::__isheader((X))

HTTP_Parser::HTTP_Parser()
{
}

std::map<std::string, std::string> HTTP_Parser::Parse(std::string request) {
	std::map<std::string, std::string> parsedRequest;

	//parsed strings
	std::string req = "";
	std::string ver = "";
	std::string query = "";

	std::string field = "";
	std::string value = "";

	std::string ch = "";

	size_t i = 0;
	bool finished = false;

	__state_t currentState = START;

	for (; !finished; i++) {
		ch = request[i];

		switch (currentState) {

		case START:
			if (isupper(ch[0])) {
				currentState = REQUEST;
				req = ch;
			}
			else {
				currentState = ERROR;
			}
			break;

		case REQUEST:
			if (isupper(ch[0]) && ch[0] != ' ') {
				req.append(ch);
			}
			else if (ch[0] == ' ') {
				currentState = WHITESPACE0;
			}
			else {
				currentState = ERROR;
			}
			break;

		case WHITESPACE0:
			if (ch[0] == ' ') {
				break;
			}

			if (iscontrol(ch[0])) {
				currentState = ERROR;
			}
			else {
				query = ch;
				currentState = QUERY;
			}
			break;

		case QUERY:
			if (ch[0] == ' ') {
				currentState = WHITESPACE1;
				break;
			}

			if (iscontrol(ch[0])) {
				currentState = ERROR;
			}
			else {
				query.append(ch);
			}

			break;

		case WHITESPACE1:
			if (ch[0] == ' ') {
				break;
			}

			if (iscontrol(ch[0]))
				currentState = ERROR;
			else {
				ver = ch;
				currentState = VER;
			}
			break;

		case VER:
			if (ch[0] == '\r') {
				currentState = CRLF0;
				break;
			}

			if (iscontrol(ch[0]) || ch[0] == ' ') {
				currentState = ERROR;
			}
			else {
				ver.append(ch);
			}

			break;

		case CRLF0:
			if (ch[0] == '\n') {
				currentState = FIELDS;
			}
			break;

		case FIELDS:
			if (ch[0] == '\r') {
				currentState = CRLF2;
				break;
			}
			if (isheader(ch[0])) {
				currentState = FIELD;
				field = ch;
			}
			break;

		case FIELD:
			if (isheader(ch[0])) {
				field += ch;
			}

			if (ch[0] == ':') {
				currentState = WHITESPACE2;
			}
			break;

		case WHITESPACE2:
			if (ch[0] == ' ')
				break;

			if (iscontrol(ch[0])) {
				currentState = ERROR;
			}
			else {
				value = ch;
				currentState = VALUE;
			}
			break;

		case VALUE:
			if (ch[0] == '\r') {
				currentState = CRLF1;
				parsedRequest[field] = value;
				value = "";
				field = "";
				break;
			}

			if (iscontrol(ch[0])) {
				currentState = ERROR;
			}
			else {
				value += ch;
			}
			break;

		case CRLF1:
			if (ch[0] == '\n') {
				currentState = FIELDS;
			}
			break;

		case CRLF2:
			if (ch[0] == '\n') {
				finished = true;
			}
			else {
				currentState = ERROR;
			}
			break;



		case ERROR:
			return std::map<std::string, std::string>();
		}
	}


	parsedRequest["verb"] = req;
	parsedRequest["query"] = query;
	parsedRequest["version"] = ver;

	return parsedRequest;

}

bool HTTP_Parser::__iscontrol(char c) {
	return iscntrl(c);
}

bool HTTP_Parser::__isheader(char c) {
	return (c != ':' && (isalnum(c) || c == '-'));
}


HTTP_Parser::~HTTP_Parser()
{
}
