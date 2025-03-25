#include <stdbool.h>

typedef struct terminal_table {
    
    // Numero di linee attuali
    int n_lines;

    // Dimensione massima della tabella (espandibile)
    int table_dim;

    // Larghezza di ogni colonna (per allineare i dati printati...)
    int *widths;

    // Vere e proprie linee fatte dalle varie entry
    char*** lines;
} TerminalTable;

void allocate_terminal_table();
char **createLine(char *perm, const char *type, char *dim, char *name, char *date);
void addLine(char** line);
void print_table();
TerminalTable* getTerminalTable();