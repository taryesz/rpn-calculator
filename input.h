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
void set_id(List* rpn, int* id, int current_is_function) {
    if (rpn->tail == NULL) {
        *id = 0;
    }
    else if (rpn->tail->is_function && current_is_function && !rpn->tail->is_function_end_symbol) { // TODO: && if current symbol is NOT a function
        *id = rpn->tail->id;
    }
    else {
        *id = rpn->tail->id + 1;
    }
}
void set_is_function_end_symbol (int* is_function_end_symbol, int value) {
    *is_function_end_symbol = value;
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
void convert_to_rpn(List* stack, List* rpn, int value, int priority, int is_operand, int is_function, int arity, int id, int is_function_end_symbol, int* flag, int* function_parsing_flag) {

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
                push(stack, popped->value, popped->priority, popped->is_operand, popped->is_function, popped->arity, popped->id, popped->is_function_end_symbol);

                *function_parsing_flag = FALSE;

                // stop the saving process
                break;

            }
        }
        else {
            if (popped->value == OPEN_PARENTHESES || popped->priority < priority) {

                // push the popped symbol to the stack
                push(stack, popped->value, popped->priority, popped->is_operand, popped->is_function, popped->arity, popped->id, popped->is_function_end_symbol);

                // stop the saving process
                break;

            }

        }

        // put the popped symbol to the RPN stack if it's valid for this
        // TODO: IF TWO FUNCTIONS IN A ROW, THEY WILL HAVE THE SAME INDICES.
        // TODO: SOME 'end_func' FLAG NEEDED TO INDICATE THAT NEXT ID IS NEEDED

        set_id(rpn, &rpn_id, popped->is_function);

        put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, popped->arity, rpn_id, popped->is_function_end_symbol, flag);

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
            push(stack, tmp->value, tmp->priority, tmp->is_operand, tmp->is_function, tmp->arity, tmp->id, tmp->is_function_end_symbol);
            push(stack, value, priority, is_operand, is_function, arity, id, is_function_end_symbol);
        }
        else {
            push(stack, tmp->value, tmp->priority, tmp->is_operand, tmp->is_function, tmp->arity, tmp->id, tmp->is_function_end_symbol);
        }
    }
    else {
        if (id == -1) id = 0;
        push(stack, value, priority, is_operand, is_function, arity, id, is_function_end_symbol);
    }

    free(iterator);

}


