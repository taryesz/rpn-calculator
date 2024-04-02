// this function will sort all the nodes inside a stack (ascending or descending order)
void bubble_sort(stack* operands, int ascending) {
    node* j;
    for (j = operands->get_head(); j->get_next() != nullptr; j = j->get_next()) {
        node* i;
        for (i = operands->get_head(); i->get_next() != nullptr; i = i->get_next()) {
            if ((i->get_content() > i->get_next()->get_content() && ascending) || (i->get_content() < i->get_next()->get_content() && !ascending)) {
                int tmp = i->get_content();
                i->set_content(i->get_next()->get_content());
                i->get_next()->set_content(tmp);
            }
        }
    }
}


// this function will perform calculations on a simple binary operator
void perform_simple_operator_calculations(stack* operands, int function_type, int* result, bool* critical_divisor_flag) {

    // remove the two necessary nodes and get their values
    node* first_operand = operands->pop();
    node* second_operand = operands->pop();
    int a = first_operand->get_content();
    int b = second_operand->get_content();

    // compare the operator and do the according action
    switch (function_type) {
        case addition:
            *result = a + b;
            break;
        case subtraction:
            *result = b - a;
            break;
        case multiplication:
            *result = a * b;
            break;
        case division:

            // if division by zero detected
            if (a == CRITICAL_DIVISOR) {

                // print the message
                printf("ERROR\n");

                // set the flag
                *critical_divisor_flag = true;

                // free memory
                delete first_operand;
                delete second_operand;

                // stop executing calculations
                return;

            }
            *result = b / a;
            break;
        default:
            break;
    }

    // free memory
    delete first_operand;
    delete second_operand;

}


// this function will perform calculations on a function
void perform_functional_calculations(stack* operands, int function_type, int arity, int* result) {

    if (function_type == negation) {

        node* first_operand = operands->pop();

        // negate the operand
        *result = -first_operand->get_content();

        // free memory
        delete first_operand;

    }
    else if (function_type == conditional) {

        node* first_operand = operands->pop();
        node* second_operand = operands->pop();
        node* third_operand = operands->pop();

        // if the third > 0, get the middle one, otherwise get the top one
        (third_operand->get_content() > COMPARATOR) ? *result = second_operand->get_content() : *result = first_operand->get_content();

        // free memory
        delete first_operand;
        delete second_operand;
        delete third_operand;

    }
    else {

        // create a variable that will decide whether to sort ascending or descending
        bool order;

        // if the function is MIN, set to true meaning "sort ascending", else "sort descending"
        (function_type == minimum) ? (order = true) : (order = false);

        // create a temporary stack to separate & store the necessary arguments of the function
        auto* arguments = new stack();

        // get only those operands that are inside the function
        for (int i = 0; i < arity; i++) {
            node* popped = operands->pop();
            arguments->push(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->get_function_id(), popped->is_operand(), popped->is_function(), popped->is_last(), popped->get_requires_additional_parenthesis());

            // free memory
            delete popped;

        }

        // sort the operands
        bubble_sort(arguments, order);

        // get the greatest/lowest operand ...
        node* popped = arguments->pop();

        // ... and set it as the result
        *result = popped->get_content();

        // free memory
        arguments->clear();
        delete popped;
        delete arguments;

    }

}


// this function will handle an operation on arguments
void perform_calculations(stack* operands, int function_type, int arity, bool* critical_divisor_flag, int* result, int function_id) {

    // print the operands stack to show the steps
    operands->print();

    // if the operator doesn't have an ID, it means it's a simple operator like '+', '-', '*', '/'
    if (function_id == UNDEFINED) perform_simple_operator_calculations(operands, function_type, result, critical_divisor_flag);

    // otherwise, it's a function
    else perform_functional_calculations(operands, function_type, arity, result);

}


// this function will print the steps of the RPN expression calculations
void calculate(stack* rpn) {

    // create a stack that will store all numbers existent in the RPN expression
    auto* operands = new stack();

    // create and initiate a flag that will tell the program if there was a division by zero, so that it stopped the loop
    bool critical_divisor_flag = false;

    // create a variable that will store the result of an operation
    int result = 0;

    do {

        // remove the head of the stack
        node* iterator = rpn->pop();

        // if the head doesn't exist, stop the loop
        if (iterator == nullptr)  break;

        // if the head is an operand (i.e. a number), save it onto the operands stack
        if (iterator->is_operand()) operands->push(iterator->get_content(), iterator->get_priority(), iterator->get_arity(), iterator->get_function_id(), iterator->is_operand(), iterator->is_function(), iterator->is_last(), iterator->get_requires_additional_parenthesis());

        // if the head is not an operand ...
        else {

            // ... but it is an operator (i.e. '+', '-', '*', '/') ...
            if (!iterator->is_function()) {

                // print the operator with a space after it
                printf("%c ", iterator->get_content());

                // do the calculation using this operator and the necessary amount of arguments
                perform_calculations(operands, iterator->get_content(), iterator->get_arity(), &critical_divisor_flag, &result, iterator->get_function_id());

                // if the division by zero detected, exit the function
                if (critical_divisor_flag) return;

            }

            // ... if it is a function (i.e. a letter) ...
            else {

                // if the letter is the last one in the function's name ...
                if (iterator->is_last()) {

                    // if this letter is a part of MIN/MAX, print the letter with the number of arguments & a space right after it
                    if (iterator->get_function_id() == minimum || iterator->get_function_id() == maximum) printf("%c%d ", iterator->get_content(), iterator->get_arity());

                    // if this letter is a part of some other function, just print the letter with a space after it
                    else printf("%c ", iterator->get_content());

                }

                // if the letter is not the last one, it means it's somewhere in the middle of a function's name ...
                else {

                    // print the letter
                    printf("%c", iterator->get_content());

                    // and restart the loop to get the next symbol
                    continue;

                }

                // after the whole function is printed, do the calculation using this function and the necessary amount of arguments
                perform_calculations(operands, iterator->get_function_id(), iterator->get_arity(), &critical_divisor_flag, &result, iterator->get_function_id());

            }

            // create and initiate operand's properties
            const int priority = zeroth_priority, arity = default_arity, function_id = UNDEFINED;
            const bool is_operand = true, is_function = false, is_last = true, requires_additional_parenthesis = false;

            // add the result of a calculation to the operands stack
            operands->push(result, priority, arity, function_id, is_operand, is_function, is_last, requires_additional_parenthesis);

        }

        // free memory
        delete iterator;

    }
    while (true);

    // print the final result of the RPN calculation
    operands->print();
    printf("\n");

    // free memory inside the temporary stack
    operands->clear();

    // ... and delete the stack, the program won't need it again
    delete operands;

    // free memory inside the RPN stack, the mathematical expression was successfully converted and calculated
    rpn->clear();

}
