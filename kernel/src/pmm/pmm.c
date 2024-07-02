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

#include <pmm/pmm.h>
#include <mbt/mbt_api.h>
#include <drv/scr/scr.h>
#include <stddef.h>

static uint32_t *bitmap = NULL;
static size_t total_blocks;
static size_t used_blocks;

static inline void bitmap_set(size_t bit)
{
    bitmap[bit / 32] |= (1 << (bit % 32));
}

static inline void bitmap_unset(size_t bit)
{
    bitmap[bit / 32] &= ~(1 << (bit % 32));
}

static inline int bitmap_test(size_t bit)
{
    return bitmap[bit / 32] & (1 << (bit % 32));
}

void pmm_init()
{
    struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)krnl_info.mbt_info.memmap;
    struct multiboot_mmap_entry *entry;

    size_t mem_bytes = 0;

    for (entry = mmap->entries;
         (uint8_t *)entry < (uint8_t *)mmap + mmap->size;
         entry = (struct multiboot_mmap_entry *)((uint8_t *)entry + mmap->entry_size))
    {
        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            mem_bytes += entry->len;
        }
    }

    total_blocks = mem_bytes / BLOCK_SIZE;
    used_blocks = total_blocks;

    bitmap = (uint32_t *)((char *)krnl_info.mbt_info.info_ptr + 0x1000);
    size_t bitmap_size = total_blocks / 32;
    for (size_t i = 0; i < bitmap_size; i++)
    {
        bitmap[i] = 0xFFFFFFFF;
    }

    for (entry = mmap->entries;
         (uint8_t *)entry < (uint8_t *)mmap + mmap->size;
         entry = (struct multiboot_mmap_entry *)((uint8_t *)entry + mmap->entry_size))
    {
        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            size_t start_block = entry->addr / BLOCK_SIZE;
            size_t blocks = entry->len / BLOCK_SIZE;
            for (size_t i = 0; i < blocks; i++)
            {
                bitmap_unset(start_block + i);
                used_blocks--;
            }
        }
    }

    scr_printf("PMM Initialized: %u blocks (%u MB) available\n", total_blocks - used_blocks, (total_blocks - used_blocks) * BLOCK_SIZE / (1024 * 1024));
}

void *pmm_alloc_block()
{
    for (size_t i = 0; i < total_blocks / 32; i++)
    {
        if (bitmap[i] != 0xFFFFFFFF)
        {
            for (int j = 0; j < 32; j++)
            {
                int bit = 1 << j;
                if (!(bitmap[i] & bit))
                {
                    bitmap_set(i * 32 + j);
                    used_blocks++;
                    return (void *)((i * 32 + j) * BLOCK_SIZE);
                }
            }
        }
    }

    scr_printf("PMM: Out of memory\n");
    return NULL;
}

void pmm_free_block(void *p)
{
    size_t block = (size_t)p / BLOCK_SIZE;
    if (bitmap_test(block))
    {
        bitmap_unset(block);
        used_blocks--;
    }
    else
    {
        scr_printf("PMM: Double free detected\n");
    }
}