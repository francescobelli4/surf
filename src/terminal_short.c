#include "terminal_short.h"
#include "files.h"
#include "utils.h"

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <math.h>

/**
 * This function will print the formatted file names in the user's console, also adjusting the spacing between them
 * to have a prettier and more readable list.
 */
void print_short() {

    // ioctl will save the user's terminal width and height in this struct
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int padding = 3;

    Files *f = getFiles();
    if (f->n_lines == 0) return;
    FileData **files = getFiles()->files_data;

    // This represents the maximum length of a "slot" in the list. It also includes padding
    // I'll use this to align all the columns
    int max_width = 0;

    for (size_t i = 0; i < f->n_lines; i++)
    {
        int len = str_len(files[i]->name)+padding;
        if (len > max_width) max_width = len;
    }

    // n_cols needs to be clamped: i'll always have more than 1 column and less than f->n_lines columns!
    int n_cols = clamp(w.ws_col / max_width, 1, f->n_lines);
    // I'm using ceil on n_rows because C rounds numbers to floor by default, but I need floor+1 rows to represent all the files!
    int n_rows = ceil((double)f->n_lines / (double)n_cols);

    // Printing out the list...
    for (size_t i = 0; i < n_rows; i++)
    {
        for (size_t j = 0; j < n_cols; j++)
        {
            if (i + j*n_rows < f->n_lines)
                printf(file_format_decoration(files[i + j*n_rows]->type), files[i + j*n_rows]->name, j != n_cols - 1 ? (max_width - str_len(files[i + j*n_rows]->name)) : 0, "");
        }
        printf("\n");
    }
}