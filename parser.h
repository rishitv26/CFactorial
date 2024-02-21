#ifndef PARSER_CF_H
#define PARSER_CF_H

#include <vector>
#include <string>
#include "lexer.h"

struct SyntaxTreeNode;
struct SyntaxTree;

// defines the type of syntax trees one may have
// can include expression trees, statement trees, etc.
enum class syntax_tree_type {
	EXPRESSION = 0,
};

// @TODO:
struct SyntaxTreeNode {
	Token& token;
	SyntaxTree* location;
	SyntaxTreeNode* next = nullptr;

	SyntaxTreeNode(Token& _token, SyntaxTree* _location, SyntaxTreeNode* _next);
};

// @TODO:
struct SyntaxTree {
	syntax_tree_type type;
	SyntaxTreeNode& father;

	SyntaxTree(syntax_tree_type& type, SyntaxTreeNode& father);
};

class Parser {
private:
	std::vector<Token>* tokens;
	std::string& code;
	const char* FILE_NAME;
	std::vector<SyntaxTree> tree;

	void syntactical_analysis();
	void semantical_analysis();
public:
	Parser(std::vector<Token>* t, std::string& _code, const char* _FILE_NAME);

	void validate();
	std::vector<SyntaxTree>* return_tree();
};


#endif // PARSER_CF_H