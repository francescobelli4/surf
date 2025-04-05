#include <stdio.h>    
#include <stdlib.h>
#include <string.h>
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
// statx macros
#include <linux/stat.h>
#include <time.h>

#include "utils.h"
#include "options.h"
#include "terminal_table.h"
#include "terminal_short.h"
#include "files.h"

#include <libintl.h>
#include <locale.h>
#define _(STRING) gettext(STRING)


#define BUFFER_SIZE 1024

// Defining linux_dirent64 struct to interact with files
struct linux_dirent64 {
    int64_t         d_ino;       
    off_t           d_off;       
    unsigned short  d_reclen;    // Size of this linux_dirent64
    unsigned char   d_type;      // File type
    char            d_name[];    // Filename 
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
    struct statx f_metadata;

    // Setting up options and files struct
    initialize_files();
    allocate_options(argc, argv);

    // This buffer will contain linux_dirent64s
    char buffer[BUFFER_SIZE];
    struct linux_dirent64 *dir_data;

    // A file descriptor is an integer that identifies a file that has been opened in a process
    int file_descriptor = open_file();

    unsigned int n_byte; 
    // getdents64 reads n_bytes <= BUFFER_SIZE bytes from the file identified by file_descriptor
    // and it saves linux_dirent64s in buffer until it's full.
    // n_bytes can be used with an offset to identify every file's linux_dirent64 in the buffer.
    // The number of iterations of this cycle depends on BUFFER_SIZE!
    while ((n_byte = syscall(SYS_getdents64, file_descriptor, buffer, BUFFER_SIZE)) > 0) {
        unsigned int offset = 0;

        if (n_byte == -1) return 0;

        while(offset < n_byte) {

            bool show_file = true;

            // Reinitializing dir_data to point to every file's linux_dirent64 until every file has been checked
            dir_data = (struct linux_dirent64 *)(buffer+offset);

            // If a file name starts with ".", then it's an hidden file
            if (dir_data->d_name[0] == '.' && !getOptions()->show_hidden) {
                show_file = false;
            }

            if (dir_data->d_type == 10 && !getOptions()->show_links) {
                show_file = false;
            }

            if (show_file) {

                // statx overwrites f_metadata to save a file's metadata (permissions, owner, group, size,...)
                // returns -1 in case of error
                long res = syscall(SYS_statx, AT_FDCWD, dir_data->d_name, AT_STATX_SYNC_AS_STAT, STATX_ALL, &f_metadata);

                // file_type will be one char if the user is using the short version, otherwise it will be file_type_to_string(type)!
                char *file_type = (char *)malloc(2*sizeof(char));
                file_type[0] = (char)dir_data->d_type;
                file_type[1] = '\0';
                
                char *perms;
                char *group;
                char *owner;
                char *creation_date;

                if (getOptions()->extended) {
                    file_type = file_type_to_string(dir_data->d_type);
                    perms     = res != -1 ? decode_permissions(f_metadata.stx_mode) : "";
                    owner     = res != -1 ? getpwuid(f_metadata.stx_uid)->pw_name : "";
                    group     = res != -1 ? getgrgid(f_metadata.stx_gid)->gr_name : "";  
                    time_t creation_time = f_metadata.stx_btime.tv_sec;
                    creation_date      = ctime(&creation_time);
                    creation_date[strcspn(creation_date, "\n")] = '\0';  
                }

                // Many of these strings will be NULL if the user is using the short version! (because less info are needed...)
                addFile(createFile(
                    perms, 
                    group, 
                    owner, 
                    file_type, 
                    dir_data->d_type == 8 ? sizeToString(f_metadata.stx_size) : "-", 
                    dir_data->d_name, 
                    creation_date
                ));
            }

            offset += dir_data->d_reclen;
        }
    }


    getOptions()->extended ? print_table() : print_short();

    // Closing the file...
    syscall(SYS_close, file_descriptor);

    exit(0);
}