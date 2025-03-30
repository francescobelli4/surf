#include "terminal_table.h"
#include <stdio.h>    
#include <stdlib.h>
#include <string.h>

#define N_COLS 7

static TerminalTable terminal_table;

TerminalTable *getTerminalTable() {
    return &terminal_table;
}

/**
 * This function returns a string's length. It has to manage emojis properly...
 */
int str_len(char *str) {
    
    int len = 0;
    char *next = str;

    bool found_emoji = false;
    while (*next)
    {
        // A char that has an int value < 0, is representing a part of the emoji
        if (next[0] > 0) {
            len++;

            if (found_emoji) {
                // Emojis need 2 spaces in the terminal
                len += 2;
                found_emoji = false;
            }
        } else {
            found_emoji = true;
        }

        next++;
    }

    // Emojis need 2 spaces in the terminal
    if (found_emoji) len+=2;

    return len;
}

/**
 * This function setups the terminal table struct
 */
void allocate_terminal_table() {
    terminal_table.n_lines = 0;
    terminal_table.table_dim = 0;

    terminal_table.widths = (int *) malloc(N_COLS * sizeof(int));
    for (size_t i = 0; i < N_COLS; i++)
    {
        terminal_table.widths[i] = 0;
    }
}

/**
 * This function creates a new line that will be a part of the terminal_table. 
 * Every line is represented as an array of strings.
 */
char **createLine(char *perm, char* group, char* owner, const char *type, char *dim, char *name, char *date) {
    char **line = (char **) malloc(N_COLS * sizeof(char *));

    line[0] = perm;
    line[1] = group;
    line[2] = owner;
    line[3] = type;
    line[4] = dim;
    line[5] = name;
    line[6] = date;

    return line;
}

/**
 * This function adds a new line to the terminal table. Each line can be created
 * using createLine(...).
 * terminal_table.lines is reallocated dynamically.
 */
void addLine(char** line) {
    terminal_table.n_lines++;

    // Reallocation of terminal_table.lines
    if (terminal_table.n_lines > terminal_table.table_dim) {
        terminal_table.table_dim += 10;
        terminal_table.lines = realloc(terminal_table.lines, terminal_table.table_dim * sizeof(char **));
    }

    terminal_table.lines[terminal_table.n_lines-1] = line;

    // Adjusting every column's width based on the last max width
    for (size_t i = 0; i < N_COLS; i++)
    {
        int s_len = str_len(line[i]);

        if (s_len > terminal_table.widths[i]) {
            terminal_table.widths[i] = s_len;
        }
    }
    
}

void print_table() {

    int right_padding = 2;

    for (size_t i = 0; i < terminal_table.n_lines; i++)
    {
        for (size_t j = 0; j < N_COLS; j++) {

            printf("| %s%*s", terminal_table.lines[i][j], (terminal_table.widths[j] - str_len(terminal_table.lines[i][j])) + right_padding, ""); 
        }
        printf("\n");
    }
}