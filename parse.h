void parse_operator(stack* operators, stack* output, int symbol) {

    int symbol_priority;
    int symbol_is_operand = false;

    if (symbol == open_parenthesis) {
        symbol_priority = forth_priority;
        operators->push(symbol, symbol_priority, symbol_is_operand);
    }
    else if (symbol == close_parenthesis) {

        node* iterator = operators->get_head();

        while (iterator != nullptr) {

            node* popped = operators->pop();

            if (popped->get_content() == open_parenthesis) {
                delete popped; // TODO: pop() doesnt create a new node, does it?
                break;
            }
            else {
                output->put(popped->get_content(), popped->get_priority(), popped->is_operand());
                delete popped;
            }

            iterator = iterator->get_next();

        }

    }
    else {

        if (symbol == addition || symbol == subtraction) symbol_priority = first_priority;
        if (symbol == multiplication || symbol == division) symbol_priority = second_priority;

        node* iterator = operators->get_head();

        while (iterator != nullptr) {

            node* popped = operators->pop();

            if (popped->get_content() == open_parenthesis || popped->get_priority() < symbol_priority) {
                operators->push(popped->get_content(), popped->get_priority(), popped->is_operand());
                break;
            }

            output->put(popped->get_content(), popped->get_priority(), popped->is_operand());
            iterator = iterator->get_next();

        }

        operators->push(symbol, symbol_priority, symbol_is_operand);

    }

}



// this function will put the operand on the output stack
void parse_operand(stack* output, int* numeric_symbol, bool* parsing_operand_flag) {

    output->put(*numeric_symbol, zeroth_priority, true);

    // reset the variables
    *numeric_symbol = 0;
    *parsing_operand_flag = false;

}


// this function will check if the symbol is an operand/operator and parse it later on
bool parse_symbol(stack* operators, stack* output, int symbol, int* numeric_symbol, bool* parsing_operand_flag) {

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
        if (*parsing_operand_flag) parse_operand(output, numeric_symbol, parsing_operand_flag);

        // if the symbol is a period, the formula was fully parsed
        if (symbol == STOP_FORMULA_INPUT) {

            node* iterator = operators->get_head();

            while (iterator != nullptr) {
                if (iterator->get_content() != open_parenthesis) output->put(iterator->get_content(), iterator->get_priority(), iterator->is_operand());
                iterator = iterator->get_next();
            }

            return true;

        }

        // if the operator symbol was given, parse it
        parse_operator(operators, output, symbol);

    }

    return false;

}


// this function will parse the infix mathematical expression
void parse_formula(stack* operators, stack* output) {

    // create a variable that stores the value of parsed digits
    int numeric_symbol = 0;

    // create a flag that will indicate whether an operand is being parsed or not
    bool parsing_operand_flag = false;

    // create a loop that will allow the user to input as many symbols as they want
    while (true) {

        // get a symbol from the user
        int symbol = getchar();

        // if the symbol is a white character, ignore it
        if (symbol == SPACE) continue;

        // if the symbol is valid, proceed to parsing it
        if (parse_symbol(operators, output, symbol, &numeric_symbol, &parsing_operand_flag)) break;

    }

}