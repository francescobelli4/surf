#include "terminal_table.h"
#include <stdio.h>    
#include <stdlib.h>
#include <string.h>

static TerminalTable terminal_table;

TerminalTable *getTerminalTable() {
    return &terminal_table;
}

char **createLine(char *perm, const char *type, char *dim, char *name, char *date) {
    char **line = (char **) malloc(5 * sizeof(char *));

    line[0] = perm;
    line[1] = type;
    line[2] = dim;
    line[3] = name;
    line[4] = date;

    return line;
}

void allocate_terminal_table() {
    terminal_table.n_lines = 0;
    terminal_table.table_dim = 0;

    terminal_table.widths = (int *) malloc(5 * sizeof(int));
    for (size_t i = 0; i < 5; i++)
    {
        terminal_table.widths[i] = 0;
    }
    
    terminal_table.lines = (char ***) malloc(terminal_table.n_lines * sizeof(char **));
}



void addLine(char** line) {
    terminal_table.n_lines++;

    if (terminal_table.n_lines > terminal_table.table_dim) {
        terminal_table.table_dim += 10;
        terminal_table.lines = realloc(terminal_table.lines, terminal_table.table_dim * sizeof(char **));
    }

    terminal_table.lines[terminal_table.n_lines-1] = line;

    for (size_t i = 0; i < 5; i++)
    {
        int s_len = strlen(line[i]);
        if (s_len > terminal_table.widths[i]) {
            terminal_table.widths[i] = s_len;
        }
    }
    
}

void print_table() {

    printf("|%-*s||%-*s||%-*s||%-*s||%-*s|\n\n",
        terminal_table.widths[0]+2, "PERMISSION",
        terminal_table.widths[1]-3, "TYPE",
        terminal_table.widths[2]+2, "DIM",
        terminal_table.widths[3]+2, "NAME",
        terminal_table.widths[4]+2, "DATE"
    );

    for (size_t i = 1; i < terminal_table.n_lines; i++)
    {
        for (size_t j = 0; j < 5; j++) {
            printf("|%-*s|", terminal_table.widths[j] + 2, terminal_table.lines[i][j]); // Allinea a sinistra e stampa con larghezza fissa
        }
        printf("\n");
    }
    
}