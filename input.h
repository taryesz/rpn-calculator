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
void set_is_function(int* is_function, int value) {
    *is_function = value;
}
void set_id(List* rpn, int* id, int current_is_function, int value) {
    if (rpn->tail == NULL) {
        *id = 0;
    }
    else if (rpn->tail->is_function && (current_is_function || value == NEGATE) && !rpn->tail->is_function_end_symbol) {
        *id = rpn->tail->id;
    }
    else {
        *id = rpn->tail->id + 1;
    }
}
void set_is_function_end_symbol (int* is_function_end_symbol, int value) {
    *is_function_end_symbol = value;
}
void set_function_id(int* function_id, int value) {
    *function_id = value;
}


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
void convert_to_rpn(List* stack, List* rpn, int value, int priority, int is_operand, int is_function, int arity, int id, int is_function_end_symbol, int function_id, int* flag, int* function_parsing_flag, List* arguments) {

    int rpn_id = DEFAULT_ID;

    // if value.is_function = false -> set flag to false
    if (!is_function) *function_parsing_flag = FALSE;

    // create an iterator that will go through the whole stack
    Node* iterator = stack->head;

    // while there are elements on the stack
    while (iterator != NULL) {

        // get the top operator symbol from the stack
        Node* popped = pop(stack);

        // if the popped symbol is a '(' or if it has a lower priority than the pushed symbol
        // FOR FUNCTION BLOCK SHOULD NOT ENTER THIS IF CODE BLOCK
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

                // push the popped symbol to the stack
                push(stack, popped->value, popped->priority, popped->is_operand, popped->is_function, popped->arity, popped->id, popped->is_function_end_symbol, popped->function_id);

                // stop the saving process
                break;

            }

        }

        // put the popped symbol to the RPN stack if it's valid for this
        set_id(rpn, &rpn_id, popped->is_function, popped->value);
        int counted_arity = arguments->head->value;
        put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, counted_arity, rpn_id, popped->is_function_end_symbol, popped->function_id, flag);
//        put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, popped->arity, rpn_id, popped->is_function_end_symbol, flag);

        iterator = iterator->next;

    };

    // if the last symbol was a letter,
    // set a variable that would check for another letter and don't let in into the if block

    if (is_function) {
        *function_parsing_flag = TRUE;
    }

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
        if (id == -1) id = 0;
        push(stack, value, priority, is_operand, is_function, arity, id, is_function_end_symbol, function_id);
    }

    free(iterator);

}


// find the length of a word
void calculate_function_name_length(const char* function, int* array_length) {
    while (function[*array_length] != STRING_TERMINATOR) {
        (*array_length)++;
    }
}


// add the function to the RPN stack
void save_function(List* stack, List* rpn, List* arguments, int priority, int is_operand, int is_function, int id, int arity, int function_id, int* flag, int* is_function_end_symbol, int* function_parsing_flag) {

    // get the appropriate function name
    char* function = get_function_name(function_id);

    // find the length of the function's name
    int array_length = 0;
    calculate_function_name_length(function, &array_length);

    // iterate through the function name and add each letter to the RPN stack
    for (int counter = array_length - 1; counter >= 0; counter--) {

        // the last symbol from the function's name has to indicate that it's
        if (counter == array_length - 1) *is_function_end_symbol = TRUE;
        else *is_function_end_symbol = FALSE;

        // add the symbol to the RPN stack
        convert_to_rpn(stack, rpn, function[counter], priority, is_operand, is_function, arity, id, *is_function_end_symbol, function_id, flag, function_parsing_flag, arguments);

        // keep the same id for each symbol of the same function
        id = stack->head->id;

    }

    push(arguments, DEFAULT_ARGUMENT_COUNT, OPERAND_PRIORITY, TRUE, FALSE, INFINITY, id, TRUE, INFINITY);

}


