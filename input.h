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
        if (input >= first_digit_ascii && input <= final_digit_ascii) {
            number_of_formulae = number_of_formulae * FACTOR + (input - first_digit_ascii);
        }

        // if pressed 'Enter', stop the loop
        if (input == stop_quantity_input) break;

    }

    return (number_of_formulae);

}
