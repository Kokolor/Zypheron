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
// #include <scr/scr.h>
#include <fs/tar.h>

struct tar_header *headers[32];
static struct tar_file files[32];
static unsigned int file_count = 0;

unsigned int tar_get_size(const char *in)
{
    unsigned int size = 0;
    unsigned int j;
    unsigned int count = 1;

    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);

    return size;
}

unsigned int tar_parse(unsigned int address)
{
    unsigned int i;

    for (i = 0;; i++)
    {
        struct tar_header *header = (struct tar_header *)address;

        if (header->filename[0] == '\0')
            break;

        unsigned int size = tar_get_size(header->size);

        headers[i] = header;

        strn_cpy(files[i].filename, header->filename, 100);
        files[i].size = size;
        files[i].start = address + 512;

        address += ((size / 512) + 1) * 512;

        if (size % 512)
            address += 512;
    }

    file_count = i;
    return i;
}

struct tar_file *tar_get_files(unsigned int *out_file_count)
{
    *out_file_count = file_count;
    return files;
}

struct tar_file *tar_open(const char *filename)
{
    for (unsigned int i = 0; i < file_count; i++)
    {
        if (strn_cmp(files[i].filename, filename, 100) == 0)
        {
            return &files[i];
        }
    }
    
    return NULL;
}

void tar_read(struct tar_file *file, void *buffer, unsigned int size)
{
    if (size > file->size)
    {
        size = file->size;
    }

    mem_cpy(buffer, (void *)file->start, size);
}