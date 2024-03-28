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
	{"~import", {"valid_file", "in_global_scope", "add_to_symbols"}},
	{"~using", {"is_defined", "no_rep", "add_to_symbols"}},
	{"~term", {"valid_math", "type_mismatch", "is_defined"}},
	{"~expr", {"valid_math", "type_mismatch", "is_defined"}},
	{"~decl", {"type_mismatch", "no_rep", "is_defined", "reserved_name", "add_to_symbols"}},
	{"~speddecl", {"context_not_oop", "reserved_name", "correct_decl_type"}},
	{"~var", {"type_mismatch", "no_rep", "is_defined", "reserved_name"}},
	{"~class", {"type_mismatch", "no_rep", "reserved_name"}},
	{"~assign", {"is_defined", "const_violation", "reserved_name"}},
	{"~fund", {"no_rep", "reserved_name"}},
	{"~funstart", {"is_defined", "reserved_name"}},
	{"~funcal", {"type_mismatch", "correct_params"}},
	{"~fact", {"type_mismatch", "const_violation", "is_defined"}},
	{"~ifhead", {"expr_not_bool"}},
	{"~elstart", {"not_bool_context"}},
	{"~whilestart", {"expr_not_bool"}},
	{"~forheads2", {"expr_not_bool"}},
	{"~state", {"type_mismatch", "not_function_context"}},
	{"~scope", {"inc_scope"}},
};

struct SyntaxTreeNode {
	Token name;
	std::vector<SyntaxTreeNode> children;
	SyntaxTreeNode* father = nullptr;

	SyntaxTreeNode() {}
	SyntaxTreeNode(Token& n, std::vector<SyntaxTreeNode>& c, SyntaxTreeNode* f);

	void print();
	Token& get_closest_token();
	Token& get_farthest_token();

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