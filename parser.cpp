#include "parser.h"
#include "utils.h"
#include "lexer.h"
#include "semantics.h"
#include <iostream>
#include <fstream>
#include <sstream>
using std::cout;
using std::endl;

/*
* Syntax Patterns. Defines the syntax grammer. Patterns that come before take more precedence.
*/
static SyntaxGrammerMap GRAMMAR = {
	// dot notation:
	{"/ID1", {"/ID", ".", "/ID"}},
	{"/ID2", {"~expr", ".", "/ID"}},
	// import and using statements:
	{"~import1", {"import", "/ID"}},
	{"~using1", {"using", "/ID"}},
	// more factors:
	{"~factB", {"(", "/TYPE", ")", "~fact"}},
	{"~factC", {"(", "/ID", ")", "~fact"}},
	// more terms:
	{"~term1", {"-", "~fact"}},
	{"~term3", {"&", "~fact"}},
	{"~term4", {"!", "~fact"}},
	{"~term5", {"~term", "/", "~fact"}},
	{"~term6", {"~term", "*", "~fact"}},
	// more expressions:
	{"~expr2", {"~expr", "+", "~term"}},
	{"~expr3", {"~expr", "-", "~term"}},
	{"~expr4", {"~expr", "[", "~expr", "]"}},
	{"~expr5", {"~expr", "+=", "~term"}},
	{"~expr6", {"~expr", "-=", "~term"}},
	{"~expr7", {"~expr", "==", "~term"}},
	{"~expr8", {"~expr", "!=", "~term"}},
	{"~exprB", {"~expr", "?", "~expr", ":", "~expr"}},
	// variables:
	{"~decl1", {"/TYPE", "/ID"}},
	{"~decl2", {"/ID", "/ID"}},
	{"~decl3", {"~decl", "(", "~eee", ")"}},
	// special declarations:
	{"~speddecl1", {"const", "~decl"}},
	{"~speddecl2", {"inline", "~decl"}},
	{"~speddecl3", {"infile", "~decl"}},
	{"~speddecl4", {"public", "~decl"}},
	{"~speddecl5", {"public", "static", "~decl"}},
	{"~speddecl6", {"private", "~decl"}},
	{"~speddecl7", {"private", "static", "~decl"}},
	{"~speddecl8", {"protected", "~decl"}},
	{"~speddecl9", {"protected", "static", "~decl"}},
	// more variables:
	{"~decl4", {"~speddecl"}},
	{"~decl5", {"~decl", "[", "]"}},
	{"~decl6", {"/TYPE", "#", "/ID"}},
	{"~decl7", {"/ID", "#", "/ID"}},
	{"~var1", {"~decl", "=", "~expr"}},
	// class:
	{"~class1", {"~decl", "~scope"}},
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
	// more if statements:
	{"~ifstart1", {"if", "("}},
	// more while statements:
	{"~whilestart1", {"while", "("}},
	// more for statements:
	{"~forstart1", {"for", "("}},
	// factors:
	{"~fact1", {"-", "-", "/ID"}},
	{"~fact2", {"+", "+", "/ID"}},
	{"~fact3", {"/ID", "-", "-"}},
	{"~fact4", {"/ID", "+", "+"}},
	{"~fact5", {"/ID"}},
	{"~fact6", {"/LIT"}},
	{"~fact7", {"(", "~expr", ")"}},
	{"~fact8", {"~funcal"}},
	{"~fact9", {"true"}},
	{"~factA", {"false"}},
	// terms:
	{"~term2", {"~fact"}},
	// expressions:
	{"~expr1", {"~term"}},
	{"~expr9", {"~expr", "&&", "~expr"}},
	{"~exprA", {"~expr", "||", "~expr"}},
	{"~exprC", {"~expr", ">=", "~expr"}},
	{"~exprD", {"~expr", "<=", "~expr"}},
	{"~exprE", {"~expr", "<", "~expr"}},
	{"~exprF", {"~expr", ">", "~expr"}},
	{"~eee1", {"~expr", ",", "~expr"}},
	{"~eee2", {"~eee", ",", "~expr"}},
	{"~eee3", {"~eee", ",", "~eee"}},
	// if statements:
	{"~ifhead1", {"~ifstart", "~expr", ")"}},
	{"~ifstate1", {"~ifhead", "~scope"}},
	{"~ifstate2", {"~ifhead", "~state"}},
	{"~elstart1", {"else"}},
	{"~elstate1", {"~elstart", "~scope"}},
	{"~elstate2", {"~elstart", "~state"}},
	// scopes:
	{"~scope1", {"{", "~sss", "}"}},
	{"~scope2", {"{", "~state", "}"}},
	{"~scope3", {"{", "~fundef", "}"}},
	{"~scope4", {"{", "}"}},
	// while loops:
	{"~whilehead1", {"~whilestart", "~expr", ")"}},
	{"~whilestate1", {"~whilehead", "~scope"}},
	{"~whilestate2", {"~whilehead", "~state"}},
	// for loops:
	{"~forheads11", {"~forstart", "~state"}},
	{"~forheads21", {"~forheads1", "~state"}},
	{"~forhead1", {"~forheads2", "~expr", ")"}},
	{"~forstate1", {"~forhead", "~scope"}},
	{"~forstate2", {"~forhead", "~state"}},
	// statements:
	{"~state1", {"~var", ";"}},
	{"~state2", {"~assign", "~expr", ";"}},
	{"~state3", {"~decl", ";"}},
	{"~state4", {"return", "~expr", ";"}},
	{"~state5", {"~expr", ";"}},
	{"~state6", {"~ifstate"}},
	{"~state7", {"~elstate"}},
	{"~state8", {"~whilestate"}},
	{"~state9", {"~forstate"}},
	{"~stateA", {"~import"}},
	{"~stateB", {"~using", ";"}},
	{"~stateC", {"~scope"}},
	{"~stateD", {";"}},
	{"~sss1", {"~state", "~state"}},
	{"~sss2", {"~state", "~fundef"}},
	{"~sss3", {"~sss", "~state"}},
	{"~sss4", {"~sss", "~fundef"}},
	{"~sss5", {"~sss", "~sss"}},
};
/*
* Precendent for each rule in GRAMMAR. Patterns that come before take more precedence.
*/
const std::string GRAMMAR_PRECEDENCE[] = {
	"/ID1",
	"/ID2",
	"~import1",
	"~using1",
	"~factB",
	"~factC",
	"~term1",
	"~term3",
	"~term4",
	"~term5",
	"~term6",
	"~expr2",
	"~expr3",
	"~expr4",
	"~expr5",
	"~expr6",
	"~expr7",
	"~expr8",
	"~exprB",
	"~decl1",
	"~decl2",
	"~decl3",
	"~speddecl1",
	"~speddecl2",
	"~speddecl3",
	"~speddecl4",
	"~speddecl5",
	"~speddecl6",
	"~speddecl7",
	"~speddecl8",
	"~speddecl9",
	"~decl4",
	"~decl5",
	"~decl6",
	"~decl7",
	"~var1",
	"~class1",
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
	"~ifstart1",
	"~whilestart1",
	"~forstart1",
	"~fact1",
	"~fact2",
	"~fact3",
	"~fact4",
	"~fact5",
	"~fact6",
	"~fact7",
	"~fact8",
	"~fact9",
	"~factA",
	"~term2",
	"~expr1",
	"~expr9",
	"~exprA",
	"~exprB",
	"~exprC",
	"~exprD",
	"~exprE",
	"~exprF",
	"~eee1",
	"~eee2",
	"~eee3",
	"~ifhead1",
	"~ifstate1",
	"~ifstate2",
	"~elstart1",
	"~elstate1",
	"~elstate2",
	"~scope1",
	"~scope2",
	"~scope3",
	"~scope4",
	"~whilehead1",
	"~whilestate1",
	"~whilestate2",
	"~forheads11",
	"~forheads21",
	"~forhead1",
	"~forstate1",
	"~forstate2",
	"~state1",
	"~state2",
	"~state3",
	"~state4",
	"~state5",
	"~state6",
	"~state7",
	"~state8",
	"~state9",
	"~stateA",
	"~stateB",
	"~stateC",
	"~stateD",
	"~sss1",
	"~sss2",
	"~sss3",
	"~sss4",
	"~sss5",
};

