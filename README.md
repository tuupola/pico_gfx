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
| hagl_put_pixel()              | 164414 |    264121 |     376766 |     376792 |
| hagl_draw_line()              |   2147 |      9530 |      13581 |      13592 |
| hagl_draw_vline()             |  46901 |     46872 |      66677 |      66787 |
| hagl_draw_hline()             |  40187 |    124997 |     178087 |     178208 |
| hagl_draw_circle()            |   2183 |     10943 |      15636 |      15603 |
| hagl_fill_circle()            |   1481 |      6690 |       9463 |       9496 |
| hagl_draw_ellipse()           |   1319 |      6037 |       8586 |       8595 |
| hagl_fill_ellipse()           |    627 |      2920 |       4169 |       4137 |
| hagl_draw_triangle()          |    711 |      3246 |       4617 |       4625 |
| hagl_fill_triangle()          |    466 |       631 |        890 |        892 |
| hagl_draw_rectangle()         |  11811 |     20342 |      28965 |      29011 |
| hagl_fill_rectangle()         |    854 |      4806 |       6901 |       6916 |
| hagl_draw_rounded_rectangle() |   5307 |     15175 |      21629 |      21625 |
| hagl_fill_rounded_rectangle() |    771 |      4227 |       6026 |       6063 |
| hagl_draw_polygon()           |    428 |      1964 |       2792 |       2788 |
| hagl_fill_polygon()           |    283 |       367 |        527 |        528 |
| hagl_put_char()               |   5445 |     20946 |      29861 |      29870 |

## License

MIT No Attribution (MIT-0). Please see [LICENSE](LICENSE) for more information.
