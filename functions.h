// this function will store and return a function name
const char* get_reserved_function_name(int function) {

    switch (function) {
        case negation: return "N";
        case conditional: return "IF";
        case minimum: return "MIN";
        case maximum: return "MAX";
        default: return "";
    }

}


// this function will count how many functions there are
int count_reserved_functions() {
    return stop_flag;
}


// this function will find the length of a string (i.e. a function name, inside the array of functions)
int count_string_length(const char** functions, const int function_id) {
    int length = 0;
    for (int counter = 0; functions[function_id][counter] != string_terminator; counter++) length++;
    return length;
}


// this function will reverse the order of characters in each string of the input array
const char** reverse_function_name(const char** functions, const int reserved_functions_quantity) {

    // create a dynamic 2D array to store the reversed strings
    char** reversed = new char*[reserved_functions_quantity];

    // iterate through the array of functions
    for (int counter = 0; counter < reserved_functions_quantity; counter++) {

        // get the length of the current function string
        int length = count_string_length(functions, counter);

        // allocate memory for the reversed string (+1 for null terminator)
        reversed[counter] = new char[length + 1];

        // iterate through the characters of the current function string
        for (int iterator = 0; iterator < length; iterator++) {

            // assign characters in reversed order to the reversed string
            reversed[counter][iterator] = functions[counter][length - iterator - 1];

        }

        // add string terminator at the end of the reversed string
        reversed[counter][length] = string_terminator;

    }

    // return the array of reversed strings
    return (const char**) reversed;

}


// this function will retrieve the reserved functions, reverse their names, and return them
const char** get_reserved_functions() {

    // count the number of reserved functions
    const int reserved_functions_quantity = count_reserved_functions();

    // create an array of pointers to strings to store the reserved function names
    const char** functions = new const char*[reserved_functions_quantity];

    // populate the array with the names of reserved functions
    for (int i = 0; i < reserved_functions_quantity; i++) functions[i] = get_reserved_function_name(i);

    // reverse the names of the reserved functions
    const char** reversed = reverse_function_name(functions, reserved_functions_quantity);

    // free memory
    delete [] functions;

    // return the reversed names of the reserved functions
    return reversed;

}


// this function will update the function and symbol ID's based on the currently parsed symbol
void update_function_id(node* iterator, const char** functions, const int functions_quantity, int* function_id, int* symbol_id) {

    // iterate through the functions starting from the lastly valid function's ID
    for (int counter = *function_id; counter < functions_quantity; counter++) {

        // iterate through the symbols of the current function
        for (int symbol_counter = *symbol_id; functions[counter][symbol_counter] != string_terminator; symbol_counter++) {

            // check if the content of the current node matches the current symbol
            if (iterator->get_content() == functions[counter][symbol_counter]) {

                // update function_id to the current function
                *function_id = counter;

                // update symbol_id to the next symbol in the current function
                *symbol_id = ++symbol_counter;

                // exit the function after updating identifiers.
                return;

            }

        }

    }

}


// this function will compare the functions in the stack with the reserved functions and assign ID's to function nodes
void compare_functions(stack* operators) {

    // retrieve the reserved functions and their quantity.
    const char** functions = get_reserved_functions();

    // get the number of reserved functions
    const int functions_quantity = count_reserved_functions();

    // create and initialize variables to track the current function and symbol ID's.
    int function_id = 0;
    int symbol_id = 0;

    // get the head of the stack
    node* iterator = operators->get_head();

    // iterate through the nodes in the stack until a non-function node is encountered or the end is reached
    while (iterator->is_function()) {

        // update the function and symbol ID's based on the content of the current node
        update_function_id(iterator, functions, functions_quantity, &function_id, &symbol_id);

        // move to the next node
        iterator = iterator->get_next();

        // exit the loop if the end of the stack is reached
        if (iterator == nullptr || iterator->is_last()) break;

    }

    // get the head of the stack
    iterator = operators->get_head();

    // assign the function ID to each function node in the stack
    while (iterator->is_function()) {

        // set the function ID for the current function node
        iterator->set_function_id(function_id);

        // move to the next node
        iterator = iterator->get_next();

        // exit the loop if the end of the stack is reached
        if (iterator == nullptr || iterator->is_last()) break;

    }

    // free memory
    for (int i = 0; i < functions_quantity; i++) {
        delete [] functions[i];
    }

    delete [] functions;

}