//
// Created by Tarazjusz Szulakiewicz on 06/03/2024.
//

#ifndef UNTITLED1_NODE_H
#define UNTITLED1_NODE_H

#endif //UNTITLED1_NODE_H


typedef struct Node {

    int value;
    int priority;
    int is_operand;
    int is_function;
    int is_function_end_symbol;
    int id;
    int arity;
    struct Node* next;
    struct Node* previous;

} Node;