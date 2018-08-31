ChimeraEngine
=============

Simple Game Engine

Para o log no ubuntu:
    apt-get install libspdlog-dev

Caso precise compilar o bullet:
    cd bullet
    mkdir build
    cd build
    cmake .. -G "Unix Makefiles" -DINSTALL_LIBS=ON -DBUILD_SHARED_LIBS=ON
    make j4
    make install