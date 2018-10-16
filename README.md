# ChimeraEngine

Simple Game Engine

## Para o log no ubuntu:
```
apt-get install libspdlog-dev
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

## Nova versao com desenvolvimento de HMD
```
apt install libyaml-cpp-dev
```

# Versão Teste com o HMD

AppEmpty esta em andamento
yaml ref: https://github.com/jbeder/yaml-cpp/wiki/How-To-Emit-YAML