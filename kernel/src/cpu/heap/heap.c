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
#include <heap/heap.h>
#include <scr/scr.h>

static heap_block_t *heap_start = NULL;
static heap_block_t *free_list = NULL;

#define INITIAL_HEAP_SIZE (16 * 1024 * 1024)
#define HEAP_INCREMENT (4 * 1024 * 1024)

void kheap_init()
{
    heap_start = (heap_block_t *)pmm_alloc_block();
    if (!heap_start)
    {
        scr_printf("Heap init failed: unable to allocate initial block\n");
        return;
    }

    for (size_t i = 0; i < INITIAL_HEAP_SIZE; i += BLOCK_SIZE)
    {
        paging_map_page((void *)((char *)heap_start + i), (void *)((char *)heap_start + i), pd0);
    }

    heap_start->size = INITIAL_HEAP_SIZE - sizeof(heap_block_t);
    heap_start->next = NULL;
    heap_start->free = 1;
    free_list = heap_start;
    scr_printf("Heap initialized at %p with %d MB\n", heap_start, INITIAL_HEAP_SIZE / (1024 * 1024));
}

void *kheap_alloc(size_t size)
{
    if (size == 0)
        return NULL;

    size = (size + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);

    heap_block_t *current = free_list;
    heap_block_t *previous = NULL;

    while (current)
    {
        if (current->free && current->size >= size)
        {
            if (current->size > size + sizeof(heap_block_t))
            {
                heap_block_t *new_block = (heap_block_t *)((char *)current + sizeof(heap_block_t) + size);
                new_block->size = current->size - size - sizeof(heap_block_t);
                new_block->next = current->next;
                new_block->free = 1;
                current->next = new_block;
                current->size = size;
            }
            current->free = 0;
            scr_printf("Allocated %u bytes at %p\n", size, (void *)((char *)current + sizeof(heap_block_t)));
            return (void *)((char *)current + sizeof(heap_block_t));
        }
        previous = current;
        current = current->next;
    }

    size_t total_size = size + sizeof(heap_block_t);
    size_t num_blocks_needed = (total_size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    heap_block_t *new_block = NULL;
    for (size_t i = 0; i < num_blocks_needed; i++)
    {
        void *block = pmm_alloc_block();
        if (!block)
        {
            scr_printf("Heap allocation failed: no sufficient block found\n");
            return NULL;
        }
        paging_map_page(block, (void *)((char *)block), pd0);
        if (new_block == NULL)
        {
            new_block = (heap_block_t *)block;
            new_block->size = total_size - sizeof(heap_block_t);
            new_block->next = NULL;
            new_block->free = 0;
        }
        else
        {
            heap_block_t *current_block = (heap_block_t *)((char *)new_block + i * BLOCK_SIZE);
            current_block->next = (heap_block_t *)block;
            current_block->size = BLOCK_SIZE - sizeof(heap_block_t);
            current_block->free = 0;
        }
    }

    if (previous != NULL)
    {
        previous->next = new_block;
    }
    else
    {
        free_list = new_block;
    }

    scr_printf("Allocated new block of %u bytes at %p\n", size, (void *)((char *)new_block + sizeof(heap_block_t)));
    return (void *)((char *)new_block + sizeof(heap_block_t));
}

void kheap_free(void *ptr)
{
    if (!ptr)
        return;

    heap_block_t *block = (heap_block_t *)((char *)ptr - sizeof(heap_block_t));
    block->free = 1;

    heap_block_t *current = free_list;
    while (current)
    {
        if (current->free && current->next && current->next->free)
        {
            current->size += current->next->size + sizeof(heap_block_t);
            current->next = current->next->next;
        }
        current = current->next;
    }
}

size_t kheap_get_free_memory()
{
    size_t free_memory = 0;
    heap_block_t *current = free_list;
    while (current)
    {
        if (current->free)
        {
            free_memory += current->size;
        }
        current = current->next;
    }
    return free_memory;
}
