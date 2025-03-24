#include <stdbool.h>

typedef struct terminal_table {
    
    int n_lines;
    int table_dim;
    int *widths;
    char*** lines;
} TerminalTable;

void allocate_terminal_table();
char **createLine(char *perm, const char *type, char *dim, char *name, char *date);
void addLine(char** line);
void print_table();
TerminalTable* getTerminalTable();