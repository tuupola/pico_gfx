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

|                               | Single | Double | Double DMA | Triple DMA |
| ----------------------------- | ------ | ------ | ---------- | ---------- |
| hagl_put_pixel()              | 186326 | 337673 | 481708     | 481702     |
| hagl_draw_line()              | 4990   | 27100  | 38652      | 38609      |
| hagl_draw_vline()             | 62356  | 71148  | 101373     | 101369     |
| hagl_draw_hline()             | 67192  | 199311 | 283852     | 283853     |
| hagl_draw_circle()            | 1376   | 7286   | 10389      | 10390      |
| hagl_fill_circle()            | 757    | 3534   | 5051       | 5053       |
| hagl_draw_ellipse()           | 1463   | 7295   | 10395      | 10394      |
| hagl_fill_ellipse()           | 721    | 3214   | 4587       | 4589       |
| hagl_draw_triangle()          | 2430   | 12614  | 18014      | 18016      |
| hagl_fill_triangle()          | 2683   | 6533   | 9308       | 9311       |
| hagl_draw_rectangle()         | 20303  | 38168  | 54382      | 54380      |
| hagl_fill_rectangle()         | 2825   | 15102  | 21505      | 21502      |
| hagl_draw_rounded_rectangle() | 4429   | 20751  | 29599      | 29600      |
| hagl_fill_rounded_rectangle() | 2245   | 10517  | 14964      | 14960      |
| hagl_draw_polygon()           | 2001   | 9810   | 14011      | 14007      |
| hagl_fill_polygon()           | 1848   | 4530   | 6471       | 6470       |
| hagl_put_char()               | 5707   | 21887  | 31226      | 31230      |
| hagl_put_text()               | 375    |        |            | 2228       |

## License

MIT No Attribution (MIT-0). Please see [LICENSE](LICENSE) for more information.
