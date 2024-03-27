void add_missing_closing_parentheses(stack* operators, stack* output, stack* arguments, int* additional_parentheses_counter);


void parse_argument(stack* operators, stack* output, stack* arguments, int symbol, int* additional_parentheses_counter) {

    node* iterator = operators->get_head();

    while (iterator != nullptr) {

        iterator = iterator->get_next();

        node* popped = operators->pop();

        if (popped->get_content() == open_parenthesis) {

            if (symbol != comma) {
                // parse function
                // reverse the symbol order
                if (operators->get_head() != nullptr) {
                    if (operators->get_head()->is_function()) {

                        node *iterator_2 = operators->get_head();
                        auto *reverse_function = new stack();

                        // reverse the order of parsed function symbols
                        while (iterator_2->is_function()) {
                            node *popped_2 = operators->pop();
                            reverse_function->push(popped_2->get_content(), popped_2->get_priority(),
                                                   popped_2->get_arity(), popped_2->is_operand(),
                                                   popped_2->is_function(), popped_2->is_last());
                            delete popped_2;
                            iterator_2 = operators->get_head();
                            if (iterator_2 == nullptr) break;
                        }

                        // add them to the output stack
                        iterator_2 = reverse_function->get_head();
                        while (iterator_2 != nullptr) {
                            node *popped_2 = reverse_function->pop();
                            if (arguments->get_head() != nullptr)
                                popped_2->set_arity(arguments->get_head()->get_content());
                            output->put(popped_2->get_content(), popped_2->get_priority(), popped_2->get_arity(),
                                        popped_2->is_operand(), popped_2->is_function(), popped_2->is_last());
                            delete popped_2;
                            iterator_2 = reverse_function->get_head();
                        }

                        if (arguments->get_head() != nullptr) delete arguments->pop();

                        reverse_function->clear();
                        delete reverse_function;

                    }

                }

                delete popped;
                break;

            }
            else {
                operators->push(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->is_operand(), popped->is_function(), popped->is_last());
                delete popped;
                return;
            }

        }
        else {
            output->put(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->is_operand(), popped->is_function(), popped->is_last());
            delete popped;
        }

    }

    // if necessary, close all the open parentheses that are left
    add_missing_closing_parentheses(operators, output, arguments, additional_parentheses_counter);

}


void parse_operator(stack* operators, stack* output, stack* arguments, int symbol, int* additional_parentheses_counter) {

    int symbol_priority;
    int symbol_arity = default_arity;
    bool symbol_is_operand = false;
    bool symbol_is_function = false;
    bool symbol_is_last = true;

    if (symbol == open_parenthesis) {
        symbol_priority = forth_priority;
        symbol_arity = 0;

        // set 'is_last':
        if (operators->get_head() != nullptr) {
            // add an element to operators stack:
            arguments->push(default_arity, operators->get_head()->get_priority(), operators->get_head()->get_arity(), operators->get_head()->is_operand(), operators->get_head()->is_function(), operators->get_head()->is_last());
        }

        operators->push(symbol, symbol_priority, symbol_arity,symbol_is_operand, symbol_is_function, symbol_is_last);
    }
    else if (symbol == close_parenthesis) {

        // todo: throw this part of code into a separate function
        // todo: use it when comma is detected in order to parse all the symbols found in between ( and ,
        parse_argument(operators, output, arguments, symbol, additional_parentheses_counter);

    }
    else if (symbol == comma) {

        if (arguments->get_head() != nullptr) {
            int arity = arguments->get_head()->get_content();
            ++arity;
            arguments->get_head()->set_content(arity);
        }

        parse_argument(operators, output, arguments, symbol, additional_parentheses_counter);

    }
    else {

        bool parsing_function_flag = false;

        if (symbol == addition || symbol == subtraction) {
            ++symbol_arity;
            symbol_priority = first_priority;
        }
        else if (symbol == multiplication || symbol == division) {
            ++symbol_arity;
            symbol_priority = second_priority;
        }
        else {
            parsing_function_flag = true;
            symbol_is_function = true;
            symbol_priority = third_priority; // when the symbol is a function
        }

        node* iterator = operators->get_head();

        while (iterator != nullptr) {

            node* popped = operators->pop();

            if (parsing_function_flag) {
                if (popped != nullptr) {
                    if (popped->get_content() == negation && popped->is_last()) { // todo: this could work only for N function, we need to implement function comparer
                        operators->push(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->is_operand(), popped->is_function(), popped->is_last());
                        operators->push(open_parenthesis, forth_priority, default_arity, false, false, true);
                        ++(*additional_parentheses_counter);
                    }
                }
            }

            if (*additional_parentheses_counter) {

                // the program popped a letter of a function above, and then pops another symbol again if *additional... is set to true
                // so, in such a case we need to push the letter back cuz we don't want to lose it
                if (popped->is_function() && popped->get_content() != negation) operators->push(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->is_operand(), popped->is_function(), popped->is_last());
                popped = operators->pop();
            }
            // ---

            // if the symbol is a function, we don't need to do anything except for saving the symbol
            if (parsing_function_flag) {
                operators->push(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->is_operand(), popped->is_function(), popped->is_last());
                // todo: do we need to check is the head is a function?
                if (operators->get_head() != nullptr && operators->get_head()->is_function()) operators->get_head()->set_last(false);
                operators->push(symbol, symbol_priority, symbol_arity, symbol_is_operand, symbol_is_function, symbol_is_last);
                return;
            }
            else {

                if (popped->get_content() == open_parenthesis || popped->get_priority() < symbol_priority) {
                    operators->push(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->is_operand(), popped->is_function(), popped->is_last());
                    break;
                }

                output->put(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->is_operand(), popped->is_function(), popped->is_last());
                iterator = iterator->get_next();

            }

        }

        // this piece of code allows us to handle situations when there are the same priority operators one after another:
        // this kind of situation is not natural for normal mathematical expressions, although, when we use functions,
        // and we have parsed the function, we the operand that now need to be put onto RPN stack, well, isn't put.
        // TODO: does it support cases when functions are being compared??
        if (operators->get_head() != nullptr) {
            if (operators->get_head()->get_priority() == symbol_priority) {
                node* tmp = operators->pop();
                output->put(tmp->get_content(), tmp->get_priority(), tmp->get_arity(), tmp->is_operand(), tmp->is_function(), tmp->is_last());
                delete tmp;
            }
        }

        operators->push(symbol, symbol_priority, symbol_arity, symbol_is_operand, symbol_is_function, symbol_is_last);

    }

}


