#include <sys/stat.h>
#include <sys/types.h>

char *file_format_decoration(char *f_type);
char *decode_permissions(mode_t mode);
char *file_type_to_string(unsigned char d_type);
char *sizeToString(off_t size);
int str_len(char *str);
int clamp(int n, int inf, int sup);