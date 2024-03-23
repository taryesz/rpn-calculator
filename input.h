//
// Created by Tarazjusz Szulakiewicz on 11/03/2024.
//

#ifndef RPN_CALCULATOR_INPUT_H
#define RPN_CALCULATOR_INPUT_H

#endif //RPN_CALCULATOR_INPUT_H


// return the name of a function
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

    return "";  // undefined function

}


// create an array of reserved functions' names
const char** get_reserved_functions() {

    // dynamically create a char array
    const char** functions = malloc(RESERVED_FUNCTIONS_COUNTER * sizeof(char*));

    // if allocation of memory for the array wasn't successful, return null
    if (functions == NULL) return NULL;

    // fill out the array with reserved functions names
    for (int i = 0; i < RESERVED_FUNCTIONS_COUNTER; i++) functions[i] = get_function_name(i);

    // return the array with functions names inside of it
    return functions;

}


// saving operators on the RPN stack TODO: clean this function up
void convert_to_rpn(List* stack, List* rpn, int value, int priority, int is_operand, int is_function, int arity, int id, int is_function_end_symbol, int function_id, int* function_parsing_flag, List* arguments) {

    int rpn_id = DEFAULT_ID;
    int counted_arity;

    // if value.is_function = false -> set flag to false
    if (!is_function) *function_parsing_flag = FALSE;

    // create an iterator that will go through the whole stack
    Node* iterator = stack->head;

    int same_function_flag = FALSE;

    // while there are elements on the stack
    while (iterator != NULL) {

        // get the top operator symbol from the stack
        Node* popped = pop(stack);

        // if the popped symbol is a '(' or if it has a lower priority than the pushed symbol
        if (is_function) {
            if ((popped->value == OPEN_PARENTHESES || popped->priority < priority) || (*function_parsing_flag == TRUE && popped->value != value && popped->id == id)) {

                // push the popped symbol to the stack
                push(stack, popped->value, popped->priority, popped->is_operand, popped->is_function, popped->arity, popped->id, popped->is_function_end_symbol, popped->function_id);

                *function_parsing_flag = FALSE;

                // stop the saving process
                break;

            }
        }
        else {
            if (popped->value == OPEN_PARENTHESES || popped->priority < priority) {

                int calculated_arity = popped->arity;

                // push the popped symbol to the stack
                push(stack, popped->value, popped->priority, popped->is_operand, popped->is_function, calculated_arity, popped->id, popped->is_function_end_symbol, popped->function_id);

                // stop the saving process
                break;

            }

        }

        if (popped->id == id) same_function_flag = TRUE;

        // put the popped symbol to the RPN stack if it's valid for this
        set_id(rpn, &rpn_id, popped->is_function);

        if (popped->is_function && !same_function_flag) {
            same_function_flag = TRUE;
            if (arguments->head != NULL) counted_arity = pop(arguments)->value;
        }
        else if (popped->is_function && same_function_flag) {
            // keep the counted arity as is
        }
        else counted_arity = popped->arity;

        put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, counted_arity, rpn_id, popped->is_function_end_symbol, popped->function_id);

        iterator = iterator->next;

    };

    // if the last symbol was a letter,
    // set a variable that would check for another letter and don't let in into the if block

    if (is_function) *function_parsing_flag = TRUE;

    Node* tmp = pop(stack);
    if (tmp != NULL) {

        if ((tmp->value == OPEN_PARENTHESES || tmp->priority < priority) || (*function_parsing_flag == TRUE && tmp->value != value && tmp->id == id)) {
            push(stack, tmp->value, tmp->priority, tmp->is_operand, tmp->is_function, tmp->arity, tmp->id, tmp->is_function_end_symbol, tmp->function_id);
            push(stack, value, priority, is_operand, is_function, arity, id, is_function_end_symbol, function_id);
        }
        else {
            push(stack, tmp->value, tmp->priority, tmp->is_operand, tmp->is_function, tmp->arity, tmp->id, tmp->is_function_end_symbol, tmp->function_id);
        }
    }
    else {
        if (id == DEFAULT_ID) id = 0;
        push(stack, value, priority, is_operand, is_function, arity, id, is_function_end_symbol, function_id);
    }

    free(iterator);

}


// find the length of a word
void calculate_function_name_length(const char* function, int* array_length) {
    while (function[*array_length] != STRING_TERMINATOR) (*array_length)++;
    (*array_length)--; // decrement the variable since we started counting from zero
}


