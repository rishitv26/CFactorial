# C! (CFactorial)

**A systems programming language designed for universal deployment without operating systems with package-manager ecosystem**

> Note that this compiler is still in early development: The frontend (lexer, parser, semantic analysis) is complete. Code generation is not yet implemented. Package managers are still a down-the-road plan

---

## Motivation

C and C++ can compile to binaries without an OS. However, they tend to be painful to work with, especially across different target architectures and learning curves amongst beginners. There's no central package manager and toolchains are often finicky.

Python is the opposite: it is very easy to learn, and has a clean and has a defined package manager and toolchain flow (ofc depends on context sometimes, but for most cases). The downside with Python's workflow is that a runtime environment, with usually an OS, is required.

C! is meant to be an intermediary: a language with simple sytax, a unified package manager, that can compile into any system that the user may program the compiler to--with or without an operating system.

---

## Syntax

C! syntax is designed to be clean and explicit. Here's what it currently looks like:

**Functions:**
```cf
void main -> () {
    const int x = 8;
    return x;
}
```

**Functions with parameters:**
```cf
void greet -> (int value, str message) {
    printf("hello");
}
```

**Imports:**
```cf
import utils

void main -> () {
    const int result = utils.compute();
    return result;
}
```

**Supported constructs so far:**
- Primitive types: `int`, `str`, void
- Variable declarations with `const`, `inline`, `unsafe` modifiers
- Functions with the `->` signature syntax
- Classes with `public`, `private`, `protected`, and `static` access modifiers
- Control flow: `if`/`else`, `while`, `for`, `do`
- Expressions: arithmetic, comparison, ternary operator (`? :`), array indexing
- Dot notation for member access
- `import` and `using` statements
- `//` and `/* */` comments

---

## Architecture

The compiler pipeline is structured as follows:

```
Source (.cf) -> Lexer -> Parser -> Semantic Analysis -> [Assembler] -> [Optimizer] -> C++ (binary in the future)
```

| Stage | File | Status |
|---|---|---|
| Lexer | `lexer.cpp` | Complete |
| Parser | `parser.cpp` | Complete |
| Semantic Analysis | `semantics.cpp` | Mostly complete |
| Assembler | `assembler.cpp` | Incomplete |
| Optimizer | `optimizer.cpp` | Incomplete |

### Lexer
Tokenizes raw source into typed tokens: identifiers, keywords, integers, decimals, strings, operators (including compound operators like `->`, `==`, `+=`, `&&`), and syntax symbols. Handles both comment styles and reports errors with file position.

### Parser
Implements a grammar-driven reduction parser. Grammar rules cover the full language surface described above — function definitions, class bodies, expressions, control flow, and more. Produces a syntax tree for the semantic stage. Still needs some work.

### Semantic Analysis
Tracks a symbol table and scope stack. Currently enforces: const violation detection, repeated definition in the same scope, undefined variable access, function vs. class context validation, and import scope rules. Needs more work with complex logic.

---

## Roadmap

The next major milestone is the assembler. For now, it will assemble into C++ and then compile into a binary. Once that is done however, we will migrate to a programmable-assembler that can target any system with or without an operating system assuming it can operate with some form of assembly.

**Longer term:**
- Customized binary output per target system (the original goal)
- A self-hosted compiler (C! compiled by C!)

---

## Building

The project is a Visual Studio solution (`.sln`) targeting Windows. To build:

1. Open `CFactorial.sln` in Visual Studio 2019 or later
2. Build in Debug or Release configuration
3. Run the compiler on a `.cf` source file

Cross-platform build support (CMake) is planned.

---

## Project Status

This is an independent research/engineering project, currently in active early development. The frontend is solid and the language grammar is well-defined. The compiler cannot yet produce runnable output, however that will be targeted extensively in the future.

Contributions, feedback, and discussion are welcome.

---
