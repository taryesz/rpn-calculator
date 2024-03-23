//
// Created by Tarazjusz Szulakiewicz on 06/03/2024.
//

#ifndef UNTITLED1_LIST_H
#define UNTITLED1_LIST_H

#endif //UNTITLED1_LIST_H

#include "node.h"


// create a list structure
typedef struct {
    Node* head;
    Node* tail;
} List;


// remove the top element of a list
Node* pop(List* stack) {

    // if the stack is empty, there is nothing to pop
    if (stack->head == NULL) return NULL;

    // if the stack is not empty
    else {

        // if there are at least two nodes in the stack
        if (stack->head->next != NULL) {

            stack->head->next->previous = NULL;     // the second symbol's previous is null
            Node* popped = stack->head;             // get the top symbol
            stack->head = stack->head->next;        // the second symbol of the stack becomes its new head
            return popped;                          // return the popped symbol

        }

        // if there is only one node in the stack
        else {

            Node* popped = stack->head;             // get the top symbol
            stack->head = NULL;                     // the stack's head is null
            stack->tail = NULL;                     // the stack's tail is null
            return popped;                          // return the popped symbol

        }

    }

}


// create and initiate a node
Node* create_node(int value, int priority, int is_operand, int is_function, int arity, int id, int is_function_end_symbol, int function_id) {

    // create the node:
    Node* node = (Node*)malloc(sizeof(Node));

    // assign the node's properties
    node->value = value;
    node->priority = priority;
    node->is_operand = is_operand;
    node->is_function = is_function;
    node->arity = arity;
    node->id = id;
    node->is_function_end_symbol = is_function_end_symbol;
    node->function_id = function_id;

    return node;

}


// add the element to the list
void add_to_list(List* stack, int value, int priority, int is_operand, int is_function, int arity, int id, int is_function_end_symbol, int function_id, int method) {

    // create and initiate a node
    Node* node = create_node(value, priority, is_operand, is_function, arity, id, is_function_end_symbol, function_id);

    // if the stack is empty
    if (stack->head == NULL) {

        stack->head = node;     // the node becomes the stack's head
        node->next = NULL;      // the node's next symbol is null
        node->previous = NULL;  // the node's previous symbol is null
        stack->tail = node;     // the node becomes the stack's tail

    }

    // if the stack is not empty
    else {

        // if the method is 'push'
        if (method) {

            // if there is only one element on the stack
            if (stack->head == stack->tail) {

                stack->head = node;                     // the node becomes the stack's head
                stack->head->next = stack->tail;        // the node's next symbol is the stack's tail
                stack->head->previous = NULL;           // the node's previous symbol is null
                stack->tail->previous = stack->head;    // the stack's tail's previous symbol is the stack's head

            }

            // if there are more than one element on the stack
            else {

                Node* saved_head = stack->head;         // save the old head node since it connects all the symbols
                stack->head = node;                     // the node becomes the stack's head
                stack->head->next = saved_head;         // the old head becomes the stack's head next symbol
                stack->head->previous = NULL;           // the stack's head's previous symbol is null
                saved_head->previous = stack->head;     // the stack's head becomes the old head's previous symbol

            }

        }

        // if the method is 'put'
        else {

            // if there is only one element on the stack
            if (stack->head == stack->tail) {

                stack->tail = node;                     // the node becomes the stack's tail
                stack->tail->previous = stack->head;    // the node's previous symbol is the stack's head
                stack->tail->next = NULL;               // the node's next symbol is null
                stack->head->next = stack->tail;        // the node becomes the stack's head's next symbol

            }

            // if there are more than one element on the stack
            else {

                Node* saved_tail = stack->tail;         // save the old tail node since it connects all the symbols
                stack->tail = node;                     // the node becomes the stack's tail
                saved_tail->next = stack->tail;         // the node becomes the old tail's next symbol
                stack->tail->previous = saved_tail;     // the old tail becomes the node's previous symbol
                stack->tail->next = NULL;               // the node's next symbol is null

            }

        }

    }

}


// add the new element to the list on the top
void push(List* stack, int value, int priority, int is_operand, int is_function, int arity, int id, int is_function_end_symbol, int function_id) {
    const int method = TRUE;
    add_to_list(stack, value, priority, is_operand, is_function, arity, id, is_function_end_symbol, function_id, method);
}


// add the new element to the list on the bottom
void put(List* stack, int value, int priority, int is_operand, int is_function, int arity, int id, int is_function_end_symbol, int function_id) {
    const int method = FALSE;
    add_to_list(stack, value, priority, is_operand, is_function, arity, id, is_function_end_symbol, function_id, method);
}


// print the content of a stack
void print(List* stack) {

    // create an iterator through the stack
    Node* iterator = stack->head;

    // if there are no elements in the stack, end the function's execution
    if (iterator == NULL) {
        free(iterator);
        return;
    }

    do {

        // if the current symbol is an operand
        if (iterator->is_operand) {
            printf("%d ", iterator->value); // print with a space
        }

        // if the current symbol is not an operand
        else {

            // if the current symbol is a function
            if (iterator->is_function) {

                // ... and if the current symbol is the end of a function's name
                if (iterator->is_function_end_symbol) {

                    // ... and if the current symbol is a part of the MIN / MAX function
                    if (iterator->function_id == 2 || iterator->function_id == 3) {
                        printf("%c%d ", iterator->value, iterator->arity); // print with the number of arity & space
                    }
                    // if the current symbol belongs to the other function
                    else printf("%c ", iterator->value); // print with the space

                }

                // if the current symbol is not a function's end
                else printf("%c", iterator->value); // print without a space

            }

            // if the current symbol is not a function (which means it's a negation function)
            else printf("%c ", iterator->value);

        }

        // get the next symbol from the stack
        iterator = iterator->next;

    }
    while(iterator != NULL);

    printf("\n");
    free(iterator);

}
