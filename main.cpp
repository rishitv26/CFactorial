#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"

char* FILE_NAME;

std::string code;
std::vector<Token>* tokens;
SyntaxTreeNode tree;

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		std::cout << "ERROR: filename (argument 1) required \n";
		exit(1);
	}
	FILE_NAME = argv[1];
	//// read file:
	std::ifstream file(FILE_NAME);
	if (!file.is_open()) {
		std::cout << "ERROR: could not open \"" << FILE_NAME << "\"\n";
		exit(1);
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	code = buffer.str();
	file.close();

	//// generate tokens (lexer):
	Lexer lexer(code, FILE_NAME);
	lexer.tokenize();
	lexer.condense();
	tokens = lexer.return_tokens();

	//// validate & generate syntax trees (parser):
	Parser parser(tokens, code, FILE_NAME);
	parser.syntactical_analysis();
	// condense all import and using statements:
	parser.condense_imports();
	tree = parser.get_syntax_tree();

	//// optimize tree (optimizer):
	//// compile tree (assembler):
	//// output executable:
}