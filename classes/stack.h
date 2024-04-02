#include "node.h"


class stack {
private:

    node* head;
    node* tail;

    void set_node(int content, int priority, int arity, int function_id, bool operand, bool function, bool last, bool requires_additional_parenthesis, bool method);

public:

    stack();

    node* get_head();

    void put(int content, int priority, int arity, int function_id, bool operand, bool function, bool last, bool requires_additional_parenthesis);

    void push(int content, int priority, int arity, int function_id, bool operand, bool function, bool last, bool requires_additional_parenthesis);

    node* pop();

    void print();

    void clear();

};
