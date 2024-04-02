# MyPython Interpreter

This project is a simple Python interpreter implemented in C++. It currently supports basic arithmetic operations, variable assignments, if/else statements, and print statements for integer values. (extra features extended and completed for phase2)

## Compiling the Interpreter

* To compile the interpreter, you will need to have GNU C++ compiler installed on your system. This project uses C++11 standard features, so make sure your compiler supports C++11.

* Navigate to the directory containing the source code and Makefile, then run the following command:

```
make 

```

* This command will invoke the `g++` compiler to compile all `.cpp` files in the current directory, linking them into an executable named `mypython`.

## Running the Interpreter

* After compilation, you can run the interpreter on a Python file with the following command:

```
./mypython file.py

```

* Replace yourfile.py with the path to the Python script you wish to run.

##Cleaning up

* If you want to clean up the compiled executable, you can use the provided clean command in the Makefile:

```
make clean

```

* This will remove the mypython executable from your directory.

# Architecture Overview

## Instructions Storage

* The interpreter represents the parsed source code using an Abstract Syntax Tree (AST), where each node corresponds to a specific language construct (e.g., operations, statements). This design allows for a clear separation between parsing and execution, simplifying the addition of new features.

## Variable Storage

* Variable management is handled by the Environment class, which uses a hash map to associate variable names with their values. This setup supports both local and global scopes, with environment chaining to handle nested scopes effectively.

* For more details on the implementation of these components, please refer to the documentation at the top of the respective source files.
