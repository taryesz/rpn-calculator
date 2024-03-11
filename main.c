// headers:
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "list.h"
#include "quantity.h"
#include "input.h"
#include "calculate.h"


// parse the formula
void get_formula(List* stack, List* rpn) {

    int negate_function_found = FALSE;
    int negate_functions_counter = 0;

    // create a variable that will store the ascii symbol's numeric value if it's a digit ascii
    int symbol_value = 0;

    // create a flag that will be set to '1' if the last symbol parsed was a digit
    int parsing_operand = FALSE;

    // create a flag TODO: what was this supposed to do again?
    int flag = FALSE;

    while(TRUE) {

        // get a symbol from a user
        int symbol_ascii = getchar();

        // if the symbol is a white character, ignore it
        if (symbol_ascii == SPACE) continue;

        // if the symbol is an ascii of a digit or an operator symbol
        check_symbol_type(symbol_ascii, &symbol_value, &parsing_operand, stack, rpn, &flag, &negate_function_found, &negate_functions_counter);

        // stop the input here
        if (symbol_ascii == EQUATION_STOP_SYMBOL) break;

    }

};


// proceed to the calculations
void calculate_rpn(List* rpn) {

    // create a stack to store operands
    List stack = {NULL, NULL};

    // create an iterator that will go through the whole RPN equation
    Node* iterator;

    // create a variable to store the number of parsed operands per single operator
    int number_of_operands = 0;

    do {

        // get the first symbol from RPN equation
        iterator = pop(rpn);

        // if the parsed symbol is an operand
        if (iterator->is_operand) {

            // TODO: change this
            int flag = FALSE;

            // save the operand on stack
            put(&stack, iterator->value, iterator->priority, iterator->is_operand, iterator->arity, &flag);

            // keep track of how many operands we have already put on stack
            ++number_of_operands;

        }

        // if the parsed symbol is an operator
        else {

            // print the operator
            printf("%c ", iterator->value);

            // since our operands are being put on the stack, and not pushed,
            // and we need to print steps in reverse, we have to create a separate
            // stack that will be a reversed version of the original stack
            List operation_steps = mirror_stack(&stack);

            // print the reversed stack that contains steps
            print(&operation_steps);

            int division_by_zero = FALSE;

            check_for_operator_arity(&stack, rpn, iterator, &number_of_operands, &division_by_zero);

            if (division_by_zero == TRUE) return;

        }

        iterator = iterator->next;

    }
    while (iterator != NULL);

    // print the result of the RPN equation
    print(rpn);

    free(iterator);

}


int main() {

    // create and initiate a stack for operators and a rpn-stack for rpn equation
    List stack = {NULL, NULL};
    List rpn = {NULL, NULL};

    // get the number of future equations from a user
    const int number_of_formulas = get_number_of_formulas();

    // do main calculations as many times as the 'number_of_formulas' goes
    for (int counter = 0; counter < number_of_formulas; counter++) {

        // clear the RPN stack to avoid data mix-up
        rpn.head = NULL;
        rpn.tail = NULL;

        // parse the input equation and turn it into an RPN one
        get_formula(&stack, &rpn);

        // print the RPN equation when completed
        print(&rpn);

        // clear the operators stack to avoid data mix-up
        stack.head = NULL;
        stack.tail = NULL;

        // turn the RPN calculator on
        calculate_rpn(&rpn);

        printf("\n");

    }

    return 0;

};