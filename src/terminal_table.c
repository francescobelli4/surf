#include "terminal_table.h"
#include "utils.h"
#include "files.h"
#include <stdio.h>    
#include <stdlib.h>
#include <string.h>

#include <libintl.h>
#include <locale.h>
#define _(STRING) gettext(STRING)

#define N_COLS 7



/**
 * This function prints the extended version of the files list
 */
void print_table() {

    // Allocating the int list I'm using to adjust the spaces between every column...
    int *widths = (int *) malloc(N_COLS * sizeof(int));
    for (size_t i = 0; i < N_COLS; i++)
    {
        widths[i] = 0;
    }
    
    int right_padding = 2;
    // The header of the table
    char *header[] = {_("PERMISSION"), _("GROUP"), _("OWNER"), _("DIM"), _("TYPE"), _("NAME"), _("DATE")};
    // Getting the list of the files I have to show up
    Files *files = getFiles();

    for (size_t j = 0; j < files->n_lines; j++) {

        for (size_t i = 0; i < N_COLS; i++)
        {
            // FileData structure is actually a list of strings. I can cast files_data[j] (which is a FileData *) to char **, so I can 
            // access every string simply using f[index]!
            char **f = (char **)files->files_data[j];
            int s_len = str_len(f[i]);

            if (s_len > widths[i]) {
                widths[i] = s_len;
            }
        }
    }

    // Printing the header...
    for (size_t i = 0; i < N_COLS; i++)
    {
        printf("| %s%*s", header[i], (widths[i] - str_len(header[i])) + right_padding, ""); 
    }
    printf("\n");

    // Printing files!
    for (size_t i = 0; i < files->n_lines; i++)
    {
        char **f = (char **)files->files_data[i];

        for (size_t j = 0; j < N_COLS; j++) {

            printf("| %s%*s", f[j], (widths[j] - str_len(f[j])) + right_padding, ""); 
        }
        printf("\n");
    }
}