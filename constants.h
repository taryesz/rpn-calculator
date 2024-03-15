//
// Created by Tarazjusz Szulakiewicz on 11/03/2024.
//

#ifndef RPN_CALCULATOR_CONSTANTS_H
#define RPN_CALCULATOR_CONSTANTS_H

#endif //RPN_CALCULATOR_CONSTANTS_H


#ifndef CONSTANTS_H
#define CONSTANTS_H

// operators
#define PLUS '+'
#define MINUS '-'
#define ASTERISK '*'
#define SLASH '/'
#define OPEN_PARENTHESES '('
#define CLOSE_PARENTHESES ')'

// ascii ranges (digits and capital letters)
#define ASCII_DIGIT_RANGE_START 48
#define ASCII_DIGIT_RANGE_FINISH 57
#define ASCII_LETTER_RANGE_START 65
#define ASCII_LETTER_RANGE_FINISH 90

// factor needed to perform a formula of conversion
// from a numeric ascii to its value
#define FACTOR 10

// critical divisor (to catch cases when trying to divide by zero)
#define ZERO 0

// functions
#define NEGATE 'N'

// boolean
#define TRUE 1
#define FALSE 0

// size of reserved functions array
#define RESERVED_FUNCTIONS_COUNTER 4

// other symbols
#define STRING_TERMINATOR '\0'
#define SPACE ' '
#define EQUATION_STOP_SYMBOL '.'
#define QUANTITY_STOP_SYMBOL '\n'
#define COMMA ','

// symbol properties
#define OPERAND_PRIORITY 0
#define OPERAND_ARITY INFINITY

#define OPERATOR_PLUS_MINUS_PRIORITY 1
#define OPERATOR_ASTERISK_SLASH_PRIORITY 2

#define FUNCTION_PRIORITY 3

#define BINARY_ARITY 2
#define PARENTHESIS_ARITY INFINITY
#define UNARY_ARITY 1
#define TERNARY_ARITY 3

#define PARENTHESIS_PRIORITY 4

#define DEFAULT_ID (-1)

// all possible functions used
enum function_names {
    N,
    IF,
    MIN,
    MAX
};

#endif
