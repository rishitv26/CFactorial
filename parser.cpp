#include "parser.h"
#include "utils.h"
#include "lexer.h"
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

SyntaxTree::SyntaxTree(syntax_tree_type _type, SyntaxTreeNode* _father)
	: type(_type), father(_father) {}

SyntaxTreeNode::SyntaxTreeNode(Token& _token, SyntaxTree* _location, SyntaxTreeNode* _next)
	: token(_token), location(_location), next(_next) {}

static bool is_reducable(Stack<Token>& stack)
// check to see if stack is reducable
{
	return false;
}

static void reduce(Stack<Token>& stack) 
// reduce the stack if stack is reducable
{

}

static bool abnormal_grammer(Stack<Token>& stack) 
// check to see if current grammer MAKES NO SENSE
{
	return false;
}

void Parser::syntactical_analysis()
// performs syntactical analysis
{
	// does shift reduce parsing
	syntactical_analysis(0, GRAMMAR.begin());
}
void Parser::syntactical_analysis(int t, SyntaxGrammerMap::iterator i)
{
	Stack<Token> stack;
	for (Token& token : *tokens) {
		stack.push(token);
		while (is_reducable(stack)) reduce(stack);
		if (abnormal_grammer(stack)) {
			// throw an error here...
		}
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
void Parser::add_trees(std::vector<Token>& t, std::vector<std::string>& matches)
{
	/*SyntaxTreeNode father(t[0], &tree[0], nullptr);
	SyntaxTree first(syntax_tree_type::EXPRESSION, &father);
	if (GRAMMAR)
	tree.push_back(first);*/
}

std::vector<SyntaxTree>* Parser::return_tree() { return &tree; }
