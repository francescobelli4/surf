#include <stdio.h>    
#include <stdlib.h>

#include <sys/syscall.h> // Per SYS_openat
#include <linux/fcntl.h> // Per i flag O_RDONLY, O_CREAT, ecc.
#include <unistd.h> // Per syscall()
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>
#include "options.h"
#include "terminal_table.h"

// Dimensione standard...
#define BUFFER_SIZE 1024

// Definizione della struttura linux_dirent64, come descritta in man getdents64
struct linux_dirent64 {
    int64_t        d_ino;       // 64-bit inode number
    off_t        d_off;       // Not an offset, see getdents()
    unsigned short d_reclen;    // Size of this dirent
    unsigned char  d_type;      // File type
    char           d_name[];    // Filename (null-terminated)
};

void setup_terminal() {
    char *items[] = {"PERMISSIONS", "TYPE", "DIM", "NAME", "DATE"};
    int cols = 5;  // Numero di colonne
    int width = 15; // Larghezza fissa per ogni colonna

    for (int i = 0; i < cols; i++) {
        printf("%-*s", width, items[i]); // Allinea a sinistra e stampa con larghezza fissa
    }
    printf("\n");
}


int open_file() {

    // Un id per i file che si trovano in una cartella
    int file_descriptor = syscall(SYS_openat, AT_FDCWD, getOptions()->path, O_RDONLY, 0);
    if (file_descriptor == -1) {
        perror("Errore");
        exit(1);
    }
    return file_descriptor;
}

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
        case 4: return "📁 - dir​";
        case 6: return "Block device";
        case 8: return "📝 - file​";
        case 10: return "Symbolic link";
        case 12: return "Socket";
        case 14: return "Whiteout";
        default: return "Unknown";
    }
}

int main (int argc, char **argv) {

    struct stat st;

    allocate_options(argc, argv);
    allocate_terminal_table();

    // Buffer in cui la syscall getdents64 salva le struct linux_dirent64 fino a riempirlo
    char buffer[BUFFER_SIZE];
    struct linux_dirent64 *dir_data;

    int file_descriptor = open_file();

    // Questa syscall resitutisce il numero di byte che legge e salva in buffer le linux_dirent64
    int n_byte = syscall(SYS_getdents64, file_descriptor, buffer, BUFFER_SIZE);

    addLine(createLine("PERMISSION", "TYPE", "DIM", "NAME", "DATE"));

    while (n_byte>0) {

        int offset = 0;

        while(offset < n_byte) {

            bool show_dir = true;

            dir_data = (struct linux_dirent64 *)(buffer+offset);

            if (dir_data->d_name[0] == '.' && !getOptions()->show_hidden) {
                show_dir = false;
            }

            if (show_dir) {
            
                long res = syscall(SYS_stat, dir_data->d_name, &st);


                addLine(createLine(res != -1 ? decode_permissions(st.st_mode) : "", file_type_to_string(dir_data->d_type), "10MB", dir_data->d_name, "10/10/2010"));
            }

            offset += dir_data->d_reclen;
        }


        n_byte = syscall(SYS_getdents64, file_descriptor, buffer, BUFFER_SIZE);
    }

    print_table();

    syscall(SYS_close, file_descriptor);
    exit(0);
}