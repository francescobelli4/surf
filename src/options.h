#include <stdbool.h>

typedef struct options {
    
    bool show_hidden;
    bool show_links;
    bool extended;

    char* path;
} Options;


void allocate_options(int argc, char **argv);
Options* getOptions();