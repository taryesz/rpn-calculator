// this function will put the operand on the output stack
void parse_operand(stack* output, int* numeric_symbol, bool* parsing_operand_flag) {

    output->put(*numeric_symbol);

    // reset the variables
    *numeric_symbol = 0;
    *parsing_operand_flag = false;

}


// this function will check if the symbol is an operand/operator and parse it later on
void parse_symbol(stack* output, int symbol, int* numeric_symbol, bool* parsing_operand_flag) {

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

    }

}


// this function will parse the infix mathematical expression
void parse_formula(stack* output) {

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
        parse_symbol(output, symbol, &numeric_symbol, &parsing_operand_flag);

        // if the symbol is a period, the formula was fully parsed
        if (symbol == STOP_FORMULA_INPUT) break;

    }

}