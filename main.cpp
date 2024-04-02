#include <iostream>
#include "define.h"
#include "classes/stack.h"
#include "input.h"
#include "functions.h"
#include "parse.h"
#include "calculate.h"


int main() {

    // create needed stacks
    auto* output = new stack();
    auto* operators = new stack();
    auto* arguments = new stack();

    // get number of formulae that the user is going to provide
    const int number_of_formulae = get_number_of_formulae();

    // process all the formulae
    for (int counter = 0; counter < number_of_formulae; counter++) {

        // clear the stacks before using them to avoid data mix-up
        output->clear();
        operators->clear();
        arguments->clear();

        // input a mathematical expression, and convert in to the RPN
        parse_formula(operators, output, arguments);

        // print the converted expression
        output->print();

        // clear the stacks for a reuse
        arguments->clear();
        operators->clear();

        // do actual calculations on the RPN expression
        calculate(output);

    }

    // clear stack in case any elements are left
    output->clear();
    operators->clear();
    arguments->clear();

    // delete stacks
    delete output;
    delete operators;
    delete arguments;

    return 0;

}
