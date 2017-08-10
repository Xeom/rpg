#include "util.h"

int str_check_len(char *str, int n)
{
    while (n--)
    {
        if (*(str++) == '\0')
            return 0;
    }

    return 1;
}
