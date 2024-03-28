#ifndef PARSER_CF_H
#define PARSER_CF_H

#include <vector>
#include <string>
#include <functional>
#include <map>
#include "lexer.h"

typedef std::map<std::string, std::vector<std::string>> SyntaxGrammerMap;
typedef std::map<std::string, std::vector<std::string>> SemanticChecksMap;

// semantical checks map.
// contains all semantical checks to be performed on each type of reduced token.
static SemanticChecksMap checks = {
	{"~import", {"valid_file"}},
	{"~using", {"valid_id", "is_defined", "no_rep"}},
	{"~term", {"valid_math", "type_mismatch", "is_defined"}},
	{"~expr", {"valid_math", "type_mismatch", "is_defined"}},
	{"~decl", {"type_mismatch", "no_rep", "is_defined", "reserved_name"}},
	{"~speddecl", {"context_not_oop", "reserved_name", "correct_decl_type"}},
	{"~var", {"type_mismatch", "no_rep", "is_defined", "reserved_name"}},
	{"~class", {"type_mismatch", "no_rep", "reserved_name"}},
	{"~assign", {"is_defined", "const_violation", "reserved_name"}},
	{"~fund", {"is_defined", "no_rep", "reserved_name"}},
	// todo...
};

// defines the type of syntax trees one may have
// can include expression trees, statement trees, etc.
enum class syntax_tree_type {
	EXPRESSION = 0,
};

struct SyntaxTreeNode {
	Token name;
	std::vector<SyntaxTreeNode> children;
	SyntaxTreeNode* father = nullptr;

	SyntaxTreeNode() {}
	SyntaxTreeNode(Token& n, std::vector<SyntaxTreeNode>& c, SyntaxTreeNode* f);

	void print();
	Token& get_closest_token();

	void traverse_left_right(std::function<void(SyntaxTreeNode&)> f);
};

class Parser {
private:
	std::vector<Token>* tokens;
	SyntaxTreeNode father;
	std::vector<SyntaxTreeNode> current_layer;
	std::string& code;
	const char* FILE_NAME;

	void syntactical_analysis();
	void semantical_analysis();
	
public:
	void throw_error(Token& t);
	Parser(std::vector<Token>* t, std::string& _code, const char* _FILE_NAME);

	std::vector< SyntaxTreeNode>& return_current_layer();
	void set_father(SyntaxTreeNode& f);
	SyntaxTreeNode& get_syntax_tree();

	void validate();
};


#endif // PARSER_CF_H