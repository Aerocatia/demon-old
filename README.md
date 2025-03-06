# NOTICE
### Future development has moved to a new project [here](https://github.com/Aerocatia/demon)

# Demon

Demon is a project with the goal of reverse engineering and reimplementing the functionality of the Halo PC demo, AKA
Halo Trial.

Please see the [Frequently Asked Questions](https://github.com/Aerocatia/demon/wiki/FAQ) for more information.

## Features

These are features added that are not originally present in the original Halo PC demo. These are done to help with
improving the functionality of the game as well as making it easier to test.

- New debugging messages to assist with modding (more will be added as the project continues).
- Backported `-console` and `-devmode` from Halo Custom Edition, allowing you to enable a fully-featured console.

## Compiling Demon

The current progress of demon can be compiled into a replacement `strings.dll`, where core game functions will be
redirected at runtime into our re-implemented versions.

### Dependencies

Demon requires the following software for the build process:

- mingw-w64
- CMake (mingw-w64)
- python

### Compilation steps

First, recursively clone the git repository.
```
git clone https://github.com/Aerocatia/demon --recursive
```
Next, `cd` into the project directory and prepare the build directory using `i686-w64-mingw32-cmake`
```
cd demon
i686-w64-mingw32-cmake -B build -S .
```
The project can now be built using `cmake`.
```
cmake --build build
```

## Testing Demon

Once `strings.dll` has been compiled, it can be run using an original install of Halo Trial

### Creating a testing environment

First, download a copy of the original Halo Trial Installer. This can be obtained [here](http://vaporeon.io/hosted/halo/original_files/HaloTrialSetup.exe).

Next, extract the contents of this installer using `7-zip` to a desired location. Installing Halo Trial the usual way
is not recommended as this Windows XP-era game assumes you are likely to have direct write access to `Program Files`.

Finally, back up the original `strings.dll` and replace it with the one created by compiling this project.

You can now start the game by running `halo.exe`

### Tips

- Use the argument `-window` to start the game in a window.
- Use the argument `-vidmode <width>,<height>[,<refresh rate>]` to set the game resolution.
- Use the argument `-novideo` to skip the promo video when exiting the game.
- Use the argument `-console` to enable the console and `-devmode` to enable all commands.