// parse a function and save it on RPN stack
void execute_function(int function_id, List* stack, List* rpn, List* arguments, int* flag, int* negate_function_found) {

    // create property variables
    int priority, is_operand, is_function, arity, is_function_end_symbol, id = DEFAULT_ID, function_parsing_flag = TRUE;

    // set the properties
    set_priority(&priority, FUNCTION_PRIORITY);
    set_is_operand(&is_operand, FALSE);
    set_is_function(&is_function, TRUE);
    set_arity(&arity, INFINITY);
    set_id(rpn, &id, is_function, INFINITY);

    // create a variable that will be passed to 'save_function()'
    int function = INFINITY;

    // check which function is being parsed right now
    switch (function_id) {
        case N:
            set_arity(&arity, UNARY_ARITY);
            set_is_function(&is_function, FALSE);
            function = N;
            *negate_function_found = TRUE;
            break;
        case IF:
            set_arity(&arity, TERNARY_ARITY);
            function = IF;
            break;
        case MIN:
            function = MIN;
            break;
        case MAX:
            function = MAX;
            break;
        default:
            return;  // unexpected behaviour
    }

    // add the function to the RPN stack
    save_function(stack, rpn, arguments, priority, is_operand, is_function, id, arity, function, flag, &is_function_end_symbol, &function_parsing_flag);

    // set the flag to false
    function_parsing_flag = FALSE;

}


// compare input letters to a predefined function name
void compare_function_names(int symbol_ascii, List *stack, List *rpn, int *flag, int *negate_function_found, int *functions_counter, int *iterator, int* last_function, int* similarity_found, List* arguments) {

    int function_id = *functions_counter;

    const char **functions = get_reserved_functions();

    for (; function_id < RESERVED_FUNCTIONS_COUNTER;) {

        const char* current_function = functions[function_id];

        if (symbol_ascii == current_function[*iterator]) {

            *similarity_found = TRUE;

            ++(*iterator);

            if (current_function[*iterator] == STRING_TERMINATOR) {

                execute_function(function_id, stack, rpn, arguments, flag, negate_function_found);

                *iterator = 0;
                *last_function = *functions_counter;
                *functions_counter = 0;
                return;

            }

            *functions_counter = function_id;
            break;

        }
        else {

            if (!*similarity_found) *iterator = 0;
            else { *similarity_found = FALSE; }
            function_id += 1;

        }

    }

}


void process_parenthesis(int symbol_ascii, List* stack, List* rpn, int* priority, int is_operand, int is_function, int arity, int is_function_end_symbol, int function_id, int* flag, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete, List* arguments);


// close all the parentheses after finishing parsing the inside of a function (this function is used when the N function's argument was another function)
void process_arguments(List* stack, List* rpn, int* priority, const int* is_operand, const int* is_function, const int* arity, const int* is_function_end_symbol, int* function_id, int* flag, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete, List* arguments, Node* iterator) {

    // if the counter of found functions is not 0
    if (*parsing_arguments_of_a_function) {

//        printf("awsd\n");

        // if the stack exists
        if (stack->head != NULL) {

//            printf("xyz\n");

            // if the top of the stack is a function
//            if (stack->head->is_function || stack->head->value == NEGATE) {
            if (iterator->is_function || iterator->value == NEGATE) {

//                printf("abc\n");

                // decrement the number of found functions
                (*parsing_arguments_of_a_function)--;

                *close_parenthesis_autocomplete = TRUE; // TODO: delete this line

                // add the missing close parenthesis
                process_parenthesis(CLOSE_PARENTHESES, stack, rpn, priority, *is_operand, *is_function, *arity,
                                    *is_function_end_symbol, *function_id, flag, last_symbol, function_open_parenthesis_id,
                                    negate_functions_counter, parsing_arguments_of_a_function,
                                    close_parenthesis_autocomplete, arguments);

            }
        }
    }

}


// if the symbol is a parenthesis ('(' or ')')
void process_parenthesis(int symbol_ascii, List* stack, List* rpn, int* priority, int is_operand, int is_function, int arity, int is_function_end_symbol, int function_id, int* flag, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete, List* arguments) {

    int id = DEFAULT_ID;

    // set the parenthesis priority
    set_priority(priority, PARENTHESIS_PRIORITY);

    // if the symbol is a '(', just push it to the operators stack
    if (symbol_ascii == OPEN_PARENTHESES) {

        // give the parenthesis an id
        set_id(rpn, &id, is_function, symbol_ascii);

        // save the parenthesis
        push(stack, symbol_ascii, *priority, is_operand, is_function, arity, id, is_function_end_symbol, function_id);

    }

    // if the symbol is a ')'
    if (symbol_ascii == CLOSE_PARENTHESES) {

        // create an iterator that will go through the whole stack
        Node* iterator = stack->head;

        // while there are elements in the stack
        do {

            // get the top operator symbol from the stack
            Node* popped = pop(stack);

            // if the symbol is a '(', stop the loop
            if (popped->value == OPEN_PARENTHESES) {

                // if we finished parsing the inside of a function, we need to close all the open parentheses
                process_arguments(stack, rpn, priority, &is_operand, &is_function, &arity, &is_function_end_symbol, &function_id, flag, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, close_parenthesis_autocomplete, arguments, stack->head);

                break;

            }

            // if the symbol is something else
            else {

                // save the symbol to the RPN stack
                set_id(rpn, &id, popped->is_function, popped->value);
                put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, popped->arity, id, popped->is_function_end_symbol, popped->function_id, flag);

            }

            iterator = iterator->next;
            free(popped);

        }
        while(iterator != NULL);

        free(iterator);

    }
}


