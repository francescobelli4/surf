#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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