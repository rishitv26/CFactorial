#ifndef PARSER_CF_H
#define PARSER_CF_H

#include <vector>
#include <string>
#include "lexer.h"
#include <map>

typedef std::map<std::string, std::vector<std::string>> SyntaxGrammerMap;

// defines the type of syntax trees one may have
// can include expression trees, statement trees, etc.
enum class syntax_tree_type {
	EXPRESSION = 0,
};

struct SyntaxTreeNode {
	Token name;
	std::vector<SyntaxTreeNode> children;
	SyntaxTreeNode* father = nullptr;

	SyntaxTreeNode() {}
	SyntaxTreeNode(Token& n, std::vector<SyntaxTreeNode>& c, SyntaxTreeNode* f);

	void print();
	Token& get_closest_token();
};

class Parser {
private:
	std::vector<Token>* tokens;
	SyntaxTreeNode father;
	std::vector<SyntaxTreeNode> current_layer;
	std::string& code;
	const char* FILE_NAME;

	void syntactical_analysis();
	void semantical_analysis();
	
public:
	void throw_error(Token& t);
	Parser(std::vector<Token>* t, std::string& _code, const char* _FILE_NAME);
	std::vector< SyntaxTreeNode>& return_current_layer();
	void set_father(SyntaxTreeNode& f);
	SyntaxTreeNode& get_syntax_tree();

	void validate();
};


#endif // PARSER_CF_H