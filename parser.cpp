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

SyntaxTree::SyntaxTree(syntax_tree_type _type, SyntaxTreeNode* _father)
	: type(_type), father(_father) {}

SyntaxTreeNode::SyntaxTreeNode(Token& _token, SyntaxTree* _location, SyntaxTreeNode* _next)
	: token(_token), location(_location), next(_next) {}

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
// recursive instance:
{
	std::vector<Token> current;
	for (; t < tokens->size(); ++t) {
		current.push_back(tokens->at(t));
		// proccess:
		std::vector<std::string> matches;
		
		for (int j = 0; j < current.size(); ++j) {
			// find match:
			
			matches.clear();
			for (int c = 0; i != GRAMMAR.end(); ++i, c++) {
				if (i->second[j][0] == '/') {
					if (
						(current.back().type == INTEGER ||
							current.back().type == STRING ||
							current.back().type == DECIMAL) &&
						i->second[j] == "/LIT"
						) {
						matches.push_back(i->first);
					}
					else if (current.back().type == IDENTIFIER && i->second[j] == "/ID") {
						matches.push_back(i->first);
					}
				}
				else if (current.back().value == i->second[j]) matches.push_back(i->first);
			}
			
		}
		
		cout << "Matches: " << matches.size() << ": " << endl;
		cout << matches.front() << ": ";
		for (auto& i : GRAMMAR[matches.front()]) {
			cout << i << " ";
		}
		cout << endl;
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