// add as many close parentheses as there were N-functions found ((this function is used when the N function didn't originally have parentheses and its argument was a single number)
void process_missing_parentheses(List* stack, List* rpn, int* priority, const int* is_operand, const int* is_function, int* arity, const int* is_function_end_symbol, int* function_id, int* flag, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete, List* arguments) {

    // if the N-function's open parentheses counter is not 0
    if (*parsing_arguments_of_a_function) {

        // if stack exists
        if (stack->head != NULL) {

            // if the top of the stack is an open parenthesis
            if (stack->head->value == OPEN_PARENTHESES) {

                // if the 2-nd top symbol of the stack exists
                if (stack->head->next != NULL) {

                    // if the 2-nd top symbol of the stack is an N-function
                    if (stack->head->next->value == NEGATE) {

                        // set parenthesis properties
                        set_priority(priority, PARENTHESIS_PRIORITY);
                        set_arity(arity, INFINITY);

                        // decrement the number of open parentheses of N-function left to parse
                        (*parsing_arguments_of_a_function)--;

                        // add a close parenthesis
                        process_parenthesis(CLOSE_PARENTHESES, stack, rpn, priority, *is_operand, *is_function, *arity,
                                            *is_function_end_symbol, *function_id, flag, last_symbol, function_open_parenthesis_id,
                                            negate_functions_counter, parsing_arguments_of_a_function,
                                            close_parenthesis_autocomplete, arguments);

                    }

                }
            }
        }
    }

}


// if the symbol is an operator ('+', '-', '*' or '/')
void process_operator(int symbol_ascii, List* stack, List* rpn, int* priority, int is_operand, int is_function, int* arity, int is_function_end_symbol, int function_id, int* flag, int priority_value, int* parsing_arguments_of_a_function, int* last_symbol, int* negate_functions_counter, int* function_open_parenthesis_id, int* close_parenthesis_autocomplete, List* arguments) {

    // before adding next operator to RPN, firstly we need to make sure that all open parentheses have a pair i.e. close parentheses
    process_missing_parentheses(stack, rpn, priority, &is_operand, &is_function, arity, &is_function_end_symbol, &function_id, flag, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, close_parenthesis_autocomplete, arguments);

    // set properties of an operator
    set_priority(priority, priority_value);
    set_arity(arity, BINARY_ARITY);
    int function_parsing_flag = FALSE;
    int id = DEFAULT_ID;
    set_id(rpn, &id, is_function, symbol_ascii);
    set_function_id(&function_id, symbol_ascii);

    // add the operator to RPN
    convert_to_rpn(stack, rpn, symbol_ascii, *priority, is_operand, is_function, *arity, id, is_function_end_symbol, function_id, flag, &function_parsing_flag, arguments);

}


// check if the symbol is a '.'
void check_for_stop(int symbol_ascii, List* stack, List* rpn, int* flag, List* arguments) {

    if (symbol_ascii == EQUATION_STOP_SYMBOL) {

        int id = DEFAULT_ID;

        // while the operators stack is not empty
        while (stack->head != NULL) {

            int function_parsing_flag = FALSE;

            if (stack->head->is_function == TRUE) {
                function_parsing_flag = TRUE;
            }

            set_id(rpn, &id, stack->head->is_function, stack->head->value);

            convert_to_rpn(stack, rpn, stack->head->value, stack->head->priority, stack->head->is_operand, stack->head->is_function, stack->head->arity, id, stack->head->is_function_end_symbol, stack->head->function_id, flag, &function_parsing_flag, arguments);

            // remove the pushed operator from the stack
            pop(stack);

        }

    }

}


