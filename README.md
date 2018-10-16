# ChimeraEngine

Simple Game Engine

## Para o log e carga de yaml no ubuntu:
```
apt-get install libspdlog-dev
apt install libyaml-cpp-dev
```

## Caso precise compilar o bullet:
```
cd bullet
mkdir build
cd build
cmake .. -G "Unix Makefiles" -DINSTALL_LIBS=ON -DBUILD_SHARED_LIBS=ON
make j4
make install
```

## Ambiente de desenvolvimento

 - vscode extenções uteis: 

    ```    
    code --list-extensions | xargs -L 1 echo code --install-extension
    ...
    code --install-extension cschlosser.doxdocgen
    code --install-extension DotJoshJohnson.xml
    code --install-extension eamodio.gitlens
    code --install-extension Gruntfuggly.todo-tree
    code --install-extension mohsen1.prettify-json
    code --install-extension ms-vscode.cpptools
    code --install-extension redhat.vscode-yaml
    code --install-extension robertohuertasm.vscode-icons
    code --install-extension slevesque.shader
    code --install-extension twxs.cmake
    code --install-extension vector-of-bool.cmake-tools
    code --install-extension wayou.vscode-todo-highlight
    ```

 - Kdevelop ainda suportado


## Nova versao com desenvolvimento de HMD

Iniciado tentativa de criar um HMD e posteriormente integrar suporte ao OpenHMD

# Versão Teste com o HMD

AppEmpty esta em andamento
yaml ref: https://github.com/jbeder/yaml-cpp/wiki/How-To-Emit-YAML