void execute_function(int function_id, const char* reserved_functions, List* stack, List* rpn, int* flag, int* negate_function_found) {

    int priority;
    int is_operand;
    int is_function;
    int arity;
    int is_function_end_symbol;
    int id = DEFAULT_ID;

    int function_parsing_flag = TRUE;

    set_priority(&priority, FUNCTION_PRIORITY);
    set_is_operand(&is_operand, FALSE);
    set_is_function(&is_function, TRUE);

    set_id(rpn, &id, is_function);

    if (function_id == N) {
        set_is_function(&is_function, FALSE);
        set_is_function_end_symbol(&is_function_end_symbol, TRUE);
        set_arity(&arity, UNARY_ARITY);
        convert_to_rpn(stack, rpn, reserved_functions[function_id], priority, is_operand, is_function, arity, id, is_function_end_symbol, flag, &function_parsing_flag);
        *negate_function_found = TRUE;
    }
    else if (function_id == IF) {

        set_arity(&arity, TERNARY_ARITY);
        char* func = get_function_name(function_id);

        int array_length = 0;
        while (func[array_length] != STRING_TERMINATOR) {
            array_length++;
        }

        for (int i = array_length - 1; i >= 0; i--) {
            // set the last symbol's property to TRUE
            if (i == 1) set_is_function_end_symbol(&is_function_end_symbol, TRUE);
            else { set_is_function_end_symbol(&is_function_end_symbol, FALSE); }
            convert_to_rpn(stack, rpn, func[i], priority, is_operand, is_function, arity, id, is_function_end_symbol, flag, &function_parsing_flag);
            id = stack->head->id;
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

    function_parsing_flag = FALSE;

}


// compare input letters to a predefined function name
void compare_function_names(int symbol_ascii, List *stack, List *rpn, int *flag, int *negate_function_found, int *functions_counter, int *iterator, int* last_function) {

    int function_id = *functions_counter;

    const char **functions = get_reserved_functions();

    for (; function_id < RESERVED_FUNCTIONS_COUNTER;) {

        const char* current_function = functions[function_id];

        if (symbol_ascii == current_function[*iterator]) {

            ++(*iterator);

            if (current_function[*iterator] == STRING_TERMINATOR) {

                execute_function(function_id, *functions, stack, rpn, flag, negate_function_found);

                *iterator = 0;
                *last_function = *functions_counter;
                *functions_counter = 0;
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


// save the id of an open parenthesis that belongs to the outer function
//void change_target_parenthesis_id(List* stack, int* function_open_parenthesis_id) {
//
//    // get a symbol from the stack
//    Node* iterator = stack->head;
//
//    // if the symbol exists
//    if (iterator != NULL) {
//
//        // iterate until an open parenthesis is found
//        while (iterator->value != OPEN_PARENTHESES) {
//
//            // if there are other symbols on the stack, continue iterating
//            if (iterator->next != NULL) {
//                iterator = iterator->next;
//            }
//
//            // if no symbols left, stop
//            else {
//                break;
//            }
//        }
//
//        // save the id of the found parenthesis
//        *function_open_parenthesis_id = iterator->id;
//
//    }
//
//}
//

void process_parenthesis(int symbol_ascii, List* stack, List* rpn, int* priority, int is_operand, int is_function, int arity, int is_function_end_symbol, int* flag, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete);


// close all the parentheses after finishing parsing the inside of a function (this function is used when the N function's argument was another function)
void process_arguments(List* stack, List* rpn, int* priority, const int* is_operand, const int* is_function, const int* arity, const int* is_function_end_symbol, int* flag, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete) {

    // if the counter of found functions is not 0
    if (*parsing_arguments_of_a_function) {

        // if the stack exists
        if (stack->head != NULL) {

            // if the top of the stack is a function
            if (stack->head->is_function || stack->head->value == NEGATE) {

                // decrement the number of found functions
                (*parsing_arguments_of_a_function)--;

                *close_parenthesis_autocomplete = TRUE; // TODO: delete this line

                // add the missing close parenthesis
                process_parenthesis(CLOSE_PARENTHESES, stack, rpn, priority, *is_operand, *is_function, *arity,
                                    *is_function_end_symbol, flag, last_symbol, function_open_parenthesis_id,
                                    negate_functions_counter, parsing_arguments_of_a_function,
                                    close_parenthesis_autocomplete);
            }
        }
    }
}


// if the symbol is a parenthesis ('(' or ')')
void process_parenthesis(int symbol_ascii, List* stack, List* rpn, int* priority, int is_operand, int is_function, int arity, int is_function_end_symbol, int* flag, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete) {

    int id = DEFAULT_ID;

    // set the parenthesis priority
    set_priority(priority, PARENTHESIS_PRIORITY);

    // if the symbol is a '(', just push it to the operators stack
    if (symbol_ascii == OPEN_PARENTHESES) {

        // give the parenthesis an id
        set_id(rpn, &id, is_function);

        // save the parenthesis
        push(stack, symbol_ascii, *priority, is_operand, is_function, arity, id, is_function_end_symbol);

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

//                print(stack);
//                print(rpn);

                // if we finished parsing the inside of a function, we need to close all the open parentheses
                process_arguments(stack, rpn, priority, &is_operand, &is_function, &arity, &is_function_end_symbol, flag, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, close_parenthesis_autocomplete);

                break;

            }

                // if the symbol is something else
            else {

                // save the symbol to the RPN stack
                set_id(rpn, &id, popped->is_function);
                put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, popped->arity, id, popped->is_function_end_symbol, flag);

            }

            iterator = iterator->next;
            free(popped);

        }
        while(iterator != NULL);

        free(iterator);

    }
}


// add as many close parentheses as there were N-functions found ((this function is used when the N function didn't originally have parentheses and its argument was a single number)
void process_missing_parentheses(List* stack, List* rpn, int* priority, const int* is_operand, const int* is_function, int* arity, const int* is_function_end_symbol, int* flag, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete) {

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
                                            *is_function_end_symbol, flag, last_symbol, function_open_parenthesis_id,
                                            negate_functions_counter, parsing_arguments_of_a_function,
                                            close_parenthesis_autocomplete);
                    }
                }
            }
        }
    }

}


