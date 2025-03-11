# File Utility Programs

This repository contains four programs implemented using C standard library functions:

## 1. cat.c
Displays the contents of a file provided by the user on the terminal.

**Compilation Command:**
```bash
gcc cat.c -o cat
```

**Usage Example:**
```bash
./cat myfile.c
```

## 2. mycp.c
Copies a file from the source to the destination. The destination can be either a file with a different name or a directory.
![image](https://github.com/user-attachments/assets/227a534d-b377-4cd9-a13b-53ea1752f8ea)

**Compilation Command:**
```bash
gcc mycp.c -o mycp
```

**Usage Examples:**

- Copy a file to a directory:

    ```bash
    ./mycp ./mycp.c ../
    ```

- Copy a file with a new name:

    ```bash
    ./mycp ./mycp.c ../mycp12.c
    ```

## 3. mymv.c
Performs the same function as `mycp.c` but removes the source file after copying (i.e., it moves the file).

**Compilation Command:**
```bash
gcc mymv.c -o mymv
```

**Usage Examples:**

- Move a file to a directory:

    ```bash
    ./mymv ./my.c ../
    ```

- Move a file with a new name:

    ```bash
    ./mymv ./build_process.pdf ../buildProcess.pdf
    ```

## 4. mypwd.c
Prints the current working directory.

**Compilation Command:**
```bash
gcc mypwd.c -o mypwd
```

**Usage Example:**
```bash
./mypwd
```
## 5-femtoShell.c
small shell works with echo and exit cmds
![image](https://github.com/user-attachments/assets/a2b2754b-a8da-415e-ae01-1c5b585004cd)
**Compilation Command:**
```bash
gcc femtoShell.c -o femto 
```

**Usage Example:**
```bash
./femto
```
![image](https://github.com/user-attachments/assets/f8f12673-e455-4d9a-a7ff-0877c26ee3b8)


# Pico Shell

Pico Shell is a simple command-line interpreter that extends the functionality of a (femto shell). It handles both built-in commands and external commands by forking a new process and executing the provided arguments.

## Compilation commmand
To compile the Pico Shell:

```bash
gcc picoshell.c -o pico
```

## Features
- Supports built-in commands:
  - `cd` (change directory)
  - `pwd` (print working directory)
  - `echo` (print arguments)
  - `exit` (terminate the shell)
- Executes external programs (e.g., `ls`, `vim`, `cat`, `sed`, etc.)
- Displays the current user and working directory in the prompt.

## Examples of Commands

### 1. **Changing Directories**
```bash
sheko:/home/sheko/Documents/sysprog/codes$ cd ~
sheko:/home/sheko$

sheko:/home/sheko$ cd /
sheko:/$
```

### 2. **Opening and Editing Files**
```bash
sheko:/home/sheko/Documents/sysprog/codes$ vim test.txt
```

### 3. **Viewing File Contents**
```bash
sheko:/home/sheko/Documents/sysprog/codes$ cat test.txt
hello world
```

### 4. **Using External Programs**
```bash
sheko:/home/sheko/Documents/sysprog/codes$ sed 's/world/linux/' test.txt
hello linux
```

### 5. **Navigating Up the Directory Structure**
```bash
sheko:/home/sheko/Documents/sysprog/codes$ cd ..
sheko:/home/sheko/Documents/sysprog$ ls
codes  ppt  ref  simple-shell-sources  simple-utility-sources
```

### 6. **Handling Invalid Directories**
```bash
sheko:/home/sheko/Documents/sysprog/simple-shell-sources/ff$ cd unknowndir
pico shell:cd: /home/sheko/Documents/sysprog/simple-shell-sources/ff: No such file or directory
```

## Exit the Shell
To exit the Pico Shell, use the `exit` command:

```bash
sheko:/home/sheko$ exit
bye bye friend
```

# Nano Shell - Features Overview

## New Features

### 1. Local Variable Management

#### Adding Local Variables
- **Structure Definition:**
  ```c
  // Local Variable Structure
  typedef struct {
      char *key; // Pointer to the variable name
      char *val; // Pointer to the variable value
  } local_var_struct;

  // Array of pointers to local variable structures (supports up to 10 variables)
  local_var_struct *local_var_st[10];
  ```

- **Functionality:**
  - Each local variable is stored in a `local_var_struct`.
  - A new structure is allocated for every variable added.
  - The first available (NULL) index in the `local_var_st` array is located and assigned to the new structure.
## Compilation commmand
To compile the Nano Shell:

```bash
gcc nanoshell.c -o nano
```

#### Removing Local Variables
- Searches the `local_var_st` array for a specified key.
- Frees the memory allocated for the matching structure.
- Sets the corresponding index to `NULL` to mark it as available.

#### Printing Local Variables
- Iterates through the `local_var_st` array.
- Displays all stored **keys** and **values**.

#### Local Variable Scope Limitation
- Local variables are only available within the current process.
- When a new process (such as a child shell) is created using `execvp()`, local variables are **not inherited**.

Example:
```bash
$ x=5
$ y=10
$ set
x=5
y=10
$ ./nano  # Start a new shell
$ set
# No output - local variables are not inherited.
```

### 2. Environment Variable Management

#### External Environment Reference
- Utilizes the global `environ` variable to manage environment variables:
  ```c
  extern char **environ; // Access to the environment variables
  ```

#### Adding Environment Variables
- Uses the `putenv()` function to add environment variables.
- Accepts a **key** and **value** to add them to the environment.

#### Removing Environment Variables
- Uses the `unsetenv()` function to remove environment variables.
- Accepts the **key** to identify and delete the environment variable.

#### Environment Variable Inheritance
- Environment variables are inherited by child processes.

Example:
```bash
$ export v=value
success
$ ./nano  # Start a new shell
$ env
...
v=value
```

### 3. Executing External Programs

#### Using `execvp()`
- Executes a new process while inheriting the current environment variables.
- Syntax:
  ```c
  int execvp(const char *file, char *const argv[]);
  ```
  - **file**: The name of the executable.
  - **argv**: An array of argument strings (NULL-terminated).

- **Behavior:**
  - Replaces the current process image with a new one.
  - Inherits the existing environment variables automatically.

#### Why Not `execvpe()`?
- `execvpe()` allows specifying custom environment variables but is deprecated and not available on most modern Linux distributions.

## Usage

- **Add Local Variable:** Stores a key-value pair in the local variable array.
- **Remove Local Variable:** Deletes a key-value pair from the local variable array.
- **Print Local Variables:** Displays all stored key-value pairs.
- **Add Environment Variable:** Uses `putenv()` to add a new environment variable.
- **Remove Environment Variable:** Uses `unsetenv()` to delete an existing environment variable.
- **Execute External Program:** Uses `execvp()` to run external programs while preserving the current environment.



# Microshell&#x20;

This project implements a simple shell (microshell) that supports handling **input**, **output**, and **error** redirection operations.

## Features

- **Input Redirection (****`<`****)**: Read from a file instead of standard input (stdin).
- **Output Redirection (****`>`****)**: Write output to a file instead of standard output (stdout).
- **Error Redirection (****`2>`****)**: Write error messages to a file instead of standard error (stderr).
- **Command Execution**: Execute external programs with proper redirection.

## How It Works

1. **Parse the Command:**
   - Identify the redirection operators (`<`, `>`, `2>`).
2. **Open Files:**
   - For `>`, open the file in write mode (create if not exists, truncate if exists).
   - For `2>`, open the file in write mode for error output.
   - For `<`, open the file in read mode.
3. **Redirect Output/Input:**
   - Use `dup2()` to redirect file descriptors to stdin (0), stdout (1), or stderr (2).
4. **Execute the Program:**
   - Execute the command using `execvp()` while preserving the redirection.

## Examples

### Example 1: Redirect Output and Error

Given the following Python script `py`:

```python
#!/usr/bin/env python3

# Simple Python Program: Greeting

# Print a welcome message
print("Hello! Welcome to the program.")

# Ask for the user's name
# name = input("What's your name? ")

# Greet the user
print(f"Nice to meet you, {name}!")

# Ask for the user's age and respond
# age = int(input("How old are you? "))
print(f"Wow, {age} years old! That's awesome.")
```

When you run the script with output and error redirection:

```bash
python3 ./py >output 2>error
```

- **`output`** will contain standard output:

```bash
$ cat output
Hello! Welcome to the program.
```

- **`error`** will capture errors:

```bash
$ cat error
Traceback (most recent call last):
  File "./py", line 12, in <module>
    print(f"Nice to meet you, {name}!")
NameError: name 'name' is not defined
```

### Example 2: Redirect Input and Output

Suppose we want to filter lines containing the word `print` from the error file and save the result to a new file:

```bash
grep print <error >output
```

- **`output`** will now contain:

```bash
$ cat output
    print(f"Nice to meet you, {name}!")
```


## Notes

-  **edge cases** are handled like missing files or invalid commands or writing in directoy i haven't permissions to write in
```bash
sheko:/home/sheko/Documents/sysprog/codes$ cd /
sheko:/$ echo hello >text
the shell could not open the file for writing
```

## Compilation

To compile the microshell:

```bash
gcc microshell.c -o microshell
```

## Running the Microshell

Start the microshell:

```bash
./microshell
```






