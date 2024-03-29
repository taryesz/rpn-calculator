const char* get_reserved_function_name(int function) {

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


int count_string_length(const char** functions, const int function_id) {
    int length = 0;
    for (int counter = 0; functions[function_id][counter] != STRING_TERMINATOR; counter++) length++;
    return length;
}


// this function will mirror the string needed, since the functions are being parsed from end to the beginning
const char** reverse_function_name(const char** functions, const int reserved_functions_quantity) {

    char** reversed = new char*[reserved_functions_quantity];

    for (int counter = 0; counter < reserved_functions_quantity; counter++) {
        int length = count_string_length(functions, counter);
        reversed[counter] = new char[length + 1];
        for (int iterator = 0; iterator < length; iterator++) {
            reversed[counter][iterator] = functions[counter][length - iterator - 1];
        }
        reversed[counter][length] = STRING_TERMINATOR;
    }

    return (const char**) reversed;

}


const char** get_reserved_functions() {

    const int reserved_functions_quantity = count_reserved_functions();
    const char** functions = new const char*[reserved_functions_quantity];

    for (int i = 0; i < reserved_functions_quantity; i++) functions[i] = get_reserved_function_name(i);
    const char** reversed = reverse_function_name(functions, reserved_functions_quantity);
    delete [] functions;

    return reversed;
}


void update_function_id(node* iterator, const char** functions, const int functions_quantity, int* function_id, int* symbol_id) {
    for (int counter = *function_id; counter < functions_quantity; counter++) {
        for (int symbol_counter = *symbol_id; functions[counter][symbol_counter] != STRING_TERMINATOR; symbol_counter++) {
            if (iterator->get_content() == functions[counter][symbol_counter]) {
                *function_id = counter;
                *symbol_id = ++symbol_counter;
                return;
            }
        }
    }
}


void compare_functions(stack* operators) {

    const char** functions = get_reserved_functions();
    const int functions_quantity = count_reserved_functions();
    int function_id = 0;
    int symbol_id = 0;

    node* iterator = operators->get_head();
    while (iterator->is_function()) {
        update_function_id(iterator, functions, functions_quantity, &function_id, &symbol_id);
        iterator = iterator->get_next();
        if (iterator == nullptr || iterator->is_last()) break;
    }

    // Assign function ID to each function node
    iterator = operators->get_head();
    while (iterator->is_function()) {
        iterator->set_function_id(function_id);
        iterator = iterator->get_next();
        if (iterator == nullptr || iterator->is_last()) break;
    }

    // deallocate elements of functions since they were allocated dynamically
    for (int i = 0; i < functions_quantity; i++) {
        delete functions[i];
    }
    delete [] functions;

}