#ifndef PARSER_CF_H
#define PARSER_CF_H

#include <vector>
#include <string>
#include "lexer.h"
#include <map>

struct SyntaxTreeNode;
struct SyntaxTree;
typedef std::map<std::string, std::vector<std::string>> SyntaxGrammerMap;

// @TODO:
struct SyntaxTreeNode {
	Token& token;
	SyntaxTree* location;
	SyntaxTreeNode* next = nullptr;

	SyntaxTreeNode(Token& _token, SyntaxTree* _location, SyntaxTreeNode* _next);
};

// @TODO:
struct SyntaxTree {
	std::string& type;
	SyntaxTreeNode* father;

	SyntaxTree(std::string& type, SyntaxTreeNode* father);
};

class Parser {
private:
	std::vector<Token>* tokens;
	std::string& code;
	const char* FILE_NAME;
	std::vector<SyntaxTree> tree;
	int pos = 0;

	void syntactical_analysis();
	void syntactical_analysis(int t, SyntaxGrammerMap::iterator i);
	void semantical_analysis();

	std::vector<std::string> find_pattern(std::vector<Token>& current, int token_index=0);
public:
	Parser(std::vector<Token>* t, std::string& _code, const char* _FILE_NAME);

	void validate();
	void add_trees(std::vector<Token>& t, std::string& type);
	std::vector<SyntaxTree>* return_tree();
};


#endif // PARSER_CF_H