// check what operator was passed
void check_for_operator(int symbol_ascii, List *stack, List *rpn, List* arguments, int *flag, int *negate_function_found, int *functions_counter, int *iterator, int* last_function, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete, int* similarity_found) {

    // create a const to indicate that the symbol doesn't contain a numerical value
    int is_operand;
    int is_function;
    int priority;
    int arity;
    int is_function_end_symbol;
    int function_id;

    // for all the symbols set 'is_operand' to false, since they are operators
    set_is_operand(&is_operand, FALSE);
    set_is_function(&is_function, FALSE);
    set_is_function_end_symbol(&is_function_end_symbol, FALSE);
    set_arity(&arity, INFINITY);
    set_function_id(&function_id, INFINITY);

    // if the symbol is a '.'
    check_for_stop(symbol_ascii, stack, rpn, flag, arguments);

    // if the symbol is a parenthesis
    if (symbol_ascii == OPEN_PARENTHESES || symbol_ascii == CLOSE_PARENTHESES) {
        set_arity(&arity, PARENTHESIS_ARITY);
        process_parenthesis(symbol_ascii, stack, rpn, &priority, is_operand, is_function, arity, is_function_end_symbol, function_id,
                            flag, last_symbol, function_open_parenthesis_id, negate_functions_counter,
                            parsing_arguments_of_a_function, close_parenthesis_autocomplete, arguments);
    }

        // if the symbol is a '+' or a '-', set the according priority and push to RPN stack
    else if (symbol_ascii == PLUS || symbol_ascii == MINUS) {
        process_operator(symbol_ascii, stack, rpn, &priority, is_operand, is_function, &arity, is_function_end_symbol, function_id,
                         flag, OPERATOR_PLUS_MINUS_PRIORITY, parsing_arguments_of_a_function, last_symbol,
                         negate_functions_counter, function_open_parenthesis_id, close_parenthesis_autocomplete, arguments);
    }

        // if the symbol is a '*' or a '/', set the according priority and push to RPN stack
    else if (symbol_ascii == ASTERISK || symbol_ascii == SLASH) {
        process_operator(symbol_ascii, stack, rpn, &priority, is_operand, is_function, &arity, is_function_end_symbol, function_id, flag, OPERATOR_ASTERISK_SLASH_PRIORITY, parsing_arguments_of_a_function, last_symbol, negate_functions_counter, function_open_parenthesis_id, close_parenthesis_autocomplete, arguments);
    }

        // if the symbol is a capital letter (i.e. a part of a function name)
    else if (symbol_ascii >= ASCII_LETTER_RANGE_START && symbol_ascii <= ASCII_LETTER_RANGE_FINISH || symbol_ascii == SPACE) {
        compare_function_names(symbol_ascii, stack, rpn, flag, negate_function_found, functions_counter, iterator, last_function, similarity_found, arguments);
    }

        // if the symbol is a comma (which means that in the context of this program, we are iterating through a function)
    else if (symbol_ascii == COMMA) {

        if (stack->head != NULL) {

            if (stack->head->is_function) {

                Node *argument = stack->head;

                int counted_arity = pop(arguments)->value; // free()

                do {

                    Node *popped = pop(stack);

                    // Put the popped symbol into the RPN expression
                    put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, counted_arity, popped->id, popped->is_function_end_symbol, popped->function_id, flag);

                    argument = argument->next;

                } while (argument != NULL && argument->value != OPEN_PARENTHESES); // Check if argument is not NULL before accessing its value

            }
            else {
                if (stack->head->next->value == NEGATE) {
//                    free(pop(stack));
                    // ignore
//                    printf("what: %d", *parsing_arguments_of_a_function);
                    process_arguments(stack, rpn, &priority, &is_operand, &is_function, &arity, &is_function_end_symbol,
                                      &function_id, flag, last_symbol, function_open_parenthesis_id,
                                      negate_functions_counter, parsing_arguments_of_a_function,
                                      close_parenthesis_autocomplete, arguments, stack->head->next);

//                    printf("KKK:\n");
//                    print(stack);
                }
            }

            if (stack->head->value != OPEN_PARENTHESES) {

                Node *argument = stack->head;

                do {

                    Node *popped = pop(stack);

                    int counted_arity = popped->arity;

                    put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, counted_arity, popped->id, popped->is_function_end_symbol, popped->function_id, flag);

                    argument = argument->next;

                } while (argument != NULL && argument->value != OPEN_PARENTHESES);

            }

        }



        arguments->head->value += 1;

        /*  TODO:
         *
         *      (*) check if the comma's previous is a function
         *          yes -> pop(arguments), set the letters' arity to the value stored on the head of the stack
         *          no -> ignore
         *      found any comma? (doesn't really matter which one 'internal' or 'external')
         *          update the 'arguments' head value
         *      found another function? -> push a new element to the stack 'arguments'
         *      go back to (*)
         *
         */

        // Check if the stack is not empty
