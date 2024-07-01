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

#pragma once

#include <stdint.h>
#include <krnl.h>

typedef struct
{
    uint32_t *framebuffer_addr;
    uint32_t framebuffer_height;
    uint32_t framebuffer_width;
    uint32_t framebuffer_pitch;
    uint32_t text_cursor_x;
    uint32_t text_cursor_y;
    uint32_t color;
} scr_info_t;

extern scr_info_t scr_info;

void scr_init(void);
void scr_pixel(uint32_t x, uint32_t y, uint32_t color);
void scr_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void scr_clear(uint32_t color);
void scr_write(char *buf, unsigned int len);
void scr_hex(uint32_t value);
void scr_color(uint32_t color);
