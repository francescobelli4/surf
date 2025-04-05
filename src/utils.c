#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "files.h"

#define RESET       "\x1b[0m"

#define BOLD        "\x1b[1m"
#define DIM         "\x1b[2m"
#define ITALIC      "\x1b[3m"
#define UNDERLINE   "\x1b[4m"
#define BLINK       "\x1b[5m"
#define REVERSE     "\x1b[7m"
#define HIDDEN      "\x1b[8m"

#define BLACK       "\x1b[30m"
#define RED         "\x1b[31m"
#define GREEN       "\x1b[32m"
#define YELLOW      "\x1b[33m"
#define BLUE        "\x1b[34m"
#define MAGENTA     "\x1b[35m"
#define CYAN        "\x1b[36m"
#define WHITE       "\x1b[37m"

int clamp(int n, int inf, int sup) {
   
    if (n >= inf && n <= sup) return n;
    if (n < inf) return inf; else return sup;
}

/**
 * This function returns a string's length. It has to manage emojis properly...
 */
int str_len(char *str) {
    
    int len = 0;
    char *next = str;

    bool found_emoji = false;
    while (*next)
    {
        // A char that has an int value < 0, is representing a part of the emoji
        if (next[0] > 0) {
            len++;

            if (found_emoji) {
                // Emojis need 2 spaces in the terminal
                len += 2;
                found_emoji = false;
            }
        } else {
            found_emoji = true;
        }

        next++;
    }

    // Emojis need 2 spaces in the terminal
    if (found_emoji) len+=2;

    return len;
}

/**
 * This function converts mode_t mode (that is a bitmask representing file's permissions) to a
 * human-readable format "(usr_perms)(group perms)(others perms)"
 */
char *decode_permissions(mode_t mode) {
    
    char *r = "r";
    char *w = "w";
    char *x = "x";
    char *m = "-";

    char *perms = (char *) malloc(9);
    memset(perms, 0, 9);

    // user
    strcat(perms, (mode & S_IRUSR) ? r : m);
    strcat(perms, (mode & S_IWUSR) ? w : m);
    strcat(perms, (mode & S_IXUSR) ? x : m);
     
    // group
    strcat(perms, (mode & S_IRGRP) ? r : m);
    strcat(perms, (mode & S_IWGRP) ? w : m);
    strcat(perms, (mode & S_IXGRP) ? x : m);
     
    // others
    strcat(perms, (mode & S_IROTH) ? r : m);
    strcat(perms, (mode & S_IWOTH) ? w : m);
    strcat(perms, (mode & S_IXOTH) ? x : m);

    return perms;
}

/**
 * This functions adds decorations to the format string I'm using to print file names in the terminal in the short version
 */
char *file_format_decoration(char *f_type) {
    switch ((int) f_type[0]) {
        case 4: return BOLD BLUE "%s%*s" RESET;
        case 8: return ITALIC "%s%*s" RESET;
        case 10: return CYAN "%s%*s" RESET;
        default: return "%s%*s" RESET;
    }
}

/**
 * This function converts a d_type to a human readble string...
 */
char *file_type_to_string(unsigned char d_type) {
    switch (d_type) {
        case 1: return "FIFO";
        case 2: return "Character device";
        case 4: return "D - 📁​​";
        case 6: return "Block device";
        case 8: return "f - 📝";
        case 10: return "l - 🔗​";
        case 12: return "Socket";
        case 14: return "Whiteout";
        default: return "Unknown";
    }
}

/**
 * This function converts an off_t size representing a file's size in bytes to a human-readable size string.
 */
char *sizeToString(off_t size) {
    double s = (double) size;
    char *str = malloc(32); 
    char *format = "b";

    if (s >= 1e9) {
        format = "Gb";
        s /= 1000 * 1000 * 1000;
    } else if (s >= 1e6)
    {
        format = "Mb";
        s /= 1000 * 1000;
    } else if (s >= 1e3)
    {
        format = "Kb";
        s /= 1000;
    } 
    
    if (str) {
        snprintf(str, 32, "%.1f %s", s, format);
    }
    return str;
}