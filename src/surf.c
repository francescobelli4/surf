#include <stdio.h>    
#include <stdlib.h>
// SYS_* macros
#include <sys/syscall.h> 
// syscall(...)
#include <unistd.h>
// Flags and macros
#include <linux/fcntl.h> 
// Defines UNIX/POSIX data types (off_t, int64_t,...)
#include <sys/types.h>
// getpwuid
#include <pwd.h>   
// getgrgid
#include <grp.h>  

#include "utils.h"
#include "options.h"
#include "terminal_table.h"

#include <libintl.h>
#include <locale.h>
#define _(STRING) gettext(STRING)


#define BUFFER_SIZE 1024

// Defining linux_dirent64 struct to interact with files
struct linux_dirent64 {
    int64_t        d_ino;       
    off_t        d_off;       
    unsigned short d_reclen;    // Size of this linux_dirent64
    unsigned char  d_type;      // File type
    char           d_name[];    // Filename 
};

/**
 * This function uses the Options static struct to identify the file the user wants to open.
 * It returns -1 in case of error
 */
int open_file() {

    // A file descriptor is an integer that identifies a file that has been opened in a process.
    int file_descriptor = syscall(SYS_openat, AT_FDCWD, getOptions()->path, O_RDONLY, 0);
    if (file_descriptor == -1) {
        perror(_("Error"));
        exit(1);
    }
    return file_descriptor;
}


int main (int argc, char **argv) {

    setlocale(LC_ALL, "");
    bindtextdomain("surf", "/usr/share/locale");
    textdomain("surf");

    // English fallback
    if (setlocale(LC_ALL, NULL) == NULL) {
        setlocale(LC_ALL, "en_US.UTF-8");
    }


    // This struct will be used to save files' metadata
    struct stat f_metadata;

    // Setting up options and terminal_table structs
    allocate_options(argc, argv);
    allocate_terminal_table();

    // This buffer will contain linux_dirent64s
    char buffer[BUFFER_SIZE];
    struct linux_dirent64 *dir_data;

    // A file descriptor is an integer that identifies a file that has been opened in a process
    int file_descriptor = open_file();

    // Header della tabella di output
    addLine(createLine(_("PERMISSION"), _("GROUP"), _("OWNER"), _("TYPE"), _("DIM"), _("NAME"), _("DATE")));

    unsigned int n_byte; 
    // getdents64 reads n_bytes <= BUFFER_SIZE bytes from the file identified by file_descriptor
    // and it saves linux_dirent64s in buffer until it's full.
    // n_bytes can be used with an offset to identify every file's linux_dirent64 in the buffer.
    // The number of iterations of this cycle depends on BUFFER_SIZE!
    while ((n_byte = syscall(SYS_getdents64, file_descriptor, buffer, BUFFER_SIZE)) > 0) {

        unsigned int offset = 0;
        while(offset < n_byte) {

            bool show_dir = true;

            // Reinitializing dir_data to point to every file's linux_dirent64 until every file has been checked
            dir_data = (struct linux_dirent64 *)(buffer+offset);

            // If a file name starts with ".", then it's an hidden file
            if (dir_data->d_name[0] == '.' && !getOptions()->show_hidden) {
                show_dir = false;
            }

            if (show_dir) {

                // stat overwrites f_metadata to save a file's metadata (permissions, owner, group, size,...)
                // returns -1 in case of error
                long res = syscall(SYS_stat, dir_data->d_name, &f_metadata);

                char *file_type = file_type_to_string(dir_data->d_type);
                char *perms = res != -1 ? decode_permissions(f_metadata.st_mode) : "";
                char *owner = res != -1 ? getpwuid(f_metadata.st_uid)->pw_name : "";
                char *group = res != -1 ? getgrgid(f_metadata.st_gid)->gr_name : "";
                
                
                addLine(createLine(
                    perms, 
                    group, 
                    owner, 
                    file_type, 
                    dir_data->d_type == 8 ? sizeToString(f_metadata.st_size) : "-", 
                    dir_data->d_name, 
                    "10/10/2010")
                );
            }

            offset += dir_data->d_reclen;
        }
    }

    print_table();

    // Closing the file...
    syscall(SYS_close, file_descriptor);

    exit(0);
}