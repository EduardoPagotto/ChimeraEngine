# ChimeraEngine

Simple Game Engine

## Dependencias Ubuntu 19.04

```bash
# Develop:
apt install htop build-essential git gitk meld cmake g++

# XML
apt install tinyxml2*

# OpenGL dev
apt install libglu1-mesa-dev freeglut3-dev mesa-common-dev libglm-dev libglew-dev libglewmx-dev

# SDL2-Dev
apt install libsdl2-dev libsdl2-doc libsdl2-gfx-dev libsdl2-gfx-doc libsdl2-image-dev libsdl2-mixer-dev libsdl2-net-dev libsdl2-ttf-dev

# FTGL-dev
apt install libftgl-dev

#formatação codigo fonte com clang
#ref: http://clang.llvm.org/docs/ClangFormatStyleOptions.html
apt install clang-format 

# YAML config
apt install libyaml-cpp-dev

# Logs
apt-get install libspdlog-dev

# BULLET ENGINE
apt install libbullet-dev libassimp-dev
# Ou Instalação full do src no site: https://github.com/bulletphysics/bullet3
# Baixar pacote e extrair em: bullet
cd bullet
mkdir build
cd build
cmake .. -G "Unix Makefiles" -DINSTALL_LIBS=ON -DBUILD_SHARED_LIBS=ON
make j4
make install
```

## Compilação:
```bash
cd ChimeraEngine
mkdir build
cd build
cmake ../
make
```

## Configuracao do clang no arquivo .clang-format
```ini
BasedOnStyle: LLVM
IndentWidth: 4
IndentCaseLabels: true
AllowShortBlocksOnASingleLine: true
ColumnLimit: 120
PointerAlignment: Left
DerivePointerAlignment: false
```

## Ambiente de desenvolvimento

### vscode extenções uteis: 
```bash
user@host:~/.vscode/extensionscode --list-extensions | xargs -L 1 echo code --install-extension

code --install-extension cschlosser.doxdocgen
code --install-extension DotJoshJohnson.xml
code --install-extension eamodio.gitlens
code --install-extension euskadi31.json-pretty-printer
code --install-extension Gruntfuggly.todo-tree
code --install-extension mohsen1.prettify-json
code --install-extension ms-vscode.cpptools
code --install-extension PKief.material-icon-theme
code --install-extension redhat.vscode-yaml
code --install-extension slevesque.shader
code --install-extension twxs.cmake
code --install-extension vector-of-bool.cmake-tools
code --install-extension wayou.vscode-todo-highlight
code --install-extension xaver.clang-format

```

### Kdevelop ainda suportado


## Nova versao com desenvolvimento de HMD

Iniciado tentativa de criar um HMD e posteriormente integrar suporte ao OpenHMD

# Versão Teste com o HMD

re-modelamento arquivos e sub-projetos 
yaml ref: https://github.com/jbeder/yaml-cpp/wiki/How-To-Emit-YAML
Logs ref: https://github.com/gabime/spdlog/blob/v1.x/example/example.cpp

OpenGL
https://gamedev.stackexchange.com/questions/60313/implementing-a-skybox-with-glsl-version-330​
https://learnopengl.com/Advanced-OpenGL/Cubemaps​
http://antongerdelan.net/opengl/cubemaps.html
https://www.khronos.org/opengl/wiki/Example_Code