//        if (stack->head != NULL) {
//
//            // Check if the head of the stack is a function
//            if (stack->head->is_function) {
//
//                Node *argument = stack->head;
//
//                int counted_arity = arguments->head->value;
//
//                do {
//
//                    Node *popped = pop(stack);
//
//                    // Put the popped symbol into the RPN expression
//                    put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, counted_arity, popped->id, popped->is_function_end_symbol, flag);
//
//                    argument = argument->next;
//
//                } while (argument != NULL && argument->value != OPEN_PARENTHESES); // Check if argument is not NULL before accessing its value
//
//                free(pop(arguments));
//
//            }
//            else {
//
//                // TODO: WHEN THERE IS A SITUATION THAT THE ARGUMENT IS A SIMPLE MATH EQUATION (LIKE 2*5)
//                // TODO: WITHOUT BRACES, THE COMMA AFTER THIS IS CONSIDERED AS A INTERNAL COMMA, AND NOT LIKE THE
//                // TODO: EXTERNAL ONE. SHOULD CHANGE THIS PIECE OF CODE TO ACTUALLY CORRECTLY COUNT COMMAS.
//
//                arguments->head->value += 1; // has to be in this place, otherwise it won't work
//
//                if (stack->head->value != OPEN_PARENTHESES) {
//
//                    Node *argument = stack->head;
//
//                    do {
//
//                        Node *popped = pop(stack);
//
//                        int counted_arity = popped->arity;
//
////                        if (stack->head->next->next->is_function && stack->head->next->next->id == arguments->head->id) {
////                            arguments->head->value += 1;
////                            counted_arity = arguments->head->value;
////                        }
//
//                        put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, counted_arity, popped->id, popped->is_function_end_symbol, flag);
//
//                        argument = argument->next;
//
//                    }
//                    while (argument != NULL && argument->value != OPEN_PARENTHESES);
//                }
//                else {
//
//                }
//
//            }
//        }

        // Update the last_symbol
        *last_symbol = symbol_ascii;

    }

}


// check if there were any 'N' functions
void check_for_negate_function(List *stack, List *rpn, int *flag, int *negate_function_found, int *negate_functions_counter,int *functions_counter, int *iterator, int* last_function, const int* current_symbol, int* last_symbol, int* function_open_parenthesis_id, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete, int* similarity_found, List* arguments) {

    // if the function is found
    if (*negate_function_found == TRUE) {

        if (*current_symbol != OPEN_PARENTHESES) {

            if (stack->head != NULL) {

                // if the head of the stack is an 'N' and the previous element is not a letter, then go inside this if-block
                if (stack->head->next != NULL) {

                    if (!(stack->head->next->value >= ASCII_LETTER_RANGE_START && stack->head->next->value <= ASCII_LETTER_RANGE_FINISH) && stack->head->value == NEGATE) {

                        // increment number of parentheses we will need to close later
                        (*parsing_arguments_of_a_function)++;

                        // add the missing open parenthesis
                        check_for_operator(OPEN_PARENTHESES, stack, rpn, arguments, flag, negate_function_found, functions_counter,
                                           iterator, last_function, last_symbol, function_open_parenthesis_id,
                                           negate_functions_counter, parsing_arguments_of_a_function,
                                           close_parenthesis_autocomplete, similarity_found);

                        // increment the number of 'N' functions
                        *negate_functions_counter += 1;
//
//                        printf("found an N\n");
//                        print(stack);
//                        printf("\n");

                    }
                    else {

                        // increment number of parentheses we will need to close later
                        (*parsing_arguments_of_a_function)++;

                        // add the missing open parenthesis
                        check_for_operator(OPEN_PARENTHESES, stack, rpn, arguments, flag, negate_function_found, functions_counter,
                                           iterator, last_function, last_symbol, function_open_parenthesis_id,
                                           negate_functions_counter, parsing_arguments_of_a_function,
                                           close_parenthesis_autocomplete, similarity_found);

                        // increment the number of 'N' functions
                        *negate_functions_counter += 1;
                    }

                }
                // TODO: THIS WAS ADDED AND IT FIXED " N N ( 13 ) . "
                else {

                    // increment number of parentheses we will need to close later
                    (*parsing_arguments_of_a_function)++;

                    // add the missing open parenthesis
                    check_for_operator(OPEN_PARENTHESES, stack, rpn, arguments, flag, negate_function_found, functions_counter,
                                   iterator, last_function, last_symbol, function_open_parenthesis_id,
                                   negate_functions_counter, parsing_arguments_of_a_function,
                                   close_parenthesis_autocomplete, similarity_found);

                    // increment the number of 'N' functions
                    *negate_functions_counter += 1;
                }

            }

        }

        *negate_function_found = FALSE;

    }

}


