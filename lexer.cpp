#include "lexer.h"
#include <iostream>
#include <sstream>
#include "errors.h"

/*
* Symbols that designate something related to math / logic
*/
constexpr const char* MATH_SYMBOLS = "0123456789%+-*/^=!><@$#&|~";
/*
* Symbols that designate punctuation in the syntax
*/
constexpr const char* SYNTAX_SYMBOLS = ":?(){}[].,;";
/*
* Symbols that define integers
*/
constexpr const char* DIGIT_SYMBOLS = "0123456789";

Token::Token(token_type _type, std::string _value, Position _pos)
		: type(_type), value(_value), pos(_pos) {}

std::string Token::operator<<(std::ostream&)
{
	return value;
}

bool Token::operator==(Token t)
{
	return t.value == value;
}

Lexer::Lexer(std::string& _code, const char* _file)
		: code(_code), file(_file) {}

bool ismathsymbol(char i) {
	const char* ptr = strchr(MATH_SYMBOLS, i);
	return (ptr == nullptr) ? false : true;
}
bool issyntaxsymbol(char i) {
	const char* ptr = strchr(SYNTAX_SYMBOLS, i);
	return (ptr == nullptr) ? false : true;
}
bool isinteger(char i) {
	const char* ptr = strchr(DIGIT_SYMBOLS, i);
	return (ptr == nullptr) ? false : true;
}
bool iskeyword(std::string i) {
	for (std::string j : KEYWORDS) if (j == i) return true;
	return false;
}
bool iscmpdoperator(char one, char two) {
	for (const std::string& i : COMPOUND_OPERATORS) {
		if (i[0] == one && i[1] == two) return true;
	}
	return false;
}
static std::string rmvchrstr(std::string& str, size_t pos) {
	str.erase(str.begin() + pos);
	return str;
}

void Lexer::tokenize()
// note that these tokens are RAW tokens, meaning code is 
// differentiated based on operation, numbers, and text
{
	std::string word = "";
	Position pos(1, 1);
	for (int i = 0; i < code.size(); ++i) {
		char current = code[i];
		char next = (i == code.size()-1) ? '\0' : code[i + 1];
		word += current;

		if (current == '\n') { ++pos.line; pos.col = 0; word = ""; }
		else if (current == '\t' || current == ' ') word = "";
		else if (current == '"') {
			while (code[++i] != '"') {
				++pos.col;
				if (i == code.size() - 2 && next != '"') {
					// throw an error here.
					ERROR(error_type::INCOMPLETE_STRING, 
						"string must be complete", pos, file, code);
				}
				word += code[i];
			}
			word += '"';
			Token token(STRING, word, pos);
			tokens.push_back(token);
			--pos.col; // accomodate for increment after if statements...
		}
		else if (issyntaxsymbol(current)) {
			tokens.push_back(Token(SYNTAX_SYMBOL, std::string(1, current), pos));
			word = "";
		} 
		else if (ismathsymbol(current)) {
			if (current == '/' && next == '/') {
				word = "";
				while (i < code.size() - 1 && code[++i] != '\n');
				pos.col = 0; // accomodate for increment after if statements
				++pos.line;
			}
			else if (current == '/' && next == '*') {
				word = "";
				int n = i;
				while (n < code.size() && !(code[n] == '*' && code[n + 1] == '/')) {
					if (code[n] == '\n') { ++pos.line; pos.col = 1; }
					else ++pos.col;
					++n;
				}
				i = n + 1;
				--pos.col; // accomodate for increment after if statements
			}
			else {
				tokens.push_back(Token(MATH_SYMBOL, std::string(1, current), pos));
			}
			word = "";
		}
		else {
			// it must be a character (or text)
			if (next == '\n' || next == '\t' || next == ' ' || issyntaxsymbol(next) || ismathsymbol(next)) {
				// pattern broken, current word is acceptable text.
				tokens.push_back(Token(UNCLASSIFIED_TEXT, word, pos));
			}
		}
		
		++pos.col;
	}
	tokens.push_back(Token(token_type::END_OF_FILE, "[END FILE]", pos));
}

