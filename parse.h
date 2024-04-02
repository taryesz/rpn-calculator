// declarations
void add_missing_closing_parentheses(stack* operators, stack* output, stack* arguments, int* additional_parentheses_counter);
void add_missing_open_parenthesis(stack* operators, stack* arguments, node* symbol, int* additional_parentheses_counter, int priority, int arity, int function_id, bool is_operand, bool is_function, bool is_last, bool requires_additional_parenthesis, bool parsing_function_flag);


// this function will reverse the function's letters' order in the stack
void reverse_function_in_stack(stack* operators, stack* reverse_function, node* iterator) {
    while (iterator->is_function()) {
        node *popped = operators->pop();
        reverse_function->push(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->get_function_id(), popped->is_operand(), popped->is_function(), popped->is_last(), popped->get_requires_additional_parenthesis());
        delete popped;
        iterator = operators->get_head();
        if (iterator == nullptr) break;
    }
}


// this function will place all the function's letters back to the original stack
void add_reversed_function(stack* output, stack* arguments, stack* reverse_function, node* iterator) {
    iterator = reverse_function->get_head();
    while (iterator != nullptr) {
        node *popped = reverse_function->pop();
        if (arguments->get_head() != nullptr) popped->set_arity(arguments->get_head()->get_content());
        output->put(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->get_function_id(), popped->is_operand(), popped->is_function(), popped->is_last(), popped->get_requires_additional_parenthesis());
        delete popped;
        iterator = reverse_function->get_head();
    }
}


// this function will handle the insides of a function
void parse_argument(stack* operators, stack* output, stack* arguments, int symbol, int* additional_parentheses_counter) {

    // get the head of the stack
    node* head = operators->get_head();

    // while there are elements on the stack
    while (head != nullptr) {

        // update the iterator until it's modified
        head = head->get_next();

        // remove the head from the stack
        node* popped = operators->pop();

        // if the popped element is an open parenthesis ...
        if (popped->get_content() == open_parenthesis) {

            // ... and if the parsed symbol is, i.e. a close parenthesis (it means that now the function's open parenthesis has to be removed)
            if (symbol != comma) {

                // if the newest head exists ...
                if (operators->get_head() != nullptr) {

                    // if the newest head is a function ...
                    // [ this snippet will process the name of a function that is a part of another function's argument ]
                    if (operators->get_head()->is_function()) {

                        // get the newest head from the stack
                        node *iterator = operators->get_head();

                        // create a temporary stack to store a reversed function
                        auto *reverse_function = new stack();

                        // since the program pushes the letters to the stack, the read of them is mirrored
                        // so, it needs to reverse the order first
                        reverse_function_in_stack(operators, reverse_function, iterator);

                        // the reversed function's symbols have to be now added to the original stack
                        add_reversed_function(output, arguments, reverse_function, iterator);

                        // since the program has finished parsing arguments of a function and assigned the correct
                        // arity to the function's letters, delete the arguments' head to keep the correct track
                        // of the number of functions
                        if (arguments->get_head() != nullptr) delete arguments->pop();

                        // free memory of the newly created stack
                        reverse_function->clear();

                        // delete the stack, the program won't use it again
                        delete reverse_function;

                    }

                }

                // free memory and stop the loop - parsing of an argument (function name) has terminated
                delete popped;
                break;

            }

            // if the parsed symbol is a comma, it means that it's not the time yet to remove the open parenthesis
            // because there are more elements waiting to be processed
            else {
                operators->push(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->get_function_id(), popped->is_operand(), popped->is_function(), popped->is_last(), popped->get_requires_additional_parenthesis());
                delete popped;
                return;
            }

        }

        // if the popped element is some other symbol, just add it to the output stack
        else output->put(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->get_function_id(), popped->is_operand(), popped->is_function(), popped->is_last(), popped->get_requires_additional_parenthesis());
        delete popped;

    }

    // if necessary, close all the open parentheses that are left
    add_missing_closing_parentheses(operators, output, arguments, additional_parentheses_counter);

}


// this function will add an open parenthesis to the stack, keeping function quantity track
void parse_open_parenthesis(stack* operators, stack* arguments, int symbol, int* symbol_priority, int* symbol_arity, int symbol_function_id, int symbol_is_operand, int symbol_is_function, int symbol_is_last, int symbol_requires_additional_parenthesis) {

    // set some properties accordingly to the open parenthesis' ones
    *symbol_priority = forth_priority;
    *symbol_arity = UNDEFINED;

    // get the head of the stack
    node* head = operators->get_head();

    // if the head exists ...
    if (head != nullptr) {

        // if the head is a function ...
        if (head->is_function()) {

            // detect what kind of function is being parsed and save the ID of that function into each of its letters
            compare_functions(operators);

            // create a universal property for arguments stack
            const int argument_counter_property = UNDEFINED;

            // add an element to operators stack (since a function was found)
            arguments->push(default_arity, argument_counter_property, argument_counter_property, argument_counter_property, argument_counter_property, argument_counter_property, argument_counter_property, argument_counter_property);

        }

    }

    // add the symbol that originally was being parsed
    operators->push(symbol, *symbol_priority, *symbol_arity, symbol_function_id, symbol_is_operand, symbol_is_function, symbol_is_last, symbol_requires_additional_parenthesis);

}


