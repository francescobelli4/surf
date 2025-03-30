#include <sys/stat.h>

char *decode_permissions(mode_t mode);
char *file_type_to_string(unsigned char d_type);
char *sizeToString(off_t size);