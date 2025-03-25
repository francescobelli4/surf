#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

char *decode_permissions(mode_t mode) {
    // Proprietario (user)

    char *r = "r";
    char *w = "w";
    char *x = "x";
    char *m = "-";

    char *perms = (char *) malloc(9);
    memset(perms, 0, 9);

    strcat(perms, (mode & S_IRUSR) ? r : m);
    strcat(perms, (mode & S_IWUSR) ? w : m);
    strcat(perms, (mode & S_IXUSR) ? x : m);
     
    // Gruppo (group)
    strcat(perms, (mode & S_IRGRP) ? r : m);
    strcat(perms, (mode & S_IWGRP) ? w : m);
    strcat(perms, (mode & S_IXGRP) ? x : m);
     
    // Altri (others)
    strcat(perms, (mode & S_IROTH) ? r : m);
    strcat(perms, (mode & S_IWOTH) ? w : m);
    strcat(perms, (mode & S_IXOTH) ? x : m);

    return perms;
}

// Funzione per convertire il tipo di file in una stringa leggibile
const char *file_type_to_string(unsigned char d_type) {
    switch (d_type) {
        case 1: return "FIFO";
        case 2: return "Character device";
        case 4: return "D - 📁​​";
        case 6: return "Block device";
        case 8: return "f - 📝";
        case 10: return "Symbolic link";
        case 12: return "Socket";
        case 14: return "Whiteout";
        default: return "Unknown";
    }
}