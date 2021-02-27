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
volatile bool switch_flag = true;
static float fx_fps;
static float fb_fps;
static uint32_t drawn = 0;

static bitmap_t *bb;
wchar_t message[32];

static char primitive[17][32] = {
    "RGB BARS",
    "PIXELS",
    "LINES",
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
    hagl_draw_polygon(5, vertices, colour);
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
    hagl_fill_polygon(5, vertices, colour);
}

void circle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    uint16_t r = (rand() % 40);
    color_t colour = rand() % 0xffff;
    hagl_draw_circle(x0, y0, r, colour);
}

void fill_circle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    uint16_t r = (rand() % 40);
    color_t colour = rand() % 0xffff;
    hagl_fill_circle(x0, y0, r, colour);
}

void ellipse_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    uint16_t a = (rand() % 40) + 20;
    uint16_t b = (rand() % 40) + 20;
    color_t colour = rand() % 0xffff;
    hagl_draw_ellipse(x0, y0, a, b, colour);
}

void fill_ellipse_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    uint16_t a = (rand() % 40) + 20;
    uint16_t b = (rand() % 40) + 20;
    color_t colour = rand() % 0xffff;
    hagl_fill_ellipse(x0, y0, a, b, colour);
}

void line_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_draw_line(x0, y0, x1, y1, colour);
}

void rectangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_draw_rectangle(x0, y0, x1, y1, colour);
}

void fill_rectangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_fill_rectangle(x0, y0, x1, y1, colour);
}

void put_character_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;

    color_t colour = rand() % 0xffff;
    char ascii = rand() % 127;
    hagl_put_char(ascii, x0, y0, colour, font6x9);
}

void put_text_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 80;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;

    color_t colour = rand() % 0xffff;

    hagl_put_text(L"YO¡ MTV raps2♥", x0, y0, colour, font6x9);
}

void put_pixel_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_put_pixel(x0, y0, colour);
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
    hagl_draw_triangle(x0, y0, x1, y1, x2, y2, colour);
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
    hagl_fill_triangle(x0, y0, x1, y1, x2, y2, colour);
}

void rgb_demo()
{
    uint16_t red = hagl_color(255, 0, 0);
    uint16_t green = hagl_color(0, 255, 0);
    uint16_t blue = hagl_color(0, 0, 255);

    int16_t x0 = 0;
    int16_t x1 = DISPLAY_WIDTH / 3;
    int16_t x2 = 2 * x1;

    hagl_fill_rectangle(x0, 0, x1 - 1, DISPLAY_HEIGHT, red);
    hagl_fill_rectangle(x1, 0, x2 - 1, DISPLAY_HEIGHT, green);
    hagl_fill_rectangle(x2, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, blue);
}

void round_rectangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t r = rand() % 10;
    color_t colour = rand() % 0xffff;
    hagl_draw_rounded_rectangle(x0, y0, x1, y1, r, colour);
}

void fill_round_rectangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t r = rand() % 10;
    color_t colour = rand() % 0xffff;
    hagl_fill_rounded_rectangle(x0, y0, x1, y1, r, colour);
}

bool switch_timer_callback(struct repeating_timer *t) {
    switch_flag = true;
    return true;
}

bool fps_timer_callback(struct repeating_timer *t) {
    fps_flag = true;
    return true;
}

int main()
{
    struct repeating_timer switch_timer;
    struct repeating_timer fps_timer;

    color_t red = hagl_color(255, 0, 0);
    color_t green = hagl_color(0, 255, 0);
    color_t blue = hagl_color(0, 0, 255);

    stdio_init_all();

    bb = hagl_init();
    if (bb) {
        printf("Back buffer: %dx%dx%d\r\n", bb->width, bb->height, bb->depth);
    } else {
        printf("No back buffer\r\n");
    }

    hagl_clear_screen();
    hagl_set_clip_window(0, 20, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 21);

    add_repeating_timer_ms(10000, switch_timer_callback, NULL, &switch_timer);
    add_repeating_timer_ms(1000, fps_timer_callback, NULL, &fps_timer);

    void (*demo[17]) ();

    demo[0] = rgb_demo;
    demo[1] = put_pixel_demo;
    demo[2] = line_demo;
    demo[3] = circle_demo;
    demo[4] = fill_circle_demo;
    demo[5] = ellipse_demo;
    demo[6] = fill_ellipse_demo;
    demo[7] = triangle_demo;
    demo[8] = fill_triangle_demo;
    demo[9] = rectangle_demo;
    demo[10] = fill_rectangle_demo;
    demo[11] = round_rectangle_demo;
    demo[12] = fill_round_rectangle_demo;
    demo[13] = polygon_demo;
    demo[14] = fill_polygon_demo;
    demo[15] = put_character_demo;
    demo[16] = put_text_demo;

    switch_flag = 1;

    while (1) {

        (*demo[current_demo])();
        drawn++;

        if (switch_flag) {
            switch_flag = 0;
            printf("%d %s per second, FB %d FPS\r\n", (uint32_t)fx_fps, primitive[current_demo], (uint32_t)fb_fps);
            current_demo = (current_demo + 1) % 17;
            fx_fps = aps(APS_RESET);
            drawn = 0;

        }

        if (fps_flag) {
            fps_flag = 0;

            fx_fps = aps(drawn);
            drawn = 0;

            swprintf(message,  sizeof(message), L"%.*f APS       ", 0, fx_fps);
            hagl_set_clip_window(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);

            color_t color = hagl_color(0, 255, 0);
            hagl_put_text(message, 8, 4, color, font6x9);
            hagl_set_clip_window(0, 20, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 21);
        }
    }

    return 0;
}
