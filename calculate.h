//
// Created by Tarazjusz Szulakiewicz on 11/03/2024.
//

#ifndef RPN_CALCULATOR_CALCULATE_H
#define RPN_CALCULATOR_CALCULATE_H

#endif //RPN_CALCULATOR_CALCULATE_H


// add the result of an operation to RPN stack
void execute_operation(List* rpn, int* priority, int* is_operand, int* is_function, int* arity, int result) {
    set_priority(priority, OPERAND_PRIORITY);
    set_is_operand(is_operand, TRUE);
    set_is_function(is_function, FALSE);
    set_arity(arity, OPERAND_ARITY);
    int id;
    if (rpn->head == NULL) id = 0;
    else { id = rpn->head->id + 1; }
    push(rpn, result, *priority, *is_operand, *is_function, *arity, id);
}


// create a mirrored version of the RPN stack
List mirror_stack(List* stack) {

    // create an iterator that will go through the whole stack
    Node* iterator = stack->head;

    // create and initiate a mirrored stack
    List mirrored_stack = {NULL, NULL};

    // while there are elements in the original stack
    do {

        if (iterator != NULL) {

            // push a symbol to the new mirrored stack
            push(&mirrored_stack, iterator->value, iterator->priority, iterator->is_operand, iterator->is_function, iterator->arity, iterator->id);
            iterator = iterator->next;
        }

    }
    while (iterator != NULL);

    free(iterator);

    return mirrored_stack;

}


// check how many operands left on the stack
void update_number_of_operands(List* stack, int* number_of_operands) {

    // if the stack is empty
    if (stack->head == NULL) {

        // set the variable to zero
        *number_of_operands = 0;
        return;
    }

    // if the stack is not empty

    // create a counter variable
    int counter = 0;

    // create an iterator that will go through the whole stack
    Node* iterator = stack->head;

    // while there are elements in a stack, increment the counter variable
    do {
        counter++;
        iterator = iterator->next;
    }
    while (iterator != NULL);

    // update the 'number_of_operands'
    *number_of_operands = counter;

    free(iterator);

}


void fill_necessary_operands_list(List* stack, List* necessary_operands, int arity) {
    for (int i = 0; i < arity; i++) {
        Node* necessary_operand = pop(stack);
        int flag = FALSE;
        if (necessary_operand != NULL) {
            put(necessary_operands, necessary_operand->value, necessary_operand->priority,
                necessary_operand->is_operand, necessary_operand->is_function, necessary_operand->arity, necessary_operand->id, &flag);
        }
        free(necessary_operand);
    }
}


// check if the mathematical equation requires a priority change (imaginary parentheses)
List check_priority_change(List* stack, int arity, int number_of_operands) {

    List necessary_operands = {NULL, NULL};

    // if there was a priority change
    if (number_of_operands > arity) {

        // create and set the variable to 2 since
        // we need to skip the first two operands that
        // will participate in the operation
        int counter = arity;

        // create a node in which we will store
        // temporarily popped values from the stack
        Node* popped;

        // create a temporary stack which will
        // store temporary popped values from the stack
        // to insert them later to the original stack back
        List temporary_stack = {NULL, NULL};

        // here we skip n values that will be used in the operation
        // but others will be needed to be popped and then pushed back in
        do {

            // get the top symbol from the operations stack
            popped = pop(stack);

            // push the symbol to a temporary stack
            push(&temporary_stack, popped->value, popped->priority, popped->is_operand, popped->is_function, popped->arity, popped->id);

            // increment the variable
            counter++;

        } while (counter != number_of_operands);

        // get the necessary operands
        fill_necessary_operands_list(stack, &necessary_operands, arity);

        // create an iterator that will go through the whole temporary stack
        Node* iterator = temporary_stack.head;

        // while there are elements in the temporary stack
        do {

            // push a symbol back to the original stack
            push(stack, iterator->value, iterator->priority, iterator->is_operand, iterator->is_function, iterator->arity, iterator->id);

            iterator = iterator->next;

        } while (iterator != NULL);

        free(popped);
        free(iterator);

    }
    // if there was no priority change
    else {

        // get the necessary operands
        fill_necessary_operands_list(stack, &necessary_operands, arity);

    }

    return necessary_operands;

}


