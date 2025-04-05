#include <stdbool.h>   

typedef struct filedata {
    char *perm;
    char *group;
    char *owner;
    char *dim;
    char *type;
    char *name;
    char *date;
} FileData;

typedef struct files {
    int n_lines;
    int max_dim;
    FileData **files_data;
} Files;

void initialize_files();

Files *getFiles();
FileData *createFile(char *perm, char* group, char* owner, char *type, char *dim, char *name, char *date);
void addFile(FileData *file);

void order_by_name(Files *files, bool ascending);
void order_by_type(Files *files, bool ascending);
void order_by_date(Files *files, bool ascending);