void add_missing_closing_parentheses(stack* operators, stack* output, stack* arguments, int* additional_parentheses_counter) {

    // todo: if the operand is a part of arguments of a non-negate function, don't run this code
    // this block of code makes sure that all the added open parentheses are closed
    if (*additional_parentheses_counter) {
        if (operators->get_head() != nullptr) {
            if (operators->get_head()->get_next() != nullptr) {
                if (operators->get_head()->get_next()->get_content() == negation) {
                    --(*additional_parentheses_counter);
                    parse_operator(operators, output, arguments, close_parenthesis, additional_parentheses_counter);
                }
            }
        }
    }

}


// this function will put the operand on the output stack
void parse_operand(stack* operators, stack* output, stack* arguments, int* numeric_symbol, bool* parsing_operand_flag, int* additional_parentheses_counter) {

    // todo: throw that into a separate function
    if (operators->get_head() != nullptr) {
        if (operators->get_head()->get_content() == negation && operators->get_head()->is_last()) { // todo: this could work only for N function, we need to implement function comparer
            operators->push(open_parenthesis, forth_priority, default_arity, false, false, true);
            ++(*additional_parentheses_counter);
        }
    }

    const bool is_operand = true;
    const bool is_function = false;
    const bool is_last = true;

    output->put(*numeric_symbol, zeroth_priority, default_arity, is_operand, is_function, is_last);

    // if necessary, close all the open parentheses that are left
    add_missing_closing_parentheses(operators, output, arguments, additional_parentheses_counter);

    // reset the variables
    *numeric_symbol = 0;
    *parsing_operand_flag = false;

}


// this function will check if the symbol is an operand/operator and parse it later on
bool parse_symbol(stack* operators, stack* output, stack* arguments, int symbol, int* numeric_symbol, bool* parsing_operand_flag, int* additional_parentheses_counter) {

    // if the symbol is a digit ...
    if (symbol >= ASCII_DIGIT_RANGE_START && symbol <= ASCII_DIGIT_RANGE_FINISH) {

        // update the variable that stores the value of parsed digits
        *numeric_symbol = *numeric_symbol * FACTOR + (symbol - ASCII_DIGIT_RANGE_START);

        // set the flag, since the program is indeed parsing operands now
        *parsing_operand_flag = true;

    }

    // if the symbol is an operator ...
    else {

        // if the program is now parsing an operator, it firstly needs to add the parsed operand to the stack
        if (*parsing_operand_flag) parse_operand(operators, output, arguments, numeric_symbol, parsing_operand_flag, additional_parentheses_counter);

        // if the symbol is a period, the formula was fully parsed
        if (symbol == STOP_FORMULA_INPUT) {

            node* iterator = operators->get_head();

            while (iterator != nullptr) {
                if (iterator->get_content() != open_parenthesis) output->put(iterator->get_content(), iterator->get_priority(), iterator->get_arity(), iterator->is_operand(), iterator->is_function(), iterator->is_last());
                iterator = iterator->get_next();
            }

            return true;

        }

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
        if (parse_symbol(operators, output, arguments, symbol, &numeric_symbol, &parsing_operand_flag, &additional_parentheses_counter)) break;

    }

}