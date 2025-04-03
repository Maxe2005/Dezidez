# Here's how our interface works

It's simple...

Don't mess with us...

bye!

# Our structures:

To develop our graphing tool, we used several structures, notably to code functions in the form of a tree and to return the various possible errors. This part was provided to us at the beginning of the project, and we made some modifications that we will present to you.
The file provided to us contained 6 different structures:

- tree: takes a token (TypeToken), a next tree, and a previous tree as parameters.
- typeToken: takes the type of token value (typeLexeme) and the token value (typeValue) as parameters.
- typeValue: takes as a parameter whether the token value, which can be a real number, a function, an operator, and we decided to add the possibility of including a variable (char) that will take the value x or y to plot a function with two variables.
- typeFunction: takes a mathematical function (exp, sin, cos, etc.) as a parameter.
- typeOperator: takes an operator (plus, minus, times, div, power) as a parameter.
- typeLexeme: takes the name of the type of token value as a parameter.

We also added an error_code structure that associates an error name with an error code, for example: NEGATIVE_ROOT=301, it is impossible to calculate the root of a negative number, so if the user requests to do so, the code will return this error.

# EVALUATOR PART:

The goal of the evaluator is to calculate a function (which is in the form of a tree) based on given variables x and y. Its role is essential for plotting curves, as it allows obtaining the points to display on the graph.
Our code is divided into two main functions: evaluator, our main function that interprets the tree, and calculate_function, an auxiliary function that applies standard mathematical functions.
Evaluator function:

Its objective is to decode the input tree. It takes as input:
- The function in the form of a tree
- The values of x and y
- The error_code pointer.
It returns the result of the input function, calculated from the variables x and y.
This function is recursive and has four stopping cases. The first is if the error_code pointer changes value, which means there is an error in the function to be calculated, and thus the tree decoding must be stopped once an error appears. The second stopping case is if the main token of the tree does not have a type. Finally, the function will stop if the main token of the tree is a real number or a variable, which means we have reached a leaf of the tree.
Once this is done, we use a switch function to look at the type of the main token of the input tree. If it is:

- A variable: we return the value of the variable obtained as input to the function.
- A real number: we return the value of the real number.
- An operator: we use a divide-and-conquer method to calculate the operation.
- A function: we then use the second function of our program.
Calculate_function function:

This function takes as input the type of mathematical function f, the variable x on which to apply f, and the error_code pointer. It returns the application f(x). The possible function types are universal mathematical functions such as sin, cos, or ln.
We first use a switch function to determine which function it is, then we check for any errors before calculating f(x).
In case of error:
In case of error (e.g., division by zero, logarithm of a negative number...), the function returns an incorrect value and modifies the error_code pointer to signal the problem. This allows, for example, stopping the evaluation or displaying an appropriate message to the user.

# Graphical Display

## 1. Home Screen

The home screen facilitates user interaction. It provides access to various features, such as viewing the user manual, displaying acknowledgments, or launching a display mode. Currently, the program offers a 2D display, but a 3D version for functions with two variables is under development by our engineers.

## 2. Return Functionalities

A return functionality allows the user to go back to previous menus without restarting the program. They can return to the home screen by clicking on the house icon in the text menus or by pressing the "Backspace" or "Return" key on the keyboard.

## 3. Top Bar

The user can display up to 20 expressions. By default, a basic function is presented for each case. The user can then click on an expression to modify it. Additionally, when typing in one of the three text areas of the top bar, they can interact with the following functions:

### 3.1 Change Color

The program offers the ability to customize the color of functions via a color selection menu. This menu is activated by clicking on the color wheel icon. Once open, it allows changing the color of functions in real-time (even while holding the left click).

### 3.2 Hide a Function

This feature allows the user to hide or display functions of their choice in real-time, according to their needs.

### 3.3 Delete a Function

The user can delete expressions by clicking on the trash icon with the left mouse button.

### 3.4 Modify Bounds

A text window allows the user to modify the bounds of an expression in real-time for time savings and more flexibility.

## 4. Expression Processing

Once the expression is entered, it undergoes lexical and then syntactic analysis to prepare for further processing.

## 5. Function Display

The tree generated during the analysis is used to associate each value of the interval with its corresponding image on the screen, thanks to the evaluation function. This process repeats in real-time, allowing for dynamic updates of the display.

### 5.1 Zoom

For better graphical analysis, the program allows zooming in and out using the mouse wheel. This modifies the displayed interval of the function.

### 5.2 Centering

The centering feature allows quickly resetting the display. Simply press the "C" key on the keyboard. A "Recenter" button will soon be added to the right bar of the window.

### 5.3 Translation of the Reference Frame

By pressing the right mouse button, the user activates a "sliding" mode that allows them to intuitively navigate the graph.

### 5.4 Evaluator in x

The evaluator, activated by a left-click, allows the user to visualize two lines that intersect the first curve of the expression list. These lines follow the position of the mouse on the x-axis. A display provides the value of `x` as well as the value of the function at the click point.
