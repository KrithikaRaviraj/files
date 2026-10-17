# SLR Parser - Binary Digit Counter

A compiler-design project implemented in C that demonstrates lexical analysis, SLR parsing and semantic execution. The program processes a small C-like input language, displays parser construction details and executes logic to calculate the number of binary digits in a decimal integer.

## Features

- **Lexical Analysis:** Tokenise keywords, identifiers, numbers, operators and delimiters.
- **SLR Parsing:** Parse the input program using an SLR parser.
- **FIRST and FOLLOW Sets:** Display grammar-related sets used in parser construction.
- **LR(0) Items:** Construct and display LR(0) item sets.
- **SLR Parsing Table:** Generate and print the parsing table.
- **Syntax Validation:** Detect syntax errors before semantic execution.
- **Symbol Table:** Track declared variables and their values.
- **Semantic Execution:** Execute the binary digit-counting algorithm with an iteration trace.
- **Build Automation:** Compile the project using GCC and the included Makefile.

## Tech Stack

| Technology         | Purpose                                                   |
| ------------------ | --------------------------------------------------------- |
| C                  | Implementation of the lexer, parser and semantic routines |
| GCC                | Compilation                                               |
| GNU Make           | Build automation                                          |
| Standard C library | File handling, input/output and string processing         |

## Project Structure
```
files/
├── main.c
├── lexer.c
├── lexer.h
├── slr_parser.c
├── slr_parser.h
├── semantic.c
├── semantic.h
├── input.txt
├── Makefile
└── README.md
```

## Getting Started

### Prerequisites

- GCC compiler
- GNU Make

### 1. Clone the repository
```
git clone https://github.com/KrithikaRaviraj/files.git
cd files
```

### 2. Compile the project

Build the executable using the supplied Makefile:
```
make
```

Alternatively, compile the source files directly:
```
gcc -Wall -Wextra -g main.c lexer.c slr_parser.c semantic.c -o slr_q7
```

### 3. Run the program

On Linux or macOS:
```
./slr_q7
```

If you compiled using the Makefile, you can also run:
```
make run
```

On Windows, use the generated `slr_q7.exe` executable:
```
.\slr_q7.exe
```

## How It Works

1. **Lexical analysis:** Reads `input.txt` and converts the source into a stream of tokens.
2. **Parser initialisation:** Constructs the grammar-related information needed for SLR parsing.
3. **Parser diagnostics:** Displays FIRST/FOLLOW sets, LR(0) items and the SLR parsing table.
4. **Syntax analysis:** Checks whether the input follows the implemented grammar.
5. **Semantic execution:** If parsing succeeds, prompts for a decimal integer and runs the binary digit-counting algorithm.
6. **Execution trace:** Displays intermediate values and the final symbol table.

## Example

For a decimal input of `13`, the expected binary representation is `1101`, which contains four digits.
```
Enter a decimal number: 13

Number of binary digits = 4
```

The semantic routine repeatedly divides the input by two using integer division while incrementing the digit counter.