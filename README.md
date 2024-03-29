
# RPN Calculator

This project is an implementation of Reverse Polish Notation calculator [ post-fix variant ].

## Examplementary Usage

At first, you will need to type in a number of how many mathematical formulas you are going to calculate.

Then, you are set to provide the formula itself, separating each symbol with a whitespace and ending it with a period.

As the output, you will see each step of calculations, until the final result is achieved.

### First Example

Let's say I want to calculate one "default" mathematical formula:

\frac{6}{8} + 1 \times 1 \times 7 \times 2 \times 3 \times 4

_**First input:**_ `How many formulas?`

> 1

_**Second input:**_ `The formula goes here`

> 6 / 8 + 1 * 1 * 7 * 2 * 3 * 4 .

**NOTE: There has to be a period at the end of your formula!**

_**Output:**_

> 6 8 / 1 1 * 7 * 2 * 3 * 4 * + `RPN formula`

> \/  8 6 

> \*  1 1 0 

> \*  7 1 0 

> \*  2 7 0 

> \*  3 14 0 

> \*  4 42 0 

> \+  168 0 

> 168 `Result`

The first output line is your RPN formula, and then you have your steps, one by one, until the total is calculated.

### Second Example

This program also has predefined functions, such as:

- _N ( x )_ [ or also _N x_] - this will negate _x_ [ _-x_ ].
- _IF ( x , y , z )_ - this checks if _x_ > 0, and if yes, returns _y_, else returns _z_.
- _MIN ( x , y , z , ... )_ - this will find the minimum out of all its arguments.
- _MAX ( x , y , z , ... )_ - this will find the maximum out of all its arguments.

So, you can use these too!

_**First input:**_

> 1

_**Second input:**_

> IF ( IF ( 17 , 1 , 11 ) , N ( ( 19 - 4 ) ) , N ( 14 ) ) . 

_**Output:**_

> 17 1 11 IF 19 4 - N 14 N IF

...