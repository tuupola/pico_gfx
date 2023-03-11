/*

MIT No Attribution

Copyright (c) 2021-2023 Mika Tuupola

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

SPDX-License-Identifier: MIT-0

*/

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <pico/stdlib.h>

#include <sys/time.h>

#include <hagl_hal.h>
#include <hagl/bitmap.h>
#include <hagl.h>
#include <font6x9.h>
#include <fps.h>
#include <aps.h>

#include "xoroshiro64star.h"

static uint8_t current_demo = 1;
volatile bool fps_flag = false;
volatile bool switch_flag = false;
volatile bool flush_flag = true;
static fps_instance_t fps;
static aps_instance_t aps;
static uint32_t drawn = 0;
static hagl_backend_t *display;
static hagl_bitmap_t glyph;

wchar_t message[32];

static char primitive[20][32] = {
    "RGB BARS",
    "PIXELS",
    "LINES",
    "VERTICAL LINES",
    "HORIZONTAL LINES",
    "CIRCLES",
    "FILLED CIRCLES",
    "ELLIPSES",
    "FILLED ELLIPSES",
    "TRIANGLES",
    "FILLED TRIANGLES",
    "RECTANGLES",
    "FILLED RECTANGLES",
    "ROUND RECTANGLES",
    "FILLED ROUND RECTANGLES",
    "POLYGONS",
    "FILLED POLYGONS",
    "CHARACTERS",
    "STRINGS",
    "SCALED CHARACTERS"
};

#include <malloc.h>

size_t
total_heap() {
    extern char __StackLimit, __bss_end__;

    return &__StackLimit  - &__bss_end__;
}

size_t
free_heap(void) {
    struct mallinfo m = mallinfo();

    return total_heap() - m.uordblks;
}

void
polygon_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    int16_t x1 = (fast_rand() % display->width + 20) - 20;
    int16_t y1 = (fast_rand() % display->height + 20) - 20;
    int16_t x2 = (fast_rand() % display->width + 20) - 20;
    int16_t y2 = (fast_rand() % display->height + 20) - 20;
    int16_t x3 = (fast_rand() % display->width + 20) - 20;
    int16_t y3 = (fast_rand() % display->height + 20) - 20;
    int16_t x4 = (fast_rand() % display->width + 20) - 20;
    int16_t y4 = (fast_rand() % display->height + 20) - 20;
    hagl_color_t colour = fast_rand() % 0xffff;
    int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};
    hagl_draw_polygon(display, 5, vertices, colour);
}

void
fill_polygon_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    int16_t x1 = (fast_rand() % display->width + 20) - 20;
    int16_t y1 = (fast_rand() % display->height + 20) - 20;
    int16_t x2 = (fast_rand() % display->width + 20) - 20;
    int16_t y2 = (fast_rand() % display->height + 20) - 20;
    int16_t x3 = (fast_rand() % display->width + 20) - 20;
    int16_t y3 = (fast_rand() % display->height + 20) - 20;
    int16_t x4 = (fast_rand() % display->width + 20) - 20;
    int16_t y4 = (fast_rand() % display->height + 20) - 20;
    hagl_color_t colour = fast_rand() % 0xffff;
    int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};
    hagl_fill_polygon(display, 5, vertices, colour);
}

void
circle_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    uint16_t r = (fast_rand() % 40);
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_draw_circle(display, x0, y0, r, colour);
}

void
fill_circle_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    uint16_t r = (fast_rand() % 40);
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_fill_circle(display, x0, y0, r, colour);
}

void
ellipse_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    uint16_t a = (fast_rand() % 40) + 20;
    uint16_t b = (fast_rand() % 40) + 20;
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_draw_ellipse(display, x0, y0, a, b, colour);
}

void
fill_ellipse_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    uint16_t a = (fast_rand() % 40) + 20;
    uint16_t b = (fast_rand() % 40) + 20;
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_fill_ellipse(display, x0, y0, a, b, colour);
}

