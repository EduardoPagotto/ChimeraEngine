#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Exception.hpp"

#include "Terrain.hpp"
//#include "chimera/render/LoadObj.hpp"

int main(int argn, char** argv) {

    try {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Iniciado");

        // Chimera::TerrainData t;
        Chimera::Terrain t;
        t.LoadBinary((char*)"./samples/terrain/data/terrain1.bin");

        SDL_Log("Sucesso");
        return 0;

    } catch (const Chimera::Exception& ex) {
        // fail 1
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::exception& ex) {
        // fali 2
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::string& ex) {
        // fail 3
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.c_str());
    } catch (...) {
        // Fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
    }

    return -1;
}