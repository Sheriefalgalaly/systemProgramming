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

