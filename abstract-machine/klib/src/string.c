#include <klib-macros.h>
#include <klib.h>
#include <stdint.h>
#define __NATIVE_USE_KLIB__
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


size_t strlen(const char *s)
{
    size_t len = 0;

    while (s[len] != '\0')
        len++;

    return len;
}

char *strcpy(char *dst, const char *src)
{
    int ptr = 0;

    while (*(src + ptr) != '\0')
    {
        *(dst + ptr) = *(src + ptr);
        ptr++;
    }
    *(dst + ptr) = '\0';

    return dst;
}

char *strncpy(char *dst, const char *src, size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++)
        dst[i] = src[i];
    for (; i < n; i++)
        dst[i] = '\0';

    return dst;
}

char *strcat(char *dst, const char *src)
{
    int dst_ptr = 0;
    int src_ptr = 0;

    while (dst[dst_ptr] != '\0')
        dst_ptr++;
    while (src[src_ptr] != '\0')
        dst[dst_ptr++] = src[src_ptr++];
    dst[dst_ptr] = '\0';

    return dst;
}

int strcmp(const char *s1, const char *s2)
{
    int s1_ptr = 0;
    int s2_ptr = 0;

    while (s1[s1_ptr] != '\0' && s2[s2_ptr] != '\0')
    {
        if (s1[s1_ptr] > s2[s2_ptr])
            return 1;
        else if (s1[s1_ptr] < s2[s2_ptr])
            return -1;
        s1_ptr++;
        s2_ptr++;
    }
    if (s1[s1_ptr] == '\0' && s2[s2_ptr] == '\0')
        return 0;
    else if (s1[s1_ptr] == '\0')
        return -1;
    else
        return 1;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;
    for (i = 0; i < n && s1[i] != '\0' && s2[i] != '\0'; i++)
    {
        if (s1[i] > s2[i])
            return 1;
        else if (s1[i] < s2[i])
            return -1;
    }
    if (s1[i] == s2[i])
        return 0;
    else if (s1[i] == '\0')
        return -1;
    else
        return 1;
}

void *memset(void *s, int c, size_t n)
{
    size_t i;
    for (i = 0; i < n; i++)
    {
        ((char *)s)[i] = c;
    }
    return s;
}

void *memmove(void *dst, const void *src, size_t n)
{
    size_t i;
    if ((uintptr_t)src < (uintptr_t)dst && (uintptr_t)src + n > (uintptr_t)dst)
    {
        // Copy from back to front
        for (i = n - 1; i >= 0; i--)
        {
            ((char *)dst)[i] = ((char *)src)[i];
        }
        ((char *)dst)[n] = '\0';
    }
    else if ((uintptr_t)dst < (uintptr_t)src && (uintptr_t)dst + n > (uintptr_t)src)
    {
        // Copy from front to back
        for (i = 0; i < n; i++)
        {
            ((char *)dst)[i] = ((char *)src)[i];
        }
        ((char *)dst)[n] = '\0';
    }
    else
    {
        // They don't overlap for sure
        memcpy(dst, src, n);
    }

    return dst;
}

void *memcpy(void *out, const void *in, size_t n)
{
    size_t i;
    for (i = 0; i < n && ((char *)in)[i] != '\0'; i++)
    {
        ((char *)out)[i] = ((char *)in)[i];
    }
    ((char *)out)[i] = '\0';

    return out;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    size_t i;
    for (i = 0; i < n; i++)
    {
        if (*(unsigned char *)(s1 + i) > *(unsigned char *)(s2 + i))
            return 1;
        else if (*(unsigned char *)(s1 + i) < *(unsigned char *)(s2 + i))
            return -1;
    }
    return 0;
}

#endif
