# DFA Minimization Project

## Overview
This project implements a Direct Method for converting a regular expression into a Deterministic Finite Automaton (DFA). It then minimizes the DFA using Hopcroft's algorithm and generates an SVG image of the DFA using the QuickChart API. Finally, the project checks if a given input string is accepted by the DFA.

## Features
- Converts regular expressions to DFA using a direct method
- Minimizes the DFA using Hopcroft's algorithm
- Generates a visual representation of the DFA as an SVG image via QuickChart API
- Checks if a specified input string is accepted by the DFA

## Regular Expression Format
- **Alphabets:** `[a-z]`, `[A-Z]`
- **Operators:**
  - `*`: Kleene Star (zero or more repetitions)
  - `.`: Concatenation (explicit concatenation operator, required between symbols)
  - `+`: Union (alternation between two expressions)
- **Parentheses:**
  - `(`, `)`: For grouping expressions, to control the order of operations

### Important Note
The concatenation operator `.` is mandatory between symbols. For example:
- ✅ `a.b*` - Correct: represents "a" followed by zero or more "b"
- ❌ `ab*` - Incorrect: will result in an error
- ✅ `(a.b)*` - Correct: zero or more repetitions of "ab"
- ❌ `(ab)*` - Incorrect: will result in an error

### Example Regular Expressions
- `a.b*`: The letter "a" followed by zero or more occurrences of "b"
- `a+b`: The letter "a" or the letter "b"
- `(a.b)+c`: Either "a" concatenated with "b", or "c"
- `(a+b)*`: Zero or more repetitions of "a" or "b"

## Project Structure
```
Reg_Exp_to_DFA/
│
├── bin/                            # Compiled executable will be placed here
├── include/                        # Header files
├── src/                            # Source files
│   ├── main.cpp                    # Main program file
│   ├── postfix.cpp                 # Postfix expression handling
│   ├── expression_tree.cpp         # Expression tree implementation
│   ├── create_DFA.cpp              # DFA creation from regex
│   ├── Hopcroft_minimization.cpp   # Hopcroft's DFA minimization algorithm
│   ├── generate_graph_image.cpp    # Call Python file for SVG generation
│   ├── generate_svg_image.py       # Generate SVG image via QuickChart API
│   └── check_string.cpp            # String acceptance checking
├── resources/
│   ├── files/
│   │   ├── dfa_output.txt          # Output of the generated DFA
│   │   └── regex.txt               # Input regular expression
│   └── graph_images/               # Directory for generated SVG images
├── tutorials/                      # Documentation and guides
└── compile.bat                     # Batch file for compiling the project
```

## Installation and Setup

### Prerequisites
- C++ compiler (supporting C++11 or later)
- Python 3.x (for SVG generation via QuickChart API)
- Internet connection (required for generating graph visualizations)

### Compilation and Execution
1. Clone the repository:
   ```bash
   git clone https://github.com/Das-sourashis/Reg_Exp_to_DFA.git
   cd Reg_Exp_to_DFA
   ```

2. Compile the project:
   ```bash
   ./compile.bat
   ```

## Usage

1. Place your regular expression in `resources/files/regex.txt`
   - Remember to use the `.` operator for concatenation
   - Example: Write `a.b.b` instead of `abb`
2. Run the compiled executable:
   ```bash
   ./bin/dfa
   ```
3. Check the output:
   - Generated DFA: `resources/files/dfa_output.txt`
   - Visualization: `resources/graph_images/dfa_graph.svg`

### Example Input
```
(a.b)*+c
```

### Example Output
The program will generate:
1. A text file containing the DFA transitions in `dfa_output.txt`
2. An SVG visualization of the minimized DFA using the QuickChart API

## Error Handling
- Invalid regular expressions (missing concatenation operator) will result in an error message
- Malformed input strings will be rejected with appropriate feedback
- Network errors during graph generation will be reported if the API call fails

## Screenshot(s)

_(Add any screenshots here to show the visual output)_


## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
- [QuickChart](https://quickchart.io) for the graph visualization API
- Tutorials and documentation in the `tutorials/` directory