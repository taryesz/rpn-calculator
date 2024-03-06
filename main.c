#include <stdio.h>
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
    printf("Number of formulas: ");

    // proceed to checking the input
    while (true) {

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

    const char if_function[] = "IF";
    const char n_function[] = "N";
    const char min_function[] = "MIN";
    const char max_function[] = "MAX";

    const char *functions[] = {
            if_function,
            n_function,
            min_function,
            max_function
    };

    return *functions;

};


// compare input letters to a predefined function name
size_t compare_to_function_name(int *symbol_ascii) {

    // get an array of reserved functions
    const char *functions = reserved_functions();

    // iterate through every function's name and compare their symbols
    for (size_t i = 0; i < sizeof(&functions) / sizeof(functions[0]); i++) {
        const char *current_function = &functions[i];

        for (size_t j = 0; current_function[j] != '\0'; j++) {

            if (*symbol_ascii == current_function[j]) {

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


void check_operator_type(int symbol_ascii) {

    // we do this if block to "sort out" symbols by their priorities

    // if the symbol is a '+' or a '-'
    if (symbol_ascii == PLUS || symbol_ascii == MINUS) {
        // create a node
        // set its priority to 1
        // push();
        printf("THIS IS A PLUS/MINUS: %c\n", symbol_ascii);
    }

        // if the symbol is a '*' or a '/'
    else if (symbol_ascii == ASTERISK || symbol_ascii == SLASH) {
        // create a node
        // set priority to 2
        // push();
        printf("THIS IS AN ASTERISK/SLASH: %c\n", symbol_ascii);
    }

        // if the symbol is a capital letter (i.e. a part of a function name)
    else if (symbol_ascii >= ASCII_LETTER_RANGE_START && symbol_ascii <= ASCII_LETTER_RANGE_FINISH) {

        // get an array of reserved functions
        const char *functions = reserved_functions();

        size_t function_id = compare_to_function_name(&symbol_ascii);

        const char function = functions[function_id];

        // TODO: do we need to skip some symbols? yes. DONE

        printf("THIS IS A FUNCTION: %c\n", symbol_ascii);

        // create a node
        // set priority to 3
        // push(function)

    }

        // if the symbol is a parentheses
    else if (symbol_ascii == OPEN_PARENTHESES || symbol_ascii == CLOSE_PARENTHESES) {

        // create a node
        // set priority to 4
        // push()

        printf("THIS IS A PARENTHESIS: %c\n", symbol_ascii);

    }

        // if the symbol is something that shouldn't be considered at all
    else {
        printf("THIS IS SOMETHING THAT SHOULD NOT BE PARSED: '%c'\n", symbol_ascii);
    }

}


void get_formula() {

    int symbol_ascii;
    int symbol_value = 0;

    const char STOP_SYMBOL = '.';

    while(true) {

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
            printf("THIS IS A DIGIT: %d\n", symbol_value);
            symbol_value = 0;
        }

            // if the symbol is an operator
        else {
            check_operator_type(symbol_ascii);
        }

    }

};

int main() {

    // get the number of formulas from a user
    const int number_of_formulas = get_number_of_formulas();

    for (int counter = 0; counter < number_of_formulas; counter++) {
        printf("Provide your infix formula:\n");
        get_formula();
    }

    return 0;

};