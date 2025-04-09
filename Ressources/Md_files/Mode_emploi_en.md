# How to Use It:

- Available functions: sin(), cos(), tan(), sqrt(), exp(), log(), abs(), sinc(), entier()
- For negative values always use parentheses ex: sin(-2) => sin((-2)) or -4 => (-4)
- For exponents use ** ex: x to the power y => x**y
- For decimal numbers use '.' not ','
- All uppercase letters are treated as lowercase

## 1. Home Screen:

The home screen provides easy access to features like the user manual, acknowledgments or launching display mode, with a 3D version in development.

## 2. Back:

To return to previous menus without restarting the program, either click the home icon or use the "Backspace" or "Return" key.

## 3. Top Bar:

Allows displaying and editing up to 20 expressions with interactive features.

   - Color change: Select a new color in real-time via the color wheel icon
   - Hide a function: Show or hide functions as needed in real-time
   - Delete a function: Click the trash icon to delete an expression
   - Adjust bounds: Modify an expression's bounds via a text window for more flexibility

## 4. Function Display:

Function evaluation dynamically updates their graphical representation.

   - Zoom: Use the mouse wheel to adjust the graph display
   - Center: Reset the display with the "C" key or upcoming "Recenter" button
   - Coordinate system pan: Right-click and drag to navigate the curve
   - X evaluator: Activate with left-click to display coordinates of a point on the curve

# Our Structures:

To develop our grapher we used several structures, particularly to code functions as trees and to return various possible errors. This part was provided at the start of the project, with some modifications we'll present.
The provided file contained 6 different structures:

- tree: takes a token (TypeToken), next tree and previous tree as parameters
- typetoken: takes the token value type (typestring) and token value (typevalue) as parameters
- typevalue: takes the token value which can be a real number, function, operator, and we added the possibility to include a variable (char) taking x or y values to graph two-variable functions
- typefunction: takes a mathematical function (exp, sin, cos,...)
- typeoperator: takes an operator (plus, minus, times, div, power)
- typestring: takes the name of the token value type

We also added an error_code structure associating error names with error codes, for example: NEGATIVE_SQUARE_ROOT=301 - it's impossible to calculate the square root of a negative number so if the user requests this the code returns this error.

# Explanation of the Lexical_Analysis Function

The 'Lexical_Analysis' function transforms a mathematical expression string into a token array. It breaks down the expression into elements that can be easily manipulated for subsequent syntax analysis operations.

This tokenization process is a fundamental preliminary step for future mathematical expression analysis and evaluation.

## Global Operation

The function takes four parameters:
* 'TokenArray': array that will contain the generated tokens
* 'Expression': the string representing the mathematical expression
* 'error': pointer to a variable that will contain the error code if a problem occurs
* 'Dimension': indicates if the expression has 1 or 2 variables (0 for 1D, 1 for 2D)

Important: The lexical analyzer treats uppercase letters as lowercase.

Important: For decimal numbers, use points not commas.

## Subfunctions Used

The 'Lexical_Analysis' function follows a three-step processing sequence, each using a dedicated subfunction:

1. 'RemoveSpaces'
   This function removes all spaces from the mathematical expression.

2. 'AddImplicitMultiplication'
   This function detects and adds implicit multiplication operators. For example, it transforms '2x' into '2*x'.

3. 'TokenDecomposition'
   This function breaks down the expression into individual tokens and stores them in 'TokenArray', accounting for the dimension parameter.

## Detailed Process with Examples

Let's take the expression "3 + cos(5x)" as an example.

### Step 1: Space Removal
'RemoveSpaces' transforms "3 + cos(5x)" into "3+cos(5x)".

### Step 2: Adding Implicit Multiplications
'AddImplicitMultiplication' detects implicit multiplications between numbers and variables. It transforms "3+cos(5x)" into "3+cos(5*x)".

