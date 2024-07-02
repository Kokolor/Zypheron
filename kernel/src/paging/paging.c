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

uint32_t *pd0;
uint32_t *pt0;

uint32_t *paging_new_directory()
{
    uint32_t *pd = (uint32_t *)pmm_alloc_block();
    if (pd == NULL)
        return (uint32_t *)-1;

    for (int i = 0; i < 1024; i++)
    {
        pd[i] = pd0[i];
    }

    return pd;
}

void paging_switch_directory(uint32_t *directory)
{
    asm volatile("mov %0, %%cr3" ::"r"(directory));
}

void paging_init()
{
    uint32_t page_addr;
    int i, pg;
    
    pd0 = (uint32_t *)pmm_alloc_block();
    pt0 = (uint32_t *)pmm_alloc_block();

    pd0[0] = (uint32_t)pt0;
    pd0[0] |= 3;
    for (i = 1; i < 1024; i++)
        pd0[i] = 0;

    page_addr = 0;
    for (pg = 0; pg < 1024; pg++)
    {
        pt0[pg] = page_addr;
        pt0[pg] |= 3;
        page_addr += 4096;
    }

    asm("    mov %0, %%eax \n \
        mov %%eax, %%cr3 \n \
        mov %%cr0, %%eax \n \
        or %1, %%eax \n \
        mov %%eax, %%cr0" ::"m"(pd0),
        "i"(0x80000000));
}

void paging_map_page(void *phys_addr, void *virt_addr, uint32_t *pd)
{
    uint32_t pd_idx = (uint32_t)virt_addr >> 22;
    uint32_t pt_idx = ((uint32_t)virt_addr >> 12) & 0x03FF;
    uint32_t *pt;

    if (pd[pd_idx] & 1)
    {
        pt = (uint32_t *)(pd[pd_idx] & ~0xFFF);
    }
    else
    {
        pt = (uint32_t *)pmm_alloc_block();
        pd[pd_idx] = (uint32_t)pt | 3;
    }

    pt[pt_idx] = (uint32_t)phys_addr | 3;
    asm volatile("invlpg (%0)" ::"r"(virt_addr) : "memory");
}

void paging_unmap_page(void *virt_addr, uint32_t *pd)
{
    uint32_t pd_idx = (uint32_t)virt_addr >> 22;
    uint32_t pt_idx = ((uint32_t)virt_addr >> 12) & 0x03FF;
    uint32_t *pt;

    if (!(pd[pd_idx] & 1))
        return;

    pt = (uint32_t *)(pd[pd_idx] & ~0xFFF);
    pt[pt_idx] = 0;
    asm volatile("invlpg (%0)" ::"r"(virt_addr) : "memory");
}

void paging_map_range(void *phys_start_addr, void *virt_start_addr, size_t size, uint32_t *pd)
{
    size_t num_pages = (size + 0xFFF) / 0x1000;
    for (size_t i = 0; i < num_pages; i++)
    {
        void *phys_addr = (void *)((uintptr_t)phys_start_addr + i * 0x1000);
        void *virt_addr = (void *)((uintptr_t)virt_start_addr + i * 0x1000);
        paging_map_page(phys_addr, virt_addr, pd);
    }
}