//
// Created by Tarazjusz Szulakiewicz on 11/03/2024.
//

#ifndef RPN_CALCULATOR_INPUT_H
#define RPN_CALCULATOR_INPUT_H

#endif //RPN_CALCULATOR_INPUT_H


void set_priority(int* priority, int value) {
    *priority = value;
}
void set_is_operand(int* is_operand, int value) {
    *is_operand = value;
}
void set_arity(int* arity, int value) {
    *arity = value;
}


char* get_function_name(int function) {

    if (function == N) {
        return "N";
    }
    else if (function == IF) {
        return "IF";
    }
    else if (function == MIN) {
        return "MIN";
    }
    else if (function == MAX) {
        return "MAX";
    }

    return "";

}


// create an array of reserved functions' names
const char** get_reserved_functions() {

    const char** functions = malloc(RESERVED_FUNCTIONS_COUNTER * sizeof(char*));

    if (functions == NULL) {
        return NULL;
    }

    for (int i = 0; i < RESERVED_FUNCTIONS_COUNTER; i++) {
        functions[i] = get_function_name(i);
    }

    return functions;

}


// saving operators on the RPN stack
void convert_to_rpn(List* stack, List* rpn, int value, int priority, int holds_operand, int arity, int* flag) {

    // create an iterator that will go through the whole stack
    Node* iterator = stack->head;

    // while there are elements on the stack
    while (iterator != NULL) {

        // get the top operator symbol from the stack
        Node* popped = pop(stack);

        // if the popped symbol is a '(' or if it has a lower priority than the pushed symbol
        if (popped->value == OPEN_PARENTHESES || popped->priority < priority) {

            // push the popped symbol to the stack
            push(stack, popped->value, popped->priority, popped->is_operand, popped->arity);

            // stop the saving process
            break;

        }

        // put the popped symbol to the RPN stack if it's valid for this
        put(rpn, popped->value, popped->priority, popped->is_operand, popped->arity, flag);

        // IF THIS IS A PART OF THE SAME FUNCTION ...
        //      PUSH THIS PART TOO

//        if (popped->is_function && is_function) {
//            put(rpn, value, priority, );
//        }

        iterator = iterator->next;

    };

    // push the originally pushed symbol to the stack
    push(stack, value, priority, holds_operand, arity);

    free(iterator);

};


void execute_function(int function_id, const char* reserved_functions, List* stack, List* rpn, int* flag, int* negate_function_found) {

    int priority;
    int is_operand;
    int arity;

    set_priority(&priority, FUNCTION_PRIORITY);
    set_is_operand(&is_operand, FALSE);

    if (function_id == N) {
        set_arity(&arity, UNARY_ARITY);
        convert_to_rpn(stack, rpn, reserved_functions[function_id], priority, is_operand, arity, flag);
        *negate_function_found = TRUE;
    }
    else if (function_id == IF) {
        set_arity(&arity, TERNARY_ARITY);
        char* func = get_function_name(function_id);
        for (int i = 0; func[i] != STRING_TERMINATOR; i++) {
            convert_to_rpn(stack, rpn, func[i], priority, is_operand, arity, flag);
        }
    }
    else if (function_id == MIN) {
        // min
        printf("MIN\n");
    }
    else if (function_id == MAX) {
        // max
        printf("MAX\n");
    }

}


// compare input letters to a predefined function name
void compare_function_names(int symbol_ascii, int* parsing_function_name, List* stack, List* rpn, int* flag, int* negate_function_found, int* functions_counter, int* iterator) {

    int function_id = *functions_counter;

    const char **functions = get_reserved_functions();

    for (; function_id < RESERVED_FUNCTIONS_COUNTER;) {

        const char* current_function = functions[function_id];

        if (symbol_ascii == current_function[*iterator]) {

            ++(*iterator);

            if (current_function[*iterator] == STRING_TERMINATOR) {

                execute_function(function_id, *functions, stack, rpn, flag, negate_function_found);

                *iterator = 0;
                return;

            }

            *functions_counter = function_id;

            break;

        }
        else {

            *iterator = 0;
            function_id += 1;

        }

    }

}


// if the symbol is a parenthesis ('(' or ')')
void process_parenthesis(int symbol_ascii, List* stack, List* rpn, int* priority, int is_operand, int arity, int* flag) {

    // set the parenthesis priority
    set_priority(priority, PARENTHESIS_PRIORITY);

    // if the symbol is a '(', just push it to the operators stack
    if (symbol_ascii == OPEN_PARENTHESES) push(stack, symbol_ascii, *priority, is_operand, arity);

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
                put(rpn, popped->value, *priority, is_operand, popped->arity, flag);

            }

            iterator = iterator->next;
            free(popped);

        }
        while(iterator != NULL);

        free(iterator);

    }
}


// if the symbol is an operator ('+', '-', '*' or '/')
void process_operator(int symbol_ascii, List* stack, List* rpn, int* priority, int is_operand, int* arity, int* flag, int priority_value) {
    set_priority(priority, priority_value);
    set_arity(arity, BINARY_ARITY);
    convert_to_rpn(stack, rpn, symbol_ascii, *priority, is_operand, *arity, flag);
}


// check if the symbol is a '.'
void check_for_stop(int symbol_ascii, List* stack, List* rpn, int* flag) {

    if (symbol_ascii == EQUATION_STOP_SYMBOL) {

        // while the operators stack is not empty
        while (stack->head != NULL) {

            // push the rest of the operators to the stack
            convert_to_rpn(stack, rpn, stack->head->value, stack->head->priority, stack->head->is_operand, stack->head->arity, flag);

            // remove the pushed operator from the stack
            pop(stack);

        }

    }

}


