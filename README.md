# ChimeraEngine

Just another simple game engine<p>
OBS: Code with comments in Portuguese mostly

## Features of project:
- Editor: vscode
- Build tool: CMAKE
- Compiler: clang (using C++17)
- Debug: lldb
- Ident: clang-format (file: .clang-format)

## Fedora 38 Dependencies:
```bash
sudo dnf -y groupinstall "Development Tools" "Development Libraries"

# libs
sudo dnf install htop git gitk meld gcc g++ cmake python3-pip vim tree curl openssh-server tinyxml2 tinyxml2-devel libyaml libyaml-devel yaml-cpp yaml-cpp-devel pugixml-devel

# LLVM / LLDM /CMAKE
sudo dnf install clang clang-tools-extra
sudo dnf install cmake cmake-data cmake-rpm-macros jsoncpp libstdc++-static llvm-static llvm-devel llvm-test autoconf automake
sudo dnf install lldb lldb-devel
sudo dnf install compiler-rt

# OpenGL
sudo dnf install mesa-dri-drivers mesa-libGL freeglut-devel glm-devel glew glew-devel libGLEW
sudo dnf install bullet bullet-devel bullet-extras bullet-extras-devel

# SDL2
sudo dnf install SDL2-devel SDL2-static SDL2_image SDL2_image-devel SDL2_mixer SDL2_mixer-devel SDL2_net SDL2_net-devel SDL2_sound SDL2_sound-devel SDL2_ttf SDL2_ttf-devel SDL2_gfx SDL2_gfx-devel SDL2_gfx-docs
```

## Ubuntu 23.04 dependencies:
```bash
# Develop:
apt install htop build-essential git gitk meld cmake g++ vim libtinyxml2-dev libyaml-cpp-dev libpugixml-dev pugixml-doc

# OpenGL dev
apt install libglu1-mesa-dev freeglut3-dev mesa-common-dev libglm-dev libglew-dev libftgl-dev

# SDL2-Dev
apt install libsdl2-dev libsdl2-doc libsdl2-gfx-dev libsdl2-gfx-doc libsdl2-image-dev libsdl2-mixer-dev libsdl2-net-dev libsdl2-ttf-dev

# Clang format and indentation ref: http://clang.llvm.org/docs/ClangFormatStyleOptions.html
apt install clang-format clang lldb

apt install libstdc++-12-dev
ref: https://stackoverflow.com/questions/74543715/usr-bin-ld-cannot-find-lstdc-no-such-file-or-directory-on-running-flutte

# BULLET ENGINE
apt install libbullet-dev libassimp-dev
# Or build from SOURCE url: https://github.com/bulletphysics/bullet3
cd bullet
mkdir build
cd build
cmake .. -G "Unix Makefiles" -DINSTALL_LIBS=ON -DBUILD_SHARED_LIBS=ON
make j4
make install
```

## Build Libs and examples app's:
```bash
cd ChimeraEngine
mkdir build
cd build
cmake ../
make
```

## Switch to clang (Ubuntu)
- CMake will uso the default of SO 
    ```bash
    sudo apt install clang # compiler
    sudo apt install lldb  # debuger
    sudo update-alternatives --config c++ # select clang
    sudo update-alternatives --config cc # select clang
    ```
- In VSCode change default kit to clang (panel of tools bellow)

## Development environment

### Setup clang in file .clang-format
```ini
BasedOnStyle: LLVM
IndentWidth: 4
IndentCaseLabels: true
AllowShortBlocksOnASingleLine: true
ColumnLimit: 140
PointerAlignment: Left
DerivePointerAlignment: false
AlwaysBreakTemplateDeclarations: true
```

