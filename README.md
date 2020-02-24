# joymap

Joymap helps you to configure a joystick or joypad on Linux to make it to work in **keyboard emulation** mode.

Many old games available online (i.e. the ones from [archive.org](https://archive.org/) - MS-DOS games, Classic PC Games, SEGA games), are controlled using the keyboard.

With joymap you can map very easily keyboard keys to buttons and axes of the joypad. Joymaps supports up to **6 axes** and **25 buttons** devices.

See the full project page at [Wizlab.it](https://www.wizlab.it/code/joymap.html)


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


Author
------
  * [Wizlab.it](https://www.wizlab.it/)


Contributors
------------
  * [markalston](https://github.com/markalston)


License
-------
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

To get a copy of the GNU General Public License, see [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/).
