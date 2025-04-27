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
sudo dnf install lld


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
## Switch to clang (Fedora)
Change to ld.lld
```bash
sudo update-alternatives --config ld
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

## Exemplo de codigo de uso de joystick
```cpp
    if (SDL_Joystick* pJoy = joyControl.get(0); pJoy != nullptr) {

        float propulsaoLRUD{5.0f};
        glm::vec3 propLateral(0.0f);
        uint ggg = SDL_JoystickGetHat(pJoy, 0);
        switch (ggg) {
            case SDL_HAT_UP:
                propLateral.z = propulsaoLRUD;
                break;
            case SDL_HAT_DOWN:
                propLateral.z = -propulsaoLRUD;
                break;
            case SDL_HAT_LEFT:
                propLateral.x = propulsaoLRUD;
                break;
            case SDL_HAT_RIGHT:
                propLateral.x = -propulsaoLRUD;
                break;
            default:
                break;
        }

        int16_t deadZone = 128;
        glm::vec3 rotacao{scale16(dead16(SDL_JoystickGetAxis(pJoy, 1), deadZone), 0x8000),  // pitch LEFTY
                          scale16(dead16(SDL_JoystickGetAxis(pJoy, 2), deadZone), 0x8000),  // roll LEFTX
                          scale16(dead16(SDL_JoystickGetAxis(pJoy, 0), deadZone), 0x8000)}; // yaw RIGHTY

        float acc = scale16(dead16(SDL_JoystickGetAxis(pJoy, 3), deadZone), 0x8000); // ACC RIGHTX
        glm::vec3 throttle{0.0,                                                      // X
                           -3.0f * (acc / 2),                                        // y
                           0.0f};                                                    // z

        if (SDL_JoystickGetButton(pJoy, 0) == SDL_PRESSED)
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick Botao 0");

        if (SDL_JoystickGetButton(pJoy, 1) == SDL_PRESSED)
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick Botao 1");

        glm::vec3 zero(0.0f);
        if ((rotacao != zero) || (throttle != zero) || (propLateral != zero)) {
            float torque = -0.5f;

            glm::vec3 rFinal = rotacao * torque;
            glm::vec3 vFinal = propLateral + throttle;

            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Torque: %f %f %f", rFinal.x, rFinal.y, rFinal.z);
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Forca : %f %f %f", vFinal.x, vFinal.y, vFinal.z);

            pCorpoRigido->applyForce(vFinal);
            pCorpoRigido->applyTorc(rFinal);
        }
    }
    ```
