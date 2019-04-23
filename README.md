# joymap
Joystick and joypad mapper for Linux

How to compile
--------------
    $ gcc joymap.c -lxdo -o joymap
Requirements: libxdo-dev


Examples
--------
[Fantasy Zone](http://www.retrosega.com/game.php?id=192)

./joymap -d /dev/input/js1 -p "Left Right" -r "Left Right" -q "Up Down" -1 d -3 s


[Bubble bobble](https://archive.org/details/msdos_Bubble_Bobble_1988)

./joymap -d /dev/input/js1 -p "Left Right" -r "Left Right" -t "Left Right" -q "Up Down" -u "Up Down" -1 d -2 Up -3 s -4 q -6 q -5 w -7 w


[Wonder boy](http://www.retrosega.com/game.php?id=1207)

./joymap -d /dev/input/js0 -t "Left Right" -u "d Down" -1 s

[Test drive 1](https://classicreload.com/test-drive.html)

./joymap -d /dev/input/js0 -t "Left Right" -u "Up Down" -6 z -7 a
