#ifndef PARSER_CF_H
#define PARSER_CF_H

#include <vector>
#include <string>
#include "lexer.h"

class Parser {
private:
	std::vector<Token>* tokens;
	std::string& code;
	const char* FILE_NAME;

	void syntactical_analysis();
	void semantical_analysis();
public:
	Parser(std::vector<Token>* t, std::string& _code, const char* _FILE_NAME);

	void validate();
};


#endif // PARSER_CF_H