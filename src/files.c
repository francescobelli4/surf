#include "files.h"
#include <stdio.h>    
#include <stdlib.h>

/**
 * This structure will contain all the files that will be printed to the terminal.
 * files->max_dim is used to realloc files->files_data memory space
 * files->n_lines represents the actual number of files saved in the struct (in files->files>data)
 * files->files_data is a list of FileDatas. Every FileData is a list of strings containing every file's info. 
 */
static Files *files;

/**
 * This function is initializing the files structure.
 */
void initialize_files() {
    files = (Files *) malloc(sizeof(Files));
    files->max_dim = 10;
    files->n_lines = 0;
    files->files_data = (FileData **) malloc(files->max_dim * sizeof(FileData *));
}

Files *getFiles() {
    return files;
}

/**
 * This function creates a new FileData pointer. 
 * NOTE: in the short version, only file->name and file->type will not be NULL!
 */
FileData *createFile(char *perm, char* group, char* owner, char *type, char *dim, char *name, char *date) {
    FileData *file = (FileData *) malloc(sizeof(FileData));

    file->perm = perm;
    file->group = group;
    file->owner = owner;
    file->type = type;
    file->dim = dim;
    file->name = name;
    file->date = date;

    return file;
}

/**
 * This function adds a FileData pointer to Files->files_data, also reallocating files_data if needed.
 */
void addFile(FileData *file) {
    
    if (files->n_lines >= files->max_dim) {
        files->max_dim += 10;
        files->files_data = realloc(files->files_data, files->max_dim * sizeof(char **));
    }

    files->files_data[files->n_lines] = file;
    files->n_lines++;
}

