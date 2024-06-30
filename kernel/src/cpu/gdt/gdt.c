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

extern void gdt_flush(void);

#include <gdt.h>

gdt_entry_t gdt[3];
gdt_ptr_t gdt_ptr;

void gdt_init(void)
{
    gdt[0] = (gdt_entry_t){0, 0, 0, 0, 0, 0};
    gdt[1] = (gdt_entry_t){0xFFFF, 0x0000, 0x00, 0x9A, 0xCF, 0x00};
    gdt[2] = (gdt_entry_t){0xFFFF, 0x0000, 0x00, 0x92, 0xCF, 0x00};

    gdt_ptr.limit = (sizeof(gdt) - 1);
    gdt_ptr.base = (uint32_t)&gdt;

    gdt_flush();
}