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

//    if (stack->head != NULL) {
//
//        if (stack->head->next == NULL) {
//            stack->head = NULL;
//            stack->tail = NULL;
//        }
//        else {
//            stack->head->next->previous = NULL;
//            return stack->head;
//        }
//
//    }
//    else {
//        return NULL;
//    }
//    return NULL;
}


void push(List* stack, int value, int priority, int holds_operand) {

    // O(1) version

    // create and initiate the node:
    Node* node = malloc(sizeof(Node));
    node->value = value;
    node->priority = priority;


    if (holds_operand) {
        node->holds_operand = 1;
    }
    else {
        node->holds_operand = 0;
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

            stack->tail->previous = saved_head;

        }

    }

//
//    // check if the stack is empty:
////    if (stack->head == NULL) {
////
////        // our input node becomes both a head and a tail:
////        stack->head = node;
////        stack->tail = node;
////
////    }
////    // if the stack is not empty:
////    else {
//
//        node->previous = NULL;
//        node->next = stack->head;
//
//        if (stack->head != NULL) {
//            stack->head->previous = node;
//        }
//
//        // set new node's next as the old head
////        node->next = stack->head;
////
////        // set new node's previous as NULL
////        node->previous = NULL;
////
////        // set old head's previous as new node
////        stack->head->previous = node;
////
////        // set head of the stack as new node
////        stack->head = node;
//
////    }
//
////    free(node);

}


void put(List* stack, int value, int priority, int holds_operand, int* flag) {

    // O(1) version

    // create and initiate the node:
    Node* node = malloc(sizeof(Node));
    node->value = value;
    node->priority = priority;

    if (holds_operand) {
        node->holds_operand = 1;
    }
    else {
        node->holds_operand = 0;
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

            // !!!!!!!
            // if not flag
            // stack->head->next = saved_tail;
            // else nothing

            if (flag == 0) {
                stack->head->next = saved_tail;
                *flag = 1;
            }

//            stack->head->next = saved_tail;

        }

    }

    // check if the stack is empty:
//    if (stack->head == NULL) {
//
//        // our input node becomes both a head and a tail:
//        stack->head = node;
//        stack->tail = node;
//
//    }
//    else {
//
//        node->previous = stack->tail;
//
//        if (stack->tail != NULL) {
//            stack->tail->next = node;
//        }

        // set new node's next as NULL
//        node->next = NULL;
//
//        // set new node's previous as the old tail
//        node->previous = stack->tail;
//
//        // set old tail's next as new node
//        stack->tail->next = node;

        // set tail of the stack as new node
//        stack->tail = node;

//    }

//    free(node);

}


void print(List* stack) {

    Node* iterator = stack->head;

    do {

        if (iterator->holds_operand) {
            printf("%d ", iterator->value);
        }
        else {
            printf("%c ", iterator->value);
        }

//        printf("ITERATOR: %d / %c\n", iterator->value, iterator->value);
        iterator = iterator->next;

    }
    while(iterator != NULL);

}
