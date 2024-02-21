#include "parser.h"

void Parser::syntactical_analysis()
// performs syntactical analysis
{

}

void Parser::semantical_analysis()
// performes semantical analysis
{

}

Parser::Parser(std::vector<Token>* t, std::string& _code, const char* _FILE_NAME)
	: tokens(t), code(_code), FILE_NAME(_FILE_NAME) {}

//// @TODO:
void Parser::validate()
// validates the token list using predefined rules.
{
	syntactical_analysis();
	semantical_analysis();
}
