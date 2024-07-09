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
#include <drv/scr/scr.h>
#include <cpu/gdt/gdt.h>
#include <cpu/idt/idt.h>
#include <heap/heap.h>
#include <pmm/pmm.h>
#include <mbt/mbt.h>
#include <drv/drv.h>
#include <lib/str.h>
#include <lib/io.h>
#include <krnl.h>

krnl_info_t krnl_info;

static drv_t scr_driver = {
    .name = "scr",
    .init = scr_init};

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

    drv_register(scr_driver);

    gdt_init();
    idt_init();

    pmm_init();
    paging_init();

    drv_init("scr");

    kheap_init();

    asm("sti");

    while (1)
        ;
}