void perform_binary_calculations(List* necessary_operands, List* stack, List* rpn, Node* iterator, int* result, int* number_of_operands, int* division_by_zero) {

    // check how many operands we have on our stack
    // i.e. if the parsed mathematical equation requires change of
    // priority, or, as to say, imaginary parentheses.
    // if there are more than two operands, it means that the priority change
    // was encountered, and we need to change the order of getting
    // operands from the stack
    *necessary_operands = check_priority_change(stack, BINARY_ARITY, *number_of_operands);

    // since we popped two necessary operands, we have to update the number of them on the stack
    update_number_of_operands(stack, number_of_operands);

    // if both operands exist
    if (necessary_operands->head != NULL && necessary_operands->tail != NULL) {

        Node* first_operand = pop(necessary_operands);
        Node* second_operand = pop(necessary_operands);

        // if the operator is an '*'
        if (iterator->value == ASTERISK) *result = first_operand->value * second_operand->value;
        // if the operator is a '/'
        else if (iterator->value == SLASH) {

            // if the divisor is 0
            if (second_operand->value == ZERO) {
                printf("ERROR\n");
                *division_by_zero = TRUE;
                return;
            } else {
                *result = first_operand->value / second_operand->value;
            }

        }
        // if the operator is a '+'
        else if (iterator->value == PLUS) *result = first_operand->value + second_operand->value;
        // if the operator is a '-'
        else if (iterator->value == MINUS) *result = first_operand->value - second_operand->value;

        if (*result != INFINITY) {
            int priority;
            int is_operand;
            int is_function;
            int arity;
            execute_operation(rpn, &priority, &is_operand, &is_function, &arity, *result);
        }

        free(first_operand);
        free(second_operand);

    }

}


void perform_unary_calculations(List* necessary_operands, List* stack, List* rpn, Node* iterator, int* result, int* number_of_operands) {

    *necessary_operands = check_priority_change(stack, UNARY_ARITY, *number_of_operands);

    // since we popped two necessary operands, we have to update the number of them on the stack
    update_number_of_operands(stack, number_of_operands);

    if (necessary_operands->head != NULL) {

        // if the operator is a function 'N'
        if (iterator->value == NEGATE) {

            Node* popped = pop(necessary_operands);
            *result = ZERO - popped->value;
            free(popped);

        }

        if (*result != INFINITY) {
            int priority;
            int is_operand;
            int is_function;
            int arity;
            execute_operation(rpn, &priority, &is_operand, &is_function, &arity, *result);
        }

    }

}


void perform_ternary_calculations(List* necessary_operands, List* stack, List* rpn, Node* iterator, int* result, int* number_of_operands) {

    *necessary_operands = check_priority_change(stack, TERNARY_ARITY, *number_of_operands);

    update_number_of_operands(stack, number_of_operands);

    // check if IF() ?
    int exists = TRUE;

    if (necessary_operands->head != NULL && necessary_operands->head->next != NULL && necessary_operands->tail != NULL) {

        Node *popped = pop(necessary_operands);

        if (popped->value > 0) {
            *result = pop(necessary_operands)->value;
            pop(necessary_operands);
        } else {
            Node *remove = pop(necessary_operands);
            *result = pop(necessary_operands)->value;
            free(remove);
        }

        if (*result != INFINITY) {
            int priority;
            int is_operand;
            int is_function;
            int arity;
            execute_operation(rpn, &priority, &is_operand, &is_function, &arity, *result);
        }

        free(popped);
    }

}


void check_for_operator_arity(List* stack, List* rpn, Node* iterator, int* number_of_operands, int* division_by_zero) {

    // create a variable to store the result of an operation and initiate it to infinity
    int result = INFINITY;

    List necessary_operands;

    if (iterator->arity == UNARY_ARITY) {
        perform_unary_calculations(&necessary_operands, stack, rpn, iterator, &result, number_of_operands);
    }
    else if (iterator->arity == BINARY_ARITY) {
        perform_binary_calculations(&necessary_operands, stack, rpn, iterator, &result, number_of_operands, division_by_zero);
    }
    else if (iterator->arity == TERNARY_ARITY) {
        perform_ternary_calculations(&necessary_operands, stack, rpn, iterator, &result, number_of_operands);
        // TODO: implement IF() function in order to print steps correctly
    }

}