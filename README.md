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
| hagl_put_pixel()              | 117261 |    364294 |     364294 |     365326 |
| hagl_draw_line()              |   2512 |     15655 |      17723 |      17794 |
| hagl_draw_vline()             |  49632 |     72712 |      82412 |      82509 |
| hagl_draw_hline()             |  49626 |     72637 |      82305 |      82497 |
| hagl_draw_circle()            |   1912 |     13519 |      15345 |      15371 |
| hagl_fill_circle()            |   1420 |      8571 |       9701 |       9744 |
| hagl_draw_ellipse()           |   1273 |      7565 |       8543 |       8563 |
| hagl_fill_ellipse()           |    596 |      3666 |       4153 |       4167 |
| hagl_draw_triangle()          |    852 |      5348 |       6045 |       6080 |
| hagl_fill_triangle()          |    501 |       699 |        806 |        806 |
| hagl_draw_rectangle()         |  12517 |     22244 |      25103 |      25244 |
| hagl_fill_rectangle()         |   1319 |      9406 |      10699 |      10712 |
| hagl_draw_rounded_rectangle() |   4401 |     16984 |      19214 |      19288 |
| hagl_fill_rounded_rectangle() |   1186 |      8223 |       9307 |       9347 |
| hagl_draw_polygon()           |    507 |      3235 |       3661 |       3678 |
| hagl_fill_polygon()           |    302 |       421 |        476 |        486 |
| hagl_put_char()               |   4658 |     25170 |      28534 |      28443 |

## License

MIT No Attribution (MIT-0). Please see [LICENSE](LICENSE) for more information.