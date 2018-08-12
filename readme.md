# dependencies in linux

## xorg-dev

run `sudo apt-get install xorg-dev`

## glfw

download the stable release then run `cmake --build . --target install`

## assimp

download the stable release then run

1. `cmake CMakeLists.txt -G 'Unix Makefiles'`
2. `make`
3. `cmake --build . --target install`

## freeimage

download the stable release then run 


1. `make`
2. `sudo make install`


for some webp/dsp files you need to fix some lines to make it compile because some incompatiblity between some macros and some c++ 11 literals
see: https://chromium-review.googlesource.com/c/webm/libwebp/+/297211