#include "semantics.h"
#include "errors.h"
#include "utils.h"

#ifdef _WIN32
    #include <io.h> 
    #define a _access_s
#else
    #include <unistd.h>
    #define a access
#endif

const char* FILE_NAME;
std::string CODE;

static bool file_exists(const std::string& filename)
{
    return a(filename.c_str(), 0) == 0;
}

static SymbolTable symbols;
static Stack<int> scopes;

void init(const char* file, std::string& c)
{
    FILE_NAME = file;
    CODE = c;
    scopes.push(0);
}

Symbol::Symbol(Token& t, bool is_c, bool is_i, bool is_f, int s)
    : name(t), is_const(is_c), is_initialized(is_i), is_file(is_f), scope(s) {}

// =========================================================================== ALL CHECKS:

void valid_file(SyntaxTreeNode& node)
{
    if (node.children[0].name.value != "import") return;
    
    else if (!file_exists(node.children[1].name.value)) {
        const std::string message = "File \"" + node.children[1].name.value + "\" not found.";
        ERROR(error_type::IMPORTED_FILE_NOT_FOUND, message.c_str(), node.children[1].name.pos, FILE_NAME, CODE);
    }

    symbols.push_back({ node.children[1].name, 0, 0, 1, scopes.access() });
}

void in_global_scope(SyntaxTreeNode& node)
{
    if (scopes.access() != 0) {
        ERROR(error_type::NOT_IN_GLOBAL_SCOPE,
            "Import statement not in global scope."
            , node.children[1].name.pos, FILE_NAME, CODE);
    }
}

void is_defined(SyntaxTreeNode& node)
{
    node.traverse_left_right([](SyntaxTreeNode& x) {
        if (x.name.type != IDENTIFIER) return; // dont check since its not an identifier
        for (Symbol& i : symbols) {
            if (x.name == i.name && i.name.scope != x.name.scope) return; // it is defined somewhere
        }
        const std::string message = "Token \"" + x.name.value + "\" not found.";
        ERROR(error_type::TOKEN_NOT_DEFINED, message.c_str(), x.name.pos, FILE_NAME, CODE);
    });
}

void inc_scope(SyntaxTreeNode& node)
{
    scopes.push(scopes.access() + 1);
}

void no_rep(SyntaxTreeNode& node)
{
    node.traverse_left_right([](SyntaxTreeNode& x) {
        if (x.name.type != IDENTIFIER) return; // dont check since its not an identifier
        for (Symbol& i : symbols) {
            if (x.name == i.name && i.name.scope != x.name.scope) return; // it is defined somewhere else
        }
        const std::string message = "Token \"" + x.name.value + "\" has already been defined in the same scope.";
        ERROR(error_type::REPEATED_DEFINITION, message.c_str(), x.name.pos, FILE_NAME, CODE);
    });
}

void add_to_symbols(SyntaxTreeNode& node)
{
    Symbol s;
    // find id to store:
    Token store = node.get_farthest_token();
    s.name = store;
    s.is_const = (node.father->get_closest_token().value == "const") ? true : false;
    s.is_file = (node.father->name.value == "~import") ? true : false;
    s.is_initialized = (node.father->name.value == "~assign" || node.father->name.value == "~var"
                        || node.father->name.value == "~fund" || node.father->name.value == "~class") ? true : false;
    s.scope = scopes.access();

    symbols.push_back(s);

    if (node.name.value == "~using") {
        // do it again since /ID must also be defined.
        store = node.children.back().name;
        s.name = store;
        s.is_const = (node.father->get_closest_token().value == "const") ? true : false;
        s.is_file = (node.father->name.value == "~import") ? true : false;
        s.is_initialized = (node.father->name.value == "~assign" || node.father->name.value == "~var"
                            || node.father->name.value == "~fund" || node.father->name.value == "~class") ? true : false;
        s.scope = scopes.access();

        symbols.push_back(s);
    }
}

void valid_math(SyntaxTreeNode& node)
{
}

void type_mismatch(SyntaxTreeNode& node)
{
}

void reserved_name(SyntaxTreeNode& node)
{
    node.traverse_left_right([](SyntaxTreeNode& x) {
        if (x.name.type != IDENTIFIER) return; // dont check since its not an identifier
        for (Symbol& i : symbols) {
            if (x.name.value == "typeof") return; // it is defined somewhere else
            if (x.name.value == "cast") return; // it is defined somewhere else
        }
        const std::string message = "Token \"" + x.name.value + "\" has already been defined in the same scope.";
        ERROR(error_type::REPEATED_DEFINITION, message.c_str(), x.name.pos, FILE_NAME, CODE);
        });
}

void context_not_oop(SyntaxTreeNode& node)
{
}

void correct_decl_type(SyntaxTreeNode& node)
{
}

void const_violation(SyntaxTreeNode& node)
{
}

void correct_params(SyntaxTreeNode& node)
{
}

void expr_not_bool(SyntaxTreeNode& node)
{
}

void not_bool_context(SyntaxTreeNode& node)
{
}

void not_function_context(SyntaxTreeNode& node)
{
}