// this function will update function's arity property and parse the argument
void parse_comma(stack* operators, stack* output, stack* arguments, int symbol, int* additional_parentheses_counter) {

    // get the head of the stack
    node* head = arguments->get_head();

    // if the head exists ...
    if (head != nullptr) {

        // get the current value stored in the head
        int arity = head->get_content();

        // increment the value (update the number of arguments of the latest function)
        ++arity;

        // set the new value
        head->set_content(arity);

    }

    // parse the symbols standing before the comma
    parse_argument(operators, output, arguments, symbol, additional_parentheses_counter);

}


// this function will set the operator's priorities
void set_operator_priorities(int symbol, int* symbol_priority, int* symbol_arity, int* symbol_function_id, int* symbol_is_function, bool* parsing_function_flag) {

    switch(symbol) {
        case addition:
        case subtraction:
            ++(*symbol_arity);
            *symbol_priority = first_priority;
            break;
        case multiplication:
        case division:
            ++(*symbol_arity);
            *symbol_priority = second_priority;
            break;
        default:
            *parsing_function_flag = true;
            *symbol_is_function = true;
            *symbol_function_id = 0;
            *symbol_priority = third_priority;
    }

}


// this function will make sure that all operators are being added where needed
void handle_neighboring_operators(stack* operators, stack* output, int symbol_priority) {

    // get the head of the stack
    node* head = operators->get_head();

    // if the head exists ...
    if (head != nullptr) {

        // if the head has the same priority as the currently parsed operator
        if (head->get_priority() == symbol_priority) {

            // remove the head from the stack
            node* tmp = operators->pop();

            // ... and put it to the output
            output->put(tmp->get_content(), tmp->get_priority(), tmp->get_arity(), tmp->get_function_id(), tmp->is_operand(), tmp->is_function(), tmp->is_last(), tmp->get_requires_additional_parenthesis());

            // free memory, the node was already used
            delete tmp;

        }

    }

}


// this function will parse a symbol that stands for a function
void parse_function(stack* operators, stack* output, stack* arguments, int symbol, int symbol_priority, int symbol_arity, int symbol_function_id, int symbol_is_operand, int symbol_is_function, int symbol_is_last, int symbol_requires_additional_parenthesis, int* additional_parentheses_counter) {

    // create and initiate a flag that will tell the program whether a function is being parsed or not
    bool parsing_function_flag = false;

    // check the symbol kind and set the appropriate properties
    set_operator_priorities(symbol, &symbol_priority, &symbol_arity, &symbol_function_id, &symbol_is_function, &parsing_function_flag);

    // get the head of the stack
    node* head = operators->get_head();

    // while there are elements on the stack
    while (head != nullptr) {

        // pop the newest head of the stack
        node* popped = operators->pop();

        // if the newest head doesn't exist, stop the loop
        if (popped == nullptr) break;

        // if a function (and it's a negation one) is being parsed and if the negation function didn't have its own parenthesis, add one
        if (parsing_function_flag) add_missing_open_parenthesis(operators, arguments, popped, additional_parentheses_counter, symbol_priority, symbol_arity, symbol_function_id, symbol_is_operand, symbol_is_function, symbol_is_last, symbol_requires_additional_parenthesis, parsing_function_flag);

        // if the symbol is a function, we don't need to do anything except for saving the symbol
        if (parsing_function_flag) {

            // if the most recent symbol was N, it means that we 100% surely added this N and '(' after it, and so, there is no need to add N one more time
            // otherwise, it's some other function that needs to be saved
            if (popped->get_content() != NEGATION) operators->push(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->get_function_id(), popped->is_operand(), popped->is_function(), popped->is_last(), popped->get_requires_additional_parenthesis());

            // free memory, the node was already used
            delete popped;

            // if the newest head exists, and it's a function, set its property to false since it's no longer a last letter
            if (operators->get_head() != nullptr && operators->get_head()->is_function()) operators->get_head()->set_last(false);

            // add the new function symbol to the stack
            operators->push(symbol, symbol_priority, symbol_arity, symbol_function_id, symbol_is_operand, symbol_is_function, symbol_is_last, symbol_requires_additional_parenthesis);

            // exit the function
            return;

        }

        // if the symbol is not a function, then it's a simple operator which follows classic RPN conversion algorithm
        else {

            // if the newest symbol was an open parenthesis or its priority is less than the priority of the currently parsed symbol ...
            if (popped->get_content() == open_parenthesis || popped->get_priority() < symbol_priority) {

                // add this newest symbol back to the stack
                operators->push(popped->get_content(), popped->get_priority(), popped->get_arity(),popped->get_function_id(), popped->is_operand(), popped->is_function(),popped->is_last(), popped->get_requires_additional_parenthesis());

                // free memory, the node was already used
                delete popped;

                // stop the loop
                break;

            }

            // if the newest symbol is not a parenthesis, and neither it has a smaller priority, add it to the output stack
            output->put(popped->get_content(), popped->get_priority(), popped->get_arity(),popped->get_function_id(), popped->is_operand(), popped->is_function(),popped->is_last(), popped->get_requires_additional_parenthesis());

            // update the iterator, grab the next element
            head = head->get_next();

        }

    }

    // this function allows us to handle situations when there are the same priority operators one after another:
    // this kind of situation is not natural for normal mathematical expressions, although, when we use functions,
    // and we have parsed the function, the operand that now needs to be put onto RPN stack, well, isn't put.
    handle_neighboring_operators(operators, output, symbol_priority);

    // add the parsed symbol to the stack
    operators->push(symbol, symbol_priority, symbol_arity, symbol_function_id, symbol_is_operand, symbol_is_function, symbol_is_last, symbol_requires_additional_parenthesis);

}


