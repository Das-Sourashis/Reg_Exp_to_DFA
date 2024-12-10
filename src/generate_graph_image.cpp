#include "../include/Hopcroft_minimization.h"

#include <sstream>
#include <cstdlib>


std::string findPythonExecutable() {
    const std::vector<std::string> pythonCommands = {
        "python",
        "python3",
        "py -3"
    };

    for (const auto& cmd : pythonCommands) {
        std::string testCmd = cmd + " --version > nul 2>&1";
        if (system(testCmd.c_str()) == 0) {
            return cmd;
        }
    }
    
    throw std::runtime_error("Python interpreter not found");
}

std::string generateGraphViz(const std::vector<std::vector<std::string>>& dfa) {
    std::ostringstream dotStream;

    dotStream << "digraph DFA {\n"
             << "    rankdir=LR;\n"
             << "    node [shape=circle];\n"
             << "    start [shape=point];\n"
             << "    start -> " << dfa[1][0] << ";\n";

    std::string alphabets = "";
    for(size_t i = 2; i < dfa[0].size(); ++i) {
        alphabets += dfa[0][i];
    }

    for (size_t j = 1; j < dfa.size(); ++j) {
        std::vector<std::string> row = dfa[j];
        std::string state = row[0];
        std::string isFinal = row[1];

        if (isFinal == "Y") {
            dotStream << "    " << state << " [style=filled,  color=lightgrey,  shape=doublecircle,];\n";
        }

        for (size_t i = 2; i < row.size(); ++i) {
            std::string symbol = std::string(1, alphabets[i-2]);
            dotStream << "    " << state << " -> " << row[i] 
                    << " [label=\"" << symbol << "\"];\n";
            
        }
    }

    dotStream << "}\n";
    return dotStream.str();
}

bool writeDotFile(const std::string& filename, const std::string& content) {
    // Open file in binary mode to avoid Windows line ending issues
    std::ofstream outFile(filename, std::ios::out | std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to create file: " << filename << std::endl;
        return false;
    }
    outFile.write(content.c_str(), content.length());
    outFile.close();
    return true;
}

bool generateVisualization(const std::vector<std::vector<std::string>>& dfa) {
    try {
        std::string pythonCmd = findPythonExecutable();
        std::cout << "Using Python command: " << pythonCmd << std::endl;

        // Generate DOT content
        std::string dot_content = generateGraphViz(dfa);
        std::string dotFile = "resources\\files\\temp.dot";
        
        // Print the DOT content for debugging
        // std::cout << "Generated DOT content:" << std::endl;
        // std::cout << dot_content << std::endl;
        
        if (!writeDotFile(dotFile, dot_content)) {
            return false;
        }

        // Create the command using the dot file
        std::string command = pythonCmd + " .\\src\\generate_svg_image_of_graph.py .\\resources\\files\\temp.dot -o .\\resources\\graph_image\\dfa_image.svg";
        
        int result = system(command.c_str());
        if (result != 0) {
            std::cerr << "Failed to execute Python script. Error code: " << result << std::endl;
            return false;
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

// int main() {
//     std::vector<std::vector<std::string>> dfa = {
//         {"states", "is_final", "a", "b"},
//         {"S", "N", "Q1", "Q4"},
//         {"Q0", "Y", "Q0", "Q0"},
//         {"Q1", "N", "Q1", "Q3"},
//         {"Q2", "N", "Q1", "Q0"},
//         {"Q3", "N", "Q0", "Q4"},
//         {"Q4", "N", "Q2", "Q4"}
//     };

//     std::cout << "Starting DFA visualization generation..." << std::endl;

//     if (!generateVisualization(dfa)) {
//         std::cerr << "Failed to generate visualization" << std::endl;
//         return 1;
//     }

//     std::cout << "Successfully generated DFA visualization" << std::endl;
//     return 0;
// }