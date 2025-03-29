#include <stdbool.h>

typedef struct terminal_table {
    
    // Actual number of lines
    int n_lines;

    // Max table dimension (expandable)
    int table_dim;

    // Every column's width
    int *widths;

    // Table lines
    char*** lines;
} TerminalTable;

void allocate_terminal_table();
char **createLine(char *perm, char* group, char* owner, const char *type, char *dim, char *name, char *date);
void addLine(char** line);
void print_table();
TerminalTable* getTerminalTable();