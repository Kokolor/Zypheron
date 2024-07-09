/*
 * MIT License
 *
 * Copyright (c) 2024 Kokolor
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <lib/str.h>

size_t str_len(char *str)
{
    size_t i = 0;

    while (str[i] != '\0')
    {
        i++;
    }

    return i;
}

int str_cmp(const char *first, const char *second)
{
    while (*first && *second)
    {
        if (*first != *second)
        {
            return *first - *second;
        }

        first++;
        second++;
    }

    return *first - *second;
}

int strn_cmp(const char *s1, const char *s2, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (s1[i] != s2[i])
        {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
        if (s1[i] == '\0')
        {
            return 0;
        }
    }

    return 0;
}

char *strn_cpy(char *dest, const char *src, size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }

    for (; i < n; i++)
    {
        dest[i] = '\0';
    }

    return dest;
}

void *mem_cpy(char *dst, char *src, int n)
{
    char *p = dst;
    while (n--)
        *dst++ = *src++;
    return p;
}

void *mem_set(void *ptr, int value, size_t num)
{
    unsigned char *p = ptr;
    while (num--)
    {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

void reverse(char *str, int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char *itoa(int value, char *str, int base)
{
    int i = 0;
    int is_negative = 0;

    if (value == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (value < 0 && base == 10)
    {
        is_negative = 1;
        value = -value;
    }

    while (value != 0)
    {
        int rem = value % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value = value / base;
    }

    if (is_negative)
        str[i++] = '-';

    str[i] = '\0';

    reverse(str, i);

    return str;
}

char *utoa(unsigned int value, char *str, int base)
{
    int i = 0;

    if (value == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    while (value != 0)
    {
        int rem = value % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value = value / base;
    }

    str[i] = '\0';

    reverse(str, i);

    return str;
}