// this function will parse a symbol that stands for an operator
void parse_operator(stack* operators, stack* output, stack* arguments, int symbol, int* additional_parentheses_counter) {

    // create and initialize variables that will store the symbol's properties
    int symbol_priority = UNDEFINED, symbol_arity = default_arity, symbol_function_id = UNDEFINED;
    bool symbol_is_operand = false, symbol_is_function = false, symbol_is_last = true, symbol_requires_additional_parenthesis = false;

    switch (symbol) {
        case open_parenthesis:
            parse_open_parenthesis(operators, arguments, symbol, &symbol_priority, &symbol_arity, symbol_function_id, symbol_is_operand, symbol_is_function, symbol_is_last, symbol_requires_additional_parenthesis);
            break;
        case close_parenthesis:
            parse_argument(operators, output, arguments, symbol, additional_parentheses_counter);
            break;
        case comma:
            parse_comma(operators, output, arguments, symbol, additional_parentheses_counter);
            break;
        default:
            parse_function(operators, output, arguments, symbol, symbol_priority, symbol_arity, symbol_function_id, symbol_is_operand, symbol_is_function, symbol_is_last, symbol_requires_additional_parenthesis, additional_parentheses_counter);
            break;
    }

}


// this function will add a close parenthesis if needed
void add_missing_closing_parentheses(stack* operators, stack* output, stack* arguments, int* additional_parentheses_counter) {

    // if the parenthesis counter is not 0 ... (it means the program has found cases where functions don't have their parenthesis)
    if (*additional_parentheses_counter) {

        // get the head of the stack
        node* head = operators->get_head();

        // if the penultimate symbol is a negation function, and it does require a parenthesis ...
        if (head != nullptr) {
            if (head->get_next() != nullptr) {
                if (head->get_next()->get_function_id() == negation && head->get_next()->get_requires_additional_parenthesis()) {

                    // reset the property to false since we are about to add all the parentheses it needs
                    head->get_next()->set_requires_additional_parenthesis(false);

                    // decrement the parentheses counter
                    --(*additional_parentheses_counter);

                    // add a close parenthesis to the stack
                    parse_operator(operators, output, arguments, close_parenthesis, additional_parentheses_counter);

                }
            }
        }

    }

}


// this function will add an open parenthesis if needed
void add_missing_open_parenthesis(stack* operators, stack* arguments, node* symbol, int* additional_parentheses_counter, int priority, int arity, int function_id, bool is_operand, bool is_function, bool is_last, bool requires_additional_parenthesis, bool parsing_function_flag) {

    // if the head of the operators stack exists ...
    if (symbol != nullptr) {

        // if the head is a negation function ... (it means there wasn't its native parenthesis and the program has to add one)
        if (symbol->get_content() == NEGATION && symbol->is_last()) {

            // set the function's property to true (this will be used to decide if the program has to add a close parenthesis or not)
            symbol->set_requires_additional_parenthesis(true);

            if (parsing_function_flag) operators->push(symbol->get_content(), symbol->get_priority(), symbol->get_arity(), symbol->get_function_id(), symbol->is_operand(), symbol->is_function(), symbol->is_last(), symbol->get_requires_additional_parenthesis());

            // detect what kind of functions it is (give all its letter according function_id)
            compare_functions(operators);

            // create a universal property for arguments stack
            const int argument_counter_property = UNDEFINED;

            // add an element to the arguments stack (it means the program has found another function)
            arguments->push(default_arity, argument_counter_property, argument_counter_property, argument_counter_property, argument_counter_property, argument_counter_property, argument_counter_property, argument_counter_property);

            // modify the properties so they suit the parenthesis' properties
            priority = forth_priority;
            is_operand = false;

            // add the parenthesis to the stack
            operators->push(open_parenthesis, priority, arity, function_id, is_operand, is_function, is_last, requires_additional_parenthesis);

            // increment the inserted parentheses counter to know later how many of close parentheses the program will have to add
            ++(*additional_parentheses_counter);

        }

    }

}


