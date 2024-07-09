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

#include <paging/paging.h>
#include <cpu/gdt/gdt.h>
#include <cpu/idt/idt.h>
#include <heap/heap.h>
#include <scr/scr.h>
#include <pmm/pmm.h>
#include <mbt/mbt.h>
#include <lib/str.h>
#include <lib/io.h>
#include <fs/tar.h>
#include <krnl.h>

krnl_info_t krnl_info;

void krnl_init(unsigned long addr)
{
    krnl_info.mbt_info.info_ptr = addr;
}

void kheap_print_free_memory()
{
    size_t free_memory = kheap_get_free_memory();
    scr_printf("Free memory in heap: %zu MB\n", free_memory / (1024 * 1024));
}

void krnl_main(unsigned long magic, unsigned long addr)
{
    (void)magic;

    krnl_init(addr);
    mbt_parse(&krnl_info.mbt_info);

    gdt_init();
    idt_init();

    pmm_init();
    paging_init();

    scr_init();

    kheap_init();

    tar_parse(krnl_info.mbt_info.modules->mod_start);
    unsigned int file_count;
    struct tar_file *files = tar_get_files(&file_count);

    for (unsigned int i = 0; i < file_count; i++)
    {
        scr_printf("File: %s, Size: %u bytes\n", files[i].filename, files[i].size);
    }

    struct tar_file *file = tar_open("data/hello.txt");
    if (file)
    {
        char buffer[256];
        tar_read(file, buffer, sizeof(buffer) - 1);
        buffer[file->size < sizeof(buffer) - 1 ? file->size : sizeof(buffer) - 1] = '\0';
        scr_printf("Content of hello.txt: %s\n", buffer);
    }
    else
    {
        scr_printf("File hello.txt not found.\n");
    }

    asm("sti");

    while (1)
        ;
}