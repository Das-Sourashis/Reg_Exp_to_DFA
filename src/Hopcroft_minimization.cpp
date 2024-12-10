#include "../include/Hopcroft_minimization.h"

#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <set>

void create_minimize_dfa_table(std::vector<std::vector<std::string>> &dfa, std::set<std::set<std::string>> states)
{
    std::vector<std::vector<std::string>> minimized_dfa;
    // Create header with final state marker
    minimized_dfa.push_back(dfa[0]);

    // Precompute state information -> the next states and final states
    std::unordered_map<std::string, bool> is_final_state;
    std::unordered_map<std::string, std::vector<std::string>> transitions;
    for (size_t i = 1; i < dfa.size(); ++i)
    {
        const std::vector<std::string> &row = dfa[i];
        is_final_state[row[0]] = (row[1] == "Y");
        std::vector<std::string> trans;
        for (size_t j = 2; j < row.size(); ++j)
        {
            trans.push_back(row[j]);
        }
        transitions[row[0]] = trans;
    }

    // Create efficient state set lookup -> old state => new minimize set of state
    std::unordered_map<std::string, std::set<std::string> *> state_to_set;
    for (const std::set<std::string> &set : states)
    {
        for (const std::string &state : set)
        {
            state_to_set[state] = const_cast<std::set<std::string> *>(&set);
        }
    }

    // Create state mapping
    std::map<const std::set<std::string> *, std::string> state_mapping;
    int state_counter = 0;
    bool start_state_assigned = false;

    // First assign start state if it exists
    for (const auto &state_set : states)
    {
        if (state_set.find("S") != state_set.end())
        {
            state_mapping[&state_set] = "S";
            start_state_assigned = true;
            break;
        }
    }

    // Then assign other states
    for (const auto &state_set : states)
    {
        if (!start_state_assigned || state_mapping.find(&state_set) == state_mapping.end())
        {
            state_mapping[&state_set] = "Q" + std::to_string(state_counter++);
        }
    }

    // Create minimized DFA rows
    for (const auto &state_set : states)
    {
        std::vector<std::string> new_row;
        std::string representative = *state_set.begin();

        // Add state name
        new_row.push_back(state_mapping[&state_set]);

        // Add final state marker
        new_row.push_back(is_final_state[representative] ? "Y" : "N");

        // Add transitions
        const auto &rep_transitions = transitions[representative];
        for (const auto &next_state : rep_transitions)
        {
            new_row.push_back(state_mapping[state_to_set[next_state]]);
        }

        minimized_dfa.push_back(new_row);
    }

    // Sort rows (S first, then other states)
    std::sort(minimized_dfa.begin() + 1, minimized_dfa.end(),
              [](const std::vector<std::string> &a, const std::vector<std::string> &b)
              {
                  if (a[0] == "S")
                      return true;
                  if (b[0] == "S")
                      return false;
                  return a[0] < b[0];
              });

    // print minimize states
    std::cout << "\033[38;5;119m" << "\nMinimized States:\n"
              << std::endl;
    for (auto states : state_mapping)
    {
        std::cout << "{ ";
        for (auto state : *states.first)
        {
            std::cout << state << " ";
        }
        std::cout << "} : " << states.second << std::endl;
    }
    std::cout << "-----------------------------------------------------------------" << "\033[0m" << std::endl;

    // Print the minimized DFA
    std::cout << "\033[38;5;190m" << "\nMinimized DFA Table:\n"
              << std::endl;
    for (const auto &row : minimized_dfa)
    {
        for (const auto &cell : row)
        {
            std::cout << std::setw(10) << cell << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << "-----------------------------------------------------------------" << "\033[0m" << std::endl;

    dfa = minimized_dfa;
}

void write_in_file(const std::vector<std::vector<std::string>> &dfa)
{
    std::string filename = "resources\\files\\dfa_output.txt";

    std::ofstream outfile(filename, std::ios::app);

    if (!outfile)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (const auto &row : dfa)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            outfile << row[i];
            if (i < row.size() - 1)
            {
                outfile << "\t";
            }
        }
        outfile << "\n";
    }

    outfile.close();
}

