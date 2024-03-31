
# RPN Calculator

This project is an implementation of Reverse Polish Notation calculator [ post-fix variant ].

## Examplementary Usage

At first, you will need to type in a number of how many mathematical formulas you are going to calculate.

Then, you are set to provide the formula itself, separating each symbol with a whitespace and ending it with a period.

As the output, you will see each step of calculations, until the final result is achieved.

### First Example

Let's say I want to transform and calculate one "default" mathematical expression:

https://latex.codecogs.com/svg.image?\frac{6}{8}&plus;1\times&space;1\times&space;7\times&space;2\times&space;3\times&space;4

_**First input:**_ `How many formulas?`

> 1

_**Second input:**_ `Provide the expression here`

> 6 / 8 + 1 * 1 * 7 * 2 * 3 * 4 .

**NOTE: There has to be a period at the end of your formula!**

_**Output:**_

> 6 8 / 1 1 * 7 * 2 * 3 * 4 * + `Converted to RPN`

> \/  8 6 `Divide 6 by 8 [ equals 0 ] `

> \*  1 1 0 `Multiply 1 by 1 [ and print the result of the previous action(s) ]`

> \*  7 1 0

> \*  2 7 0

> \*  3 14 0

> \*  4 42 0

> \+  168 0

> 168 `Result`

The first output line is your RPN formula, and then you have your steps, one by one, until the total is calculated.

### Second Example

This program also has predefined functions, such as:

- _N ( x )_ [ or also _N x_ ] - this will negate _x_ [ _-x_ ].
- _IF ( x , y , z )_ - this checks if _x_ > 0, and if yes, returns _y_, else returns _z_.
- _MIN ( x , y , z , ... )_ - this will find the minimum out of all its arguments.
- _MAX ( x , y , z , ... )_ - this will find the maximum out of all its arguments.

So, you can use these too!

_**First input:**_

> 1

_**Second input:**_

> IF ( IF ( 17 , 1 , 11 ) , N ( ( 19 - 4 ) ) , N ( MAX ( 2 , 6 , 8 , 0 , 34 , N 12 ) ) ) .

_**Output:**_

> 17 1 11 IF 19 4 - N 2 6 8 0 34 12 N MAX6 N IF

> IF 11 1 17

> \- 4 19 1

> N 15 1

> N 12 34 0 8 6 2 -15 1

> MAX6 -12 34 0 8 6 2 -15 1

> N 34 -15 1

> IF -34 -15 1

> -15 `Result`

**NOTE: _MIN_ and _MAX_ functions show how many arguments they have processed [ in this case '_MAX6_' means that _MAX_ has 6 arguments ].**

## Running Tests

This repository contains a folder called "tests" where you can find 10 different `.in` files with various inputs.