# ChimeraEngine

Just another simple game engine using SDL3, OpenGL and Collada(XML)<p>
OBS: Code with comments in Portuguese mostly

## Features of project:
- Editor: VSCode
- Build tool: CMAKE
- Compiler: Clang (mostly C++20)
- Debug: lldb
- Ident: clang-format (file: .clang-format)
- SDL3; SDL3_Image; SDL3_TTF

## Fedora 42 Dependencies:
```bash
# Develop base
sudo dnf group upgrade core
sudo dnf group install c-development
sudo dnf group install development-tools
sudo dnf group install sound-and-video

# Develop compiler and tools like clang, llvm, lldb, CMAKE
sudo dnf install clang clang-tools-extra
sudo dnf install cmake cmake-data cmake-rpm-macros libstdc++-static llvm-static llvm-devel llvm-test autoconf automake
sudo dnf install lld lldb lldb-devel lld-devel lld-libs.x86_64
sudo dnf install compiler-rt

# Lobs and other tools
sudo dnf install htop git gitk meld gcc g++ python3-pip vim tree curl openssh-server tinyxml2 tinyxml2-devel libyaml libyaml-devel yaml-cpp yaml-cpp-devel pugixml-devel jsoncpp

# OpenGL
sudo dnf install mesa-dri-drivers mesa-libGL freeglut-devel glm-devel glew glew-devel libGLEW
sudo dnf install bullet bullet-devel bullet-extras bullet-extras-devel

# SDL3
sudo dnf copr enable ngompa/SDL2onSDL3
sudo dnf swap SDL2 sdl2-compat
sudo dnf install SDL3-static SDL3-devel SDL3_image-devel
sudo dnf install SDL3_ttf SDL3_ttf-devel (fedora testing update)
```

## Switch Link and toolchain to Clang
Toolchain: [clang.cmake](./toolchain/clang.cmake)<p>
Change to ld.lld
```bash
sudo update-alternatives --config ld
#change to -> 2           /usr/bin/ld.lld
```

## Build engine and examples and test:
```bash
cd ChimeraEngine

cmake -G "Unix Makefiles" \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE \
      -DCMAKE_TOOLCHAIN_FILE=./toolchain/clang.cmake \
      -B build

make -C build -j 4

# wayland still has erros, force X11
SDL_VIDEODRIVER=x11 ./bin/terrain
```




## Refs
<b>SDL3</b>
- [Main SDL3](https://wiki.libsdl.org/SDL3/FrontPage)
- [Migration SDL3](https://wiki.libsdl.org/SDL3/README-migration)

<b>OpenGL</b>
- [Collada](https://www.khronos.org/collada/)
- [learnopengl​](https://learnopengl.com/Introduction)
- [khrono](https://www.khronos.org/opengl/wiki/Example_Code)
- [gamedev](https://www.gamedev.com/)

<b>YAML</b> (future)
 - [jbeder](https://github.com/jbeder/yaml-cpp/wiki/How-To-Emit-YAML)
