#include "debug.h"
#include <NDL.h>
#include <stdarg.h>
uint32_t SDL_GetTicks();

int SDL_Init(uint32_t flags)
{
    SDL_GetTicks();
    return NDL_Init(flags);
}

void SDL_Quit()
{
    NDL_Quit();
}

char *SDL_GetError()
{
    panic("Please implement me!");
    return "Navy does not support SDL_GetError()";
}

int SDL_SetError(const char *fmt, ...)
{
    //panic("Please implement me!");
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
                    putchar(*arg1);
                    arg1++;
                }
                break;
            case 'c':
                arg2 = va_arg(v, int);
                putchar(arg2);
                return 0;
            case 'd':
                arg2 = va_arg(v, int);
                char num_buffer[32];
                i = 0;
                if (arg2 == 0)
                    putchar('0');
                else
                {
                    while (arg2 > 0 && i < 32)
                    {
                        num_buffer[i++] = (arg2 % 10) + '0';
                        arg2 /= 10;
                    }
                }

                for (int j = i - 1; j >= 0; j--)
                    putchar(num_buffer[j]);
                break;
            case 'l':
                arg2 = va_arg(v, int);
                char num_buffer2[64];
                copy++;
                i = 0;
                if (arg2 == 0)
                    putchar('0');
                else
                {
                    while (arg2 > 0)
                    {
                        num_buffer2[i++] = (arg2 % 10) + '0';
                        arg2 /= 10;
                    }
                }

                for (int j = i - 1; j >= 0; j--)
                    putchar(num_buffer2[j]);
                break;
            case 'x':
                arg2 = va_arg(v, int);
                printf("here\n");
                if (arg2 == 0)
                {
                    putchar('0');
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
                        putchar(hex_buffer[j]);
                }
                break;
            case 'p':  // Handle pointer address
                arg3 = va_arg(v, void *);
                unsigned int ptr_val = (unsigned int)arg3; // Cast pointer to unsigned int
                if (ptr_val == 0)
                {
                    putchar('0');
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
                    putchar('0');
                    putchar('x');

                    // Print the hex string in reverse order
                    for (int j = i - 1; j >= 0; j--)
                        putchar(hex_buffer[j]);
                }
                break;
            }
        }
        else
        {
            putchar(*copy);
        }
        copy++;
    }
    return -1;
}

int SDL_ShowCursor(int toggle)
{
    panic("Please implement me!");
    return 0;
}

void SDL_WM_SetCaption(const char *title, const char *icon)
{
}
