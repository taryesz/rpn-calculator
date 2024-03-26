/*const char* get_reserved_function_name(int function) {

    switch (function) {
        case negation: return "N";
        case conditional: return "IF";
        case minimum: return "MIN";
        case maximum: return "MAX";
        default: return "";
    }

}


int count_reserved_functions() {
    return stop_flag;
}


// find the length of a word
int count_reserved_function_name_length(const char* function) {
    int string_length = 0;
    while (function[string_length] != STRING_TERMINATOR) ++string_length;
    return string_length;
}


const char** get_reserved_functions() {

    const int reserved_functions_quantity = count_reserved_functions();
    const char** functions = new const char*[reserved_functions_quantity];

    // if (functions == nullptr) return nullptr;

    for (int i = 0; i < reserved_functions_quantity; i++) functions[i] = get_reserved_function_name(i);

    return functions;

}


void save_function() {

    // get the appropriate function name
    char* function = get_reserved_function_name(function_id);

    // find the length of the function's name
    const int function_name_length = count_reserved_function_name_length(function);

    // iterate through the function name and add each letter to the RPN stack
    for (int counter = function_name_length; counter >= 0; counter--) {

    }

}


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


void compare_functions(int symbol, int* last_symbol_detected_id, int* last_function_detected_id, bool* similarity_detected_flag) {

    int function_id = *last_function_detected_id;

    const char **functions = get_reserved_functions();
    const int reserved_functions_quantity = count_reserved_functions();

    for (; function_id < reserved_functions_quantity;) {

        const char* current_function = functions[function_id];

        if (symbol == current_function[*last_symbol_detected_id]) {
            *similarity_detected_flag = true;
            ++(*last_symbol_detected_id);

            if (current_function[*last_symbol_detected_id] == STRING_TERMINATOR) {

                // TODO: exec

                *last_symbol_detected_id = 0;
                *last_function_detected_id = 0;
                return;

            }

            *last_function_detected_id = function_id;
            break;

        }
        else {

            (*similarity_detected_flag) ? (*similarity_detected_flag = false) : (*last_symbol_detected_id = 0);
            ++function_id;

        }

    }


}
*/

void add_missing_closing_parentheses(stack* operators, stack* output, int* additional_parentheses_counter);


void parse_operator(stack* operators, stack* output, int symbol, int* additional_parentheses_counter) {

    int symbol_priority;
    int symbol_is_operand = false;

    if (symbol == open_parenthesis) {
        symbol_priority = forth_priority;
        operators->push(symbol, symbol_priority, symbol_is_operand);
    }
    else if (symbol == close_parenthesis) {

        node* iterator = operators->get_head();

        while (iterator != nullptr) {

            iterator = iterator->get_next();

            node* popped = operators->pop();

            if (popped->get_content() == open_parenthesis) {
                delete popped;
                break;
            }
            else {
                output->put(popped->get_content(), popped->get_priority(), popped->is_operand());
                delete popped;
            }

        }

        // if necessary, close all the open parentheses that are left
        add_missing_closing_parentheses(operators, output, additional_parentheses_counter);

    }
    else {

        int parsing_function_flag = false;

        if (symbol == addition || symbol == subtraction) symbol_priority = first_priority;
        else if (symbol == multiplication || symbol == division) symbol_priority = second_priority;
        else {
            parsing_function_flag = true;
            symbol_priority = third_priority; // when the symbol is a function
        }

        node* iterator = operators->get_head();

        /*
         *
         * TODO: when can negation function be without parentheses?
         *
         * 1. when its argument is a simple operand : N 10
         * 2. when its argument is another function: N IF ( ... )
         *      2.a. the argument could be another negation function without parentheses: N N N ( 2 ) | N N N 2
         *      2.b. the argument could be another negation function with the parentheses: N N ( 2 )
         *
         * SOLUTIONS:
         *
         * 1. if negation doesn't have its own parenthesis:
         *          if parsed operand and next symbol is some other operator:
         *              add the parenthesis
         *              close the parenthesis after the operand
         *
         * 2. if negation doesn't have its own parenthesis:
         *          if another function found and the previous one is a negation function:
         *              add the parenthesis
         *              if ")" found (and it will be since the other functions always have parentheses):
         *                      add closing parenthesis
         *
         * theoretically, the second solution should be able to handle cases 2.a & 2.b (with the help of solution 1)
         *
         */

        while (iterator != nullptr) {

            node* popped = operators->pop();

            // --- this block of code makes sure that all the parentheses are handled correctly even if there are the default ones
            if (parsing_function_flag) {
                if (popped != nullptr) {
                    if (popped->get_content() == negation) { // todo: this could work only for N function, we need to implement function comparer
                        operators->push(popped->get_content(), popped->get_priority(), popped->is_operand());
                        operators->push(open_parenthesis, forth_priority, false);
                        ++(*additional_parentheses_counter);
                    }
                }
            }

            if (*additional_parentheses_counter) popped = operators->pop();
            // ---

            if (popped->get_content() == open_parenthesis || popped->get_priority() < symbol_priority) {
                operators->push(popped->get_content(), popped->get_priority(), popped->is_operand());
                break;
            }

            output->put(popped->get_content(), popped->get_priority(), popped->is_operand());
            iterator = iterator->get_next();

        }

        // this piece of code allows us to handle situations when there are the same priority operators one after another:
        // this kind of situation is not natural for normal mathematical expressions, although, when we use functions,
        // and we have parsed the function, we the operand that now need to be put onto RPN stack, well, isn't put.
        // TODO: does it support cases when functions are being compared??
        if (operators->get_head() != nullptr) {
            if (operators->get_head()->get_priority() == symbol_priority) {
                node* tmp = operators->pop();
                output->put(tmp->get_content(), tmp->get_priority(), tmp->is_operand());
                delete tmp;
            }
        }

        operators->push(symbol, symbol_priority, symbol_is_operand);

    }

}


void add_missing_closing_parentheses(stack* operators, stack* output, int* additional_parentheses_counter) {

    // this block of code makes sure that all the added open parentheses are closed
    if (*additional_parentheses_counter) {
        --(*additional_parentheses_counter);
        parse_operator(operators, output, close_parenthesis, additional_parentheses_counter);
    }

}


// this function will put the operand on the output stack
void parse_operand(stack* operators, stack* output, int* numeric_symbol, bool* parsing_operand_flag, int* additional_parentheses_counter) {

    output->put(*numeric_symbol, zeroth_priority, true);

    // if necessary, close all the open parentheses that are left
    add_missing_closing_parentheses(operators, output, additional_parentheses_counter);

    // reset the variables
    *numeric_symbol = 0;
    *parsing_operand_flag = false;

}


// this function will check if the symbol is an operand/operator and parse it later on
bool parse_symbol(stack* operators, stack* output, int symbol, int* numeric_symbol, bool* parsing_operand_flag, int* additional_parentheses_counter) {

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
        if (*parsing_operand_flag) parse_operand(operators, output, numeric_symbol, parsing_operand_flag, additional_parentheses_counter);

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
        parse_operator(operators, output, symbol, additional_parentheses_counter);

    }

    return false;

}


// this function will parse the infix mathematical expression
void parse_formula(stack* operators, stack* output) {

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
        if (parse_symbol(operators, output, symbol, &numeric_symbol, &parsing_operand_flag, &additional_parentheses_counter)) break;

    }

}