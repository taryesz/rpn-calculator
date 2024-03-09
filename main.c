// TODO: debug calculations
// TODO: implement functions


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "list.h"

#define PLUS '+'
#define MINUS '-'
#define ASTERISK '*'
#define SLASH '/'
#define OPEN_PARENTHESES '('
#define CLOSE_PARENTHESES ')'

// define constants needed to convert an ascii into its according value
#define ASCII_DIGIT_RANGE_START 48
#define ASCII_DIGIT_RANGE_FINISH 57

#define FACTOR 10

#define ASCII_LETTER_RANGE_START 65
#define ASCII_LETTER_RANGE_FINISH 90

#define CRITICAL_DIVISOR 0


// inspiration: https://www.geeksforgeeks.org/program-that-allows-integer-input-only/
int get_number_of_formulas() {

    // define a constant which will be a flag to stop the input
    const char STOP_SYMBOL = '\n';

    // this variable stores a value of an input
    int user_input_value = 0;

    // proceed to checking the input
    while (1) {

        // get the input from a user
        int user_input_ascii = getchar();

        // convert ascii to its value
        if (user_input_ascii >= ASCII_DIGIT_RANGE_START && user_input_ascii <= ASCII_DIGIT_RANGE_FINISH) {
            user_input_value = user_input_value * FACTOR + (user_input_ascii - ASCII_DIGIT_RANGE_START);
        }

        // if pressed 'Enter', stop the loop
        if (user_input_ascii == STOP_SYMBOL) {
            break;
        }

    };

    return (user_input_value);

};


// create an array of reserved functions' names
//const char* reserved_functions() {
//
//        static const char if_function[] = "IF";
//        static const char n_function[] = "N";
//        static const char min_function[] = "MIN";
//        static const char max_function[] = "MAX";
//
//        static const char* const functions[] = {
//                if_function,
//                n_function,
//                min_function,
//                max_function
//        };
//
//        return *functions;
//
////    char* if_function = "IF";
////    char* n_function = "N";
////    char* min_function = "MIN";
////    char* max_function = "MAX";
////
////    char** functions;
////
////    functions = (char**) calloc(4, sizeof(char*));
////
////    functions[0] = if_function;
////    functions[1] = n_function;
////    functions[2] = min_function;
////    functions[3] = max_function;
////
////    return functions;
//
//};


// compare input letters to a predefined function name
//size_t compare_to_function_name(int symbol_ascii) {
//
//    // get an array of reserved functions
//    const char* functions = reserved_functions();
//
//    // iterate through every function's name and compare their symbols
//    for (size_t i = 0; i < sizeof(&functions) / sizeof(functions[0]); i++) {
//        const char *current_function = &functions[i];
//
//        for (size_t j = 0; current_function[j] != '\0'; j++) {
//
//            if (symbol_ascii == current_function[j]) {
//
//                if (current_function[j + 2] == '\0') {
//                    return i;  // return the index of a function name
//                }
//
//            }
//            else {
//                break;
//            }
//
//        }
//
////        free((void *)current_function);  // ?
//
//    }
//
////    free((void *)functions);  // ?
//
//    return -1;
//
//}


// saving operators on the RPN stack
void convert_to_rpn(List** stack, List** rpn, int value, int priority, int holds_operand, int** flag) {

    // create an iterator that will go through the whole stack
    Node* iterator = (*stack)->head;

    // while there are elements on the stack
    while (iterator != NULL) {

        // get the top operator symbol from the stack
        Node* popped = pop(*stack);

        // if the popped symbol is a '(' or if it has a lower priority than the pushed symbol
        if (popped->value == OPEN_PARENTHESES || popped->priority < priority) {

            // push the popped symbol to the stack
            push(*stack, popped->value, popped->priority, popped->holds_operand);

            // stop the saving process
            break;

        }

        // put the popped symbol to the RPN stack if it's valid for this
        put(*rpn, popped->value, popped->priority, popped->holds_operand, *flag);

        iterator = iterator->next;

    };

    // push the originally pushed symbol to the stack
    push(*stack, value, priority, holds_operand);

    free(iterator);

};


