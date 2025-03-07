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


