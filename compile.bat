@echo off
if not exist "bin" mkdir bin

g++ -o bin/dfa.exe ^
    src/main.cpp ^
    src/postfix.cpp ^
    src/expression_tree.cpp ^
    src/create_DFA.cpp ^
    src/Hopcroft_minimization.cpp ^
    src/generate_graph_image.cpp ^
    src/check_string.cpp ^
    -I include

if %errorlevel% equ 0 (
    echo Compilation successful! Running program...
    bin\dfa.exe
) else (
    echo Compilation failed.
    pause
)