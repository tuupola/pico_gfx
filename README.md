## Graphics speed tests for Raspberry Pi Pico

![Circles](https://appelsiini.net/img/2020/pod-draw-circle.png)


## Compile

Below instructions assume you do not have [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) installed yet.

```
$ git clone --recursive https://github.com/raspberrypi/pico-sdk.git
$ git clone --recursive https://github.com/tuupola/pico_gfx.git
$ cd pico_gfx/build
$ cmake ..
$ make -j8
```

If the SDK is already installed you can pass the installation path manually.

```
$ git clone --recursive https://github.com/tuupola/pico_gfx.git
$ cd pico_gfx/build
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

Below testing was done with Waveshare [RP2040-LCD-0.96](https://www.waveshare.com/wiki/RP2040-LCD-0.96). Buffered refresh rate was set to 30 frames per second. Number represents operations per seconsd ie. bigger number is better.

|                               | Single | Double    | Double DMA | Triple DMA |
|-------------------------------|--------|-----------|------------|------------|
| hagl_put_pixel()              | 111637 |    277118 |     314984 |     314989 |
| hagl_draw_line()              |   2495 |     15457 |      17564 |      17544 |
| hagl_draw_vline()             |  48253 |     69240 |      78676 |      78676 |
| hagl_draw_hline()             |  48221 |     69339 |      78793 |      78790 |
| hagl_draw_circle()            |   1891 |     12006 |      13671 |      13673 |
| hagl_fill_circle()            |   1401 |      8514 |       9696 |       9696 |
| hagl_draw_ellipse()           |   1283 |      7504 |       8528 |       8540 |
| hagl_fill_ellipse()           |    591 |      3646 |       4153 |       4144 |
| hagl_draw_triangle()          |    845 |      5296 |       6024 |       6029 |
| hagl_fill_triangle()          |    500 |       705 |        800 |        806 |
| hagl_draw_rectangle()         |  12383 |     21737 |      24794 |      24753 |
| hagl_fill_rectangle()         |   1324 |      9324 |      10582 |      10601 |
| hagl_draw_rounded_rectangle() |   4369 |     16706 |      18980 |      18991 |
| hagl_fill_rounded_rectangle() |   1202 |      8173 |       9282 |       9287 |
| hagl_draw_polygon()           |    514 |      3211 |       3653 |       3652 |
| hagl_fill_polygon()           |    299 |       423 |        472 |        474 |
| hagl_put_char()               |   4630 |     24728 |      28071 |      28082 |

## License

The MIT License (MIT). Please see [LICENSE](LICENSE) for more information.