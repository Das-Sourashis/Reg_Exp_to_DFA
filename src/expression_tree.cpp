#include "../include/expression_tree.h"

node *head = NULL;
int id_count = 0;

std::stack<node *> node_stack;
std::vector<follow_table_row> follow_table;

node *create_node(char data, node *left, node *right)
{
    node *new_Node = new node;
    new_Node->data = data;
    new_Node->left = left;
    new_Node->right = right;
    if ((data <= 'z' && data >= 'a') || (data <= 'Z' && data >= 'A') || data == END_OF_STRING)
    {
        new_Node->id = id_count++;
    }
    else
    {
        new_Node->id = -1;
    }

    return new_Node;
}

void calculate_first_and_last_pos(node *root)
{
    if (root->left == NULL && root->right == NULL)
    {
        root->first_pos.insert(root->id);
        root->last_pos.insert(root->id);
        if ((root->data <= 'z' && root->data >= 'a') || (root->data <= 'Z' && root->data >= 'A') || root->data == END_OF_STRING)
        {
            root->nullable = false;
        }
        return;
    }

    if (root->data == '*')
    {
        calculate_first_and_last_pos(root->left);
        root->first_pos.insert(root->left->first_pos.begin(), root->left->first_pos.end());
        root->last_pos.insert(root->left->last_pos.begin(), root->left->last_pos.end());
        root->nullable = true;
    }
    else
    {
        calculate_first_and_last_pos(root->left);
        calculate_first_and_last_pos(root->right);

        if (root->data == '.')
        {
            root->nullable = (root->left->nullable && root->right->nullable);
            // first position
            if (root->left->nullable)
            {
                root->first_pos.insert(root->left->first_pos.begin(), root->left->first_pos.end());
                root->first_pos.insert(root->right->first_pos.begin(), root->right->first_pos.end());
            }
            else
            {
                root->first_pos.insert(root->left->first_pos.begin(), root->left->first_pos.end());
            }
            // last position
            if (root->right->nullable)
            {
                root->last_pos.insert(root->left->last_pos.begin(), root->left->last_pos.end());
                root->last_pos.insert(root->right->last_pos.begin(), root->right->last_pos.end());
            }
            else
            {
                root->last_pos.insert(root->right->last_pos.begin(), root->right->last_pos.end());
            }
        }
        else if (root->data == '+')
        {
            root->nullable = (root->left->nullable || root->right->nullable);
            root->first_pos.insert(root->left->first_pos.begin(), root->left->first_pos.end());
            root->first_pos.insert(root->right->first_pos.begin(), root->right->first_pos.end());
            root->last_pos.insert(root->left->last_pos.begin(), root->left->last_pos.end());
            root->last_pos.insert(root->right->last_pos.begin(), root->right->last_pos.end());
        }
    }
}

void initialize_table(node *root)
{
    if (root == nullptr)
    {
        return;
    }

    if (root->left == nullptr && root->right == nullptr)
    {
        follow_table_row new_table_row = {root->id, root->data, {}};
        follow_table.push_back(new_table_row);
        return;
    }

    initialize_table(root->left);
    initialize_table(root->right);
}

void update_table_by_tree_traversal(node *root)
{
    if (root == NULL)
    {
        return;
    }

    update_table_by_tree_traversal(root->left);
    update_table_by_tree_traversal(root->right);

    if (root->data == '*')
    {
        for (int i : root->last_pos)
        {
            for (follow_table_row &row : follow_table)
            {
                if (row.id == i)
                {
                    row.follow_set.insert(root->first_pos.begin(), root->first_pos.end());
                }
            }
        }
    }
    else if (root->data == '.')
    {
        for (int i : root->left->last_pos)
        {
            for (follow_table_row &row : follow_table)
            {
                if (row.id == i)
                {
                    row.follow_set.insert(root->right->first_pos.begin(), root->right->first_pos.end());
                }
            }
        }
    }
}

void create_follow_table(void)
{
    initialize_table(head);
    update_table_by_tree_traversal(head);
}

void print_node(node *root)
{
    std::cout << "id: " << root->id << " data: " << root->data << " first_pos: {";
    for (auto it = root->first_pos.begin(); it != root->first_pos.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "}  last_pos: {";
    for (auto it = root->last_pos.begin(); it != root->last_pos.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "}  nullable: " << root->nullable << std::endl;
}

void print_tree(node *root)
{
    if (root == NULL)
    {
        return;
    }
    // print first last position
    print_node(root);
    print_tree(root->left);
    print_tree(root->right);
}

void print_table()
{
    for (follow_table_row row : follow_table)
    {
        std::cout << "id: " << row.id << " data: " << row.data << " follow_set: {";
        for (int it : row.follow_set)
        {
            std::cout << it + 1 << " ";
        }
        std::cout << "}" << std::endl;
    }
}

void expression_tree(std::vector<char> postfix_expression)
{
    for (char c : postfix_expression)
    {
        if ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || c == END_OF_STRING)
        {
            node_stack.push(create_node(c, NULL, NULL));
        }
        else if (c == '*')
        {
            node *temp = create_node(c, node_stack.top(), NULL);
            node_stack.pop();
            node_stack.push(temp);
        }
        else
        {
            node *temp1 = node_stack.top();
            node_stack.pop();
            node *temp2 = node_stack.top();
            node_stack.pop();
            node_stack.push(create_node(c, temp2, temp1));
        }
    }
    head = node_stack.top();
    node_stack.pop();

    calculate_first_and_last_pos(head);
    std::cout<<"Expression Tree : "<<std::endl;
    print_tree(head);
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    create_follow_table();
    std::cout<<"Follow Table : "<<std::endl;
    print_table();
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    create_DFA(follow_table, head->first_pos);
}

// int main(int argc, char const *argv[])
// {
//     std::vector<char> postfix_expression;
//     // ab+*a.b.b.
//     // ab+*ac.*+z.
//     std::string reg_exp = "ab+*ac.*+$.";
//     for (char c : reg_exp)
//     {
//         postfix_expression.push_back(c);
//     }
//     expression_tree(postfix_expression);
//     calculate_first_and_last_pos(head);
//     print_tree(head);
//     std::cout << "-----------------------------------------------------" << std::endl;
//     std::cout << std::endl;
//     create_follow_table();
//     print_table();
//     std::cout << "-----------------------------------------------------" << std::endl;
//     std::cout << std::endl;
//     create_DFA(follow_table, head->first_pos);
//     return 0;
// }
