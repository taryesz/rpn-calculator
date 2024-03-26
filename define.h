#define ASCII_DIGIT_RANGE_START 48
#define ASCII_DIGIT_RANGE_FINISH 57
#define FACTOR 10
#define STOP_QUANTITY_INPUT '\n'
#define STOP_FORMULA_INPUT '.'
#define SPACE ' '
#define STRING_TERMINATOR '\0'

enum priority {
    zeroth_priority,
    first_priority,
    second_priority,
    third_priority,
    forth_priority
};

enum operation {
    addition = '+',
    subtraction = '-',
    multiplication = '*',
    division = '/',
    open_parenthesis = '(',
    close_parenthesis = ')',
};

enum function {
    negation = 'N',
    conditional,
    minimum,
    maximum,
    stop_flag
};