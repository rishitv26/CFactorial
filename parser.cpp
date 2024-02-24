#include "parser.h"
#include <iostream>
using std::cout;
using std::endl;

/*
* Syntax Patterns. Defines the syntax grammer.
*/
static SyntaxGrammerMap GRAMMAR = {
	// expressions:
	{"~expr1", {"~term"}},
	{"~expr2", {"~expr", "+", "~term"}},
	{"~expr3", {"~expr", "-", "~term"}},
	{"~term1", {"~fact"}},
	{"~term2", {"~term", "/", "~fact"}},
	{"~term3", {"~term", "*", "~fact"}},
	{"~fact1", {"/ID"}},
	{"~fact2", {"/LIT"}},
	{"~fact3", {"(", "~expr", ")"}},
	{"goofy", {","}},
};

Parser::Parser(std::vector<Token>* t, std::string& _code, const char* _FILE_NAME)
	: tokens(t), code(_code), FILE_NAME(_FILE_NAME) {}

SyntaxTree::SyntaxTree(std::string& _type, SyntaxTreeNode* _father)
	: type(_type), father(_father) {}

SyntaxTreeNode::SyntaxTreeNode(Token& _token, SyntaxTree* _location, SyntaxTreeNode* _next)
	: token(_token), location(_location), next(_next) {}

std::vector<std::string> Parser::find_pattern(std::vector<Token>& current, int token_index)
// finds the pattern/patterns of tokens that match.
{
	for (Token& i : current) {

	}
}

void Parser::syntactical_analysis()
// performs syntactical analysis
{
	// check first node:
	// track possible patterns and types:
	// if pattern requires new abstraction / recursion:
	//		remember old tree
	//		start new tree (with corresponding type):
	// when complete with tree, and more trees are in memory:
	// modify pointers.
	// throw resonable errors during the proccess.
	syntactical_analysis(0, GRAMMAR.begin());
}

void Parser::syntactical_analysis(int t, SyntaxGrammerMap::iterator i)
{
	std::vector<Token> current;
	for (; t < tokens->size(); ++t) {
		current.push_back(tokens->at(t));
	}
}

void Parser::semantical_analysis()
// performes semantical analysis
{

}

//// @TODO:
void Parser::validate()
// validates the token list using predefined rules.
{
	syntactical_analysis();
	semantical_analysis();
}

//// @TODO:
void Parser::add_trees(std::vector<Token>& t, std::string& type)
{
	SyntaxTreeNode father(t[0], &tree.back(), nullptr);
	SyntaxTree new_tree(type, &father);
	tree.push_back(new_tree);
	SyntaxTreeNode* prev = &father;
	for (int i = 1; i < t.size(); ++i) {
		SyntaxTreeNode next(t[i], &tree.back(), nullptr);
		prev->next = &next;
		prev = &next;
	}
	father.location = &tree.back();
}

std::vector<SyntaxTree>* Parser::return_tree() { return &tree; }
