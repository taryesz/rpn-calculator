//
// Created by Tarazjusz Szulakiewicz on 11/03/2024.
//

#ifndef RPN_CALCULATOR_QUANTITY_H
#define RPN_CALCULATOR_QUANTITY_H

#endif //RPN_CALCULATOR_QUANTITY_H


// inspiration: https://www.geeksforgeeks.org/program-that-allows-integer-input-only/
int get_number_of_formulas() {

    // this variable stores a value of an input
    int user_input_value = 0;

    // proceed to checking the input
    while (TRUE) {

        // get the input from a user
        int user_input_ascii = getchar();

        // convert ascii to its value
        if (user_input_ascii >= ASCII_DIGIT_RANGE_START && user_input_ascii <= ASCII_DIGIT_RANGE_FINISH) {
            user_input_value = user_input_value * FACTOR + (user_input_ascii - ASCII_DIGIT_RANGE_START);
        }

        // if pressed 'Enter', stop the loop
        if (user_input_ascii == QUANTITY_STOP_SYMBOL) {
            break;
        }

    };

    return user_input_value;

};