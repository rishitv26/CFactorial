#include "parser.h"
#include "utils.h"
#include "lexer.h"
#include <iostream>
using std::cout;
using std::endl;

/*
* Syntax Patterns. Defines the syntax grammer. Patterns that come before take more precedence.
*/
static SyntaxGrammerMap GRAMMAR = {
	// variables:
	{"~decl1", {"/TYPE", "/ID"}},
	{"~var1", {"~decl", "=", "~expr"}},
	// functions:
	{"...1", {"~decl", ",", "~decl"}},
	{"...2", {"...", ",", "~decl"}},
	{"~fund1", {"~decl", "->", "(", "...", ")"}},
	{"~fund2", {"~decl", "->", "(", "~decl", ")"}},
	{"~fund3", {"~decl", "->", "(", ")"}},
	// expressions:
	{"~expr1", {"~term"}},
	{"~expr5", {"~expr", "/", "~expr"}},
	{"~expr4", {"~expr", "*", "~expr"}},
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
};
std::vector<std::string> GRAMMAR_PRECEDENCE = {

};

// contains all "types" seperate form keywords.
static const std::vector<std::string> TYPES = {
	"int",
	"void"
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
	else {
		for (const std::string& i : TYPES) {
			if (i == str) {
				str = "/TYPE";
				break;
			}
		}
	}
	return str;
}
static int findpattern(std::vector<Token>& vec, std::vector<std::string>& pattern) {
	int vecSize = vec.size();
	int patternSize = pattern.size();

	// Iterate through the vector to find the pattern
	for (int i = 0; i <= vecSize - patternSize; ++i) {
		bool found = true;
		// Check if the pattern matches starting from index i
		for (int j = 0; j < patternSize; ++j) {
			if (parseToken(vec[i + j]) != pattern[j]) {
				found = false;
				break;
			}
		}
		if (found) {
			return i; 
		}
	}

	return -1;
}
static bool is_reducable(Stack<Token>& stack)
// check to see if stack is reducable
{
	for (auto pair = GRAMMAR.begin(); pair != GRAMMAR.end(); ++pair) {
		// find offset of the pattern if it exists.
		int offset = findpattern(stack.cvect(), pair->second);
		if (offset < 0) continue;
		
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

static void replace(std::vector<Token>& arr, std::vector<std::string>& pattern, Token& replacement) {
	for (size_t i = 0; i < arr.size(); ++i) {
		if (i + pattern.size() <= arr.size()) {
			bool match = true;
			for (size_t j = 0; j < pattern.size(); ++j) {
				if (parseToken(arr.at(i + j)) != pattern[j]) {
					match = false;
					break;
				}
			}
			if (match) {
				arr.erase(arr.begin() + i, arr.begin() + i + pattern.size());
				arr.insert(arr.begin() + i, replacement);
				break;  // Assuming pattern appears only once
			}
		}
	}
}

static bool reduce(Stack<Token>& stack) 
// reduce the stack if stack is reducable
{
	bool tor = false;
	for (auto pair = GRAMMAR.begin(); pair != GRAMMAR.end(); ++pair) {
		// find offset of the pattern if it exists.
		cout << "{";
		for (std::string& i : pair->second) {
			cout << i << ", ";
		}
		cout << "\b\b}\n";
		int offset = findpattern(stack.cvect(), pair->second);
		if (offset < 0) continue;

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
				Token replacement(REDUCED, s, stack.cvect()[stack.cvect().size() - i - 1].pos);
				replace(stack.cvect(), pair->second, replacement);
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
		if (token.value != ";") continue;
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
