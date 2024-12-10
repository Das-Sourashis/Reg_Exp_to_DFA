#ifndef FOLLOW_AND_DFA_ROW_H
#define FOLLOW_AND_DFA_ROW_H

#include <set>

typedef struct follow_table_row
{
    int id;
    char data;
    std::set<int> follow_set;
} follow_table_row;

typedef struct dfa_row
{
    std::set<int> state;
    char symbol;
    char type;

} dfa_row;

#endif