// check if the mathematical equation uses 'N' function and if so, put the missing parentheses where necessary
void process_operand(List *stack, List *rpn, int *flag, int *negate_function_found, int *negate_functions_counter, int *symbol_value, int priority, int is_operand, int is_function, int arity, int is_function_end_symbol, int function_id, int *functions_counter, int *iterator, int* last_function, int* last_symbol, int* function_open_parenthesis_id, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete, int* similarity_found, List* arguments) {

    // check if there were any 'N' functions that have no open parenthesis, and if so, add one
    check_for_negate_function(stack, rpn, flag, negate_function_found, negate_functions_counter, functions_counter, iterator, last_function, symbol_value, last_symbol, function_open_parenthesis_id, parsing_arguments_of_a_function, close_parenthesis_autocomplete, similarity_found, arguments);

    // save the operand to the RPN stack
    int id = DEFAULT_ID;
    set_id(rpn, &id, is_function, *symbol_value);

    put(rpn, *symbol_value, priority, is_operand, is_function, arity, id, is_function_end_symbol, function_id, flag);

    // reset the variables
    *negate_function_found = FALSE;

}


// check if the last parsed symbol was a digit
void check_for_operand(int *parsing_operand, int *symbol_value, List *stack, List *rpn, int *flag, int *negate_function_found, int *negate_functions_counter, int *functions_counter, int *iterator, int* last_function, int* close_parenthesis_flag, int* function_open_parenthesis_id, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete, int* similarity_found, List* arguments) {

    // if the flag is set to true (if we are parsing a number currently)
    if (*parsing_operand) {

        // create variables to store properties
        int priority;
        int is_operand;
        int is_function;
        int arity;
        int is_function_end_symbol;
        int function_id;

        // give the operand its properties
        set_priority(&priority, OPERAND_PRIORITY);
        set_is_operand(&is_operand, TRUE);
        set_arity(&arity, OPERAND_ARITY);
        set_is_function(&is_function, FALSE);
        set_is_function_end_symbol(&is_function_end_symbol, FALSE);
        set_function_id(&function_id, INFINITY);

        // check if the mathematical equation uses 'N' function and if so, put the missing parentheses where necessary
        process_operand(stack, rpn, flag, negate_function_found, negate_functions_counter, symbol_value, priority, is_operand, is_function, arity, is_function_end_symbol, function_id, functions_counter, iterator, last_function, close_parenthesis_flag, function_open_parenthesis_id, parsing_arguments_of_a_function, close_parenthesis_autocomplete, similarity_found, arguments);

        // reset the variables
        *symbol_value = 0;
        *parsing_operand = FALSE;

    }

}


// if the symbol is an ascii of a digit ...
void check_symbol_type(int symbol_ascii, int *symbol_value, int *parsing_operand, List *stack, List *rpn, List* arguments, int *flag, int *negate_function_found, int *negate_functions_counter, int *functions_counter, int *iterator, int* last_function, int* last_symbol, int* function_open_parenthesis_id, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete, int* arguments_counter, int* similarity_found) {

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
        check_for_operand(parsing_operand, symbol_value, stack, rpn, flag, negate_function_found, negate_functions_counter, functions_counter, iterator, last_function, last_symbol, function_open_parenthesis_id, parsing_arguments_of_a_function, close_parenthesis_autocomplete, similarity_found, arguments);

        // check if there are still 'N' functions left for us to handle parentheses
        check_for_negate_function(stack, rpn, flag, negate_function_found, negate_functions_counter, functions_counter, iterator, last_function, &symbol_ascii, last_symbol, function_open_parenthesis_id, parsing_arguments_of_a_function, close_parenthesis_autocomplete, similarity_found, arguments);

        // if the symbol is not an ascii of a digit
        check_for_operator(symbol_ascii, stack, rpn, arguments, flag, negate_function_found, functions_counter, iterator, last_function, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, close_parenthesis_autocomplete, similarity_found);

    }

}