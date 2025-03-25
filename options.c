#include "options.h"
#include <stdio.h>    
#include <stdlib.h>
#include <unistd.h>

static Options options;

void allocate_options(int argc, char **argv) {


    for (size_t i = 1; i < argc; i++)
    {

        if (argv[i][0] == '-') {
            
            switch (argv[i][1])
            {
            case 'h':
                
                options.show_hidden = true;
                break;
            
            default:
                break;
            }
        } else {
            if (options.path == NULL) {
                options.path = argv[i];
                setenv("PWD", argv[i], true);
                // Il cambio della CWD (current working directory) è necessario per poter
                // ottenere le info di stat() anche di cartelle "superiori" a quella in cui
                // il programma è situato!
                chdir(argv[i]);
            }
        }
    }

    if (options.path == NULL) {
        options.path = getenv("PWD");
    }
}


Options* getOptions() {
    return &options;
}