#ifndef EXPRESSION_TREE_H
#define EXPRESSION_TREE_H

#include "follow_and_dfa_row.h"
#include "create_DFA.h"

#include <stack>

typedef struct node
{
    char data;
    struct node *left;
    struct node *right;
    int id;
    bool nullable;
    std::set<int> first_pos;
    std::set<int> last_pos;
} node;

node *create_node(char data, node *left, node *right);
void expression_tree(std::vector<char> postfix_expression);
void calculate_first_and_last_pos(node *root);
void print_node(node *root);
void print_tree(node *root);

void create_follow_table(void);
void initialize_table(node *root);
void update_table_by_tree_traversal(node *root);
void print_table();

#endif