// Hopcroft's DFA minimization Algorithm
std::string get_Next_State(const std::string &state, char symbol, const std::vector<std::vector<std::string>> &dfa)
{
    for (const auto &row : dfa)
    {
        if (row[0] == state)
        {
            for (size_t i = 2; i < row.size(); i++)
            {
                if (dfa[0][i] == std::string(1, symbol))
                {
                    return row[i];
                }
            }
        }
    }
    return "";
}

void Hopcroft_minimization(std::vector<std::vector<std::string>> dfa)
{
    std::string alphabets = "";
    for (size_t i = 2; i < dfa[0].size(); i++)
    {
        alphabets += dfa[0][i];
    }

    // Initialize final and non-final states
    std::set<std::string> final_states, non_final_states;
    for (const std::vector<std::string> &row : dfa)
    {
        if (row[0] != "states")
        {
            if (row[1] == "Y")
            {
                final_states.insert(row[0]);
            }
            else
            {
                non_final_states.insert(row[0]);
            }
        }
    }

    // Initialize partition
    std::set<std::set<std::string>> state_set;
    state_set.insert(final_states);
    state_set.insert(non_final_states);

    // Initialize waiting set W
    std::list<std::pair<std::set<std::string>, char>> W;
    std::set<std::string> choose_smaller_set = (final_states.size() > non_final_states.size()) ? non_final_states : final_states;
    for (char c : alphabets)
    {
        W.push_back(std::make_pair(choose_smaller_set, c));
    }

    std::pair<std::set<std::string>, char> popped_element;
    ;

    // Main algorithm loop
    while (!W.empty())
    {
        popped_element = W.front();
        W.pop_front();

        std::set<std::set<std::string>> new_state_set;
        bool is_changed = false;

        for (const auto &C : state_set)
        {
            std::set<std::string> c_dash, c_dash_dash;

            for (const auto &state : C)
            {
                std::string next_state = get_Next_State(state, popped_element.second, dfa);

                if (popped_element.first.find(next_state) != popped_element.first.end())
                {
                    c_dash.insert(state);
                }
                else if (!next_state.empty())
                {
                    c_dash_dash.insert(state);
                }
            }

            if (!c_dash.empty() && !c_dash_dash.empty())
            {
                is_changed = true;
                new_state_set.insert(c_dash);
                new_state_set.insert(c_dash_dash);

                choose_smaller_set = (c_dash.size() > c_dash_dash.size()) ? c_dash_dash : c_dash;

                for (char symbol : alphabets)
                {
                    W.push_back(std::make_pair(choose_smaller_set, symbol));
                }
            }
            else
            {
                new_state_set.insert(C);
            }
        }
        if (is_changed)
        {
            state_set = new_state_set;
        }
    }

    create_minimize_dfa_table(dfa, state_set);
    write_in_file(dfa);

    // Generate Image
    std::cout << "Starting DFA visualization generation..." << std::endl;

    if (!generateVisualization(dfa)) {
        std::cerr << "\033[31m" << "Failed to generate visualization. (Maybe INTERNET PROBLEM or other issue.)" << "\033[0m"<<std::endl;
    } else{
        std::cout << "\033[32m" << "Successfully generated DFA visualization"<<"\033[0m" << std::endl;
    }

}

// int main()
// {
//     // Example DFA definition based on the table provided
//     std::vector<std::vector<std::string>> dfa = {
//         {"states", "is_final", "a", "b"},
//         {"M0", "N", "M0", "M2"},
//         {"M1", "N", "M0", "F2"},
//         {"F0", "Y", "F1", "F2"},
//         {"F1", "Y", "F1", "F2"},
//         {"M2", "N", "F0", "M4"},
//         {"F2", "Y", "F0", "F3"},
//         {"S", "N", "M0", "M4"},
//         {"M4", "N", "M1", "M4"},
//         {"F3", "Y", "F0", "F3"}};

//     Hopcroft_minimization(dfa);

//     return 0;
// }