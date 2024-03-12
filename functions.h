//
// Created by Tarazjusz Szulakiewicz on 10/03/2024.
//

#ifndef RPN_CALCULATOR_FUNCTIONS_H
#define RPN_CALCULATOR_FUNCTIONS_H

#endif //RPN_CALCULATOR_FUNCTIONS_H


#include "list.h"
#include <stdlib.h>

#define CLOSE_PARENTHESIS ')'
#define OPEN_PARENTHESIS '('


void open_parenthesis(List* stack, int symbol_ascii, int priority, int holds_operand, int arity, const int* negate_function_found) {
    if (symbol_ascii == OPEN_PARENTHESIS) {
        if (*negate_function_found == 1) {
            // do nothing
        }
        else {
            push(stack, symbol_ascii, priority, holds_operand, arity);
        }
    }
}


void close_parenthesis(List* stack, List* rpn, int symbol_ascii, int priority, int holds_operand, int* flag) {

    if (symbol_ascii == CLOSE_PARENTHESIS) {

        // create an iterator that will go through the whole stack
        Node* iterator = stack->head;

        // while there are elements in the stack
        do {

            // get the top operator symbol from the stack
            Node* popped = pop(stack);

            // if the symbol is a '(', stop the loop
            if (popped->value == CLOSE_PARENTHESIS) break;

            // if the symbol is something else
            else {

                // save the symbol to the RPN stack
                put(rpn, popped->value, priority, holds_operand, popped->arity, flag);

            }

            iterator = iterator->next;
            free(popped);

        }
        while(iterator != NULL);

        free(iterator);

    }

}
