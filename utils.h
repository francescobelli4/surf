#include <sys/stat.h>

char *decode_permissions(mode_t mode);
const char *file_type_to_string(unsigned char d_type);