// check what operator was passed
void check_operator_type(int symbol_ascii, List* stack, List* rpn, int* flag, const char STOP_SYMBOL) {

    // create a const to indicate that the symbol doesn't contain a numerical value
    const int holds_operand = 0;

    // if the symbol is a '.'
    if (symbol_ascii == STOP_SYMBOL) {

        // while the operators stack is not empty
        while (stack->head != NULL) {

            // push the rest of the operators to the stack
            convert_to_rpn(&stack, &rpn, stack->head->value, stack->head->priority, stack->head->holds_operand, &flag);

            // remove the pushed operator from the stack
            pop(stack);

        }

        // stop the input
        return;

    }

    int priority = INFINITY;

    // if the symbol is a parenthesis
    if (symbol_ascii == OPEN_PARENTHESES || symbol_ascii == CLOSE_PARENTHESES) {

        // set the parenthesis priority
        priority = 4;

        // if the symbol is a '(', just push it to the operators stack
        if (symbol_ascii == OPEN_PARENTHESES) push(stack, symbol_ascii, priority, holds_operand);

        // if the symbol is a ')'
        if (symbol_ascii == CLOSE_PARENTHESES) {

            // create an iterator that will go through the whole stack
            Node* iterator = stack->head;

            // while there are elements in the stack
            do {

                // get the top operator symbol from the stack
                Node* popped = pop(stack);

                // if the symbol is a '(', stop the loop
                if (popped->value == OPEN_PARENTHESES) break;

                // if the symbol is something else
                else {

                    // save the symbol to the RPN stack
                    put(rpn, popped->value, priority, holds_operand, flag);

                }

                iterator = iterator->next;
                free(popped);

            }
            while(iterator != NULL);

            free(iterator);

        }

    }

    // if the symbol is a '+' or a '-', set the according priority and push to RPN stack
    else if (symbol_ascii == PLUS || symbol_ascii == MINUS) {
        priority = 1;
        convert_to_rpn(&stack, &rpn, symbol_ascii, priority, holds_operand, &flag);
    }

    // if the symbol is a '*' or a '/', set the according priority and push to RPN stack
    else if (symbol_ascii == ASTERISK || symbol_ascii == SLASH) {
        priority = 2;
        convert_to_rpn(&stack, &rpn, symbol_ascii, priority, holds_operand, &flag);
    }

    // if the symbol is a capital letter (i.e. a part of a function name)
    else if (symbol_ascii >= ASCII_LETTER_RANGE_START && symbol_ascii <= ASCII_LETTER_RANGE_FINISH) {

        // TODO: implement function support

    }

}


// parse the formula
void get_formula(List* stack, List* rpn) {

    // create a variable that will store the ascii symbol's numeric value if it's a digit ascii
    int symbol_value = 0;

    // create a constant that will tell the program when to stop accepting input from a user
    const char STOP_SYMBOL = '.';

    // create a flag that will be set to '1' if the last symbol parsed was a digit
    int parsing_operand = 0;

    // create a flag TODO: what was this supposed to do again?
    int flag = 0;

    while(1) {

        // get a symbol from a user
        int symbol_ascii = getchar();

        // if the symbol is an ascii of a digit
        if (symbol_ascii >= ASCII_DIGIT_RANGE_START && symbol_ascii <= ASCII_DIGIT_RANGE_FINISH) {

            // get its value and add to the 'symbol_value' variable as part of a formula of
            // conversion ascii to its according value
            symbol_value = symbol_value * FACTOR + (symbol_ascii - ASCII_DIGIT_RANGE_START);

            // set the flag to '1'
            parsing_operand = 1;

        }

        // if the symbol is an operator
        else {

            // check if the last parsed symbol was a digit
            if (parsing_operand) {

                // give the operand its properties
                const int priority = 0;
                const int holds_operand = 1;

                // save the operand to the RPN stack
                put(rpn, symbol_value, priority, holds_operand, &flag);

                // reset the variables
                symbol_value = 0;
                parsing_operand = 0;

            }

            // if the symbol is not an ascii of a digit
            check_operator_type(symbol_ascii, stack, rpn, &flag, STOP_SYMBOL);

        }

        // stop the input here
        if (symbol_ascii == STOP_SYMBOL) {
            break;
        }

    }

    // print the RPN equation when completed
    print(rpn);

};


