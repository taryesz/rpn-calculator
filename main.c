#include <stdio.h>
#include <stdlib.h>
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


// inspiration: https://www.geeksforgeeks.org/program-that-allows-integer-input-only/
int get_number_of_formulas() {

    // define a constant which will be a flag to stop the input
    const char STOP_SYMBOL = '\n';

    // this variable stores an ascii representation of an input
    int user_input_ascii;

    // this variable stores a value of an input
    int user_input_value = 0;

    // ask a user to provide a value
//    printf("Number of formulas: ");

    // proceed to checking the input
    while (1) {

        // get the input
        user_input_ascii = getchar();

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
const char* reserved_functions() {

    static const char if_function[] = "IF";
    static const char n_function[] = "N";
    static const char min_function[] = "MIN";
    static const char max_function[] = "MAX";

    static const char* const functions[] = {
            if_function,
            n_function,
            min_function,
            max_function
    };

    return *functions;

};


// compare input letters to a predefined function name
size_t compare_to_function_name(int symbol_ascii) {

    // get an array of reserved functions
    const char *functions = reserved_functions();

    // iterate through every function's name and compare their symbols
    for (size_t i = 0; i < sizeof(&functions) / sizeof(functions[0]); i++) {
        const char *current_function = &functions[i];

        for (size_t j = 0; current_function[j] != '\0'; j++) {

            if (symbol_ascii == current_function[j]) {

                if (current_function[j + 2] == '\0') {
                    return i;  // return the index of a function name
                }

            }
            else {
                break;
            }

        }

    }

    return -1;

}


void logic(List** stack, List** rpn, int value, int priority, int holds_operand, int** flag) {

    List* new_stack = *stack;
    List* new_rpn = *rpn;
    int* new_flag = *flag;

    if (new_stack != NULL) {

        Node* iterator = new_stack->head;

        if (iterator != NULL) {

            do {
                Node* popped = pop(new_stack);

//                printf("POPPED OPERATOR: %c\n", popped->value);

                if (popped->value == OPEN_PARENTHESES || popped->priority < priority) {
//                    printf("POPPED OPERATOR (%c) IS '(' OR ...", popped->value);
                    push(new_stack, popped->value, popped->priority, popped->holds_operand);
                    break;
                }

//                printf("POPPED OPERATOR (%c) IS NOT '(' OR ...\n", popped->value);

                if (new_rpn != NULL) {
//                    printf("PUT NEW OPERATOR TO RPN");
                    put(new_rpn, popped->value, popped->priority, popped->holds_operand, new_flag);
                }

                iterator = iterator->next;

            } while (iterator != NULL);

//            while(iterator->next != NULL) { // !!!
//                Node* popped = pop(new_stack);
//                if (popped->value == OPEN_PARENTHESES || popped->priority < priority) {
//                    push(new_stack, popped->value, popped->priority, popped->holds_operand);
//                    break;
//                }
//
//                if (new_rpn != NULL) {
//                    put(new_rpn, popped->value, popped->priority, popped->holds_operand);
//                }
//                iterator = iterator->next;
//            }
        }
        push(new_stack, value, priority, holds_operand);
    }

}


// check what operator was passed
void check_operator_type(int symbol_ascii, List* stack, List* rpn, int* flag) {

    const int holds_operand = 0;

    // we do this if block to "sort out" symbols by their priorities

    // if the symbol is a parentheses
    if (symbol_ascii == OPEN_PARENTHESES || symbol_ascii == CLOSE_PARENTHESES) {

        const int priority = 4;

        if (symbol_ascii == OPEN_PARENTHESES) push(stack, symbol_ascii, priority, holds_operand);

        if (symbol_ascii == CLOSE_PARENTHESES) {

            Node* iterator = stack->head;

            while(iterator->next != NULL) {
                Node* popped = pop(stack);
                if (popped->value == OPEN_PARENTHESES) break;
                else {
                    put(rpn, symbol_ascii, priority, holds_operand, flag);
                }
                iterator = iterator->next;
            }

        }

    }

    // if the symbol is a '+' or a '-'
    else if (symbol_ascii == PLUS || symbol_ascii == MINUS) {
        const int priority = 1;
        logic(&stack, &rpn, symbol_ascii, priority, holds_operand, &flag);
    }

    // if the symbol is a '*' or a '/'
    else if (symbol_ascii == ASTERISK || symbol_ascii == SLASH) {
        const int priority = 2;
        logic(&stack, &rpn, symbol_ascii, priority, holds_operand, &flag);
    }

    // if the symbol is a capital letter (i.e. a part of a function name)
    else if (symbol_ascii >= ASCII_LETTER_RANGE_START && symbol_ascii <= ASCII_LETTER_RANGE_FINISH) {

        // get an array of reserved functions
//        const char *functions = reserved_functions();
//
//        size_t function_id = compare_to_function_name(symbol_ascii);
//
//        // get the necessary name
//        const char function = functions[function_id];
//
//        // TODO: HOW TO SAVE A STRING?
//
////        const int priority = 3;
////
////        push(stack, function, priority);

        printf("THIS IS A FUNCTION: %c\n", symbol_ascii);

    }

}


// parse the formula
void get_formula() {

    int symbol_ascii;
    int symbol_value = 0;

    const char STOP_SYMBOL = '.';

    int parsing_operand = 0;

    List stack = {NULL, NULL};

    List rpn = {NULL, NULL};

    int flag = 0;

    while(1) {

        // input a symbol
        symbol_ascii = getchar();

        // check the type of this symbol (operand, operator, part of a function or something else)

        // if the symbol is a dot, stop the loop
        if (symbol_ascii == STOP_SYMBOL) {
            break;
        }

        // if the symbol is a number, get its value and print it
        if (symbol_ascii >= ASCII_DIGIT_RANGE_START && symbol_ascii <= ASCII_DIGIT_RANGE_FINISH) {
            symbol_value = symbol_value * FACTOR + (symbol_ascii - ASCII_DIGIT_RANGE_START);
            parsing_operand = 1;
        }

        // if the symbol is an operator
        else {

            // check if all the numbers in row were parsed, then print the complete number at once
            if (parsing_operand) {

                const int priority = 0;
                const int holds_operand= 1;

                put(&rpn, symbol_value, priority, holds_operand, &flag);

//                printf("THIS IS AN OPERAND: %d\n", symbol_value);
                symbol_value = 0;
                parsing_operand = 0;
            }

            check_operator_type(symbol_ascii, &stack, &rpn, &flag);

        }

    }

    Node* iterator = stack.head;

    while(iterator->next != NULL) {

        Node* popped = pop(&stack);

        const int holds_operand = popped->holds_operand;

        put(&rpn, popped->value, popped->priority, holds_operand, &flag);
        iterator = iterator->next;
    }

    print(&rpn);

};


int main() {

    // get the number of formulas from a user
    const int number_of_formulas = get_number_of_formulas();

    for (int counter = 0; counter < number_of_formulas; counter++) {
        printf("\nProvide your infix formula:\n");
        get_formula();
    }

    return 0;

};