//Copyright 2019 Maciej Stanuch

#include <iostream>
#include "HTTP_Parser.h"


std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
	return str;
}

int main() {

	std::string example = "GET /hello.htm HTTP/1.1\
		User - Agent: Mozilla / 4.0 (compatible; MSIE5.01; Windows NT)\
		Host : www.tutorialspoint.com\
		Accept - Language : en - us\
		Accept - Encoding : gzip, deflate\
		Connection : Keep - Alive\r\n\r\n";

	example = ReplaceAll(example, "\t\t", "\r\n");

	std::map < std::string, std::string > tmp;
	tmp = HTTP_Parser::Parse(example);

	for (std::pair<std::string, std::string> x : tmp) {
		std::cout << x.first << ": " << x.second << "\n";
	}

	return 0;
}