// add the result of an operation to RPN stack
void execute_operation(List* rpn, int result) {
    const int priority = 0;
    const int holds_operand = 1;
    push(rpn, result, priority, holds_operand);
}


// create a mirrored version of the RPN stack
List mirror_stack(List* stack) {

    // create an iterator that will go through the whole stack
    Node* iterator = stack->head;

    // create and initiate a mirrored stack
    List mirrored_stack = {NULL, NULL};

    // while there are elements in the original stack
    do {

        if (iterator != NULL) {

            // push a symbol to the new mirrored stack
            push(&mirrored_stack, iterator->value, iterator->priority, iterator->holds_operand);
            iterator = iterator->next;
        }

    }
    while (iterator != NULL);

    free(iterator);

    return mirrored_stack;

}


// check how many operands left on the stack
void update_number_of_operands(List* stack, int* number_of_operands) {

    // if the stack is empty
    if (stack->head == NULL) {

        // set the variable to zero
        *number_of_operands = 0;
        return;
    }

    // if the stack is not empty

    // create a counter variable
    int counter = 0;

    // create an iterator that will go through the whole stack
    Node* iterator = stack->head;

    // while there are elements in a stack, increment the counter variable
    do {
        counter++;
        iterator = iterator->next;
    }
    while (iterator != NULL);

    // update the 'number_of_operands'
    *number_of_operands = counter;

    free(iterator);

}


// check if the mathematical equation requires a priority change (imaginary parentheses)
void check_priority_change(List* stack, Node** first_operand, Node** second_operand, int number_of_operands) {

    // if there was a priority change
    if (number_of_operands > 2) {

        // save the first element that doesn't participate in an operation
        Node* first = pop(stack);

        // get the necessary operands
        *first_operand = pop(stack);
        *second_operand = pop(stack);

        // push the saved operand back, we don't need to lose it
        push(stack, first->value, first->priority, first->holds_operand);

        free(first);

    }
    // if there was no priority change
    else {

        // get the necessary operands
        *first_operand = pop(stack);
        *second_operand = pop(stack);

    }

}


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
        if (iterator->holds_operand) {

            // TODO: change this
            int flag = 0;

            // save the operand on stack
            put(&stack, iterator->value, iterator->priority, iterator->holds_operand, &flag);

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

            // we are using binary operators, which means we need only two operands
            Node* first_operand;
            Node* second_operand;

            // check how many operands we have on our stack
            // i.e. if the parsed mathematical equation requires change of
            // priority, or, as to say, imaginary parentheses.
            // if there are more than two operands, it means that the priority change
            // was encountered, and we need to change the order of getting
            // operands from the stack
            check_priority_change(&stack, &first_operand, &second_operand, number_of_operands);

            // since we popped two necessary operands, we have to update the number of them on the stack
            update_number_of_operands(&stack, &number_of_operands);

            // if both operands exist
            if (first_operand != NULL && second_operand != NULL) {

                // create a variable to store the result of an operation and initiate it to infinity
                int result = INFINITY;

                // if the operator is an '*'
                if (iterator->value == ASTERISK) result = first_operand->value * second_operand->value;
                // if the operator is a '/'
                else if (iterator->value == SLASH) {

                    // if the divisor is 0
                    if (second_operand->value == CRITICAL_DIVISOR) {
                        printf("ERROR\n");
                        return;
                    }
                    else {
                        result = first_operand->value / second_operand->value;
                    }

                }
                // if the operator is a '+'
                else if (iterator->value == PLUS) result = first_operand->value + second_operand->value;
                // if the operator is a '-'
                else if (iterator->value == MINUS) result = first_operand->value - second_operand->value;

                // if calculations are successful, push the result to the RPN stack
                if (result != INFINITY) {
                    execute_operation(rpn, result);
                }

            }

            free(first_operand);
            free(second_operand);

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

        // clear the operators stack to avoid data mix-up
        stack.head = NULL;
        stack.tail = NULL;

        // turn the RPN calculator on
        calculate_rpn(&rpn);

        printf("\n");

    }

    return 0;

};