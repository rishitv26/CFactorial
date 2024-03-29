#ifndef PARSER_CF_H
#define PARSER_CF_H

#include <vector>
#include <string>
#include <functional>
#include <map>
#include "lexer.h"

typedef std::map<std::string, std::vector<std::string>> SyntaxGrammerMap;
typedef std::map<std::string, std::vector<std::string>> SemanticChecksMap;

// semantical checks map.
// contains all semantical checks to be performed on each type of reduced token.
// TODO: add appropriae check for each keyword.
static SemanticChecksMap checks = {
	{"~import", {}},
	{"~using", {}},
	{"~term", {}},
	{"~expr", {}},
	{"~decl", {}},
	{"~speddecl", {}},
	{"~var", {}},
	{"~class", {}},
	{"~assign", {}},
	{"~fund", {}},
	{"~funstart", {}},
	{"~funcal", {}},
	{"~fact", {}},
	{"~ifhead", {}},
	{"~elstart", {}},
	{"~whilestart", {}},
	{"~forheads2", {}},
	{"~state", {}},
	{"~scope", {}},
};

class SyntaxTreeNode {
public:
	Token name;
	std::vector<SyntaxTreeNode> children;
	SyntaxTreeNode* father = nullptr;

	SyntaxTreeNode() {}
	SyntaxTreeNode(Token& n, std::vector<SyntaxTreeNode>& c, SyntaxTreeNode* f);

	void print();
	Token& get_closest_token();
	Token& get_farthest_token();

	void traverse_left_right(std::function<void(SyntaxTreeNode&)> f);
	bool contains_token(Token& i);
};

class Parser {
private:
	std::vector<Token>* tokens;
	SyntaxTreeNode father;
	std::vector<SyntaxTreeNode> current_layer;
	std::string& code;
	const char* FILE_NAME;
	
public:
	void syntactical_analysis();
	void semantical_analysis();

	void throw_error(Token& t);
	Parser(std::vector<Token>* t, std::string& _code, const char* _FILE_NAME);

	std::vector< SyntaxTreeNode>& return_current_layer();
	void set_father(SyntaxTreeNode& f);
	SyntaxTreeNode& get_syntax_tree();

	void condense_imports();
};


#endif // PARSER_CF_H