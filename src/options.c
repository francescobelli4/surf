#include "options.h"
#include <stdio.h>    
#include <stdlib.h>
#include <unistd.h>

static Options options;

/**
 * This function setups the Options struct
 */
void allocate_options(int argc, char **argv) {


    for (size_t i = 1; i < argc; i++)
    {
        // Handle options
        if (argv[i][0] == '-') {
            
            switch (argv[i][1])
            {
            case 'h':
                
                options.show_hidden = true;
                break;
            case 'e':
            
                options.extended = true;
                break;
            case 'l':

                options.show_links = true;
                break;
            default:
                break;
            }
        // Handle chosen path
        } else {
            if (options.path == NULL) {
                options.path = argv[i];
                setenv("PWD", argv[i], true);
                

                // In order to get "upper" folders' stats, it's necessary to change the CWD
                // (current working directory).
                chdir(argv[i]);
            }
        }
    }

    // If option.path is null, the user wants to list current directory
    if (options.path == NULL) {
        options.path = getenv("PWD");
    }
}


Options* getOptions() {
    return &options;
}