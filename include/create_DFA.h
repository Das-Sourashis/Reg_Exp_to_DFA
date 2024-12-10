#ifndef CREATE_DFA_H
#define CREATE_DFA_H

#include <map>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>

#include "follow_and_dfa_row.h"

#define END_OF_STRING '$'

void create_DFA(std::vector<follow_table_row> follow_table, std::set<int> start_state);
void create_final_DFA_table(std::map<std::set<int>, std::map<char, std::set<int>>> dfa_map, std::set<int> start_state, std::set<char> alphabets, int end_char_id);
void print_map(std::set<int> curr_state, std::map<char, std::set<int>> next_states);

extern std::vector<std::vector<std::string>> dfa;

#endif