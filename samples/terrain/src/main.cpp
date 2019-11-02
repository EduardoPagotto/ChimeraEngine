#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Exception.hpp"

int main(int argn, char** argv) {

    try {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Iniciado");

        // Chimera::CanvasGL* video = new Chimera::CanvasGL("simples", 640, 480);

        // Chimera::MeshData m;
        // std::vector<Chimera::Triangle> listPolygons;

        // std::string materialFile;
        // loadObj("./data/models/square1.obj", m, materialFile);
        // loadObj("./data/models/cubo_textura_simples.obj", m, materialFile);

        // m.changeSize(1.0, true);

        // glm::vec3 max = m.getMax();
        // glm::vec3 min = m.getMin();

        // Chimera::AABB teste;
        // teste.set(min, max);

        // bool val = teste.pointInside(glm::vec3(1.5f));

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