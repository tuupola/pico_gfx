## Graphics speed tests for Raspberry Pi Pico

![Circles](https://appelsiini.net/img/2020/pod-draw-circle.png)


## Compile

Below instructions assume you have [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) installed and `PICO_SDK_PATH` environment correctly set up.

```
$ git clone --recursive https://github.com/tuupola/pico_gfx.git
$ cd pico_gfx/build
$ cmake ..
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