void
line_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    int16_t x1 = (fast_rand() % display->width + 20) - 20;
    int16_t y1 = (fast_rand() % display->height + 20) - 20;
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_draw_line(display, x0, y0, x1, y1, colour);
}

void
vline_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    int16_t w = (fast_rand() % display->width + 20) - 20;
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_draw_vline(display, x0, y0, w, colour);
}

void
hline_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    int16_t h = (fast_rand() % display->width + 20) - 20;
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_draw_hline(display, x0, y0, h, colour);
}

void
rectangle_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    int16_t x1 = (fast_rand() % display->width + 20) - 20;
    int16_t y1 = (fast_rand() % display->height + 20) - 20;
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_draw_rectangle(display, x0, y0, x1, y1, colour);
}

void
fill_rectangle_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    int16_t x1 = (fast_rand() % display->width + 20) - 20;
    int16_t y1 = (fast_rand() % display->height + 20) - 20;
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_fill_rectangle(display, x0, y0, x1, y1, colour);
}

void
put_character_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;

    hagl_color_t colour = fast_rand() % 0xffff;
    char ascii = fast_rand() % 127;
    hagl_put_char(display, ascii, x0, y0, colour, font6x9);
}

void
scaled_character_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    hagl_color_t colour = fast_rand() % 0xffff;
    char ascii = fast_rand() % 127;

    if (0 == hagl_get_glyph(display, ascii, colour, &glyph, font6x9)) {
        hagl_blit_xywh(display, x0, y0, 24, 36, &glyph);
    }
}

void
put_text_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 80;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;

    hagl_color_t colour = fast_rand() % 0xffff;

    hagl_put_text(display, L"YO¡ MTV raps2♥", x0, y0, colour, font6x9);
}

void
put_pixel_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_put_pixel(display, x0, y0, colour);
}

void
triangle_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    int16_t x1 = (fast_rand() % display->width + 20) - 20;
    int16_t y1 = (fast_rand() % display->height + 20) - 20;
    int16_t x2 = (fast_rand() % display->width + 20) - 20;
    int16_t y2 = (fast_rand() % display->height + 20) - 20;
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_draw_triangle(display, x0, y0, x1, y1, x2, y2, colour);
}

void
fill_triangle_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    int16_t x1 = (fast_rand() % display->width + 20) - 20;
    int16_t y1 = (fast_rand() % display->height + 20) - 20;
    int16_t x2 = (fast_rand() % display->width + 20) - 20;
    int16_t y2 = (fast_rand() % display->height + 20) - 20;
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_fill_triangle(display, x0, y0, x1, y1, x2, y2, colour);
}

void
rgb_demo()
{
    uint16_t red = hagl_color(display, 255, 0, 0);
    uint16_t green = hagl_color(display, 0, 255, 0);
    uint16_t blue = hagl_color(display, 0, 0, 255);

    int16_t x0 = 0;
    int16_t x1 = display->width / 3;
    int16_t x2 = 2 * x1;

    hagl_fill_rectangle(display, x0, 0, x1 - 1, display->height, red);
    hagl_fill_rectangle(display, x1, 0, x2 - 1, display->height, green);
    hagl_fill_rectangle(display, x2, 0, display->width, display->height, blue);
}

void
round_rectangle_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    int16_t x1 = (fast_rand() % display->width + 20) - 20;
    int16_t y1 = (fast_rand() % display->height + 20) - 20;
    int16_t r = fast_rand() % 10;
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_draw_rounded_rectangle(display, x0, y0, x1, y1, r, colour);
}

void
fill_round_rectangle_demo()
{
    int16_t x0 = (fast_rand() % display->width + 20) - 20;
    int16_t y0 = (fast_rand() % display->height + 20) - 20;
    int16_t x1 = (fast_rand() % display->width + 20) - 20;
    int16_t y1 = (fast_rand() % display->height + 20) - 20;
    int16_t r = fast_rand() % 10;
    hagl_color_t colour = fast_rand() % 0xffff;
    hagl_fill_rounded_rectangle(display, x0, y0, x1, y1, r, colour);
}