// add the function to the RPN stack
void save_function(List* stack, List* rpn, List* arguments, int priority, int is_operand, int is_function, int id, int arity, int function_id, int* is_function_end_symbol, int* function_parsing_flag) {

    // get the appropriate function name
    char* function = get_function_name(function_id);

    // create a variable that will store a name's length
    int array_length = 0;

    // find the length of the name
    calculate_function_name_length(function, &array_length);

    // iterate through the function's name and add each letter to the RPN stack
    for (int counter = array_length; counter >= 0; counter--) {

        // store the parsed symbol is the variable
        char symbol = function[counter];

        // all the name's symbol can't be its end symbols so set them all to false by default
        // except for the last symbol: set true for it
        (counter == array_length) ? (*is_function_end_symbol = TRUE) : (*is_function_end_symbol = FALSE);

        // add the symbol to the RPN stack
        convert_to_rpn(stack, rpn, symbol, priority, is_operand, is_function, arity, id, *is_function_end_symbol, function_id, function_parsing_flag, arguments);

        // keep the same id for each symbol of the same function
        id = stack->head->id;

    }

    // add an element to the arguments stack to keep track of the arity of a function
    push(arguments, DEFAULT_ARGUMENT_COUNT, OPERAND_PRIORITY, TRUE, FALSE, INFINITY, id, TRUE, INFINITY);

}


// parse a function and save it on RPN stack
void execute_function(int function_id, List* stack, List* rpn, List* arguments, int* negate_function_found) {

    // create property variables
    int priority, is_operand, is_function, arity, is_function_end_symbol, id;

    // create a flag that indicates if a function is being parsed or not
    int function_parsing_flag = TRUE;

    // set the properties
    set_priority(&priority, FUNCTION_PRIORITY);     // all functions have a set priority (3)
    set_is_operand(&is_operand, FALSE);             // function can't be operands
    set_is_function(&is_function, TRUE);            // function are indeed functions
    set_id(rpn, &id, is_function);        // each element on the stack has its ID
    set_arity(&arity, UNDEFINED);                   // we don't know yet how many arguments a function has

    // set the flag to true if negation function was found
    if (function_id == N) *negate_function_found = TRUE;

    // add the function to the RPN stack
    save_function(stack, rpn, arguments, priority, is_operand, is_function, id, arity, function_id, &is_function_end_symbol, &function_parsing_flag);

}


// compare input letters to a predefined function name
void compare_function_names(int symbol_ascii, List* stack, List* rpn, int* negate_function_found, int* functions_counter, int* iterator, int* similarity_found, List* arguments) {

    // get the function's id we lastly saved
    int function_id = *functions_counter;

    // get the array filled with reserved functions' names
    const char **functions = get_reserved_functions();

    // iterate through the whole array
    for (; function_id < RESERVED_FUNCTIONS_COUNTER;) {

        // store the currently parsed function in this variable
        const char* current_function = functions[function_id];

        // if the parsed symbol is the same as the n-th symbol of the current function
        if (symbol_ascii == current_function[*iterator]) {

            // set the flag to true since we found a similarity.
            // it will be useful in case we found similarities, but now
            // there is a difference in names - we will be able to locate
            // where we will need to continue our comparison
            *similarity_found = TRUE;

            // get the next symbol of the function's name
            ++(*iterator);

            // if the current function's symbol is a terminator
            if (current_function[*iterator] == STRING_TERMINATOR) {

                // the function is found, and we need to add it to the RPN stack
                execute_function(function_id, stack, rpn, arguments, negate_function_found);

                // reset the variables
                *iterator = 0;
                *functions_counter = 0;

                return;

            }

            // save the id of a function we lastly compared our symbols with
            *functions_counter = function_id;
            break;

        }

        // if the symbols are different
        else {

            // if no similarities were found, start comparing symbols from the beginning
            // else, keep the same iterator - we will keep comparing from the old spot
            (!*similarity_found) ? (*iterator = 0) : (*similarity_found = FALSE);

            // get the next function
            function_id += 1;

        }

    }

}


// declaration of a function
void process_parenthesis(int symbol_ascii, List* stack, List* rpn, int* priority, int is_operand, int is_function, int arity, int is_function_end_symbol, int function_id, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, List* arguments);


