#include <cstdio>
#include <iostream>
#include <map>

#include "chimera/core/CanvasGL.hpp"

#ifdef OVR_SET_TO_USE
#include "OvrDevice.hpp"
#endif

#include "Game.hpp"

#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/Singleton.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/loader/VisualScene.hpp"
#include "chimera/node/HUD.hpp"
#include "chimera/node/ParticleEmitter.hpp"
#include "chimera/node/ShadowMapVisitor.hpp"
#include "chimera/physic_loader/PhysicsScene.hpp"
#include "chimera/render/Transform.hpp"

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
        std::string config_file = "./samples/models/etc/appcargamanual.yaml";
        SDL_Log("Carregar arquivo: %s", config_file.c_str());
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        YAML::Node canvas = screen["canvas"];

        // Controlador de video
        Chimera::CanvasGL* video =
            new Chimera::CanvasGL(screen["name"].as<std::string>(), canvas["w"].as<int>(), canvas["h"].as<int>());

        YAML::Node shaders = config["shaders"];
        SDL_Log("Shaders identificados: %d", (int)shaders.size());
        for (std::size_t i = 0; i < shaders.size(); i++) {
            YAML::Node shader_item = shaders[i];

            std::string nome = shader_item["name"].as<std::string>();
            std::string vertex = shader_item["vertex"].as<std::string>();
            std::string fragm = shader_item["fragment"].as<std::string>();
            Chimera::Shader* pShader =
                new Chimera::Shader(nome.c_str(), Chimera::shadeLoadProg(nome.c_str(), vertex.c_str(), fragm.c_str()));

            mapa[pShader->getCurrentProgram()] = pShader;
        }

        std::string model = config["model"].as<std::string>();
        std::string font = config["font"].as<std::string>();

        // Cria grupo shader como filho de scene
        Chimera::Group* pRoot = new Chimera::Group(nullptr, "root_real");

        Chimera::Group* group1 = new Chimera::Group(pRoot, "none");

        ChimeraLoaders::VisualScene libV(model, group1);
        libV.target();

        // Cria mundo fisico e o vincula a scena
        Chimera::PhysicsControl* pPC = new Chimera::PhysicsControl();
        ChimeraLoaders::PhysicsScene libP(model, pPC);
        libP.target();

        // Vincula o shader de calculo de sobra e ShadowMap com textura de resultado
        group1->setShader(mapa["mesh-default"]);
        group1->setNodeVisitor(new Chimera::ShadowMapVisitor(mapa["simpleDepthShader"], 2048, 2048));

        // create and add particle to scene
        Chimera::Group* gParticle = new Chimera::Group(pRoot, "ParticleGroup");
        gParticle->setShader(mapa["particle-default"]);
        Chimera::ParticleEmitter* pParticleEmitter = new Chimera::ParticleEmitter(gParticle, "testeZ1", 10000);
        pParticleEmitter->setTransform(
            new Chimera::Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 5.0, 4.0))));

        pParticleEmitter->loadTexDiffuse("TexParticleEmmiter", std::string("./data/images/Particle2.png"));

        // Create and add hud data text
        Chimera::Group* gHud = new Chimera::Group((Chimera::Node*)pRoot, "HUD-Group");
        gHud->setShader(mapa["hud-default"]);
        Chimera::HUD* pHUD = new Chimera::HUD(gHud, "HUD-Default");
        Chimera::Font* pFont = new Chimera::Font(font, 18); // TODO: carregar size da fonte
        pHUD->addFont(pFont);

        // Gerenciador do grapho de cena
        // Passa a game a video, node e o mundo fisico
        Game* game = new Game(video, pRoot, pPC);

        // Controle de fluxo do programa
        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        SDL_Log("Loop de Game encerrado!!!!");
        delete pControle;
        delete game;
        delete video;

        SDL_Log("AppShader finalizado com sucesso");
        return 0;

    } catch (const Chimera::Exception& ex) {
        // Fail 1
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::exception& ex) {
        // Fail 2
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::string& ex) {
        // Fail 3
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.c_str());
    } catch (...) {
        // Fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
    }

    return -1;
}