### Step 3: Token Creation
'TokenDecomposition' breaks down the expression into tokens:
* '3' → REAL type token with value 3.0
* '+' → OPERATOR type token with value PLUS
* 'cos' → FUNCTION type token with value COS
* '(' → OPEN_PAR
* '5' → REAL type token with value 5.0
* '*' → OPERATOR type token with value TIMES
* 'x' → VARIABLE type token with value 'x'
* ')' → CLOSE_PAR
* Adds an END token at the end

## How 'TokenDecomposition' Identifies Tokens

The 'TokenDecomposition' function analyzes character by character using several subfunctions to correctly identify each token:

* 'PositiveRealToken': Creates a token for positive numbers
* 'NegativeRealToken': Handles negative numbers (ex: "(-2.5)")
* 'OperatorToken': Identifies operators (+, -, *, /, **)
* 'FunctionToken': Identifies functions (sin, cos, abs, etc.)
* 'VariableToken': Identifies variables (x, y) based on the 'Dimension' parameter

## Variable Handling by Dimension

The 'VariableToken' function uses the 'Dimension' parameter to determine allowed variables:
* If 'Dimension' = 0: only variable 'x' is allowed
* If 'Dimension' = 1: variables 'x' and 'y' are allowed

## Error Handling

The function signals several possible error types:
* UNKNOWN_FUNCTION (101): Unrecognized function
* INVALID_NUMBER (102): Incorrect number format (ex: "1.2.3")
* UNKNOWN_CHARACTER (103): Unrecognized character
* UNKNOWN_VARIABLE (104): Variable not allowed by dimension

## Complete Example

For the expression "2x + sin(3.5)" with 'Dimension' = 0:

1. Space removal: "2x+sin(3.5)"
2. Adding implicit multiplications: "2*x+sin(3.5)"
3. Token creation:
   * REAL (2.0)
   * OPERATOR (TIMES)
   * VARIABLE ('x')
   * OPERATOR (PLUS)
   * FUNCTION (SIN)
   * OPEN_PAR
   * REAL (3.5)
   * CLOSE_PAR
   * END

# Syntax Analysis

### createEmptyNode

Purpose: Returns an empty tree

Parameters:
- None

Function operation:
- Creates empty left and right branches

### buildExpressionTree

Purpose: Recursively builds the tree from a token array

Parameters:
- A token array
- Index of first cell to analyze
- Index of last cell to analyze
- Error from last recursive call (default error is 0)

Function operation:

If error in last recursive call: returns empty tree at branch end

If start index > end index: returns error = EMPTY_MEMBER

If there's an operator: splits tree on operator into left and right branches

Otherwise, returns error in these cases:
- FUNCTION_PARENTHESIS_PROBLEM: missing parenthesis after function or unclosed parenthesis
- EMPTY_MEMBER: operator not surrounded by processable objects, empty parentheses
- CLOSED_PARENTHESIS_FIRST_TOKEN: closed parenthesis without being opened
- PARENTHESIS_COUNT_MISMATCH: number of opening parentheses ≠ closing parentheses
- PROBLEM_AFTER_REAL_OR_VARIABLE: Supposedly impossible
- MISSING_END: if "END" token is missing

### findLowestPriorityOperator

Purpose: Returns the operator where to split between left and right branches

Parameters:
- A token array
- Index of first cell to analyze
- Index of last cell to analyze
- Error from last recursive call (default error is 0)

Function operation:

Initializes parenthesis nesting depth to 0
Sets default split index to -1

minOperatorIndex = -1 if no operator in function
minOperatorPriority represents the operator to split the token array on

Scans array cell by cell looking for operators

If cell is operator, current parenthesis depth is zero and current cell operator is better split candidate:
minOperatorIndex = current index
minOperatorPriority = current cell

If cell is OPEN_PAR:
parenthesis depth increases

If cell is CLOSE_PAR:
parenthesis depth decreases

If parenthesis depth isn't zero (=> OPEN_PAR count ≠ CLOSE_PAR count)
error = PARENTHESIS_COUNT_MISMATCH

