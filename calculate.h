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


void perform_calculations(stack* operands, stack* rpn, int function_type, int arity, int* critical_divisor_flag, int* result) {

    operands->print();

    if (function_type == addition) {
        int a = operands->pop()->get_content();
        int b = operands->pop()->get_content();
        *result = a + b;
    }
    else if (function_type == subtraction) {
        int a = operands->pop()->get_content();
        int b = operands->pop()->get_content();
        *result = b - a;
    }
    else if (function_type == multiplication) {
        int a = operands->pop()->get_content();
        int b = operands->pop()->get_content();
        *result = a * b;
    }
    else if (function_type == division) {
        int divisor = operands->pop()->get_content();
        int divident = operands->pop()->get_content();
        if (divisor == CRITICAL_DIVISOR) {
            printf("ERROR\n");
            *critical_divisor_flag = true;
            return;
        }
        *result = divident / divisor;
    }
    else if (function_type == negation) {
        int a = operands->pop()->get_content();
        *result = -a;
//        printf(">>> %d\n", *result);
    }
    else if (function_type == conditional) {
        int a = operands->pop()->get_content();
        int b = operands->pop()->get_content();
        int c = operands->pop()->get_content();
        (c > 0) ? *result = b : *result = a;
    }
    else if (function_type == minimum) {
        const bool order = true;

        auto* arguments = new stack();
        for (int i = 0; i < arity; i++) {
            node* popped = operands->pop();
            arguments->push(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->get_function_id(), popped->is_operand(), popped->is_function(), popped->is_last());
        }
        bubble_sort(arguments, order);
        *result = arguments->pop()->get_content();
        arguments->clear();
        delete arguments;
    }
    else if (function_type == maximum) {
        const bool order = false;

        auto* arguments = new stack();
        for (int i = 0; i < arity; i++) {
            node* popped = operands->pop();
            arguments->push(popped->get_content(), popped->get_priority(), popped->get_arity(), popped->get_function_id(), popped->is_operand(), popped->is_function(), popped->is_last());
        }
        bubble_sort(arguments, order);
        *result = arguments->pop()->get_content();
        arguments->clear();
        delete arguments;
    }

}


void calculate(stack* rpn) {

    auto* operands = new stack();
    node* iterator;

    int critical_divisor_flag = false;
    int result = 0;

    do {

        iterator = rpn->pop();
        if (iterator == nullptr)  break;

        if (iterator->is_operand()) {
            operands->push(iterator->get_content(), iterator->get_priority(), iterator->get_arity(), iterator->get_function_id(), iterator->is_operand(), iterator->is_function(), iterator->is_last());
        }
        else {

            if (!iterator->is_function()) {

                printf("%c ", iterator->get_content());
                perform_calculations(operands, rpn, iterator->get_content(), iterator->get_arity(), &critical_divisor_flag, &result);
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

                perform_calculations(operands, rpn, iterator->get_function_id(), iterator->get_arity(), &critical_divisor_flag, &result);

            }

            const bool is_operand = true;
            const bool is_function = false;
            const bool is_last = true;

            operands->push(result, zeroth_priority, default_arity, UNDEFINED, is_operand, is_function, is_last);
//            printf("pushed: ");
//            operands->print();

        }

    }
    while (true);

    operands->print();
    operands->clear();
    delete operands;
    rpn->clear();

}