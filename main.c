/*

MIT No Attribution

Copyright (c) 2021 Mika Tuupola

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
#include <bitmap.h>
#include <hagl.h>
#include <font6x9.h>
#include <fps.h>
#include <aps.h>

static uint8_t current_demo = 1;
volatile bool fps_flag = false;
volatile bool switch_flag = false;
volatile bool flush_flag = true;
static fps_instance_t fps;
static aps_instance_t aps;
static uint32_t drawn = 0;
static hagl_backend_t *display;

wchar_t message[32];

static char primitive[19][32] = {
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
    "STRINGS"
};

void polygon_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x2 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y2 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x3 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y3 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x4 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y4 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};
    hagl_draw_polygon(display, 5, vertices, colour);
}

void fill_polygon_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x2 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y2 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x3 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y3 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x4 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y4 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};
    hagl_fill_polygon(display, 5, vertices, colour);
}

void circle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    uint16_t r = (rand() % 40);
    color_t colour = rand() % 0xffff;
    hagl_draw_circle(display, x0, y0, r, colour);
}

void fill_circle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    uint16_t r = (rand() % 40);
    color_t colour = rand() % 0xffff;
    hagl_fill_circle(display, x0, y0, r, colour);
}

void ellipse_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    uint16_t a = (rand() % 40) + 20;
    uint16_t b = (rand() % 40) + 20;
    color_t colour = rand() % 0xffff;
    hagl_draw_ellipse(display, x0, y0, a, b, colour);
}

void fill_ellipse_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    uint16_t a = (rand() % 40) + 20;
    uint16_t b = (rand() % 40) + 20;
    color_t colour = rand() % 0xffff;
    hagl_fill_ellipse(display, x0, y0, a, b, colour);
}

void line_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_draw_line(display, x0, y0, x1, y1, colour);
}

void vline_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t w = (rand() % DISPLAY_WIDTH + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_draw_vline(display, x0, y0, w, colour);
}


void hline_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t h = (rand() % DISPLAY_WIDTH + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_draw_vline(display, x0, y0, h, colour);
}

void rectangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_draw_rectangle(display, x0, y0, x1, y1, colour);
}

void fill_rectangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_fill_rectangle(display, x0, y0, x1, y1, colour);
}

void put_character_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;

    color_t colour = rand() % 0xffff;
    char ascii = rand() % 127;
    hagl_put_char(display, ascii, x0, y0, colour, font6x9);
}

void put_text_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 80;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;

    color_t colour = rand() % 0xffff;

    hagl_put_text(display, L"YO¡ MTV raps2♥", x0, y0, colour, font6x9);
}

void put_pixel_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_put_pixel(display, x0, y0, colour);
}

void triangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x2 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y2 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_draw_triangle(display, x0, y0, x1, y1, x2, y2, colour);
}

void fill_triangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x2 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y2 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_fill_triangle(display, x0, y0, x1, y1, x2, y2, colour);
}

void rgb_demo()
{
    uint16_t red = hagl_color(display, 255, 0, 0);
    uint16_t green = hagl_color(display, 0, 255, 0);
    uint16_t blue = hagl_color(display, 0, 0, 255);

    int16_t x0 = 0;
    int16_t x1 = DISPLAY_WIDTH / 3;
    int16_t x2 = 2 * x1;

    hagl_fill_rectangle(display, x0, 0, x1 - 1, DISPLAY_HEIGHT, red);
    hagl_fill_rectangle(display, x1, 0, x2 - 1, DISPLAY_HEIGHT, green);
    hagl_fill_rectangle(display, x2, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, blue);
}

void round_rectangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t r = rand() % 10;
    color_t colour = rand() % 0xffff;
    hagl_draw_rounded_rectangle(display, x0, y0, x1, y1, r, colour);
}

void fill_round_rectangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t r = rand() % 10;
    color_t colour = rand() % 0xffff;
    hagl_fill_rounded_rectangle(display, x0, y0, x1, y1, r, colour);
}

bool switch_timer_callback(struct repeating_timer *t) {
    switch_flag = true;
    return true;
}

bool fps_timer_callback(struct repeating_timer *t) {
    fps_flag = true;
    return true;
}

bool flush_timer_callback(struct repeating_timer *t) {
    flush_flag = true;
    return true;
}


int main()
{
    stdio_init_all();
    sleep_ms(5000);

    struct repeating_timer switch_timer;
    struct repeating_timer fps_timer;
    struct repeating_timer flush_timer;

    display = hagl_init();

    color_t red = hagl_color(display, 255, 0, 0);
    color_t green = hagl_color(display, 0, 255, 0);
    color_t blue = hagl_color(display, 0, 0, 255);

    printf("Hello world!\n");

    hagl_clear(display);
    hagl_set_clip_window(display, 0, 20, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 21);

    add_repeating_timer_ms(10000, switch_timer_callback, NULL, &switch_timer);
    add_repeating_timer_ms(1000, fps_timer_callback, NULL, &fps_timer);
    add_repeating_timer_ms(33, flush_timer_callback, NULL, &flush_timer);

    void (*demo[18]) ();

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

    fps_init(&fps);
    aps_init(&aps);

    while (1) {
        (*demo[current_demo])();
        drawn++;

#ifdef HAGL_HAL_USE_DOUBLE_BUFFER
        if (flush_flag) {
            flush_flag = 0;
            hagl_flush(display);
            fps_update(&fps);
        }
#endif /* HAGL_HAL_USE_DOUBLE_BUFFER */

        if (switch_flag) {
            switch_flag = 0;
            printf("%d %s per second, display %d FPS\r\n", (uint32_t)aps.current, primitive[current_demo], (uint32_t)fps.current);
            current_demo = (current_demo + 1) % 18;
            aps_reset(&aps);
            drawn = 0;
        }

        if (fps_flag) {
            fps_flag = 0;

            aps_update(&aps, drawn);
            drawn = 0;

            hagl_set_clip_window(display, 0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);

            /* Print the message on upper left corner. */
            swprintf(message,  sizeof(message), L"%.*f APS       ", 0, aps.current);
            hagl_put_text(display, message, 8, 4, green, font6x9);

#ifdef HAGL_HAL_USE_DOUBLE_BUFFER
            /* Print the message on lower right corner. */
            swprintf(message, sizeof(message), L"%.*f FPS  ", 0, fps.current);
            hagl_put_text(display, message, DISPLAY_WIDTH - 40, DISPLAY_HEIGHT - 14, green, font6x9);
#endif /* HAGL_HAL_USE_DOUBLE_BUFFER */

            hagl_set_clip_window(display, 0, 20, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 21);
        }
    }

    return 0;
}