### VSCode Extensions: 
```bash
user@host:~/.vscode/extensions/code --list-extensions | xargs -L 1 echo code --install-extension

code --install-extension cschlosser.doxdocgen
code --install-extension DotJoshJohnson.xml
code --install-extension dtoplak.vscode-glsllint
code --install-extension eamodio.gitlens
code --install-extension Gruntfuggly.todo-tree
code --install-extension jeff-hykin.better-cpp-syntax
code --install-extension ms-vscode.cmake-tools
code --install-extension ms-vscode.cpptools
code --install-extension ms-vscode.cpptools-extension-pack
code --install-extension ms-vscode.cpptools-themes
code --install-extension PKief.material-icon-theme
code --install-extension slevesque.shader
code --install-extension twxs.cmake
code --install-extension vadimcn.vscode-lldb
code --install-extension xaver.clang-format

```
## Nova versao com desenvolvimento de HMD

Iniciado tentativa de criar um HMD e posteriormente integrar suporte ao OpenHMD

### Versão Teste com o HMD

re-modelamento arquivos e sub-projetos 
yaml ref: https://github.com/jbeder/yaml-cpp/wiki/How-To-Emit-YAML
Logs ref: https://github.com/gabime/spdlog/blob/v1.x/example/example.cpp

OpenGL
https://gamedev.stackexchange.com/questions/60313/implementing-a-skybox-with-glsl-version-330​
https://learnopengl.com/Advanced-OpenGL/Cubemaps​
http://antongerdelan.net/opengl/cubemaps.html
https://www.khronos.org/opengl/wiki/Example_Code

## FrameBuffer cfg's
- Renderizar tela: <p>
    <i>TexParam(TexFormat::RGBA, TexFormat::RGBA, TexFilter::LINEAR, TexWrap::CLAMP, TexDType::UNSIGNED_BYTE)</i>

- Captura de dados para uniform: <p>
    <i>TexParam(TexFormat::RED_INTEGER, TexFormat::R32I, TexFilter::LINEAR, TexWrap::CLAMP_TO_EDGE, TexDType::UNSIGNED_BYTE)</i>

- RBO: <p>
    <i>TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_ATTACHMENT, TexFilter::NONE, TexWrap::NONE, TexDType::UNSIGNED_BYTE)</i>
    
- SwadowMap: <p>
    <i>TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_COMPONENT, TexFilter::NEAREST, TexWrap::CLAMP_TO_BORDER, TexDType::FLOAT)</i>

1 0 0 X 0 1 0 Y 0 0 1 Z 0 0 0 1


DEBUG: Joystick index 0: PG-9021S
DEBUG: game controller: 05005322491900000204000000000000,
Ipega PG-9087S,a:b0,b:b1,back:b10,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,dpup:h0.1,leftshoulder:b6,leftstick:b13,lefttrigger:b8,leftx:a0,lefty:a1,rightshoulder:b7,rightstick:b14,righttrigger:b9,rightx:a2,righty:a3,start:b11,x:b3,y:b4,platform:Linux
DEBUG: Joystick instance id: 0
DEBUG: Joystick axes: 6
DEBUG: Joystick hats: 1
DEBUG: Joystick buttons: 15
DEBUG: Joystick trackballs: 0

DEBUG: Joystick (0): PG-9021S
DEBUG: Joy axis: 0 128
DEBUG: Joy axis: 1 128
DEBUG: Joy axis: 2 128
DEBUG: Joy axis: 3 128
DEBUG: Joy axis: 4 -32768
DEBUG: Joy axis: 5 -32768
DEBUG: Joy buttons 0 State: RELEASE
DEBUG: Joy buttons 1 State: RELEASE
DEBUG: Joy buttons 3 State: RELEASE
DEBUG: Joy buttons 4 State: RELEASE
DEBUG: Joy buttons 6 State: RELEASE
DEBUG: Joy buttons 7 State: RELEASE
DEBUG: Joy buttons 8 State: RELEASE
DEBUG: Joy buttons 9 State: RELEASE
DEBUG: Joy buttons 10 State: RELEASE
DEBUG: Joy buttons 11 State: RELEASE
DEBUG: Joy buttons 13 State: RELEASE
DEBUG: Joy buttons 14 State: RELEASE
DEBUG: Joy hats: 0 0 [ C ]