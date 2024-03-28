#ifndef SEMANTICS_CF_H
#define SEMANTICS_CF_H

#include <map>
#include <string>
#include <functional>
#include <vector>
#include "parser.h"

struct Symbol {
	Token name;
	bool is_const;
	bool is_initialized;
	bool is_file;

	int scope;

	Symbol() {}
	Symbol(Token& t, bool is_c, bool is_i, bool is_f, int scope);
};

typedef std::map <std::string, std::function<void(SyntaxTreeNode&)>> SemanticChecks;
typedef std::vector<Symbol> SymbolTable;

void init(const char* file, std::string& code);

void valid_file(SyntaxTreeNode& node);
void in_global_scope(SyntaxTreeNode& node);
void is_defined(SyntaxTreeNode& node);
void inc_scope(SyntaxTreeNode& node);
void no_rep(SyntaxTreeNode& node);
void add_to_symbols(SyntaxTreeNode& node);
void valid_math(SyntaxTreeNode& node);
void type_mismatch(SyntaxTreeNode& node);
void reserved_name(SyntaxTreeNode& node);
void context_not_oop(SyntaxTreeNode& node);
void correct_decl_type(SyntaxTreeNode& node);
void const_violation(SyntaxTreeNode& node);
void correct_params(SyntaxTreeNode& node);
void expr_not_bool(SyntaxTreeNode& node);
void not_bool_context(SyntaxTreeNode& node);
void not_function_context(SyntaxTreeNode& node);

static const SemanticChecks semantic_checks = {
	{"valid_file", &valid_file},
	{"in_global_scope", &in_global_scope},
	{"is_defined", &is_defined},
	{"no_rep", &no_rep},
	{"inc_scope", &inc_scope},
	{"add_to_symbols", &add_to_symbols},
	{"valid_math", &valid_math},
	{"type_mismatch", &type_mismatch},
	{"reserved_name", &reserved_name},
	{"context_not_oop", &context_not_oop},
	{"correct_decl_type", &correct_decl_type},
	{"const_violation", &const_violation},
	{"correct_params", &correct_params},
	{"expr_not_bool", &expr_not_bool},
	{"not_bool_context", &not_bool_context},
	{"not_function_context", &not_function_context},
};

#endif // SEMANTICS_CF_H