# Tetris-SFML

A small (mostly) Tetris standards compient game written in pure C++/SFML.

![image](https://user-images.githubusercontent.com/79599920/165860384-14f1bd21-16b6-4bf5-bf35-4bbe908267ea.png)
![image](https://user-images.githubusercontent.com/79599920/165860472-f6809e72-ae90-454e-9ed8-5f57baf8d52c.png)
![image](https://user-images.githubusercontent.com/79599920/165860554-0c35bd3b-ffd1-4915-895d-d75a37c50975.png)
![image](https://user-images.githubusercontent.com/79599920/165860571-ec3ab926-df15-4c33-a07b-c69cb29ce04a.png)

## Requirements

C++11 Complient version of GCC
SFML (Idealy 2.5.1, not tested with other versions)
windres (Included in the GNU Binary Utilities)

## Complation

There is a makefile provided. (Designed for staticaly linked SFML, some minor changes can allow for dynamicaly linked.)

It is made for Mingw-w64, but again, minor changes (Such as removing the ```-mwindows``` flag) can make it comatible with all gcc versions, windows and linux.

```CXXLIBPATHS``` needs to be modified to the locations of you're SFML extlibs and SFML libs. The flag denoting the path to the extlibs can be removed if you are linking to sfml dynamicaly.

```UPX``` is used by default in the makefile, if you don't want to use it, just get rid of the ```upx:``` rule.

After all the adjustments needed, just ```make``` or ```mingw32-make``` and you now have Tetris!

## Download

If all of that mess seems much to complicated, and you just want to play Tetris, the releases section has pre-built bianaries for windows devices.
