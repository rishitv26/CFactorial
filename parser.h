#ifndef PARSER_CF_H
#define PARSER_CF_H

#include <vector>
#include <string>
#include "lexer.h"
#include <unordered_map>

struct SyntaxTreeNode;
struct SyntaxTree;
typedef std::unordered_map<std::string, std::vector<std::string>> SyntaxGrammerMap;

// defines the type of syntax trees one may have
// can include expression trees, statement trees, etc.
enum class syntax_tree_type {
	EXPRESSION = 0,
};

class Parser {
private:
	std::vector<Token>* tokens;
	std::string& code;
	const char* FILE_NAME;

	void syntactical_analysis();
	void semantical_analysis();
	
public:
	void throw_error(Token& t);
	Parser(std::vector<Token>* t, std::string& _code, const char* _FILE_NAME);

	void validate();
	void add_trees(std::vector<Token>& t, std::vector<std::string>& matches);
};


#endif // PARSER_CF_H