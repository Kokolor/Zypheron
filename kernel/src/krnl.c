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

#include <drv/scr/scr.h>
#include <cpu/gdt/gdt.h>
#include <cpu/idt/idt.h>
#include <pmm/pmm.h>
#include <mbt/mbt.h>
#include <drv/drv.h>
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

void print_memmap()
{
    struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)krnl_info.mbt_info.memmap;
    struct multiboot_mmap_entry *entry;

    scr_printf("Memory Map:\n");
    for (entry = mmap->entries;
         (uint8_t *)entry < (uint8_t *)mmap + mmap->size;
         entry = (struct multiboot_mmap_entry *)((uint8_t *)entry + mmap->entry_size))
    {
        scr_printf("Base: 0x%llx, Length: 0x%llx, Type: 0x%x\n", entry->addr, entry->len, entry->type);
    }
}

void krnl_main(unsigned long magic, unsigned long addr)
{
    (void)magic;

    krnl_init(addr);
    mbt_parse(&krnl_info.mbt_info);

    drv_register(scr_driver);
    drv_init("scr");

    gdt_init();
    idt_init();
    asm("sti");

    scr_color(0xFFFFFF);
    scr_printf("Hello, World!\n");

    print_memmap();

    pmm_init();

    while (1)
        ;
}