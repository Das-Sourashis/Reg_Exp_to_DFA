#include "../include/create_DFA.h"
#include "../include/Hopcroft_minimization.h"
#include <queue>

std::vector<std::vector<std::string>> dfa;

void print_map(std::set<int> curr_state, std::map<char, std::set<int>> next_states)
{
    std::cout << "{";
    for (auto it : curr_state)
    {
        std::cout << it + 1 << " ";
    }
    std::cout << "} : [";
    for (auto it : next_states)
    {
        std::cout << it.first << " : { ";
        for (auto it2 : it.second)
        {
            std::cout << it2 + 1 << " ";
        }
        std::cout << "} , ";
    }
    std::cout << "]" << std::endl;
}

void create_final_DFA_table(std::map<std::set<int>, std::map<char, std::set<int>>> dfa_map, std::set<int> start_state, std::set<char> alphabets, int end_char_id)
{
    std::map<std::set<int>, std::string> state_map;
    std::vector<int> state_number(2, 0);

    state_map.insert({start_state, "S"});
    for (const auto &it : dfa_map)
    {
        if (it.first.find(end_char_id) != it.first.end())
        {
            // If it's a final state
            state_map.insert({it.first, "F" + std::to_string(state_number[1])});
            state_number[1]++;
        }
        else
        {
            // If it's a non-final state
            state_map.insert({it.first, "M" + std::to_string(state_number[0])});
            state_number[0]++;
        }
    }

    //
    for (auto it : state_map)
    {
        std::cout << it.second << " : ";
        for (auto it2 : it.first)
        {
            std::cout << it2 + 1 << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    //

    std::vector<std::string> head{"states", "is_final"};
    for (auto it : alphabets)
    {
        head.push_back(std::string(1, it));
    }

    dfa.push_back(head);

    for (auto it : dfa_map)
    {

        std::vector<std::string> row;
        row.push_back(state_map[it.first]);
        if (it.first.find(end_char_id) != it.first.end())
        {
            row.push_back("Y");
        }
        else
        {
            row.push_back("N");
        }

        for (auto it2 : it.second)
        {
            row.push_back(state_map[it2.second]);
        }

        dfa.push_back(row);
    }

    const std::string YELLOW = "\033[33m"; // Yellow text
    std::cout << YELLOW;
    for (const auto &row : dfa)
    {
        for (const auto &entry : row)
        {
            std::cout << std::setw(10) << entry; // Align each entry
        }
        std::cout << std::endl;
    }
    std::cout << "\033[0m" << "-----------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    // Minimize the generated DFA.
    Hopcroft_minimization(dfa);
}

void create_DFA(std::vector<follow_table_row> follow_table, std::set<int> start_state)
{
    std::queue<std::set<int>> q;
    std::set<char> alphabets;
    std::map<std::set<int>, std::map<char, std::set<int>>> dfa_map;

    for (follow_table_row row : follow_table)
    {
        alphabets.insert(row.data);
    }
    alphabets.erase(END_OF_STRING);

    q.push(start_state);

    while (!q.empty())
    {
        std::set<int> curr_state = q.front();
        q.pop();

        std::map<char, std::set<int>> next_states;
        for (char c : alphabets)
        {
            next_states.insert({c, {}});
        }

        for (follow_table_row row : follow_table)
        {
            if (curr_state.find(row.id) != curr_state.end() && row.data != END_OF_STRING)
            {
                next_states[row.data].insert(row.follow_set.begin(), row.follow_set.end());
            }
        }

        dfa_map.insert({curr_state, next_states});

        for (auto it : next_states)
        {
            if (dfa_map.find(it.second) == dfa_map.end())
            {
                q.push(it.second);
            }
        }
    }

    for (auto it : dfa_map)
    {
        print_map(it.first, it.second);
    }
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    int end_char_id;
    for (follow_table_row row : follow_table)
    {
        if (row.data == END_OF_STRING)
        {
            end_char_id = row.id;
        }
    }
    create_final_DFA_table(dfa_map, start_state, alphabets, end_char_id);
}
