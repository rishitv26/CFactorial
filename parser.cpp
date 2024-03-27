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
	// more terms:
	{"~term1", {"-", "~fact"}},
	{"~term3", {"~term", "/", "~fact"}},
	{"~term4", {"~term", "*", "~fact"}},
	// more expressions:
	{"~expr2", {"~expr", "+", "~term"}},
	{"~expr3", {"~expr", "-", "~term"}},
	// variables:
	{"~decl1", {"/TYPE", "/ID"}},
	{"~var1", {"~decl", "=", "~expr"}},
	// assignment:
	{"~assign1", {"/ID", "="}},
	// functions:
	{"~...1", {"~decl", ",", "~decl"}},
	{"~...2", {"~...", ",", "~decl"}},
	{"~...3", {"~...", ",", "~..."}},
	{"~fund1", {"~decl", "->", "(", "~...", ")"}},
	{"~fund2", {"~decl", "->", "(", "~decl", ")"}},
	{"~fund3", {"~decl", "->", "(", ")"}},
	{"~fundef1", {"~fund", "~scope"}},
	{"~funstart1", {"/ID", "("}},
	{"~funcal1", {"~funstart", "~eee", ")"}},
	{"~funcal2", {"~funstart", "~expr", ")"}},
	{"~funcal3", {"~funstart", ")"}},
	// factors:
	{"~fact1", {"-", "-", "/ID"}},
	{"~fact2", {"+", "+", "/ID"}},
	{"~fact3", {"/ID", "-", "-"}},
	{"~fact4", {"/ID", "+", "+"}},
	{"~fact5", {"/ID"}},
	{"~fact6", {"/LIT"}},
	{"~fact7", {"(", "~expr", ")"}},
	{"~fact8", {"~funcal"}},
	// terms:
	{"~term2", {"~fact"}},
	// expressions:
	{"~expr1", {"~term"}},
	{"~eee1", {"~expr", ",", "~expr"}},
	{"~eee2", {"~eee", ",", "~expr"}},
	{"~eee3", {"~eee", ",", "~eee"}},
	// statements:
	{"~state1", {"~var", ";"}},
	{"~state2", {"~assign", "~expr", ";"}},
	{"~state3", {"~decl", ";"}},
	{"~state4", {"return", "~expr", ";"}},
	{"~state5", {"~expr", ";"}},
	{"~state6", {"~scope"}},
	{"~sss1", {"~state", "~state"}},
	{"~sss2", {"~sss", "~state"}},
	{"~sss3", {"~sss", "~sss"}},
	// scopes:
	{"~scope1", {"{", "~sss", "}"}},
	{"~scope2", {"{", "~state", "}"}},
	{"~scope3", {"{", "}"}},
};
/*
* Precendent for each rule in GRAMMAR. Patterns that come before take more precedence.
*/
const std::string GRAMMAR_PRECEDENCE[] = {
	"~term1",
	"~term3",
	"~term4",
	"~expr2",
	"~expr3",
	"~decl1",
	"~var1",
	"~assign1",
	"~...1",
	"~...2",
	"~...3",
	"~fund1",
	"~fund3",
	"~fund2",
	"~fundef1",
	"~funstart1",
	"~funcal1",
	"~funcal2",
	"~funcal3",
	"~fact1",
	"~fact2",
	"~fact3",
	"~fact4",
	"~fact5",
	"~fact6",
	"~fact7",
	"~fact8",
	"~term2",
	"~expr1",
	"~eee1",
	"~eee2",
	"~eee3",
	"~state1",
	"~state2",
	"~state3",
	"~state4",
	"~state5",
	"~state6",
	"~sss1",
	"~sss2",
	"~sss3",
	"~scope1",
	"~scope2",
	"~scope3",
};

// contains all "types" seperate form keywords.
static const std::vector<std::string> TYPES = {
	"int",
	"str",
	"void",
	"class",
};

Parser::Parser(std::vector<Token>* t, std::string& _code, const char* _FILE_NAME)
	: tokens(t), code(_code), FILE_NAME(_FILE_NAME) {}

std::vector<SyntaxTreeNode>& Parser::return_current_layer()
{
	return current_layer;
}

void Parser::set_father(SyntaxTreeNode& f)
{
	father = f;
}

SyntaxTreeNode& Parser::get_syntax_tree()
{
	return father;
}

SyntaxTreeNode::SyntaxTreeNode(Token& n, std::vector<SyntaxTreeNode>& c, SyntaxTreeNode* f)
	: name(n), children(c), father(f) {}

void SyntaxTreeNode::print()
{
	for (int i = 0; i < children.size(); ++i) cout << '-';
	cout << ' ' << name.value << ": " << children.size() << endl;
	if (children.size() == 0) return;
	for (SyntaxTreeNode& i : children) {
		i.print();
	}
}