// close all the parentheses after finishing parsing the inside of a function
void process_arguments(List* stack, List* rpn, int* priority, const int* is_operand, const int* is_function, const int* arity, const int* is_function_end_symbol, const int* function_id, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, List* arguments, Node* iterator) {

    // if the counter of found functions is not 0
    if (*parsing_arguments_of_a_function) {

        // if the stack exists
        if (stack->head != NULL) {

            // if the top of the stack is a function
            if (iterator->is_function) {

                // decrement the number of found functions
                (*parsing_arguments_of_a_function)--;

                // add the missing close parenthesis
                process_parenthesis(CLOSE_PARENTHESES, stack, rpn, priority, *is_operand, *is_function, *arity, *is_function_end_symbol, *function_id, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, arguments);

            }
        }
    }

}


// if the symbol is a parenthesis ('(' or ')') TODO: clean up
void process_parenthesis(int symbol_ascii, List* stack, List* rpn, int* priority, int is_operand, int is_function, int arity, int is_function_end_symbol, int function_id, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, List* arguments) {

    int id = DEFAULT_ID;

    int finished_parsing_function = FALSE;

    // set the parenthesis priority
    set_priority(priority, PARENTHESIS_PRIORITY);

    // if the symbol is a '(', just push it to the operators stack
    if (symbol_ascii == OPEN_PARENTHESES) {

        // give the parenthesis an id
        set_id(rpn, &id, is_function);

        // save the parenthesis
        push(stack, symbol_ascii, *priority, is_operand, is_function, arity, id, is_function_end_symbol, function_id);

    }

    // if the symbol is a ')'
    if (symbol_ascii == CLOSE_PARENTHESES) {

        Node* counted_arity = pop(arguments); // free()
        int argument_arity_flag = FALSE;

        // create an iterator that will go through the whole stack
        Node* iterator = stack->head;

        // while there are elements in the stack
        do {

            if (finished_parsing_function == TRUE && arguments->head != NULL) {
                counted_arity = pop(arguments); // free()
            }

            // get the top operator symbol from the stack
            Node* popped = pop(stack);

            // if the symbol is a '(', stop the loop
            if (popped->value == OPEN_PARENTHESES) {

                if (stack->head != NULL) {
                    if (stack->head->is_function || !argument_arity_flag) {
                        if (counted_arity != NULL) push(arguments, counted_arity->value, counted_arity->priority, counted_arity->is_operand, counted_arity->is_function, counted_arity->arity, counted_arity->id, counted_arity->is_function_end_symbol, counted_arity->function_id);
                    }
                }

                // if we finished parsing the inside of a function, we need to close all the open parentheses
                process_arguments(stack, rpn, priority, &is_operand, &is_function, &arity, &is_function_end_symbol, &function_id, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, arguments, stack->head);

                break;

            }

                // if the symbol is something else
            else {

                argument_arity_flag = TRUE;

                // save the symbol to the RPN stack
                set_id(rpn, &id, popped->is_function);

                if (popped->is_function || popped->value == NEGATE) {
                    finished_parsing_function = popped->is_function_end_symbol;
                }
                else {
                    if (counted_arity != NULL) {
                        push(arguments, counted_arity->value, counted_arity->priority, counted_arity->is_operand, counted_arity->is_function, counted_arity->arity, counted_arity->id, counted_arity->is_function_end_symbol, counted_arity->function_id);
                        counted_arity = NULL; // ????
                    }
                }

                if (popped != NULL) {
                    if (counted_arity != NULL) put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function,counted_arity->value, id, popped->is_function_end_symbol, popped->function_id);
                    else put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, popped->arity, id, popped->is_function_end_symbol, popped->function_id);
                }

            }

            iterator = iterator->next;
            free(popped);

        }
        while(iterator != NULL);

        free(iterator);

    }
}


