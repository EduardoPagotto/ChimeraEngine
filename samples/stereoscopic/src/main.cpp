#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/windows/CanvasHmd.hpp"
#include <yaml-cpp/yaml.h>

int main(int argn, char** argv) {

    try {
        using namespace Chimera;

        std::vector<std::string> listShades;

        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("appTest Iniciado");

        for (int i = 0; i < argn; i++) {
            SDL_Log("Parametros %d: %s", i, argv[i]);
        }

        std::string config_file = "./samples/stereoscopic/appteste.yaml";
        SDL_Log("Carregar arquivo: %s", config_file.c_str());

        YAML::Node config = YAML::LoadFile(config_file);
        YAML::Node screen = config["screen"];
        YAML::Node canvas = screen["canvas"];

        // Carga dos shaders
        YAML::Node shaders = config["shaders"];

        // Chimera::ShadersManager *shader =  sceneMng->getShadersManager();
        SDL_Log("Shaders identificados: %d", (int)shaders.size());
        for (std::size_t i = 0; i < shaders.size(); i++)
            listShades.push_back(shaders[i].as<std::string>());

        // Wrapper do game
        Game* game = new Game(new CanvasHmd(screen["name"].as<std::string>(), canvas["w"].as<int>(), canvas["h"].as<int>()), listShades);
        game->run();
        SDL_Log("Loop de Game encerrado!!!!");

        delete game;
        SDL_Log("appTest finalizado com sucesso");
        return 0;

    } catch (const Chimera::Exception& ex) {
        // fail 1
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::exception& ex) {
        // fail 2
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::string& ex) {
        // fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.c_str());
    } catch (...) {
        // fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
    }

    return -1;
}
