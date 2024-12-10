#include "../include/check_string.h"
#include <iomanip>

bool check_string_in_DFA(const std::string &input_string,
                         const std::map<std::string, std::vector<std::string>> &dfa_transaction,
                         const std::map<std::string, int> &alphabets,
                         const std::set<std::string> &final_states)
{
    std::string current_state = "S";

    for (char c : input_string)
    {
        std::string char_str(1, c);
        if (alphabets.find(char_str) == alphabets.end())
        {
            return false;
        }
        // std::cout<<"( "<<current_state<<" : "<<char_str<<" ),  ";

        current_state = dfa_transaction.at(current_state).at(alphabets.at(char_str));
    }

    // std::cout<<std::endl;
    return final_states.find(current_state) != final_states.end();
}

void create_transaction_table(std::map<std::string, std::vector<std::string>> &dfa_transaction,
                              std::map<std::string, int> &alphabets,
                              std::set<std::string> &final_states)
{
    std::ifstream inputFile("resources\\files\\dfa_output.txt");
    if (!inputFile)
    {
        std::cerr << "Unable to open file dfa_output.txt" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    std::string word;
    int count = 0;

    std::getline(inputFile, line);
    std::getline(inputFile, line);
    std::stringstream iss(line);
    iss >> word;
    iss >> word;
    while (iss >> word)
    {
        alphabets.insert({word, count++});
    }

    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);

        std::string state;
        iss >> state;

        iss >> word;
        if (word == "Y")
        {
            final_states.insert(state);
        }

        std::vector<std::string> next_state;

        while (iss >> word)
        {
            next_state.push_back(word);
        }

        dfa_transaction.insert({state, next_state});
    }

    inputFile.close();

    std::cout << "Transaction Table: " << std::endl;
    std::cout << "      ";
    for (auto const &row : alphabets)
    {
        std::cout << std::setw(3) << row.first << " ";
    }
    std::cout << std::endl;
    for (auto const &row : dfa_transaction)
    {
        std::cout << std::setw(3) << row.first << " : ";

        for (auto const &col : row.second)
        {
            std::cout << std::setw(3) << col << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Final State(s): ";
    for (auto const &row : final_states)
    {
        std::cout << std::setw(3) << row << " ";
    }
    std::cout << std::endl;
    std::cout << "-----------------------------------------------------------------------------------------" << std::endl;
}

bool is_dfa_exist(std::string reg_exp)
{
    std::string filename = "resources\\files\\dfa_output.txt";
    std::ifstream infile(filename);

    if (!infile)
    {
        std::cerr << "Unable to open file for reading: " << filename << std::endl;
        return false;
    }

    std::string firstLine;
    std::getline(infile, firstLine);
    infile.close();

    if (firstLine.compare(reg_exp) == 0)
    {
        return true;
    }
    else
    {
        std::ofstream outfile(filename);
        if (!outfile)
        {
            std::cerr << "Unable to open file for writing: " << filename << std::endl;
            return 1;
        }

        outfile << reg_exp;
        outfile << "\n";
        outfile.close();
        return false;
    }
}