// add as many close parentheses as there were N-functions found (this function is used when the N function didn't originally have parentheses and its argument was a single number)
void process_missing_parentheses(List* stack, List* rpn, int* priority, const int* is_operand, const int* is_function, int* arity, const int* is_function_end_symbol, const int* function_id, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, List* arguments) {

    // if the N-function's open parentheses counter is not 0
    if (*parsing_arguments_of_a_function) {

        // if stack exists
        if (stack->head != NULL) {

            // if the top of the stack is an open parenthesis
            if (stack->head->value == OPEN_PARENTHESES) {

                // if the 2-nd top symbol of the stack exists
                if (stack->head->next != NULL) {

                    // if the 2-nd top symbol of the stack is an N-function
                    if (stack->head->next->function_id == N) {

                        // set parenthesis properties
                        set_priority(priority, PARENTHESIS_PRIORITY);
                        set_arity(arity, UNDEFINED);

                        // decrement the number of open parentheses of N-function left to parse
                        (*parsing_arguments_of_a_function)--;

                        // add a close parenthesis
                        process_parenthesis(CLOSE_PARENTHESES, stack, rpn, priority, *is_operand, *is_function, *arity, *is_function_end_symbol, *function_id, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, arguments);

                    }

                }
            }
        }
    }

}


// if the symbol is an operator ('+', '-', '*' or '/')
void process_operator(int symbol_ascii, List* stack, List* rpn, int* priority, int is_operand, int is_function, int* arity, int is_function_end_symbol, int function_id, int priority_value, int* parsing_arguments_of_a_function, int* last_symbol, int* negate_functions_counter, int* function_open_parenthesis_id, List* arguments) {

    // create an ID for an operator
    int id = DEFAULT_ID;

    // before adding next operator to RPN, firstly we need to make sure that all open parentheses have a pair i.e. close parentheses
    process_missing_parentheses(stack, rpn, priority, &is_operand, &is_function, arity, &is_function_end_symbol, &function_id, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, arguments);

    // set properties of an operator
    set_priority(priority, priority_value);         // priority depends on the operator
    set_arity(arity, UNDEFINED);                    // we haven't found arity for the operator yet
    set_function_id(&function_id, UNDEFINED);       // simple operators don't have enum function id
    set_id(rpn, &id, is_function);        // each operator has its own ID

    // create a flag that indicates if a function is being parsed or not
    int function_parsing_flag = FALSE;

    // add the operator to RPN
    convert_to_rpn(stack, rpn, symbol_ascii, *priority, is_operand, is_function, *arity, id, is_function_end_symbol, function_id, &function_parsing_flag, arguments);

}


// parse the rest of the symbols if there are any
void check_for_stop(int symbol_ascii, List* stack, List* rpn, List* arguments) {

    // if the symbol is a period '.'
    if (symbol_ascii == EQUATION_STOP_SYMBOL) {

        // create an ID for an element
        int id = DEFAULT_ID;

        // create a flag that indicates if a function is being parsed or not
        int function_parsing_flag;

        // while the operators stack is not empty
        while (stack->head != NULL) {

            // if the symbol is a function, set the flag
            (stack->head->is_function) ? (function_parsing_flag = TRUE) : (function_parsing_flag = FALSE);

            // set the ID for the element
            set_id(rpn, &id, stack->head->is_function);

            // add the symbol to the RPN stack
            convert_to_rpn(stack, rpn, stack->head->value, stack->head->priority, stack->head->is_operand, stack->head->is_function, stack->head->arity, id, stack->head->is_function_end_symbol, stack->head->function_id, &function_parsing_flag, arguments);

            // remove the pushed operator from the stack
            pop(stack);

        }

    }

}