// if the symbol is an operator ('+', '-', '*' or '/')
void process_operator(int symbol_ascii, List* stack, List* rpn, int* priority, int is_operand, int is_function, int* arity, int is_function_end_symbol, int* flag, int priority_value, int* parsing_arguments_of_a_function, int* last_symbol, int* negate_functions_counter, int* function_open_parenthesis_id, int* close_parenthesis_autocomplete) {

    // before adding next operator to RPN, firstly we need to make sure that all open parentheses have a pair i.e. close parentheses
    process_missing_parentheses(stack, rpn, priority, &is_operand, &is_function, arity, &is_function_end_symbol, flag, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, close_parenthesis_autocomplete);

    // set properties of an operator
    set_priority(priority, priority_value);
    set_arity(arity, BINARY_ARITY);
    int function_parsing_flag = FALSE;
    int id = DEFAULT_ID;
    set_id(rpn, &id, is_function);

    // add the operator to RPN
    convert_to_rpn(stack, rpn, symbol_ascii, *priority, is_operand, is_function, *arity, id, is_function_end_symbol, flag, &function_parsing_flag);

}


// check if the symbol is a '.'
void check_for_stop(int symbol_ascii, List* stack, List* rpn, int* flag, int* negate_functions_counter, int* last_symbol, int* function_open_parenthesis_id) {

    if (symbol_ascii == EQUATION_STOP_SYMBOL) {

        int id = DEFAULT_ID;

        // while the operators stack is not empty
        while (stack->head != NULL) {

            int function_parsing_flag = FALSE;

            if (stack->head->is_function == TRUE) {
                function_parsing_flag = TRUE;
            }

            set_id(rpn, &id, stack->head->is_function);

            convert_to_rpn(stack, rpn, stack->head->value, stack->head->priority, stack->head->is_operand, stack->head->is_function, stack->head->arity, id, stack->head->is_function_end_symbol, flag, &function_parsing_flag);

            // remove the pushed operator from the stack
            pop(stack);

        }

    }

}


//void check_for_operator(int symbol_ascii, List *stack, List *rpn, int *flag, int *negate_function_found, int *functions_counter, int *iterator, int* last_function, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete);

//
//void add_close_parenthesis(List* stack, List* rpn, int* negate_functions_counter, int* negate_function_found, int* functions_counter, int* iterator, int* last_function, int* last_symbol, int* function_open_parenthesis_id, int* flag, int* parsing_arguments_of_a_function) {
//    if (stack->head != NULL) {
//        if (stack->head->value == OPEN_PARENTHESES) {
//            if (stack->head->next != NULL) {
//                if (stack->head->next->value == NEGATE) {
//                    for (; *negate_functions_counter != 0; (*negate_functions_counter)--) {
//                        check_for_operator(CLOSE_PARENTHESES, stack, rpn, flag, negate_function_found, functions_counter, iterator, last_function, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function);
//                    }
//                }
//            }
//        }
//    }
//}


//// put arguments of a function to RPN
//void process_arguments(List* stack, List* rpn, int* negate_functions_counter, int* negate_function_found, int* functions_counter, int* iterator, int* last_function, int* last_symbol, int* function_open_parenthesis_id, int* flag, int* parsing_arguments_of_a_function) {
//
//    // put as many close parentheses as there were negate functions
//    add_close_parenthesis(stack, rpn, negate_functions_counter, negate_function_found, functions_counter, iterator, last_function, last_symbol, function_open_parenthesis_id, flag, parsing_arguments_of_a_function);
//
//    // put all symbols to RPN until the parenthesis wrapping those symbols is found
//    Node* stack_iterator = stack->head;
//
//    // TODO: THIS
//    if (stack_iterator != NULL) {
//        if (stack_iterator->value != CLOSE_PARENTHESES && stack_iterator->value != OPEN_PARENTHESES) {
//            do {
//                stack_iterator = stack_iterator->next;
//                Node* tmp = pop(stack);
//                int popped_id = DEFAULT_ID;
//                set_id(rpn, &popped_id, tmp->is_function);
//                put(rpn, tmp->value, tmp->priority, tmp->is_operand, tmp->is_function, tmp->arity, popped_id, tmp->is_function_end_symbol, flag);
//                free(tmp);
//            } while (stack_iterator->value != OPEN_PARENTHESES && stack_iterator->id != *function_open_parenthesis_id);
//        }
//    }
//
//}