// contains all "types" seperate form of keywords.
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

extern std::string code;

static void _condense_imports(SyntaxTreeNode& x) {
	if (x.name.value != "~import") return;
	const std::string& raw_filename = x.children.back().get_closest_token().value;
	const std::string filename_string = raw_filename + ".cf";
	const char* filename = filename_string.c_str();

	std::ifstream file(filename);
	if (!file.is_open()) {
		const std::string message = "File \"" + filename_string + "\" not found.";
		ERROR(error_type::IMPORTED_FILE_NOT_FOUND, message.c_str(), x.children.back().get_closest_token().pos, filename, code);
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string insert_code = buffer.str();
	file.close();

	Lexer lexer(insert_code, filename);
	lexer.tokenize();
	lexer.condense();
	std::vector<Token>* tokens = lexer.return_tokens();

	Parser parser(tokens, insert_code, filename);
	parser.syntactical_analysis();
	// condense all import and using statements:
	parser.condense_imports();
	SyntaxTreeNode tree = parser.get_syntax_tree();
	// proccess tree further:
	SyntaxTreeNode father; father.name = Token(REDUCED, "~class", Position(0, 0));
	SyntaxTreeNode file_class; file_class.name = Token(REDUCED, "~decl", Position(0, 0));
	SyntaxTreeNode class1; class1.name = Token(KEYWORD, "class", Position(0, 0));
	SyntaxTreeNode class2; class2.name = Token(KEYWORD, raw_filename, Position(0, 0));
	SyntaxTreeNode scope; scope.name = Token(REDUCED, "~scope", Position(0, 0));
	SyntaxTreeNode open_curly; open_curly.name = Token(OPERATOR, "{", Position(0, 0));
	SyntaxTreeNode close_curly; close_curly.name = Token(OPERATOR, "}", Position(0, 0));
	scope.children.push_back(open_curly);
	scope.children.push_back(tree.children[0]); // to ignore ~prog part
	scope.children.push_back(close_curly);
	file_class.children.push_back(class1);
	file_class.children.push_back(class2);
	father.children.push_back(file_class);
	father.children.push_back(scope);

	x = father; // import statement replaced by class equivalent of file.
}

void Parser::condense_imports()
{
	father.traverse_left_right(&_condense_imports);
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

Token& SyntaxTreeNode::get_farthest_token()
{
	if (children.size() == 0) return name;
	else {
		return children.back().get_closest_token();
	}
}

void SyntaxTreeNode::traverse_left_right(std::function<void(SyntaxTreeNode&)> f)
{
	if (children.size() == 0) return;
	else {
		for (SyntaxTreeNode& i : children) {
			if (i.children.size() != 0) i.traverse_left_right(f);
			f(i);
		}
	}
}

bool SyntaxTreeNode::contains_token(Token& token)
{
	if (children.size() == 0) {
		if (name.value == token.value) {
			return true;
		}
	}
	else {
		for (SyntaxTreeNode& i : children) {
			return i.contains_token(token);
		}
	}
	return false;
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
			current_layer.at(j).name.value == "~class" ||
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

Stack<int> scopes;
static void assign_scopes(SyntaxTreeNode& x) {
	if (x.name.value == "{" || x.name.value == "for") scopes.push(scopes.access() + 1);
	else if (x.name.value == "}") scopes.pull();
	x.name.scope = scopes.access();
}

void Parser::semantical_analysis()
// performes semantical analysis
{
	scopes.push(0);
	init(FILE_NAME, code);
	// assign scopes:
	father.traverse_left_right(&assign_scopes);
	// do semantical checks:
	father.traverse_left_right([](SyntaxTreeNode& x) {
		if (x.name.type != REDUCED) return;
		for (const std::string& i : checks[x.name.value]) {
			semantic_checks.at(i)(x);
		}
	});
	// check undeclared variables:
	// multiple declarations in the same scope:
	// misuse of reserved identifiers:
	// attempting to access variable out of scope:
	// type mismatches:
	// parameter type mismatch when calling functions:
	// function return value match the return type:
	// arithemetic operators operate on numeric types or have defined behaviour:
	// condition of if statement resolves to true or false only:
	// exit condition of a loop resolves to true or false only:
}