bool
switch_timer_callback(struct repeating_timer *t)
{
    switch_flag = true;
    return true;
}

bool
fps_timer_callback(struct repeating_timer *t)
{
    fps_flag = true;
    return true;
}

bool
flush_timer_callback(struct repeating_timer *t)
{
    flush_flag = true;
    return true;
}


int
main()
{
    stdio_init_all();
    sleep_ms(5000);

    printf("[main] %d total heap \r\n", total_heap());
    printf("[main] %d free heap \r\n", free_heap());

    struct repeating_timer switch_timer;
    struct repeating_timer fps_timer;
    struct repeating_timer flush_timer;

    display = hagl_init();

    hagl_color_t red = hagl_color(display, 255, 0, 0);
    hagl_color_t green = hagl_color(display, 0, 255, 0);
    hagl_color_t blue = hagl_color(display, 0, 0, 255);

    hagl_bitmap_init(&glyph, 6, 9, display->depth, NULL);
    glyph.buffer = (uint8_t *) malloc(glyph.size);

    hagl_clear(display);
    hagl_set_clip(display, 0, 20, display->width - 1, display->height - 21);

    add_repeating_timer_ms(10000, switch_timer_callback, NULL, &switch_timer);
    add_repeating_timer_ms(1000, fps_timer_callback, NULL, &fps_timer);
    add_repeating_timer_ms(33, flush_timer_callback, NULL, &flush_timer);

    void (*demo[20]) ();

    demo[0] = rgb_demo;
    demo[1] = put_pixel_demo;
    demo[2] = line_demo;
    demo[3] = vline_demo;
    demo[4] = hline_demo;
    demo[5] = circle_demo;
    demo[6] = fill_circle_demo;
    demo[7] = ellipse_demo;
    demo[8] = fill_ellipse_demo;
    demo[9] = triangle_demo;
    demo[10] = fill_triangle_demo;
    demo[11] = rectangle_demo;
    demo[12] = fill_rectangle_demo;
    demo[13] = round_rectangle_demo;
    demo[14] = fill_round_rectangle_demo;
    demo[15] = polygon_demo;
    demo[16] = fill_polygon_demo;
    demo[17] = put_character_demo;
    demo[18] = put_text_demo;
    demo[19] = scaled_character_demo;

    fps_init(&fps);
    aps_init(&aps);

    printf("[main] %d free heap \r\n", free_heap());

    while (1) {
        (*demo[current_demo])();
        drawn++;

#ifdef HAGL_HAS_HAL_BACK_BUFFER
        if (flush_flag) {
            flush_flag = 0;
            hagl_flush(display);
            fps_update(&fps);
        }
#endif /* HAGL_HAS_HAL_BACK_BUFFER */

        if (switch_flag) {
            switch_flag = 0;
            printf("[main] %d %s per second, display %d fps, %d free heap\r\n", (uint32_t)aps.current, primitive[current_demo], (uint32_t)fps.current,  free_heap());
            current_demo = (current_demo + 1) % 20;
            aps_reset(&aps);
            drawn = 0;
        }

        if (fps_flag) {
            fps_flag = 0;

            aps_update(&aps, drawn);
            drawn = 0;

            hagl_set_clip(display, 0, 0, display->width - 1, display->height - 1);

            /* Print the message on upper left corner. */
            swprintf(message,  sizeof(message), L"%.*f APS       ", 0, aps.current);
            hagl_put_text(display, message, 8, 4, green, font6x9);

#ifdef HAGL_HAS_HAL_BACK_BUFFER
            /* Print the message on lower right corner. */
            swprintf(message, sizeof(message), L"%.*f FPS  ", 0, fps.current);
            hagl_put_text(display, message, display->width - 40, display->height - 14, green, font6x9);
#endif /* HAGL_HAS_HAL_BACK_BUFFER */

            hagl_set_clip(display, 0, 20, display->width - 1, display->height - 21);
        }
    }

    return 0;
}
