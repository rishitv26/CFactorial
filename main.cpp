#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"

constexpr const char* FILE_NAME = "test.cf";

std::string code;
std::vector<Token>* tokens;

int main() {
	// read file:
	std::ifstream file(FILE_NAME);
	if (!file.is_open()) {
		std::cout << "ERROR: could not open \"" << FILE_NAME << "\"\n";
		exit(1);
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	code = buffer.str();
	file.close();

	// generate tokens (lexer):
	Lexer lexer(code, FILE_NAME);
	lexer.tokenize();
	lexer.condense();
	tokens = lexer.return_tokens();
	lexer.print_tokens();

	// validate generate syntax trees (parser):
	Parser parser(tokens, code, FILE_NAME);
	parser.validate();

	// optimize tree (optimizer):
	// compile tree (assembler):
	// output executable:
}