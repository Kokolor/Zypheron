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

screen_info_t screen_info;

void screen_init()
{
    screen_info.framebuffer_addr = (uint32_t *)(uintptr_t)kernel_info.multiboot_info.framebuffer->common.framebuffer_addr;
    screen_info.framebuffer_width = kernel_info.multiboot_info.framebuffer->common.framebuffer_width;
    screen_info.framebuffer_height = kernel_info.multiboot_info.framebuffer->common.framebuffer_height;
    screen_info.framebuffer_pitch = kernel_info.multiboot_info.framebuffer->common.framebuffer_pitch / 4;
}

void screen_clear(uint32_t color)
{
    for (uint32_t y = 0; y < screen_info.framebuffer_height; y++)
    {
        for (uint32_t x = 0; x < screen_info.framebuffer_width; x++)
        {
            screen_info.framebuffer_addr[y * screen_info.framebuffer_pitch + x] = color;
        }
    }
}
