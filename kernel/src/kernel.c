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

#include <screen.h>
#include <kernel.h>
#include <gdt.h>

kernel_info_t kernel_info;

void kernel_init(unsigned long addr)
{
    kernel_info.multiboot_info.info_ptr = addr;
}

void _init(unsigned long magic, unsigned long addr)
{
    (void)magic;

    kernel_init(addr);
    multiboot_parse(&kernel_info.multiboot_info);

    screen_init();

    screen_clear(0x1E1E1E);

    screen_draw_rect(0, 0, 1920, 8, 0x001AE5);
    screen_set_color(0xFFFFFF);
    screen_draw_str("Zypheron Kernel\n\n");

    gdt_init();
    
    screen_draw_str("GDT Initialized.\n");

    while (1)
        ;
}
