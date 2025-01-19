#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>
#define __NATIVE_USE_KLIB__
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...)
{
    va_list v;
    va_start(v, fmt);
    const char *copy = fmt;
    const char *arg1;
    int arg2;
    void *arg3;
    int i;

    while (*copy != '\0')
    {
        if (*copy == '%')
        {
            copy++;
            switch (*copy)
            {
            case 's':
                arg1 = va_arg(v, char *);
                while (*arg1 != '\0')
                {
                    putch(*arg1);
                    arg1++;
                }
                break;
            case 'c':
                arg2 = va_arg(v, int);
                putch(arg2);
                return 0;
            case 'd':
                arg2 = va_arg(v, int);
                char num_buffer[32];
                i = 0;
                if (arg2 == 0)
                    putch('0');
                else
                {
                    while (arg2 > 0 && i < 32)
                    {
                        num_buffer[i++] = (arg2 % 10) + '0';
                        arg2 /= 10;
                    }
                }

                for (int j = i - 1; j >= 0; j--)
                    putch(num_buffer[j]);
                break;
            case 'l':
                arg2 = va_arg(v, int);
                char num_buffer2[64];
                copy++;
                i = 0;
                if (arg2 == 0)
                    putch('0');
                else
                {
                    while (arg2 > 0)
                    {
                        num_buffer2[i++] = (arg2 % 10) + '0';
                        arg2 /= 10;
                    }
                }

                for (int j = i - 1; j >= 0; j--)
                    putch(num_buffer2[j]);
                break;
            case 'x':
                arg2 = va_arg(v, int);
                if (arg2 == 0)
                {
                    putch('0');
                }
                else
                {
                    char hex_buffer[9]; // For 32-bit integer, max 8 hex digits + null terminator
                    int i = 0;
                    while (arg2 > 0)
                    {
                        int digit = arg2 % 16;
                        if (digit < 10)
                            hex_buffer[i++] = digit + '0';
                        else
                            hex_buffer[i++] = (digit - 10) + 'a'; // For lower case hexadecimal letters
                        arg2 /= 16;
                    }

                    // Print the hex string in reverse order
                    for (int j = i - 1; j >= 0; j--)
                        putch(hex_buffer[j]);
                }
                break;
            case 'p':  // Handle pointer address
                arg3 = va_arg(v, void *);
                uintptr_t ptr_val = (uintptr_t)arg3; // Cast pointer to unsigned int
                if (ptr_val == 0)
                {
                    putch('0');
                }
                else
                {
                    char hex_buffer[9]; // For 32-bit address (max 8 hex digits + null terminator)
                    int i = 0;
                    while (ptr_val > 0)
                    {
                        int digit = ptr_val % 16;
                        if (digit < 10)
                            hex_buffer[i++] = digit + '0';
                        else
                            hex_buffer[i++] = (digit - 10) + 'a'; // Lower case hexadecimal
                        ptr_val /= 16;
                    }

                    // Print "0x" prefix for pointer addresses
                    putch('0');
                    putch('x');

                    // Print the hex string in reverse order
                    for (int j = i - 1; j >= 0; j--)
                        putch(hex_buffer[j]);
                }
                break;
            }
        }
        else
        {
            putch(*copy);
        }
        copy++;
    }
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
            switch (*copy)
            {
            case 's':
                arg1 = va_arg(v, char *);
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

                if (arg2 == 0)
                {
                    num_buffer[i++] = '0';
                }
                else
                {
                    while (arg2 > 0)
                    {
                        num_buffer[i++] = (arg2 % 10) + '0';
                        arg2 /= 10;
                    }
                }

                for (int j = i - 1; j >= 0; j--)
                {
                    *buffer++ = num_buffer[j];
                }
                break;
            }
        }
        else
        {
            *buffer++ = *copy;
        }
        copy++;
    }
    *buffer = '\0';
    return 1;
}

int snprintf(char *out, size_t n, const char *fmt, ...)
{
    va_list v;
    const char *copy = fmt;
    char *buffer = out;
    const char *arg1;
    int arg2;
    int count = 0;

    va_start(v, fmt);
    while (*copy != '\0')
    {
        if (*copy == '%')
        {
            copy++;
            switch (*copy)
            {
            case 's':
                arg1 = va_arg(v, char *);
                while (*arg1 != '\0')
                {
                    *buffer = *arg1;
                    count++;
                    buffer++;
                    arg1++;
                }
                break;
            case 'd':
                arg2 = va_arg(v, int);
                char num_buffer[34];
                int i = 0;

                if (arg2 == 0)
                {
                    num_buffer[i++] = '0';
                }
                else
                {
                    while (arg2 > 0)
                    {
                        num_buffer[i++] = (arg2 % 10) + '0';
                        arg2 /= 10;
                    }
                }

                for (int j = i - 1; j >= 0; j--)
                {
                    *buffer++ = num_buffer[j];
                    count++;
                }
                break;
            }
        }
        else
        {
            *buffer++ = *copy;
            count++;
        }
        copy++;
        if (count == n) break;
    }
    *buffer = '\0';
    return count;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap)
{
    panic("Not implemented");
}

#endif
