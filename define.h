#define FACTOR 10
#define SPACE ' '
#define UNDEFINED (-1)
#define NEGATION 'N'
#define CRITICAL_DIVISOR 0
#define COMPARATOR 0


// a list of symbols that tell the program when to stop the input
enum stop_input_flag {
    stop_quantity_input = '\n',
    stop_formula_input = '.',
    string_terminator = '\0'
};

// a range of ascii's that are visible (used to avoid invisible symbols)
enum visible_ascii_range {
    first_visible_ascii = 32,
    final_visible_ascii = 126
};

// a range of ascii's representing digits
enum digit_ascii_range {
    first_digit_ascii = 48,
    final_digit_ascii = 57
};

// a default arity used when counting arguments
enum arity {
    default_arity = 1
};

// a list of operators' priorities
enum priority {
    zeroth_priority,
    first_priority,
    second_priority,
    third_priority,
    forth_priority
};

// a list of allowed operations (non-function ones)
enum operation {
    addition = '+',
    subtraction = '-',
    multiplication = '*',
    division = '/',
    open_parenthesis = '(',
    close_parenthesis = ')',
    comma = ','
};

// a list of functions' ID's
enum function {
    negation,
    conditional,
    minimum,
    maximum,
    stop_flag
};