// check what operator was passed
void check_for_operator(int symbol_ascii, List *stack, List *rpn, int *flag, int *negate_function_found, int *functions_counter, int *iterator, int* last_function, int* last_symbol, int* function_open_parenthesis_id, int* negate_functions_counter, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete) {

    // create a const to indicate that the symbol doesn't contain a numerical value
    int is_operand;
    int is_function;
    int priority;
    int arity;
    int is_function_end_symbol;

    // for all the symbols set 'is_operand' to false, since they are operators
    set_is_operand(&is_operand, FALSE);
    set_is_function(&is_function, FALSE);
    set_is_function_end_symbol(&is_function_end_symbol, FALSE);
    set_arity(&arity, INFINITY);

    // if the symbol is a '.'
    check_for_stop(symbol_ascii, stack, rpn, flag, negate_functions_counter, last_symbol, function_open_parenthesis_id);

    // if the symbol is a parenthesis
    if (symbol_ascii == OPEN_PARENTHESES || symbol_ascii == CLOSE_PARENTHESES) {
        set_arity(&arity, PARENTHESIS_ARITY);
        process_parenthesis(symbol_ascii, stack, rpn, &priority, is_operand, is_function, arity, is_function_end_symbol, flag, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, close_parenthesis_autocomplete);
    }

        // if the symbol is a '+' or a '-', set the according priority and push to RPN stack
    else if (symbol_ascii == PLUS || symbol_ascii == MINUS) {
        process_operator(symbol_ascii, stack, rpn, &priority, is_operand, is_function, &arity, is_function_end_symbol, flag, OPERATOR_PLUS_MINUS_PRIORITY, parsing_arguments_of_a_function, last_symbol, negate_functions_counter, function_open_parenthesis_id, close_parenthesis_autocomplete);
    }

        // if the symbol is a '*' or a '/', set the according priority and push to RPN stack
    else if (symbol_ascii == ASTERISK || symbol_ascii == SLASH) {
        process_operator(symbol_ascii, stack, rpn, &priority, is_operand, is_function, &arity, is_function_end_symbol, flag, OPERATOR_ASTERISK_SLASH_PRIORITY, parsing_arguments_of_a_function, last_symbol, negate_functions_counter, function_open_parenthesis_id, close_parenthesis_autocomplete);
    }

        // if the symbol is a capital letter (i.e. a part of a function name)
    else if (symbol_ascii >= ASCII_LETTER_RANGE_START && symbol_ascii <= ASCII_LETTER_RANGE_FINISH || symbol_ascii == SPACE) {
        compare_function_names(symbol_ascii, stack, rpn, flag, negate_function_found, functions_counter, iterator, last_function);
    }

        // if the symbol is a comma (which means that in the context of this program, we are iterating through a function)
    else if (symbol_ascii == COMMA) {

        // put braces back
        process_missing_parentheses(stack, rpn, &priority, &is_operand, &is_function, &arity, &is_function_end_symbol, flag, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, close_parenthesis_autocomplete);

        if (stack->head != NULL) {
            if (stack->head->is_function) {
                Node* argument = stack->head;
                do {
                    Node *popped = pop(stack);
                    put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, popped->arity, popped->id, popped->is_function_end_symbol, flag);
                    argument = argument->next;
                } while (argument->value != OPEN_PARENTHESES);
            }
            // if nothing from the above did any change, it means that the argument here is a simple formula (like 2 * 5)
            // but it's not wrapped in parentheses
            else {
                if (stack->head->value != OPEN_PARENTHESES) {
                    Node* popped = pop(stack);
                    put(rpn, popped->value, popped->priority, popped->is_operand, popped->is_function, popped->arity, popped->id, popped->is_function_end_symbol, flag);
                }
            }
        }
    }

    *last_symbol = symbol_ascii;

}


