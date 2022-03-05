# ChimeraEngine

Just another simple game engine<p>
OBS: Code with comments in Portuguese mostly

## Ubuntu 20.04 dependencies
```bash
# Develop:
apt install htop build-essential git gitk meld cmake g++ vim libtinyxml2-dev libyaml-cpp-dev

# OpenGL dev
apt install libglu1-mesa-dev freeglut3-dev mesa-common-dev libglm-dev libglew-dev libftgl-dev

# SDL2-Dev
apt install libsdl2-dev libsdl2-doc libsdl2-gfx-dev libsdl2-gfx-doc libsdl2-image-dev libsdl2-mixer-dev libsdl2-net-dev libsdl2-ttf-dev

# Clang format and indentation ref: http://clang.llvm.org/docs/ClangFormatStyleOptions.html
apt install clang-format clang lldb

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

### Mesh com material e textura manual
```cpp
    // Material Cubo sem textura
    Material* pMat = new Material();
    pMat->setAmbient(glm::vec4(0.5f, 0.5f, 0.31f, 1.0f));
    pMat->setDiffuse(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    pMat->setSpecular(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    pMat->setShine(32.0f);
    pMat->addTexture(new TextureSurface(SHADE_TEXTURE_DIFFUSE, "./assets/textures/grid1.png"));

    NodeMesh* pMesh = new NodeMesh(group1, name);
    pMesh->setTransform(new Transform(glm::translate(glm::mat4(1.0f), _position)));
    pMesh->setMaterial(_pMap);

    LoaderObj loader;
    loader.getMesh(file, pMesh->meshData);

    pMesh->meshData.changeSize(scale, _pMap->hasTexture());
```
### Mesh com material e textura loader
```cpp
    Material* pMap = new Material();
    NodeMesh* pMesh = new NodeMesh(group1, name);

    LoaderObj loader;
    loader.getMesh(file, pMesh->meshData);
    loader.getMaterial(*pMap);

    pMesh->meshData.changeSize(scale, pMap->hasTexture());
    pMesh->setTransform(new Transform(glm::translate(glm::mat4(1.0f), _position)));
    pMesh->setMaterial(pMap);
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
- Renderizae tela: <p>
    <i>TexParam(TexFormat::RGBA, TexFormat::RGBA, TexFilter::LINEAR, TexWrap::CLAMP, TexDType::UNSIGNED_BYTE)</i>
- Captura de dados para uniform: <p>
    <i>TexParam(TexFormat::RED_INTEGER, TexFormat::R32I, TexFilter::LINEAR, TexWrap::CLAMP_TO_EDGE, TexDType::UNSIGNED_BYTE)</i>
- RBO: <p>
    <i>TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_ATTACHMENT, TexFilter::NONE, TexWrap::NONE, TexDType::UNSIGNED_BYTE)</i>
- SwadowMap: <p>
    <i>TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_COMPONENT, TexFilter::NEAREST, TexWrap::CLAMP_TO_BORDER, TexDType::FLOAT)</i>