// check what operator was passed
void check_for_operator(int symbol_ascii, List* stack, List* rpn, int* flag, int* negate_function_found, int* parsing_function_name, int* functions_counter, int* iterator) {

    // create a const to indicate that the symbol doesn't contain a numerical value
    int is_operand;
    int priority;
    int arity;

    // for all the symbols set 'is_operand' to false, since they are operators
    set_is_operand(&is_operand, FALSE);

    // if the symbol is a '.'
    check_for_stop(symbol_ascii, stack, rpn, flag);

    // if the symbol is a parenthesis
    if (symbol_ascii == OPEN_PARENTHESES || symbol_ascii == CLOSE_PARENTHESES) {
        set_arity(&arity, PARENTHESIS_ARITY);
        process_parenthesis(symbol_ascii, stack, rpn, &priority, is_operand, arity, flag);
    }

    // if the symbol is a '+' or a '-', set the according priority and push to RPN stack
    else if (symbol_ascii == PLUS || symbol_ascii == MINUS) {
        process_operator(symbol_ascii, stack, rpn, &priority, is_operand, &arity, flag, OPERATOR_PLUS_MINUS_PRIORITY);
    }

    // if the symbol is a '*' or a '/', set the according priority and push to RPN stack
    else if (symbol_ascii == ASTERISK || symbol_ascii == SLASH) {
        process_operator(symbol_ascii, stack, rpn, &priority, is_operand, &arity, flag, OPERATOR_ASTERISK_SLASH_PRIORITY);
    }

    // if the symbol is a capital letter (i.e. a part of a function name)
    else if (symbol_ascii >= ASCII_LETTER_RANGE_START && symbol_ascii <= ASCII_LETTER_RANGE_FINISH || symbol_ascii == SPACE) {
        compare_function_names(symbol_ascii, parsing_function_name, stack, rpn, flag, negate_function_found, functions_counter, iterator);
    }

}


// check if there were any 'N' functions
void check_for_negate_function(List* stack, List* rpn, int* flag, int* negate_function_found, int* negate_functions_counter, int* parsing_function_name, int* functions_counter, int* iterator) {

    // if the function is found
    if (*negate_function_found == TRUE) {

        // add the missing open parenthesis
        check_for_operator(OPEN_PARENTHESES, stack, rpn, flag, negate_function_found, parsing_function_name, functions_counter, iterator);

        // increment the number of 'N' functions
        *negate_functions_counter += 1;

    }

}


// check if the mathematical equation uses 'N' function and if so, put the missing parentheses where necessary
void add_missing_parentheses(List* stack, List* rpn, int* flag, int* negate_function_found,
                              int* negate_functions_counter, const int* symbol_value, int priority, int holds_operand, int arity, int* parsing_function_name, int* functions_counter, int* iterator) {

    // check if there were any 'N' functions
    check_for_negate_function(stack, rpn, flag, negate_function_found, negate_functions_counter, parsing_function_name, functions_counter, iterator);

    // save the operand to the RPN stack
    put(rpn, *symbol_value, priority, holds_operand, arity, flag);

    // here we need to close all the open parentheses - as many as there were 'N' functions
    for (int i = 0; i < *negate_functions_counter; i++) {
        check_for_operator(CLOSE_PARENTHESES, stack, rpn, flag, negate_function_found, parsing_function_name, functions_counter, iterator);
    }

    // reset the variables
    *negate_function_found = FALSE;
    *negate_functions_counter = 0;

}

// check if the last parsed symbol was a digit
void check_for_operand(int* parsing_operand, int* symbol_value, List* stack, List* rpn, int* flag, int* negate_function_found, int* negate_functions_counter, int* parsing_function_name, int* functions_counter, int* iterator) {

    // if the flag is set to true (if we are parsing a number currently)
    if (*parsing_operand) {

        // create variables to store properties
        int priority;
        int is_operand;
        int arity;

        // give the operand its properties
        set_priority(&priority, OPERAND_PRIORITY);
        set_is_operand(&is_operand, TRUE);
        set_arity(&arity, OPERAND_ARITY);

        // check if the mathematical equation uses 'N' function and if so, put the missing parentheses where necessary
        add_missing_parentheses(stack, rpn, flag, negate_function_found, negate_functions_counter, symbol_value, priority, is_operand, arity, parsing_function_name, functions_counter, iterator);

        // reset the variables
        *symbol_value = 0;
        *parsing_operand = FALSE;

    }
}


// if the symbol is an ascii of a digit ...
void check_symbol_type(int symbol_ascii, int* symbol_value, int* parsing_operand, List* stack, List* rpn, int* flag, int* negate_function_found, int* negate_functions_counter, int* parsing_function_name, int* functions_counter, int* iterator) {

    // if the symbol is in a numeric ascii range
    if (symbol_ascii >= ASCII_DIGIT_RANGE_START && symbol_ascii <= ASCII_DIGIT_RANGE_FINISH) {

        // get its value and add to the 'symbol_value' variable as part of a formula of
        // conversion ascii to its according value
        *symbol_value = *symbol_value * FACTOR + (symbol_ascii - ASCII_DIGIT_RANGE_START);

        // set the flag to true
        *parsing_operand = TRUE;

    }
    // if the symbol is an operator
    else {

        // check if the previously parsed symbol was an operand
        check_for_operand(parsing_operand, symbol_value, stack, rpn, flag, negate_function_found, negate_functions_counter, parsing_function_name, functions_counter, iterator);

        // check if there are still 'N' functions left for us to handle parentheses
        check_for_negate_function(stack, rpn, flag, negate_function_found, negate_functions_counter, parsing_function_name, functions_counter, iterator);

        // if the symbol is not an ascii of a digit
        check_for_operator(symbol_ascii, stack, rpn, flag, negate_function_found, parsing_function_name, functions_counter, iterator);

    }

}