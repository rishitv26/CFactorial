#include "semantics.h"
#include "errors.h"
#include "utils.h"

const char* FILE_NAME;
extern std::string code;

static SymbolTable symbols;
static Stack<int> scopes;
std::string context = "";

void init(const char* file, std::string& c)
{
    FILE_NAME = file;
    scopes.push(0);
}

static void print_symbols() {
    if (symbols.size() == 0) return;
    cout << "{";
    for (Symbol& i : symbols) {
        cout << i.name.value << ", ";
    }
    cout << "\b\b}";
}

Symbol::Symbol(Token& t, bool is_c, bool is_i, bool is_f, int s)
    : name(t), is_const(is_c), is_initialized(is_i), is_file(is_f), scope(s) {}

// =========================================================================== ALL CHECKS:

void valid_file(SyntaxTreeNode& node)
{
    // no need to do anything here...
}

void in_global_scope(SyntaxTreeNode& node)
{
    if (scopes.access() != 0) {
        ERROR(error_type::NOT_IN_GLOBAL_SCOPE,
            "Import statement not in global scope."
            , node.children[1].name.pos, FILE_NAME, code);
    }
}

void is_defined(SyntaxTreeNode& node)
{
    node.traverse_left_right([](SyntaxTreeNode& x) {
        if (x.name.type != IDENTIFIER) return; // dont check since its not an identifier
        for (Symbol& i : symbols) {
            if (x.name == i.name && i.name.scope >= x.name.scope) return; // it is defined somewhere
        }
        const std::string message = "Token \"" + x.name.value + "\" not found.";
        ERROR(error_type::TOKEN_NOT_DEFINED, message.c_str(), x.name.pos, FILE_NAME, code);
    });
}

void inc_scope(SyntaxTreeNode& node)
{
    scopes.push(scopes.access() + 1);
}

void no_rep(SyntaxTreeNode& node)
{
    node.traverse_left_right([](SyntaxTreeNode& x) {
        print_symbols();
        cout << " at " << x.name.value << endl;
        if (x.name.type != IDENTIFIER) return; // dont check since its not an identifier
        for (Symbol& i : symbols) {
            if (x.name.pos == i.name.pos) return; // already checked, dont check again
            if (x.name.value == i.name.value && i.name.scope == x.name.scope) {
                const std::string message = "Token \"" + x.name.value + "\" has already been defined in the same scope.";
                ERROR(error_type::REPEATED_DEFINITION, message.c_str(), x.name.pos, FILE_NAME, code);
            }
        }
    });
}

void add_to_symbols(SyntaxTreeNode& node)
{
    // CAN ONLY ADD RAW DECLARATIONS AS NEW SYMBOLS!
    if (node.name.value != "~decl") return;
    if (node.children.size() < 2) return;
    Symbol s;
    // find id to store:
    Token store = node.get_farthest_token();
    s.name = store;
    s.is_const = (node.father->get_closest_token().value == "const") ? true : false;
    s.is_file = (node.father->name.value == "~import") ? true : false;
    s.is_initialized = (node.father->name.value == "~assign" || node.father->name.value == "~var"
                        || node.father->name.value == "~fundef" || node.father->name.value == "~class") ? true : false;
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
            if (x.name.value == "super") {
                const std::string message = "Token \"" + x.name.value + "\" is a reserved identifier";
                ERROR(error_type::REPEATED_DEFINITION, message.c_str(), x.name.pos, FILE_NAME, code);
            }
            if (x.name.value == "this") {
                const std::string message = "Token \"" + x.name.value + "\" is a reserved identifier";
                ERROR(error_type::REPEATED_DEFINITION, message.c_str(), x.name.pos, FILE_NAME, code);
            }
        }
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
    //node.traverse_left_right([](SyntaxTreeNode& x) {
    //    if (x.name.type != IDENTIFIER) return; // dont check since its not an identifier
    //    for (Symbol& i : symbols) {
    //        if (x.name.value == "super") {
    //            const std::string message = "Token \"" + x.name.value + "\" is a reserved identifier";
    //            ERROR(error_type::REPEATED_DEFINITION, message.c_str(), x.name.pos, FILE_NAME, code);
    //        }
    //        if (x.name.value == "this") {
    //            const std::string message = "Token \"" + x.name.value + "\" is a reserved identifier";
    //            ERROR(error_type::REPEATED_DEFINITION, message.c_str(), x.name.pos, FILE_NAME, code);
    //        }
    //    }
    //    });
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

void bad_use_of_tokens(SyntaxTreeNode& node)
{
    if (node.name.value != "~speddecl") return;
    node.traverse_left_right([](SyntaxTreeNode& x) {
        if (x.name.value == "~speddecl") {
            const std::string message = "Special initialization is used badly.";
            ERROR(error_type::REPEATED_DEFINITION, message.c_str(), x.children[0].name.pos, FILE_NAME, code);
        }
    });
}
