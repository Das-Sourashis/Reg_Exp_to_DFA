#ifndef HOPCROFT_MINIMIZATION_H
#define HOPCROFT_MINIMIZATION_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>


// Hopcroft_minimization.cpp
void Hopcroft_minimization(std::vector<std::vector<std::string>> dfa);

// generate_graph_image.cpp
bool generateVisualization(const std::vector<std::vector<std::string>>& dfa);


#endif