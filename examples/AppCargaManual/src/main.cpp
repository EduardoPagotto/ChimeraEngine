#include <cstdio>
#include <iostream>
#include <map>

#include "chimera/core/CanvasGL.hpp"

#ifdef OVR_SET_TO_USE
#include "OvrDevice.hpp"
#endif

#include "Game.hpp"

#include "chimera/loader/PhysicsScene.hpp"
#include "chimera/loader/VisualScene.hpp"

#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/ShadersLoader.hpp"
#include "chimera/core/Singleton.hpp"

#include "chimera/node/HUD.hpp"
#include "chimera/node/ParticleEmitter.hpp"
#include "chimera/node/ShadowMapVisitor.hpp"
#include "chimera/node/Transform.hpp"

#include <yaml-cpp/yaml.h>

int main(int argn, char** argv) {

    std::map<std::string, Chimera::Shader*> mapa;

    // SDL_LogSetPriority(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_DEBUG);
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("AppShader Iniciado");

    for (int i = 0; i < argn; i++) {
        SDL_Log("Parametros %i: %s", i, argv[i]);
    }

    try {
        std::string config_file = "./examples/AppCargaManual/etc/appcargamanual.yaml";
        SDL_Log("Carregar arquivo: %s", config_file.c_str());
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        YAML::Node canvas = screen["canvas"];

        // Controlador de video
        Chimera::CanvasGL* video =
            new Chimera::CanvasGL(screen["name"].as<std::string>(), canvas["w"].as<int>(), canvas["h"].as<int>());

        // Gerenciador do grapho de cena
        Chimera::SceneMng* sceneMng = new Chimera::SceneMng();

        Chimera::ShadersLoader sl; // = new ChimeraLoaders::ShadersLoader();

        YAML::Node shaders = config["shaders"];
        // Chimera::ShadersManager *shader =  sceneMng->getShadersManager();
        SDL_Log("Shaders identificados: %d", (int)shaders.size());
        for (std::size_t i = 0; i < shaders.size(); i++) {
            YAML::Node shader_item = shaders[i];
            Chimera::Shader* pShader =
                sl.loadShader(shader_item["name"].as<std::string>(), shader_item["vertex"].as<std::string>(),
                              shader_item["fragment"].as<std::string>());

            mapa[pShader->getCurrentProgram()] = pShader;
        }

        std::string model = config["model"].as<std::string>();
        std::string font = config["font"].as<std::string>();

        // Cria grupo shader como filho de scene
        Chimera::Group* group1 = new Chimera::Group(sceneMng, "none");
        ChimeraLoaders::VisualScene libV(model, group1);
        libV.target();

        // Cria mundo fisico e o vincula a scena
        Chimera::PhysicsControl* pPC = new Chimera::PhysicsControl();
        ChimeraLoaders::PhysicsScene libP(model, pPC);
        libP.target();

        // Vincula o shader de calculo de sobra e ShadowMap com textura de resultado
        group1->setShader(mapa["mesh-default"]);
        group1->setNodeVisitor(new Chimera::ShadowMapVisitor(mapa["simpleDepthShader"]));
        Chimera::ShadowMap* pShadowMap = new Chimera::ShadowMap(group1, "shadow1", 2048, 2048);

        // Novo Grupos com shader de Emissor particula GLSL de particula e o vincula a
        // cena e ao shader
        Chimera::Group* gParticle = new Chimera::Group(sceneMng, "ParticleGroup");
        gParticle->setShader(mapa["particle-default"]);

        Chimera::Transform* posParticle = new Chimera::Transform(gParticle, "posicaoParticle");
        posParticle->setPosition(glm::vec3(-5.0, 5.0, 4.0));
        Chimera::ParticleEmitter* pParticleEmitter = new Chimera::ParticleEmitter(posParticle, "testeZ1", 10000);
        pParticleEmitter->loadTexDiffuse("TexParticleEmmiter", std::string("./models/Particle2.png"));

        // Novo Grupos com shader de um HUD ao Grapho da cena
        Chimera::Group* gHud = new Chimera::Group((Chimera::Node*)sceneMng, "HUD-Group");
        gHud->setShader(mapa["hud-default"]);
        Chimera::HUD* pHUD = new Chimera::HUD(gHud, "HUD-Default");
        Chimera::Font* pFont = new Chimera::Font(font, 18); // TODO: carregar size da fonte
        pHUD->addFont(pFont);

        // Passa a game a scena, o video e o mundo fisico
        Game* game = new Game(sceneMng, video, pPC);

        // Controle de fluxo do programa
        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        SDL_Log("Loop de Game encerrado!!!!");
        delete pControle;
        delete game;
        delete sceneMng;

        delete pShadowMap;

        delete video;

    } catch (const Chimera::Exception& ex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.getMessage().c_str());
        return -1;
    } catch (const std::exception& ex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
        return -1;
    } catch (const std::string& ex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.c_str());
        return -1;
    } catch (...) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
        return -1;
    }

    SDL_Log("AppShader finalizado com sucesso");
    return 0;
}
