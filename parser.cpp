#include "parser.h"
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

SyntaxTree::SyntaxTree(std::string& _type, SyntaxTreeNode* _father)
	: type(_type), father(_father) {}

SyntaxTreeNode::SyntaxTreeNode(Token& _token, SyntaxTree* _location, SyntaxTreeNode* _next)
	: token(_token), location(_location), next(_next) {}

static void insert(std::vector<std::string>& injected, std::vector<std::string>& content, size_t index) {
	injected.erase(injected.begin() + index);
	for (size_t i = index; i < content.size() + index; ++i) {
		injected.insert(injected.begin() + i, content[i - index]);
	}
}

bool Parser::perfect_match(std::vector<Token>& current)
{
	if (matches.size() != 1) return false;
	else {
		for (Token& i : current) {
			for (std::string& j : matches) {
				if (i.value != j) return false;
			}
		}
	}
	return true;
}

//// =================================================================== TODO:
int Parser::find_pattern(std::vector<Token>& current)
// finds the pattern/patterns of tokens that match.
{
	// find initial matches:
	for (auto pair = GRAMMAR.begin(); pair != GRAMMAR.end(); ++pair) {
		std::vector<std::string> line = pair->second;
		for (int i = 0; i < current.size(); ++i) {

		}
	}
	return 0; // TEMPORARY
}

void Parser::syntactical_analysis()
// performs syntactical analysis
{
	// check each node
	// add to array of checked nodes
	// try to find a match of the checked nodes
	//		if internal recursion has to be checked, substitute rules untill there is no recursion.
	// add to matches that could work.
	// if it fully matches, save in a tree.
	// else keep going
	// if at any point there are no matches
	//		throw a syntax error.
	syntactical_analysis(0, GRAMMAR.begin());
}

//// =================================================================== TODO:
void Parser::syntactical_analysis(int t, SyntaxGrammerMap::iterator i)
{
	std::vector<Token> current;
	int best_match = 0; // make ordered map...
	for (; t < tokens->size(); ++t) {
		current.push_back(tokens->at(t));
		best_match = find_pattern(current);
		if (best_match == -1) {
			// return an error here... @TODO
		}
		else if (perfect_match(current)) {
			// add the tree here:
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
