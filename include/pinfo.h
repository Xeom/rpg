#if ! defined(PINFO_H)
# define PINFO_H

# include <time.h>
# include <stdio.h>

# define pinfo(...)                        \
    do {                                   \
        time_t     _tval;                  \
        struct tm *_tdata;                 \
        char _info[256];                   \
        snprintf(_info, 256, __VA_ARGS__); \
        _tval = time(NULL);                \
        _tdata = localtime(&_tval);        \
        printf(                            \
            " %02d:%02d:%02d [i] %s\n",    \
            _tdata->tm_hour,               \
            _tdata->tm_min,                \
            _tdata->tm_sec,                \
            _info                          \
        );                                 \
    } while (0)

# define perr(...)                         \
    do {                                   \
        time_t     _tval;                  \
        struct tm *_tdata;                 \
        char _info[256];                   \
        snprintf(_info, 256, __VA_ARGS__); \
        _tval = time(NULL);                \
        _tdata = localtime(&_tval);        \
        fprintf(                           \
            stderr,                        \
            " %02d:%02d:%02d [!] %s\n",    \
            _tdata->tm_hour,               \
            _tdata->tm_min,                \
            _tdata->tm_sec,                \
            _info                          \
        );                                 \
    } while (0)

#endif
