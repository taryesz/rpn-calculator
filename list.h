//
// Created by Tarazjusz Szulakiewicz on 06/03/2024.
//

#ifndef UNTITLED1_LIST_H
#define UNTITLED1_LIST_H

#endif //UNTITLED1_LIST_H

#include "node.h"

//#define INDEX_INCREMENTATION 1

typedef struct {
    Node* head;
    Node* tail;
} List;


Node* pop(List* stack) {

    // if the stack is empty:
    if (stack->head == NULL) {
        return NULL;
    }
    // if the stack has something:
    else {

        // if there are at least two nodes:
        if (stack->head->next != NULL) {
            stack->head->next->previous = NULL;
            Node* popped = stack->head;
            stack->head = stack->head->next;
            return popped;
        }
        // if there is only one node:
        else {
            Node* popped = stack->head;

            stack->head = NULL;
            stack->tail = NULL;
            return popped;
        }

    }

}


void push(List* stack, int value, int priority, int is_operand, int is_function, int arity, int id, int is_function_end_symbol, int function_id) {

    // O(1) version

    // create and initiate the node:
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = value;
    node->priority = priority;
    node->arity = arity;
    node->id = id;
    node->is_function_end_symbol = is_function_end_symbol;
    node->function_id = function_id;

    if (is_function) {
        node->is_function = 1;
    }
    else {
        node->is_function = 0;
    }

    if (is_operand) {
        node->is_operand = 1;
    }
    else {
        node->is_operand = 0;
    }

    // add to an empty stack
    if (stack->head == NULL) {

        stack->head = node;

        node->next = NULL;
        node->previous = NULL;

        stack->tail = node;

    }
    // add to non-empty stack
    else {

        // if head == tail
        if (stack->head == stack->tail) {

            stack->head = node;
            stack->head->next = stack->tail;
            stack->head->previous = NULL;

            stack->tail->previous = stack->head;

        }
        // if head != tail
        else {

            Node* saved_head = stack->head;  // save the head, we don't want to lose it

            stack->head = node;
            stack->head->next = saved_head;
            stack->head->previous = NULL;
             saved_head->previous = stack->head;

//            stack->tail->previous = saved_head;

//            free(saved_head);

        }

    }

}


void put(List* stack, int value, int priority, int is_operand, int is_function, int arity, int id, int is_function_end_symbol, int function_id, int* flag) {

    // O(1) version

    // create and initiate the node:
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = value;
    node->priority = priority;
    node->arity = arity;
    node->id = id;
    node->is_function_end_symbol = is_function_end_symbol;
    node->function_id = function_id;

    if (is_function) {
        node->is_function = 1;
    }
    else {
        node->is_function = 0;
    }

    if (is_operand) {
        node->is_operand = 1;
    }
    else {
        node->is_operand = 0;
    }

    // add to an empty stack
    if (stack->head == NULL) {

        stack->head = node;

        node->next = NULL;
        node->previous = NULL;

        stack->tail = node;

    }
    // add to non-empty stack
    else {

        // if head == tail
        if (stack->head == stack->tail) {

            stack->tail = node;
            stack->tail->previous = stack->head;
            stack->tail->next = NULL;

            stack->head->next = stack->tail;  // !!!

        }
        // if head != tail
        else {

            Node* saved_tail = stack->tail;  // save the tail, we don't want to lose it

            stack->tail = node;
            saved_tail->next = stack->tail; // !!!
            stack->tail->previous = saved_tail;
            stack->tail->next = NULL;

//            if (flag == 0) {
//                stack->head->next = saved_tail;
//                *flag = 1;
//            }

        }

    }

}


void print(List* stack) {

    int printing_function = 0;
    int previous_id = 0;
    Node* previous_symbol;

    Node* iterator = stack->head;

    if (iterator == NULL) return;

    do {

        if (iterator->is_operand) {
            if (printing_function) printf(" ");
            printing_function = 0;
            printf("%d ", iterator->value);
        }
        else {
            // TODO: 'M' & 'I' have the same id's, while the 'N' has a different one
            if (iterator->is_function || iterator->value == 'N') {
                if (iterator->id != previous_id && (previous_symbol->is_function || previous_symbol->value == 'N')) printf(" ");
                printf("%c", iterator->value);
                printing_function = 1;
            } else {
                if (printing_function) printf(" %c ", iterator->value);
                else {
                    printf("%c ", iterator->value);
                }
                printing_function = 0;
            }
        }

//        printf("(%d)", iterator->function_id);
        previous_id = iterator->id;
        previous_symbol = iterator;

        iterator = iterator->next;

    }
    while(iterator != NULL);

    printf("\n");
    free(iterator);

}

