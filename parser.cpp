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
	{"~expr2", {"~expr", "+", "~expr"}},
	{"~expr3", {"~expr", "-", "~expr"}},
	// terms:
	{"~term1", {"~fact"}},
	{"~term2", {"~term", "/", "~fact"}},
	{"~term3", {"~term", "*", "~fact"}},
	// factors:
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

static std::string parseToken(Token& i) {
	std::string str = i.value;
	if (i.type == INTEGER ||
		i.type == DECIMAL ||
		i.type == STRING) {
		str = "/LIT";
	}
	else if (i.type == IDENTIFIER) {
		str = "/ID";
	}
	return str;
}

static bool is_reducable(Stack<Token>& stack)
// check to see if stack is reducable
{
	for (auto pair = GRAMMAR.begin(); pair != GRAMMAR.end(); ++pair) {
		int offset = 0;
		bool b = false;
		// find offset where this rule starts:
		for (int i = 0; i < stack.cvect().size(); ++i) {
			if (parseToken(stack.cvect()[i]) == pair->second[0]) { offset = i; break; }
			if (i == stack.cvect().size() - 1) b = true;
		} if (b) continue;

		// continue from there:
		for (int i = 0; i < stack.cvect().size(); ++i) {
			if (i >= pair->second.size() || i + offset >= stack.cvect().size()) break;
			std::string str = parseToken(stack.cvect()[i + offset]); // if i is an integer, convert to /INT and so on

			if (str != pair->second[i]) break; // this rule does not work...
			if (i == pair->second.size() - 1) return true;
		}
	}
	
	return false; // no matches
}

//// ==================================================================== TODO:
static void replace(std::vector<Token>& obj, std::vector<std::string>& replace, Token& val) {
	// find initial index:
	int index = 0;
	for (int i = 0; i < obj.size(); ++i) {
		int temp = 0;
		for (std::string& j : replace) {

		}
	}
}

static bool reduce(Stack<Token>& stack) 
// reduce the stack if stack is reducable
{
	bool tor = false;
	for (auto pair = GRAMMAR.begin(); pair != GRAMMAR.end(); ++pair) {
		int offset = 0;
		bool b = false;
		// find offset where this rule starts:
		for (int i = 0; i < stack.cvect().size(); ++i) {
			if (parseToken(stack.cvect()[i]) == pair->second[0]) { offset = i; break; }
			if (i == stack.cvect().size() - 1) b = true;
		} if (b) continue;

		// continue from there:
		for (int i = 0; i < stack.cvect().size(); ++i) {
			if (i >= pair->second.size() || i + offset >= stack.cvect().size()) break;
			std::string str = parseToken(stack.cvect()[i + offset]); // if i is an integer, convert to /INT and so on

			if (str != pair->second[i]) break; // this rule does not work...
			if (i == pair->second.size() - 1) {
				tor = true;
				// reduction step:
				std::string s = pair->first;
				s.erase(s.begin() + s.size() - 1);

				/*for (int j = 0; j < pair->second.size() - 1; ++j) {
					if (j < 0) break;
					stack.cvect().erase(stack.cvect().begin() + stack.cvect().size() - i - 1);
				}
				stack.cvect()[stack.cvect().size() - i - 1].value = s;
				stack.cvect()[stack.cvect().size() - i - 1].type = REDUCED;*/
			}
		}
	}
	stack.print();
	return tor;
}

static bool abnormal_grammer(Stack<Token>& stack)
// check to see if current grammer MAKES NO SENSE
{
	for (Token& i : stack.cvect()) {
		std::string str = parseToken(i); // if i is an integer, convert to /INT and so on
		for (auto pair = GRAMMAR.begin(); pair != GRAMMAR.end(); ++pair) {
			if (str == pair->second[0]) return false;
		}
	}
	return true;
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
		stack.print();
		if (is_reducable(stack)) while (is_reducable(stack)) {
			cout << "REDUCE!!!" << endl;
			if (!reduce(stack)) continue;
		}
		
		else if (abnormal_grammer(stack)) {
			// throw an error here...
			cout << "this made negative sense..." << endl;
			exit(1);
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
