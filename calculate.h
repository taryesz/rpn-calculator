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


void perform_calculations(stack* operands, int function_type, int arity, int* critical_divisor_flag, int* result) {

    operands->print();

    if (function_type == addition) {
        node* first_operand = operands->pop();
        node* second_operand = operands->pop();
        int a = first_operand->get_content();
        int b = second_operand->get_content();
        *result = a + b;
        delete first_operand;
        delete second_operand;
    }
    else if (function_type == subtraction) {
        node* first_operand = operands->pop();
        node* second_operand = operands->pop();
        int a = first_operand->get_content();
        int b = second_operand->get_content();
        *result = b - a;
        delete first_operand;
        delete second_operand;
    }
    else if (function_type == multiplication) {
        node* first_operand = operands->pop();
        node* second_operand = operands->pop();
        int a = first_operand->get_content();
        int b = second_operand->get_content();
        *result = a * b;
        delete first_operand;
        delete second_operand;
    }
    else if (function_type == division) {
        node* first_operand = operands->pop();
        node* second_operand = operands->pop();
        int divisor = first_operand->get_content();
        int divident = second_operand->get_content();
        if (divisor == CRITICAL_DIVISOR) {
            printf("ERROR\n");
            *critical_divisor_flag = true;
            return;
        }
        *result = divident / divisor;
        delete first_operand;
        delete second_operand;
    }
    else if (function_type == negation) {
        node* first_operand = operands->pop();
        int a = first_operand->get_content();
        *result = -a;
        delete first_operand;
    }
    else if (function_type == conditional) {
        node* first_operand = operands->pop();
        node* second_operand = operands->pop();
        node* third_operand = operands->pop();
        int a = first_operand->get_content();
        int b = second_operand->get_content();
        int c = third_operand->get_content();
        (c > 0) ? *result = b : *result = a;
        delete first_operand;
        delete second_operand;
        delete third_operand;
    }
    else if (function_type == minimum) {
        const bool order = true;

        auto* arguments = new stack();
        for (int i = 0; i < arity; i++) {
            node* popped = operands->pop();
            arguments->push(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->get_function_id(), popped->is_operand(), popped->is_function(), popped->is_last());
            delete popped;
        }
        bubble_sort(arguments, order);
        node* popped = arguments->pop();
        *result = popped->get_content();
        arguments->clear();
        delete popped;
        delete arguments;
    }
    else if (function_type == maximum) {
        const bool order = false;

        auto* arguments = new stack();
        for (int i = 0; i < arity; i++) {
            node* popped = operands->pop();
            arguments->push(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->get_function_id(), popped->is_operand(), popped->is_function(), popped->is_last());
            delete popped;
        }
        bubble_sort(arguments, order);
        node* popped = arguments->pop();
        *result = popped->get_content();
        arguments->clear();
        delete popped;
        delete arguments;
    }

}


void calculate(stack* rpn) {

    auto* operands = new stack();

    int critical_divisor_flag = false;
    int result = 0;

    do {

        node* iterator;
        iterator = rpn->pop();
        if (iterator == nullptr)  break;

        if (iterator->is_operand()) {
            operands->push(iterator->get_content(), iterator->get_priority(), iterator->get_arity(), iterator->get_function_id(), iterator->is_operand(), iterator->is_function(), iterator->is_last());
        }
        else {

            if (!iterator->is_function()) {

                printf("%c ", iterator->get_content());
                perform_calculations(operands, iterator->get_content(), iterator->get_arity(), &critical_divisor_flag, &result);
                if (critical_divisor_flag) return;

            } else {

                if (iterator->is_last()) {
                    if (iterator->get_function_id() == minimum || iterator->get_function_id() == maximum) {
                        printf("%c%d ", iterator->get_content(), iterator->get_arity());
                    } else printf("%c ", iterator->get_content());
                }
                else {
                    printf("%c", iterator->get_content());
                    continue;
                }

                perform_calculations(operands, iterator->get_function_id(), iterator->get_arity(), &critical_divisor_flag, &result);

            }

            const bool is_operand = true;
            const bool is_function = false;
            const bool is_last = true;

            operands->push(result, zeroth_priority, default_arity, UNDEFINED, is_operand, is_function, is_last);

        }

        delete iterator;

    }
    while (true);

    operands->print();
    operands->clear();
    delete operands;
    rpn->clear();

}