void Lexer::label_integers()
{
	// check for ints:
	for (Token& i : tokens) {
		if (i.type == MATH_SYMBOL && isinteger(i.value[0])) {
			i.type = INTEGER;
		}
	}

	// combine larger ints:
	bool change = false;
	for (int i = 0; i < tokens.size() - 1; !change ? ++i : i += 0) {
		change = false;
		if (
			isinteger(tokens[i].value[0]) &&
			isinteger(tokens[i+1].value[0])
		) {
			// found match, combine them:
			Token token(INTEGER, tokens[i].value + tokens[i + 1].value, tokens[i].pos);
			tokens.erase(tokens.begin() + i + 1);
			tokens[i] = token;
			change = true;
		}
	}

	// negate ints:
	for (int i = 0; i < tokens.size() - 1; ++i) {
		if (tokens[i].value[0] == '-' && tokens[i + 1].type == INTEGER) {
			// found negater, combine into 1 integer:
			Token token(INTEGER, tokens[i].value + tokens[i + 1].value, tokens[i].pos);
			tokens.erase(tokens.begin() + i + 1);
			tokens[i] = token;
		}
	}
}

void Lexer::condense_identifiers()
{
	bool change = false;
	for (int i = 0; i < tokens.size()-1; !change ? ++i : i+=0) {
		change = false;
		if (tokens[i].type == UNCLASSIFIED_TEXT && tokens[i + 1].type == INTEGER) {
			// found match, combine them:
			Token token(UNCLASSIFIED_TEXT, tokens[i].value + tokens[i + 1].value, tokens[i].pos);
			tokens.erase(tokens.begin() + i + 1);
			tokens[i] = token;
			change = true;
		}
	}
}

void Lexer::condense_decimals()
{
	// find .(int)
	for (int i = 0; i < tokens.size() - 1; ++i) {
		if (tokens[i].value[0] == '.' && tokens[i+1].type == INTEGER) {
			// found decimal match, combine
			Token token(DECIMAL, "0" + tokens[i].value + tokens[i + 1].value, tokens[i].pos);
			tokens.erase(tokens.begin() + i + 1);
			tokens[i] = token;
		}
	}
	// fint (int) & .(int)
	for (int i = 0; i < tokens.size() - 1; ++i) {
		if (tokens[i].type == INTEGER && tokens[i + 1].type == DECIMAL) {
			// found decimal match, combine
			Token token(DECIMAL, tokens[i].value + rmvchrstr(tokens[i + 1].value, 0), tokens[i].pos);
			tokens.erase(tokens.begin() + i + 1);
			tokens[i] = token;
		}
	}
}

void Lexer::condense_strings()
{
	for (int i = 0; i < tokens.size() - 1; ++i) {
		if (tokens[i].type == STRING && tokens[i + 1].type == STRING) {
			// found 2 common adjacent strings, combine them.
			Token token(
				STRING, 
				rmvchrstr(tokens[i].value, tokens[i].value.size()-1) + rmvchrstr(tokens[i + 1].value, 0), 
				tokens[i].pos
			);
			tokens.erase(tokens.begin() + i + 1);
			tokens[i] = token;
		}
	}
}

void Lexer::label_keywords()
{
	// identify all keywords:
	for (Token& i : tokens) {
		if (i.type == UNCLASSIFIED_TEXT) {
			// check if this is keyword:
			if (iskeyword(i.value)) i.type = KEYWORD;
			else i.type = IDENTIFIER;
		}
	}
}

void Lexer::condense_operations()
{
	for (Token& i : tokens)
		if (i.type == MATH_SYMBOL || i.type == SYNTAX_SYMBOL) i.type = OPERATOR;

	for (int i = 0; i < tokens.size() - 1; ++i) {
		if (
			iscmpdoperator(tokens[i].value[0], tokens[i+1].value[0]) &&
			tokens[i].type == OPERATOR &&
			tokens[i+1].type == OPERATOR
		) {
			// found 2 common operators
			Token token(OPERATOR, tokens[i].value + tokens[i + 1].value, tokens[i].pos);
			tokens.erase(tokens.begin() + i + 1);
			tokens[i] = token;
		}
	}
}

void Lexer::condense()
// condenses the code into more meaningfull tokens for parser to proccess.
{
	// find integers:
	label_integers();
	// identifier condensing:
	condense_identifiers();
	// number/decimal condensing:
	condense_decimals();
	// condense strings:
	condense_strings();
	// keyword labeling:
	label_keywords();
	// operation condensing:
	condense_operations();
	// add a final endfile token:
}

std::vector<Token>* Lexer::return_tokens()
{
	return &tokens;
}

void Lexer::print_tokens()
{
	for (Token i : tokens) {
		std::cout << i.type << ": " << i.value << " (line " << i.pos.line << ", col " << i.pos.col << ")\n";
	}
}
