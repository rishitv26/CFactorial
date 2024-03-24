#ifndef LEXER_CF_H
#define LEXER_CF_H

#include <string>
#include <vector>
#include "errors.h"

/*
* Tokens that represent keywords to locate.
*/
const std::string KEYWORDS[] = {
	// for efficiency, add most used keywords on top, and least used on the bottom
	"var",
	"if",
	"else",
};
/*
* Operations that are combined from multiple combinations
*/
const std::string COMPOUND_OPERATORS[] = {
	// for efficiency, add most used keywords on top, and least used on the bottom
	"->",
	"<-",
	"==",
	"!=",
	"<=",
	">=",
	">>",
	"<<",
};

enum token_type {
	// post tokenization:
	MATH_SYMBOL = 0,
	SYNTAX_SYMBOL = 1,
	UNCLASSIFIED_TEXT = 2,
	// complete tokenization:
	INTEGER = 3,
	STRING = 4,
	DECIMAL = 5,
	KEYWORD = 6,
	IDENTIFIER = 7,
	OPERATOR = 8,
	END_OF_FILE = 9,
	REDUCED = 10,
};

enum class expression_node_type {
	OPERATORATION = 0,
	LITERAL = 1,
};

struct Token {
	token_type type;
	std::string value;
	Position pos;
	Token(token_type type, std::string value, Position pos);
	std::string operator<<(std::ostream&);
};

class Lexer {
private:
	const char* file;
	std::string& code;
	std::vector<Token> tokens;

	void label_integers();
	void condense_identifiers();
	void condense_decimals();
	void condense_strings();
	void label_keywords();
	void condense_operations();
public:
	Lexer(std::string& code, const char* file);
	void tokenize();
	void condense();
	std::vector<Token>* return_tokens();
	void print_tokens();
};

bool isinteger(char i);
bool issyntaxsymbol(char i);
bool ismathsymbol(char i);
bool iskeyword(std::string i);
bool iscmpdoperator(char one, char two);

#endif // LEXER_CF_H