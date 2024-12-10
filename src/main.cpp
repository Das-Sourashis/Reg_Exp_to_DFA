#include <fstream>
#include <limits>

#include "../include/check_string.h"
#include "../include/postfix.h"

const std::string RESET = "\033[0m";  // Reset to default color
const std::string RED = "\033[31m";   // Red text
const std::string GREEN = "\033[32m"; // Green text
const std::string CYAN = "\033[36m";  // Cyan text

// alphabets are [a-z] , [A-Z] . operators are '*', '.' and '+',


std::string read_reg_exp_from_file(std::string file_name)
{
    std::ifstream file(file_name);
    if (file.is_open())
    {
        std::string line;
        std::getline(file, line);
        file.close();
        std::cout << std::endl;
        std::cout << line << std::endl;
        std::cout << "-----------------------------------------------------------------" << std::endl;
        return line;
    }
    else
    {
        std::cout << "Unable to open file" << std::endl;
        return "";
    }
}

int main()
{
    std::string reg_exp = read_reg_exp_from_file("resources\\files\\regex.txt");
    if (!is_dfa_exist(reg_exp) && reg_exp != "")
    {
        reg_exp_processor(reg_exp);
    }

    std::map<std::string, std::vector<std::string>> dfa_transaction;
    std::map<std::string, int> alphabets;
    std::set<std::string> final_states;

    create_transaction_table(dfa_transaction, alphabets, final_states);

    // check string if it if accepted by dfa
    char yes_no = 'y';
    std::string input_string;
    do
    {
        std::cout << "Enter string: ";
        std::getline(std::cin, input_string);
        std::cout << ::std::endl;

        if (check_string_in_DFA(input_string, dfa_transaction, alphabets, final_states))
        {
            std::cout << GREEN << input_string << " : Accepted by DFA" << std::endl;
        }
        else
        {
            std::cout << RED << input_string << " : Not Accepted by DFA" << std::endl;
        }

        std::cout << CYAN << "Do you want to check another string? (y/n): ";
        std::cin >> yes_no;
        std::cout << RESET;
        std::cout << "-----------------------------------------------------------------" << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    } while (yes_no == 'y' || yes_no == 'Y');

    return 0;
}