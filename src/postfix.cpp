#include "../include/postfix.h"
#include <iostream>

std::stack<char> postfix_stack;
std::vector<char> postfix_vector;

std::string modify_reg_exp(std::string reg_exp)
{
    std::string new_reg_exp = "(";
    new_reg_exp += reg_exp;
    new_reg_exp += ")." ;
    new_reg_exp += END_OF_STRING;
    return new_reg_exp;
}

int precedence(char c)
{
    if (c == '+')
    {
        return 1;
    }
    else if (c == '.')
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

void reg_exp_processor(std::string reg_exp)
{
    reg_exp = modify_reg_exp(reg_exp);
    for (char c : reg_exp)
    {
        if ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || c == '*' || c == END_OF_STRING)
        {
            postfix_vector.push_back(c);
        }
        else if (c == '(')
        {
            postfix_stack.push(c);
        }
        else if (c == ')')
        {
            while (postfix_stack.top() != '(')
            {
                postfix_vector.push_back(postfix_stack.top());
                postfix_stack.pop();
            }
            postfix_stack.pop();
        }
        else
        {
            while (!postfix_stack.empty() && postfix_stack.top() != '(' && precedence(c) <= precedence(postfix_stack.top()))
            {
                postfix_vector.push_back(postfix_stack.top());
                postfix_stack.pop();
            }
            postfix_stack.push(c);
        }
    }
    while (!postfix_stack.empty())
    {
        postfix_vector.push_back(postfix_stack.top());
        postfix_stack.pop();
    }

    std::cout<<"Postfix Expression : ";
    for (char c : postfix_vector)
    {
        std::cout << c;
    }
    std::cout << std::endl;
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    expression_tree(postfix_vector);
}

// int main(int argc, char const *argv[])
// {
//     std::string reg_exp = "((a+b)*+(a.c)*).$";
//     reg_exp_processor(reg_exp);
//     return 0;
// }