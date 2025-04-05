# Surf - (educational)

**Surf** is a simple reimplementation of the Linux `ls` command, written in C. The project is still in development and focuses on using **Linux system calls** directly, minimizing the use of C standard library wrappers for core operations.

![image](https://github.com/user-attachments/assets/f4d39388-65f1-4d89-8200-3989c255e3bc)

![image](https://github.com/user-attachments/assets/37cb6187-e78a-4543-9479-ec965b601edb)

## Features

- **File and directory listing**, similar to the `ls` command.
- **Available languages**: Italian (`it_IT`) and English (`en_US`).

## Installation

You can install the application using one of the following methods:

### Standard installation (recommended)

1. Clone the repository:
```bash
git clone https://github.com/francescobelli4/surf.git
```
2. Navigate to the project folder:
```bash
cd surf
```

3. Run the installation command:
```bash
sudo make install
```

### Compile the source and translations manually

If you prefer to recompile the source code and generate translation files, use the following commands:

1. Clone the repository:
```bash
git clone https://github.com/francescobelli4/surf.git
```

2. Navigate to the project folder:
```bash
cd surf
```

3. Run the compilation command:
```bash
make compile
```

### Uninstall

To remove Surf from your system, run:
```bash
sudo make clean
```
### Usage

The program functions similarly to ls. Basic syntax:

```bash
surf [option] [directory]
```

### Available options:

```bash
-e: Show a files list with more info
-h: Show hidden files
-l: Show symbolic links
... WIP :D ...
```

### Example:
```bash
surf -h /usr/local/bin
```
This will display all files, including hidden ones, in the directory.
If no path is specified, it will show the current directory.

### Known Issues

Bug: Running surf (subfolder)/ results in an error. This issue is known and will be addressed in future updates.
TODO: fix "DATE" column

### Notes

This project is created for **educational purposes**, aiming to explore Linux system calls in a practical way and understand how to build low-level utilities using the C programming language.
