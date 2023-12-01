![](header.png?raw=true)

A modded version of RSDKv4 with fan game development in mind.

# **SUPPORT THE OFFICIAL RELEASE OF SONIC 1 & 2**
+ Without the offical releases of Sonic 1 & Sonic 2, this project wouldn't be possible!

+ You can get official releases of Sonic 1 & Sonic 2 from:
  * Windows
    * Via Steam, from [Sonic Origins](https://store.steampowered.com/app/1794960)
    * Via the Epic Games Store, from [Sonic Origins](https://store.epicgames.com/en-US/p/sonic-origins)
  * iOS
    * [Sonic 1, Via the App Store](https://apps.apple.com/au/app/sonic-the-hedgehog-classic/id316050001)
    * [Sonic 2, Via the App Store](https://apps.apple.com/au/app/sonic-the-hedgehog-2-classic/id347415188)
    * A tutorial for finding the game assets from the iOS versions can be found [here](https://gamebanana.com/tuts/14491).
  * Android
    * [Sonic 1, Via Google Play](https://play.google.com/store/apps/details?id=com.sega.sonic1px)
    * [Sonic 2, Via Google Play](https://play.google.com/store/apps/details?id=com.sega.sonic2.runner)
    * [Sonic 1, Via Amazon](https://www.amazon.com.au/Sega-of-America-Sonic-Hedgehog/dp/B00D74DVKM)
    * [Sonic 2, Via Amazon](https://www.amazon.com.au/Sega-of-America-Sonic-Hedgehog/dp/B00HAPRVWS)
    * A tutorial for finding the game assets from the Android versions can be found [here](https://gamebanana.com/tuts/14492).

   * However, if you want a project that was made with Scarlet in mind, check out its companion project: [RSDK-GDT 2.0](https://github.com/youngerelk1/RSDK-GDT/tree/main), the first (and only) framework for the Retro Engine!

Even if your platform isn't supported by the official releases, you **must** buy or officially download it for the assets (you don't need to run the official release, you just need the game assets). (if running the original games (Sonic 1 & Sonic 2 in this case))

# Additional Tweaks over RSDKv4
* Added all the features from every revision of RSDKv4 (REV00, REV01, REV02 & REV03)
* Removed the 2MBs Audio limit
* Re-adds the Disable Focus Pause that was missing from the Mobile versions of RSDKv4

# Planned features of the Scarlet Engine:
* The video playback from RSDKv3
* Add the tweaks from RSDKv5(U) on it

# Scarlet Engine Required Games
* Virtua Scarlet
# Scarlet Engine Compatible Games
Sonic 2
Another Sonic Adventure
RSDK-GDT (Framework) 

# How to Build?

This project uses [CMake](https://cmake.org/), a versatile building system that supports many different compilers and platforms. You can download CMake [here](https://cmake.org/download/). **(Make sure to enable the feature to add CMake to the system PATH during the installation!)**

## Get the source code

In order to clone the repository, you need to install Git, which you can get [here](https://git-scm.com/downloads).

Clone the repo **recursively**, using:
`git clone --recursive https://github.com/Rubberduckycooly/Sonic-1-2-2013-Decompilation`

If you've already cloned the repo, run this command inside of the repository:
```git submodule update --init```

## Follow the build steps

### Windows
To handle dependencies, you'll need to install [Visual Studio Community](https://visualstudio.microsoft.com/downloads/) (make sure to install the `Desktop development with C++` package during the installation) and [vcpkg](https://github.com/microsoft/vcpkg#quick-start-windows).

After installing those, run the following in Command Prompt (make sure to replace `[vcpkg root]` with the path to the vcpkg installation!):
- `[vcpkg root]\vcpkg.exe install glew sdl2 libogg libvorbis --triplet=x64-windows-static` (If you're compiling a 32-bit build, replace `x64-windows-static` with `x86-windows-static`.)

Finally, follow the [compilation steps below](#compiling) using `-DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_PREFIX_PATH=[vcpkg root]/installed/x64-windows-static/` as arguments for `cmake -B build`.
  - Make sure to replace each instance of `[vcpkg root]` with the path to the vcpkg installation!
  - If you're compiling a 32-bit build, replace each instance of `x64-windows-static` with `x86-windows-static`.

### Linux
Install the following dependencies: then follow the [compilation steps below](#compiling):
- **pacman (Arch):** `sudo pacman -S base-devel cmake glew sdl2 libogg libvorbis`
- **apt (Debian/Ubuntu):** `sudo apt install build-essential cmake libglew-dev libglfw3-dev libsdl2-dev libogg-dev libvorbis-dev`
- **rpm (Fedora):** `sudo dnf install make gcc cmake glew-devel glfw-devel sdl2-devel libogg-devel libvorbis-devel zlib-devel`
- **apk (Alpine/PostmarketOS)** `sudo apk add build-base cmake glew-dev glfw-dev sdl2-dev libogg-dev libvorbis-dev`
- Your favorite package manager here, [make a pull request](https://github.com/Rubberduckycooly/Sonic-1-2-2013-Decompilation/fork)

## Android
Follow the android build instructions [here.](./dependencies/android/README.md)

### Compiling

Compiling is as simple as typing the following in the root repository directory:
```
cmake -B build
cmake --build build --config release
```

The resulting build will be located somewhere in `build/` depending on your system.

The following cmake arguments are available when compiling:
- Use these on the first `cmake -B build` step like so: `cmake -B build -DRETRO_DISABLE_PLUS=on`

### Scarlet Engine flags
- `RETRO_REVISION`: What revision to compile for. Takes an integer, defaults to `3` (Origins), even though doesn't really makes any difference in Scarlet so don't worry.
- `RETRO_DISABLE_PLUS`: Whether or not to disable the Plus DLC. Takes a boolean (on/off): build with `on` when compiling for distribution. Defaults to `off`. This one will be useful for when Virtua Scarlet gets a Plus DLC!
- `RETRO_FORCE_CASE_INSENSITIVE`: Forces case insensivity when loading files. Takes a boolean, defaults to `off`.
- `RETRO_MOD_LOADER`: Enables or disables the mod loader. Takes a boolean, defaults to `on`.
- `RETRO_NETWORKING`: Enables or disables networking features used for Sonic 2's 2P VS mode. Takes a boolean, defaults to `on`.
- `RETRO_USE_HW_RENDER`: Enables the Hardware Renderer used by the main menu and touch controls UI. Takes a boolean, defaults to `on`.
- `RETRO_ORIGINAL_CODE`: Removes any custom code. *A playable game will not be built with this enabled.* Takes a boolean, defaults to `off`.
- `RETRO_SDL_VERSION`: *Only change this if you know what you're doing.* Switches between using SDL1 or SDL2. Takes an integer of either `1` or `2`, defaults to `2`.


# Server
The multiplayer server requires Python 3.8 or later. You can download Python [here](https://www.python.org/downloads/).
To use the server, open Command Prompt in the folder [Server.py](./Server/Server.py) is located in, then run the command `py -3 Server.py [local IPv4 address] [port] debug`. You can find your local IPv4 address using the command `ipconfig`.
Note that the C++ server found in the `Server` folder has been deprecated and no longer works. It has been kept in the repo for reference purposes.

# FAQ
You can find the FAQ [here](./FAQ.md).

# **Reminder this branch is unofficial, neither REMS nor the original decomp team can't provide support for this and it may not be up-to-date all the time.**


# Special Thanks
* [st×tic](https://github.com/stxticOVFL) for helping me fix bugs, tweaking up my sometimes sloppy code and generally being really helpful and fun to work with on this project.
* [The Weigman](https://github.com/TheWeigman) for creating the header you see up here along with similar assets.
* Everyone in the [Retro Engine Modding Server](https://dc.railgun.works/retroengine) for being supportive of me and for giving me a place to show off these things that I've found.

# Contact:
Join the [Retro Engine Modding Discord Server](https://dc.railgun.works/retroengine) for any extra questions you may need to know about the decompilation or modding it.
