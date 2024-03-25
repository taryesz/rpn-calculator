// this function will get the quantity of how many mathematical formulae the user wants to convert to RPN
// inspiration: https://www.geeksforgeeks.org/program-that-allows-integer-input-only/
int get_number_of_formulae() {

    // this variable stores a value of an input
    int number_of_formulae = 0;

    // proceed to checking the input
    while (true) {

        // get the input from a user
        int input = getchar();

        // convert ascii to its value
        if (input >= ASCII_DIGIT_RANGE_START && input <= ASCII_DIGIT_RANGE_FINISH) {
            number_of_formulae = number_of_formulae * FACTOR + (input - ASCII_DIGIT_RANGE_START);
        }

        // if pressed 'Enter', stop the loop
        if (input == STOP_QUANTITY_INPUT) break;

    }

    return (number_of_formulae);

}
