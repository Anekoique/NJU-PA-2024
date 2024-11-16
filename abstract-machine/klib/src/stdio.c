#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...)
{
    return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap)
{
    panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...)
{
    va_list v;
    const char *copy = fmt;
    char *buffer = out;
    const char *arg1;
    int arg2;

    va_start(v, fmt);
    while (*copy != '\0')
    {
        if (*copy == '%')
        {
            copy++;
            switch(*copy)
            {
                case 's':
                    arg1 = va_arg(v, char*);
                    while (*arg1 != '\0')
                    {
                        *buffer = *arg1;
                        buffer++;
                        arg1++;
                    }
                    break;
                case 'd':
                    arg2 = va_arg(v, int);
                    char num_buffer[34];
                    int i = 0;

                    if (arg2 == 0) {
                        num_buffer[i++] = '0';
                    } else {
                        while (arg2 > 0) {
                            num_buffer[i++] = (arg2 % 10) + '0';
                            arg2 /= 10;
                        }
                    }

                    for (int j = i - 1; j >= 0; j--) {
                        *buffer = num_buffer[j];
                        buffer++;
                    }
                    break;
            }
        }
        else 
        {
            *buffer = *copy;
            buffer++;
        }
        copy++;
    }
    *buffer = '\0';
    return 1;
}

int snprintf(char *out, size_t n, const char *fmt, ...)
{
    panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap)
{
    panic("Not implemented");
}

#endif
