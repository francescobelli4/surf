#include "terminal_table.h"
#include <stdio.h>    
#include <stdlib.h>
#include <string.h>

static TerminalTable terminal_table;

TerminalTable *getTerminalTable() {
    return &terminal_table;
}

// Bisogna gestire bene le emoji...
int str_len(char *str) {
    
    int len = 0;
    char *next = str;

    bool found_emoji = false;
    while (*next)
    {
        // Se il carattere ha valore numerico minore di zero, sto scorrendo una emoji
        if (next[0] > 0) {
            len++;

            if (found_emoji) {
                // Le emoji occupano 2 spazi
                len += 2;
                found_emoji = false;
            }
        } else {
            found_emoji = true;
        }

        next++;
    }

    // Le emoji occupano 2 spazi
    if (found_emoji) len+=2;

    return len;
}

// Preparo la struct per l'output del terminale...
void allocate_terminal_table() {
    terminal_table.n_lines = 0;
    terminal_table.table_dim = 0;

    terminal_table.widths = (int *) malloc(5 * sizeof(int));
    for (size_t i = 0; i < 5; i++)
    {
        terminal_table.widths[i] = 0;
    }
}

// Creo una nuova linea per l'output del terminale
char **createLine(char *perm, const char *type, char *dim, char *name, char *date) {
    char **line = (char **) malloc(5 * sizeof(char *));

    line[0] = perm;
    line[1] = type;
    line[2] = dim;
    line[3] = name;
    line[4] = date;

    return line;
}

// Aggiungo una linea per l'output del terminale
void addLine(char** line) {
    terminal_table.n_lines++;

    // Se la struct non riesce ad ospitare la nuova riga, espando l'allocazione di memoria
    if (terminal_table.n_lines > terminal_table.table_dim) {
        terminal_table.table_dim += 10;
        terminal_table.lines = realloc(terminal_table.lines, terminal_table.table_dim * sizeof(char **));
    }

    // Aggiungo la linea
    terminal_table.lines[terminal_table.n_lines-1] = line;

    // Per ogni colonna, vedo se il contenuto della nuova riga è più grande del
    // massimo precedente: serve per "normalizzare" la larghezza di ogni colonna
    for (size_t i = 0; i < 5; i++)
    {
        int s_len = str_len(line[i]);

        if (s_len > terminal_table.widths[i]) {
            terminal_table.widths[i] = s_len;
        }
    }
    
}

void print_table() {

    int right_padding = 5;

    for (size_t i = 0; i < terminal_table.n_lines; i++)
    {
        for (size_t j = 0; j < 5; j++) {

            // Lascio a destra di ogni entry tanti spazi quanti sono width[i] - len(line[i][j]) + padding
            printf("%s%*s", terminal_table.lines[i][j], (terminal_table.widths[j] - str_len(terminal_table.lines[i][j])) + right_padding, ""); 
        }
        printf("\n");
    }
}