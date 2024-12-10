#ifndef CHECK_STRING_H
#define CHECK_STRING_H

#include "follow_and_dfa_row.h"
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>


bool check_string_in_DFA(const std::string &input_string,
                        const std::map<std::string, std::vector<std::string>> &dfa_transaction,
                        const std::map<std::string, int> &alphabets,
                        const std::set<std::string> &final_states);

void create_transaction_table(  std::map<std::string, std::vector<std::string>> &dfa_transaction,
                                std::map<std::string, int> &alphabets,
                                std::set<std::string> &final_states);

bool is_dfa_exist(std::string reg_exp);


#endif