Token& SyntaxTreeNode::get_closest_token()
{
	if (children.size() == 0) return name;
	else {
		return children.front().get_closest_token();
	}
}

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
	int vecSize = (int)vec.size();
	int patternSize = (int)pattern.size();

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
	for (const std::string& first : GRAMMAR_PRECEDENCE) {
		// find offset of the pattern if it exists.
		int offset = findpattern(stack.cvect(), GRAMMAR[first]);
		if (offset < 0) continue;
		
		// continue from there:
		for (int i = 0; i < stack.cvect().size(); ++i) {
			if (i >= GRAMMAR[first].size() || i + offset >= stack.cvect().size()) break;
			std::string str = parseToken(stack.cvect()[i + offset]); // if i is an integer, convert to /INT and so on

			if (str != GRAMMAR[first][i]) break; // this rule does not work...
			if (i == GRAMMAR[first].size() - 1) return true;
		}
	}
	
	return false; // no matches
}
static void replace(std::vector<Token>& arr, std::vector<std::string>& pattern, Token& replacement, Parser& parser) {
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
				const size_t start = i;
				const size_t end = i + pattern.size();
				auto& current_layer = parser.return_current_layer();

				// create new syntax tree with the child nodes:
				SyntaxTreeNode s;
				s.name = replacement;
				for (size_t j = start; j < end; ++j) {
					s.children.push_back(current_layer.at(j));
					s.children.back().father = &s;
				}
				// copy paste children:
				current_layer.erase(current_layer.begin() + start, current_layer.begin() + end);
				current_layer.insert(current_layer.begin() + start, s);

				arr.erase(arr.begin() + start, arr.begin() + end);
				arr.insert(arr.begin() + start, replacement);
				break;  // pattern appears only once
			}
		}
	}
}
static bool reduce(Stack<Token>& stack, Parser& parse) 
// reduce the stack if stack is reducable
{
	for (const std::string& first : GRAMMAR_PRECEDENCE) {
		// find offset of the pattern if it exists.y
		int offset = findpattern(stack.cvect(), GRAMMAR[first]);
		if (offset < 0) continue;

		// continue from there:
		for (int i = 0; i < stack.cvect().size(); ++i) {
			if (i >= GRAMMAR[first].size() || i + offset >= stack.cvect().size()) break;
			std::string str = parseToken(stack.cvect()[i + offset]); // if i is an integer, convert to /INT and so on

			if (str != GRAMMAR[first][i]) break; // this rule does not work...
			if (i == GRAMMAR[first].size() - 1) {
				// reduction step:
				std::string s = first;
				s.erase(s.begin() + s.size() - 1);
				Token replacement(REDUCED, s, stack.cvect()[stack.cvect().size() - i - 1].pos);
				replace(stack.cvect(), GRAMMAR[first], replacement, parse);
				stack.print();
				return true; // ONLY DO 1 SIMPLIFICATION AT A TIME!
			}
		}
	}
	stack.print();
	return false;
}

static void check_errors(Stack<Token>& stack, Parser& s) {// change this in the future to make more effiecient
	for (Token& i : stack.cvect()) {
		if (i.type == END_OF_FILE) break;
		if (i.type != REDUCED && (i.value != "{")) {
			s.throw_error(i);
		}
	}
}
static void final_check(Stack<Token>& stack, Parser& parser) {
	check_errors(stack, parser);
	// check for unequal amount of { or [ or < or (
	size_t curly = 0;
	size_t sqrbrack = 0;
	size_t angbrack = 0;
	size_t paranth = 0;
	for (Token& i : stack.cvect()) {
		if (i.value == "{") curly++;
		else if (i.value == "}") curly--;
		if (curly < 0) parser.throw_error(i);

		if (i.value == "[") sqrbrack++;
		else if (i.value == "]") sqrbrack--;
		if (sqrbrack < 0) parser.throw_error(i);

		if (i.value == "<") angbrack++;
		else if (i.value == ">") angbrack--;
		if (angbrack < 0) parser.throw_error(i);

		if (i.value == "(") paranth++;
		else if (i.value == ")") paranth--;
		if (paranth < 0) parser.throw_error(i);
	}
	// finish up tree:
	auto& current_layer = parser.return_current_layer();
	size_t start = 0;
	size_t end = current_layer.size();

	SyntaxTreeNode s;
	s.name = Token(REDUCED, "~prog", Position(0, 0));
	for (size_t j = start; j < end; ++j) {
		if (
			current_layer.at(j).name.value == "~sss" ||
			current_layer.at(j).name.value == "~state" ||
			current_layer.at(j).name.value == "~fundef" ||
			current_layer.at(j).name.type == END_OF_FILE
			) {
			s.children.push_back(current_layer.at(j));
			s.children.back().father = &s;
		}
		else {
			// throw error here.
			// blame first token with no children.
			parser.throw_error(current_layer.at(j).get_closest_token());
		}
	}
	parser.set_father(s);

	// ============================ TODO: more final checks
}

void Parser::throw_error(Token& i)
{
	/*cout << "this made negative sense..." << endl;
	cout << "this token tripped me: [";
	cout << i.value << ", " << i.type << ", (line: " << i.pos.line << ", col: " << i.pos.col;
	cout << ")]\n";
	exit(1);*/
	const std::string message = "Token \"" + i.value + "\" was not expected.";
	ERROR(error_type::UNEXPECTED_TOKEN, message.c_str(), i.pos, FILE_NAME, code);
}

void Parser::syntactical_analysis()
// performs syntactical analysis (shift reduce parsing)
{
	Stack<Token> stack;
	for (Token& token : *tokens) {
		stack.push(token);
		SyntaxTreeNode s;
		s.name = token;
		current_layer.push_back(s);
		stack.print();
		if (token.value != ";") continue;
		if (is_reducable(stack)) do {
			reduce(stack, *this);
		} while (is_reducable(stack));
		check_errors(stack, *this);
	}
	// reduce extra if neccessary...
	if (is_reducable(stack)) while (is_reducable(stack)) reduce(stack, *this);
	final_check(stack, *this); // checks are different in the end.
	stack.print();
}

void Parser::semantical_analysis()
// performes semantical analysis
{

}

void Parser::validate()
// validates the token list using predefined rules.
{
	syntactical_analysis();
	semantical_analysis();
}
