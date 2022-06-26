# BabyOS

For kids who like to push buttons

[example](https://imgur.com/a/2x8vo82)

## Description

When my kids were little they always liked to push the buttons on the computer.  I made this as something fun they could do without breaking anything.  When you push keys on the keyboard it picks a random image and animates it onto the screen.  Images are JPGs or PNGs from the current directory or from a directory specified on the command line.

## Dependencies

Depends on cmake, libfmt, sfml:

Arch:
    # pacman -S base-devel cmake sfml fmt

Debian/Ubuntu:
    # apt-get install build-essential cmake libsfml-dev libfmt-dev

## Building

Use normal CMake build commands or just run the build script:

    $ ./build.sh -j8

The program is built in the "build" directory, `build/babyos`

## Running

Run the program from the directory containing the images you want to use or specify `--imagedir` on the commandline.  The program exits when a particular sequence of characters is typed, QUIT by default.  This can be changed on the command line.

## License

[MIT](https://opensource.org/licenses/MIT)
