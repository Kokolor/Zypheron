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

#include <lib/io.h>
#include <lib/str.h>
#include <drv/scr/scr.h>
#include <cpu/idt/idt.h>

extern void isr_0(void);
extern void isr_1(void);
extern void isr_2(void);
extern void isr_3(void);
extern void isr_4(void);
extern void isr_5(void);
extern void isr_6(void);
extern void isr_7(void);
extern void isr_8(void);
extern void isr_9(void);
extern void isr_10(void);
extern void isr_11(void);
extern void isr_12(void);
extern void isr_13(void);
extern void isr_14(void);
extern void isr_15(void);
extern void isr_16(void);
extern void isr_17(void);
extern void isr_18(void);
extern void isr_19(void);
extern void isr_20(void);
extern void isr_21(void);
extern void isr_22(void);
extern void isr_23(void);
extern void isr_24(void);
extern void isr_25(void);
extern void isr_26(void);
extern void isr_27(void);
extern void isr_28(void);
extern void isr_29(void);
extern void isr_30(void);
extern void isr_31(void);
extern void isr_32(void);
extern void isr_33(void);

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idt_ptr;

void idt_set_entry(uint16_t sel, uint32_t base, uint16_t type, idt_entry_t *entry)
{
    entry->base_low = (base & 0xffff);
    entry->sel = sel;
    entry->type = type;
    entry->base_high = (base & 0xffff0000) >> 16;

    return;
}

void isr_handler(uint32_t int_no)
{
    const char *errors[] = {
        "Division by Zero",
        "Debug",
        "Non Maskable Interrupt",
        "Breakpoint",
        "Into Detected Overflow",
        "Out of Bounds",
        "Invalid Opcode",
        "No Coprocessor",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection Fault",
        "Page Fault",
        "Unknown Interrupt",
        "Coprocessor Fault",
        "Alignment Check",
        "Machine Check",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
    };

    if (int_no < 32)
    {
        scr_printf("Kernel Panic: %s", errors[int_no]);
        while(1)
        {
            asm volatile("cli; hlt");
        }
    }
    else if (int_no == 32)
    {
        static int tic = 0;
        static int sec = 0;
        tic++;
        if (tic % 50 == 0)
        {
            sec++;
            tic = 0;
            scr_printf("Clock!\n");
        }
    }
    else if (int_no == 33)
    {
        uint8_t scancode = inb(0x60);
        scr_printf("%x\n", scancode);
    }
}

void idt_init(void)
{
    idt_set_entry(0x08, (uint32_t)isr_0, 0x8E00, &idt[0]);
    idt_set_entry(0x08, (uint32_t)isr_1, 0x8E00, &idt[1]);
    idt_set_entry(0x08, (uint32_t)isr_2, 0x8E00, &idt[2]);
    idt_set_entry(0x08, (uint32_t)isr_3, 0x8E00, &idt[3]);
    idt_set_entry(0x08, (uint32_t)isr_4, 0x8E00, &idt[4]);
    idt_set_entry(0x08, (uint32_t)isr_5, 0x8E00, &idt[5]);
    idt_set_entry(0x08, (uint32_t)isr_6, 0x8E00, &idt[6]);
    idt_set_entry(0x08, (uint32_t)isr_7, 0x8E00, &idt[7]);
    idt_set_entry(0x08, (uint32_t)isr_8, 0x8E00, &idt[8]);
    idt_set_entry(0x08, (uint32_t)isr_9, 0x8E00, &idt[9]);
    idt_set_entry(0x08, (uint32_t)isr_10, 0x8E00, &idt[10]);
    idt_set_entry(0x08, (uint32_t)isr_11, 0x8E00, &idt[11]);
    idt_set_entry(0x08, (uint32_t)isr_12, 0x8E00, &idt[12]);
    idt_set_entry(0x08, (uint32_t)isr_13, 0x8E00, &idt[13]);
    idt_set_entry(0x08, (uint32_t)isr_14, 0x8E00, &idt[14]);
    idt_set_entry(0x08, (uint32_t)isr_15, 0x8E00, &idt[15]);
    idt_set_entry(0x08, (uint32_t)isr_16, 0x8E00, &idt[16]);
    idt_set_entry(0x08, (uint32_t)isr_17, 0x8E00, &idt[17]);
    idt_set_entry(0x08, (uint32_t)isr_18, 0x8E00, &idt[18]);
    idt_set_entry(0x08, (uint32_t)isr_19, 0x8E00, &idt[19]);
    idt_set_entry(0x08, (uint32_t)isr_20, 0x8E00, &idt[20]);
    idt_set_entry(0x08, (uint32_t)isr_21, 0x8E00, &idt[21]);
    idt_set_entry(0x08, (uint32_t)isr_22, 0x8E00, &idt[22]);
    idt_set_entry(0x08, (uint32_t)isr_23, 0x8E00, &idt[23]);
    idt_set_entry(0x08, (uint32_t)isr_24, 0x8E00, &idt[24]);
    idt_set_entry(0x08, (uint32_t)isr_25, 0x8E00, &idt[25]);
    idt_set_entry(0x08, (uint32_t)isr_26, 0x8E00, &idt[26]);
    idt_set_entry(0x08, (uint32_t)isr_27, 0x8E00, &idt[27]);
    idt_set_entry(0x08, (uint32_t)isr_28, 0x8E00, &idt[28]);
    idt_set_entry(0x08, (uint32_t)isr_29, 0x8E00, &idt[29]);
    idt_set_entry(0x08, (uint32_t)isr_30, 0x8E00, &idt[30]);
    idt_set_entry(0x08, (uint32_t)isr_31, 0x8E00, &idt[31]);
    idt_set_entry(0x08, (uint32_t)isr_32, 0x8E00, &idt[32]);
    idt_set_entry(0x08, (uint32_t)isr_33, 0x8E00, &idt[33]);

    idt_ptr.base = (uintptr_t)&idt[0];
    idt_ptr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;

    mem_cpy((char *)idt_ptr.base, (char *)idt, idt_ptr.limit);

    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x70);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    asm("lidtl (idt_ptr)");
}