// check what operator was passed TODO: clean up COMMA
void check_for_operator(int symbol_ascii, List *stack, List *rpn, List* arguments, int *negate_function_found, int *functions_counter, int *iterator, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, int* similarity_found) {

    // create property variables
    int is_operand, is_function, priority, arity, is_function_end_symbol, function_id;

    // set the properties
    set_is_operand(&is_operand, FALSE);                         // operators can't be operand
    set_is_function(&is_function, FALSE);                       // by default operator is not a function
    set_is_function_end_symbol(&is_function_end_symbol, FALSE); // not all symbols end the name
    set_arity(&arity, UNDEFINED);                               // arity is not yet calculated
    set_function_id(&function_id, UNDEFINED);                   // not all operators have enum function id

    // parse the rest of the symbols if there are any
    check_for_stop(symbol_ascii, stack, rpn, arguments);

    // if the symbol is a parenthesis
    if (symbol_ascii == OPEN_PARENTHESES || symbol_ascii == CLOSE_PARENTHESES) {
        set_arity(&arity, PARENTHESIS_ARITY);
        process_parenthesis(symbol_ascii, stack, rpn, &priority, is_operand, is_function, arity, is_function_end_symbol, function_id, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, arguments);
    }

    // if the symbol is a '+' or a '-', set the according priority and push to RPN stack
    else if (symbol_ascii == PLUS || symbol_ascii == MINUS) {
        process_operator(symbol_ascii, stack, rpn, &priority, is_operand, is_function, &arity, is_function_end_symbol, function_id, OPERATOR_PLUS_MINUS_PRIORITY, parsing_arguments_of_a_function, last_symbol, negate_functions_counter, function_open_parenthesis_id, arguments);
    }

    // if the symbol is a '*' or a '/', set the according priority and push to RPN stack
    else if (symbol_ascii == ASTERISK || symbol_ascii == SLASH) {
        process_operator(symbol_ascii, stack, rpn, &priority, is_operand, is_function, &arity, is_function_end_symbol, function_id, OPERATOR_ASTERISK_SLASH_PRIORITY, parsing_arguments_of_a_function, last_symbol, negate_functions_counter, function_open_parenthesis_id, arguments);
    }

    // if the symbol is a capital letter (i.e. a part of a function name)
    else if (symbol_ascii >= ASCII_LETTER_RANGE_START && symbol_ascii <= ASCII_LETTER_RANGE_FINISH) {
        compare_function_names(symbol_ascii, stack, rpn, negate_function_found, functions_counter, iterator, similarity_found, arguments);
    }

    // if the symbol is a comma (which means that in the context of this program, we are iterating through a function)
    else if (symbol_ascii == COMMA) {

        // if the stack is not empty
        if (stack->head != NULL) {

            // if the last symbol found is a function
            if (stack->head->is_function) {

                // create an iterator
                Node *argument = stack->head;

                // have to delete the head of the 'arguments' stack in order to keep the right track of arity
                Node* popped_argument_counter = pop(arguments);
                int counted_arity;
                if (popped_argument_counter != NULL) counted_arity = popped_argument_counter->value;

                do {

                    // get the latest symbol from the stack
                    Node *popped = pop(stack);

                    // if it's not a function, keep its original arity (this is a simple operator)
                    if (!popped->is_function) counted_arity = popped->arity;

                    // put the popped symbol into the RPN expression
                    put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, counted_arity, popped->id, popped->is_function_end_symbol, popped->function_id);

                    // get the next iterator
                    argument = argument->next;

                }
                // iterate until an open parenthesis is found
                while (argument != NULL && argument->value != OPEN_PARENTHESES);

            }
            // if the last symbol is not a function
            else {

                // and if its predecessor is a negation function
                if (stack->head->next->value == NEGATE) {
                    // we need to get all the stuff inside this function
                    process_arguments(stack, rpn, &priority, &is_operand, &is_function, &arity, &is_function_end_symbol, &function_id, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, arguments, stack->head->next);
                }
            }

            // if the symbol from the stack is not an open parenthesis
            if (stack->head->value != OPEN_PARENTHESES) {

                // create an iterator
                Node *argument = stack->head;

                do {

                    // get the latest symbol from the stack
                    Node *popped = pop(stack);

                    int counted_arity = INFINITY;

                    // if the symbol is a negate function, remove the head from 'arguments'
                    // 'cause technically negation is also a function
                    if (popped != NULL) {
                        if (popped->value == NEGATE) {
                            if (arguments->head != NULL) counted_arity = pop(arguments)->value;
                        }
                        // or else keep its original arity
                        else counted_arity = popped->arity;
                    }

                    // save the parsed argument
                    put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, counted_arity, popped->id, popped->is_function_end_symbol, popped->function_id);

                    // increment the iterator
                    argument = argument->next;

                }
                // iterate until an open parenthesis is found
                while (argument != NULL && argument->value != OPEN_PARENTHESES);

            }

        }

        if (arguments->head != NULL) arguments->head->value += 1;

    }

    // Update the last_symbol
    if (last_symbol != NULL) *last_symbol = symbol_ascii;

}


// check if there were any 'N' functions
void check_for_negate_function(List *stack, List *rpn, int *negate_function_found, int *negate_functions_counter,int *functions_counter, int *iterator, int* last_function, const int* current_symbol, int* last_symbol, int* function_open_parenthesis_id, int* parsing_arguments_of_a_function, int* similarity_found, List* arguments) {

    // if the function is found
    if (*negate_function_found == TRUE) {

        // if the negation function has its own parenthesis, we don't need to add any ourselves
        if (*current_symbol != OPEN_PARENTHESES) {

            if (stack->head != NULL) {

                // increment number of parentheses we will need to close later
                (*parsing_arguments_of_a_function)++;

                // add the missing open parenthesis
                check_for_operator(OPEN_PARENTHESES, stack, rpn, arguments, negate_function_found, functions_counter, iterator, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, similarity_found);

                // increment the number of 'N' functions
                *negate_functions_counter += 1;

            }

        }

        // reset the flag
        *negate_function_found = FALSE;

    }

}