// this function will put the operand on the output stack
void parse_operand(stack* operators, stack* output, stack* arguments, int* numeric_symbol, bool* parsing_operand_flag, int* additional_parentheses_counter) {

    // create properties of an operand
    const int priority = zeroth_priority, arity = default_arity, function_id = UNDEFINED;
    const bool is_operand = true, is_function = false, is_last = true, requires_additional_parenthesis = false;

    // if the negation function didn't have its own parenthesis, add one
    add_missing_open_parenthesis(operators, arguments, operators->get_head(), additional_parentheses_counter, priority, arity, function_id, is_operand, is_function, is_last, requires_additional_parenthesis, false);

    // put the parsed operand to the output stack
    output->put(*numeric_symbol, priority, arity, function_id, is_operand, is_function, is_last, requires_additional_parenthesis);

    // if the negation function, that had argument as a simple number, didn't have its own parenthesis
    // the program should've added '(' before the operand and the close it with ')'
    add_missing_closing_parentheses(operators, output, arguments, additional_parentheses_counter);

    // reset the variables
    *numeric_symbol = 0;            // - the number has to be reset to 0 since the program will store a new number here
    *parsing_operand_flag = false;  // - the program is not longer parsing a number

}


// this function will check if the symbol is a period and if so, put the remaining symbols to the output stack
bool parse_period(stack* operators, stack* output, int symbol) {

    // if the symbol is a period ...
    if (symbol == stop_formula_input) {

        // create an iterator that will go through the whole stack
        node* iterator = operators->get_head();

        // while there are elements on the stack
        while (iterator != nullptr) {

            // if the current element is not an open parenthesis, put it to the output stack
            if (iterator->get_content() != open_parenthesis) output->put(iterator->get_content(), iterator->get_priority(), iterator->get_arity(), iterator->get_function_id(), iterator->is_operand(), iterator->is_function(), iterator->is_last(), iterator->get_requires_additional_parenthesis());

            // get the next element
            iterator = iterator->get_next();

        }

        // since the period was found, return true
        return true;

    }

    // the period wasn't yet found
    return false;

}


// this function will check if the symbol is an operand/operator and parse it later on
bool parse_symbol(stack* operators, stack* output, stack* arguments, int symbol, int* numeric_symbol, bool* parsing_operand_flag, int* additional_parentheses_counter) {

    // if the symbol is a digit ...
    if (symbol >= first_digit_ascii && symbol <= final_digit_ascii) {

        // update the variable that stores the value of parsed digits
        *numeric_symbol = *numeric_symbol * FACTOR + (symbol - first_digit_ascii);

        // set the flag, since the program is indeed parsing operands now
        *parsing_operand_flag = true;

    }

    // if the symbol is an operator ...
    else {

        // if the program is now parsing an operator, it firstly needs to add the parsed operand to the stack
        if (*parsing_operand_flag) parse_operand(operators, output, arguments, numeric_symbol, parsing_operand_flag, additional_parentheses_counter);

        // if the symbol is a period, the formula was fully parsed
        if (parse_period(operators, output, symbol)) return true;

        // if the operator symbol was given, parse it
        parse_operator(operators, output, arguments, symbol, additional_parentheses_counter);

    }

    return false;

}


// this function will parse the infix mathematical expression
void parse_formula(stack* operators, stack* output, stack* arguments) {

    // create a variable that stores the value of parsed digits
    int numeric_symbol = 0;

    // create a variable that keeps track of added parentheses to the stack
    int additional_parentheses_counter = 0;

    // create a flag that will indicate whether an operand is being parsed or not
    bool parsing_operand_flag = false;

    // create a loop that will allow the user to input as many symbols as they want
    while (true) {

        // get a symbol from the user
        int symbol = getchar();

        // if the symbol is a white character, ignore it
        if (symbol == SPACE) continue;

        // if the symbol is valid, proceed to parsing it
        if (symbol >= first_visible_ascii && symbol <= final_visible_ascii) {

            // if the function returns true, it means that a period '.' was found and the loop has to stop
            if (parse_symbol(operators, output, arguments, symbol, &numeric_symbol, &parsing_operand_flag, &additional_parentheses_counter)) break;

        }

    }

}
