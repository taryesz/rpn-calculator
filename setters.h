//
// Created by Tarazjusz Szulakiewicz on 23/03/2024.
//

#ifndef RPN_SETTERS_H
#define RPN_SETTERS_H

#endif //RPN_SETTERS_H


void set_priority(int* priority, int value) {
    *priority = value;
}


void set_is_operand(int* is_operand, int value) {
    *is_operand = value;
}


void set_arity(int* arity, int value) {
    *arity = value;
}


void set_is_function(int* is_function, int value) {
    *is_function = value;
}


void set_id(List* rpn, int* id, int current_is_function) {
    if (rpn->tail == NULL) {
        *id = 0;
    }
    else if (rpn->tail->is_function && current_is_function && !rpn->tail->is_function_end_symbol) {
        *id = rpn->tail->id;
    }
    else {
        *id = rpn->tail->id + 1;
    }
}


void set_is_function_end_symbol (int* is_function_end_symbol, int value) {
    *is_function_end_symbol = value;
}


void set_function_id(int* function_id, int value) {
    *function_id = value;
}