// check if the mathematical equation uses 'N' function and if so, put the missing parentheses where necessary
void process_operand(List *stack, List *rpn, int *negate_function_found, int *negate_functions_counter, int *symbol_value, int priority, int is_operand, int is_function, int arity, int is_function_end_symbol, int function_id, int *functions_counter, int *iterator, int* last_function, int* last_symbol, int* function_open_parenthesis_id, int* parsing_arguments_of_a_function, int* similarity_found, List* arguments) {

    // check if there were any 'N' functions that have no open parenthesis, and if so, add one
    check_for_negate_function(stack, rpn, negate_function_found, negate_functions_counter, functions_counter, iterator, last_function, symbol_value, last_symbol, function_open_parenthesis_id, parsing_arguments_of_a_function, similarity_found, arguments);

    // create and set an ID for an operand
    int id = DEFAULT_ID;
    set_id(rpn, &id, is_function);

    // save the operand to the RPN stack
    put(rpn, *symbol_value, priority, is_operand, is_function, arity, id, is_function_end_symbol, function_id);

    // reset the variables
    *negate_function_found = FALSE;

}


// check if the last parsed symbol was a digit
void check_for_operand(int *parsing_operand, int *symbol_value, List *stack, List *rpn, int *negate_function_found, int *negate_functions_counter, int *functions_counter, int *iterator, int* last_function, int* close_parenthesis_flag, int* function_open_parenthesis_id, int* parsing_arguments_of_a_function, int* similarity_found, List* arguments) {

    // if we are parsing a number currently
    if (*parsing_operand) {

        // create variables to store properties
        int priority, is_operand, is_function, arity, is_function_end_symbol, function_id;

        // give the operand its properties
        set_priority(&priority, OPERAND_PRIORITY);                      // the operand has its own property
        set_is_operand(&is_operand, TRUE);                              // the operand is in fact an operand
        set_arity(&arity, UNDEFINED);                                   // the operand doesn't have the arity property
        set_is_function(&is_function, FALSE);                           // the operand can't be a function
        set_is_function_end_symbol(&is_function_end_symbol, UNDEFINED); // the operand can't store end symbol info
        set_function_id(&function_id, UNDEFINED);                       // the operand doesn't have enum function id

        // put the operand on the RPN stack, with additional check for negation function
        process_operand(stack, rpn, negate_function_found, negate_functions_counter, symbol_value, priority, is_operand, is_function, arity, is_function_end_symbol, function_id, functions_counter, iterator, last_function, close_parenthesis_flag, function_open_parenthesis_id, parsing_arguments_of_a_function, similarity_found, arguments);

        // reset the variables
        *symbol_value = 0;
        *parsing_operand = FALSE;

    }

}


// if the symbol is an ascii of a digit
void check_symbol_type(int symbol_ascii, int *symbol_value, int *parsing_operand, List *stack, List *rpn, List* arguments, int *negate_function_found, int *negate_functions_counter, int *functions_counter, int *iterator, int* last_function, int* last_symbol, int* function_open_parenthesis_id, int* parsing_arguments_of_a_function, int* similarity_found) {

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
        check_for_operand(parsing_operand, symbol_value, stack, rpn, negate_function_found, negate_functions_counter, functions_counter, iterator, last_function, last_symbol, function_open_parenthesis_id, parsing_arguments_of_a_function, similarity_found, arguments);

        // check if there are still 'N' functions left for us to handle parentheses
        check_for_negate_function(stack, rpn, negate_function_found, negate_functions_counter, functions_counter, iterator, last_function, &symbol_ascii, last_symbol, function_open_parenthesis_id, parsing_arguments_of_a_function, similarity_found, arguments);

        // if the symbol is not an ascii of a digit
        check_for_operator(symbol_ascii, stack, rpn, arguments, negate_function_found, functions_counter, iterator, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, similarity_found);

    }

}
