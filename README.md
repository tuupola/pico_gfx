## Graphics speed tests for Raspberry Pi Pico

![Circles](https://appelsiini.net/img/2020/pod-draw-circle.png)


## Compile

Below instructions assume you do not have [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) installed yet.

```
$ git clone --recursive https://github.com/raspberrypi/pico-sdk.git
$ git clone --recursive https://github.com/tuupola/pico_gfx.git
$ cd pico_gfx/build
$ cmake ..
$ make -j8
```

If the SDK is already installed you can pass the installation path manually.

```
$ git clone --recursive https://github.com/tuupola/pico_gfx.git
$ cd pico_gfx/build
$ cmake .. -DPICO_SDK_PATH=../pico-sdk
$ make -j8
```

You can then "flash" the firmware by putting the Pico into `BOOTSEL` mode and copying the uf2 file to the automatically mounted volume.

```
$ cp firmware.uf2 /run/media/<username>/RPI-RP2/
```

If you are using macOS the command would be the following.

```
$ cp firmware.uf2 /Volumes/RPI-RP2
```

Alternatively you can use picotool to flash the firmware.

```
$ picotool load firmware.uf2 -f
```

## Speed

Below testing was done with Waveshare [RP2040-GEEK (240x135 ST7789VW)](https://www.waveshare.com/wiki/RP2040-GEEK). Buffered refresh rate was set to 30 frames per second. Number represents operations per seconds ie. bigger number is better.

|                               | Single | Double    | Double DMA | Triple DMA |
|-------------------------------|--------|-----------|------------|------------|
| hagl_put_pixel()              | 185298 |    332931 |     474968 |     474985 |
| hagl_draw_line()              |   5058 |     21171 |      30235 |      30257 |
| hagl_draw_vline()             |  62700 |     71762 |     102313 |     102313 |
| hagl_draw_hline()             |  67611 |    204532 |     291288 |     291295 |
| hagl_draw_circle()            |   1281 |      6200 |       8839 |       8841 |
| hagl_fill_circle()            |    614 |      2977 |       4248 |       4246 |
| hagl_draw_ellipse()           |   1446 |      6861 |       9779 |       9779 |
| hagl_fill_ellipse()           |    725 |      3360 |       4792 |       4794 |
| hagl_draw_triangle()          |   2379 |      9958 |      14180 |      14176 |
| hagl_fill_triangle()          |   1112 |      1105 |       1576 |       1576 |
| hagl_draw_rectangle()         |  20446 |     38846 |      55441 |      55443 |
| hagl_fill_rectangle()         |   2834 |     15089 |      21489 |      21491 |
| hagl_draw_rounded_rectangle() |   4374 |     19375 |      27558 |      27562 |
| hagl_fill_rounded_rectangle() |   2276 |     10892 |      15510 |      15508 |
| hagl_draw_polygon()           |   1938 |      7848 |      11245 |      11250 |
| hagl_fill_polygon()           |    834 |       865 |       1234 |       1234 |
| hagl_put_char()               |   5702 |     21454 |      30609 |      30612 |

## License

MIT No Attribution (MIT-0). Please see [LICENSE](LICENSE) for more information.