// check if there were any 'N' functions
void check_for_negate_function(List *stack, List *rpn, int *flag, int *negate_function_found, int *negate_functions_counter,int *functions_counter, int *iterator, int* last_function, const int* current_symbol, int* last_symbol, int* function_open_parenthesis_id, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete) {

    // if the function is found
    if (*negate_function_found == TRUE) {

        if (*current_symbol != OPEN_PARENTHESES) {

            // increment number of parentheses we will need to close later
            (*parsing_arguments_of_a_function)++;

            // add the missing open parenthesis
            check_for_operator(OPEN_PARENTHESES, stack, rpn, flag, negate_function_found, functions_counter, iterator, last_function, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, close_parenthesis_autocomplete);

            // increment the number of 'N' functions
            *negate_functions_counter += 1;

        }

        *negate_function_found = FALSE;

    }

}


// check if the mathematical equation uses 'N' function and if so, put the missing parentheses where necessary
void process_operand(List *stack, List *rpn, int *flag, int *negate_function_found, int *negate_functions_counter, int *symbol_value, int priority, int is_operand, int is_function, int arity, int is_function_end_symbol, int *functions_counter, int *iterator, int* last_function, int* last_symbol, int* function_open_parenthesis_id, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete) {

    // check if there were any 'N' functions that have no open parenthesis, and if so, add one
    check_for_negate_function(stack, rpn, flag, negate_function_found, negate_functions_counter, functions_counter, iterator, last_function, symbol_value, last_symbol, function_open_parenthesis_id, parsing_arguments_of_a_function, close_parenthesis_autocomplete);

    // save the operand to the RPN stack
    int id = DEFAULT_ID;
    set_id(rpn, &id, is_function);
    put(rpn, *symbol_value, priority, is_operand, is_function, arity, id, is_function_end_symbol, flag);

    // reset the variables
    *negate_function_found = FALSE;

}


// check if the last parsed symbol was a digit
void check_for_operand(int *parsing_operand, int *symbol_value, List *stack, List *rpn, int *flag, int *negate_function_found, int *negate_functions_counter, int *functions_counter, int *iterator, int* last_function, int* close_parenthesis_flag, int* function_open_parenthesis_id, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete) {

    // if the flag is set to true (if we are parsing a number currently)
    if (*parsing_operand) {

        // create variables to store properties
        int priority;
        int is_operand;
        int is_function;
        int arity;
        int is_function_end_symbol;

        // give the operand its properties
        set_priority(&priority, OPERAND_PRIORITY);
        set_is_operand(&is_operand, TRUE);
        set_arity(&arity, OPERAND_ARITY);
        set_is_function(&is_function, FALSE);
        set_is_function_end_symbol(&is_function_end_symbol, FALSE);

        // check if the mathematical equation uses 'N' function and if so, put the missing parentheses where necessary
        process_operand(stack, rpn, flag, negate_function_found, negate_functions_counter, symbol_value, priority, is_operand, is_function, arity, is_function_end_symbol, functions_counter, iterator, last_function, close_parenthesis_flag, function_open_parenthesis_id, parsing_arguments_of_a_function, close_parenthesis_autocomplete);

        // reset the variables
        *symbol_value = 0;
        *parsing_operand = FALSE;

    }

}


// if the symbol is an ascii of a digit ...
void check_symbol_type(int symbol_ascii, int *symbol_value, int *parsing_operand, List *stack, List *rpn, int *flag, int *negate_function_found, int *negate_functions_counter, int *functions_counter, int *iterator, int* last_function, int* last_symbol, int* function_open_parenthesis_id, int* parsing_arguments_of_a_function, int* close_parenthesis_autocomplete) {

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
        check_for_operand(parsing_operand, symbol_value, stack, rpn, flag, negate_function_found, negate_functions_counter, functions_counter, iterator, last_function, last_symbol, function_open_parenthesis_id, parsing_arguments_of_a_function, close_parenthesis_autocomplete);

        // check if there are still 'N' functions left for us to handle parentheses
        check_for_negate_function(stack, rpn, flag, negate_function_found, negate_functions_counter, functions_counter, iterator, last_function, &symbol_ascii, last_symbol, function_open_parenthesis_id, parsing_arguments_of_a_function, close_parenthesis_autocomplete);

        // if the symbol is not an ascii of a digit
        check_for_operator(symbol_ascii, stack, rpn, flag, negate_function_found, functions_counter, iterator, last_function, last_symbol, function_open_parenthesis_id, negate_functions_counter, parsing_arguments_of_a_function, close_parenthesis_autocomplete);

    }

}