### findExpressionLength

Purpose: Returns array length

Parameters:
- A token array

Function operation:

Scans array and stops if cell is END
If no END found then end = -1

### checkParenthesesBalance

Purpose: Returns boolean about parenthesis balance

Parameters:
- Start index of array to analyze
- End index of array to analyze
- Token array

Function operation:

Scans array counting OPEN_PAR and CLOSE_PAR

### buildSyntaxTree

Purpose: Builds complete syntax tree from token sequence

Parameters:
- Token array
- Last error from buildExpressionTree (0 for first call)

Function operation:

Checks if END token exists
If exists then tree builds by calling buildExpressionTree
Else error = MISSING_END

# EVALUATOR PART:

The evaluator's purpose is to calculate a function (in tree form) from given x and y variables. Its role is essential for plotting curves as it provides points to display on the graph.
Our code divides into two main functions: evaluator, our main function that interprets the tree, and compute_function, a helper function that applies standard math functions.

Function evaluator:

Its purpose is to decode the input tree. It takes as input:
- The function as a tree
- x and y values
- The error_code pointer
It returns the result of the input function calculated from x and y variables.
This recursive function has four stop cases. First is if error_code pointer changes value, meaning there's an error in the function to compute so tree decoding must stop when error appears. Second stop case is if the tree's main token has no type. Finally the function stops if the tree's main token is a real number or variable, meaning we reached a tree leaf.
Then using a switch function we check the main token's type:
- Variable: returns the variable's value from function input
- Real number: returns the real value
- Operator: uses divide and conquer to calculate the operation
- Function: uses our program's second function

Function compute_function:

This function takes as input the math function type f, variable x to apply f to, and error_code pointer. It returns f(x). Possible function types are universal math functions like sin, cos or ln.
First we check with a switch which function it is, then verify no error exists before calculating f(x).

Error cases:
In case of error (ex: division by zero, negative logarithm...), the function returns an incorrect value and modifies error_code pointer to signal the problem. This allows stopping evaluation or displaying appropriate user messages.

# Graphical Display

## 1. Home Screen

The home screen facilitates user interaction. It provides access to various features like consulting the manual, displaying acknowledgments or launching display mode. Currently the program offers 2D display, but our engineers are developing a 3D version for two-variable functions.

## 2. Back Features

A back feature lets users return to previous menus without restarting the program. They can return home by clicking the home icon in text menus or pressing the "Backspace" or "Return" key.

## 3. Top Bar

Users can display up to 20 expressions. By default, a base function shows for each case. Users can then click an expression to modify it. When editing in one of the top bar's three text zones, they can interact with these functions:

### 3.1 Color Change

The program offers function color customization via a color selection menu. This activates by clicking the color wheel icon. Once open, it allows real-time color changes (even while holding left click).

### 3.2 Hide a Function

This feature lets users show or hide functions in real-time as needed.

### 3.3 Delete a Function

Users can delete expressions by left-clicking the trash icon.

### 3.4 Adjust Bounds

A text window lets users modify expression bounds in real-time for time savings and flexibility.

## 4. Expression Processing

Once entered, the expression undergoes lexical then syntactic analysis to prepare for later processing.

## 5. Function Display

The tree generated during analysis associates each interval value with its corresponding on-screen image via the evaluation function. This process repeats in real-time for dynamic display updates.

### 5.1 Zoom

For better graph analysis, the program allows zooming with the mouse wheel, modifying the displayed function interval.

### 5.2 Centering

The centering feature quickly resets the display. Just press the "C" key. A "Recenter" button will soon be added to the window's right bar.

### 5.3 Coordinate System Pan

Right-clicking activates a "slide" mode for intuitive graph navigation.

### 5.4 X Evaluator

The evaluator, activated by left-click, displays two lines intersecting the first expression list curve. These lines follow mouse position on the x-axis. A display shows the 'x' value and function value at click point.