#include <iostream>
#include "stack.h"
#include "../define.h"


void stack::set_node(int content, int priority, int arity, int function_id, bool operand, bool function, bool last, bool requires_additional_parenthesis, bool method) {

    // create a new node
    auto* symbol = new node();

    // give the node some properties
    symbol->set_content(content);
    symbol->set_priority(priority);
    symbol->set_arity(arity);
    symbol->set_function_id(function_id);
    symbol->set_operand(operand);
    symbol->set_function(function);
    symbol->set_last(last);
    symbol->set_requires_additional_parenthesis(requires_additional_parenthesis);

    // if the stack is empty ...
    if (head == nullptr) {

        // set both head & tail as the same node
        head = symbol;
        tail = symbol;

    }

    // if the stack is not empty ...
    else {

        // if the program puts the node ...
        if (method) {

            // connect a previous node to the new node
            this->tail->set_next(symbol);

            // update the tail
            this->tail = symbol;

        }

        // if the program pushes the node ...
        else {

            // connect a new node to the first node in the stack
            symbol->set_next(this->head);

            // update the head
            this->head = symbol;

        }

    }

}

stack::stack() {
    this->head = nullptr;
    this->tail = nullptr;
}

node* stack::get_head() {
    return this->head;
}

void stack::put(int content, int priority, int arity, int function_id, bool operand, bool function, bool last, bool requires_additional_parenthesis) {

    // create a variable that will tell a setter to *put* the element
    const bool method = true;

    // set the node
    set_node(content, priority, arity, function_id, operand, function, last, requires_additional_parenthesis, method);

}

void stack::push(int content, int priority, int arity, int function_id, bool operand, bool function, bool last, bool requires_additional_parenthesis) {

    // create a variable that will tell a setter to *push* the element
    const bool method = false;

    // set the node
    set_node(content, priority, arity, function_id, operand, function, last, requires_additional_parenthesis, method);

}

node* stack::pop() {

    // if the head of the stack doesn't exist, return null
    if (this->head == nullptr) return nullptr;

    // if the head exists, get it
    node* popped = this->head;

    // if there was only one element on the stack and the program has popped it, set to null both head and tail
    if (this->head == this->tail) {
        this->head = nullptr;
        this->tail = nullptr;
    }

    // if there were more than one element on the stack, scoot the whole stack by one 'to the left'
    else this->head = this->head->get_next();

    // reset the popped's connection
    popped->set_next(nullptr);

    // return the popped symbol
    return popped;

}

void stack::print() {

    // get the head of the stack
    node* iterator = this->head;

    // while there are elements on the stack
    while (iterator != nullptr) {

        // if the currently processed symbol is an operand (i.e. a number), print it with a space after it
        if (iterator->is_operand()) printf("%d ", iterator->get_content());

        // if the currently processed symbol is a function (i.e. a letter) ...
        else if (iterator->is_function()) {

            // if this letter is a part of MIN/MAX, and it's the last letter of the function's name ...
            if ((iterator->get_function_id() == maximum || iterator->get_function_id() == minimum) && iterator->is_last()) {

                // print it with the number of arguments & a space right after it
                printf("%c%d ", iterator->get_content(), iterator->get_arity());

            }

            // if the letter is the last letter of the function's name, print it with a space after it
            else if (iterator->is_last()) printf("%c ", iterator->get_content());

            // if the letter is neither last, nor a part of MIN/MAX, it means that it's somewhere inside a function's name
            // so, just print the letter
            else printf("%c", iterator->get_content());

        }

        // if the currently processed symbol is an operator (i.e. '+', '-', '*', '/'), print it with a space after it
        else printf("%c ", iterator->get_content());

        // get the next symbol from the stack
        iterator = iterator->get_next();

    }

    // print the new line char for the sake of aesthetic look of the output
    printf("\n");

}

void stack::clear() {

    // get the head of the stack
    node* current = this->head;

    // while there are elements on the stack
    while (current != nullptr) {

        // get the next symbol from the stack
        node* next = current->get_next();

        // delete the currently parsed symbol
        delete current;

        // update the currently parsed symbol with the next element
        current = next;

    }

    // after deleting all elements from the stack, make sure the key nodes are set to null
    head = nullptr;
    tail = nullptr;

}
