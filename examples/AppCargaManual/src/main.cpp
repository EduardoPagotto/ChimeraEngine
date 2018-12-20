#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.hpp"
#endif

#include <iostream>

#include <map>

#include "chimera/core/CanvasGL.hpp"

#ifdef OVR_SET_TO_USE
#include "OvrDevice.hpp"
#endif

#include "Game.hpp"

#include "chimera/loader/PhysicsScene.hpp"
#include "chimera/loader/ShadersLoader.hpp"
#include "chimera/loader/VisualScene.hpp"

#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/Logger.hpp"
#include "chimera/core/Singleton.hpp"
#include "chimera/node/HUD.hpp"
#include "chimera/node/ParticleEmitter.hpp"
#include "chimera/node/ShadowMapVisitor.hpp"
#include "chimera/node/Transform.hpp"

#include <yaml-cpp/yaml.h>

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif

    std::map<std::string, Chimera::Shader*> mapa;

    Chimera::Logger* console = Chimera::Logger::get();
    console->info("AppShader Iniciado");

    for (int i = 0; i < argn; i++) {
        console->info("Parametros %i: %s", i, argv[i]);
    }

    try {
        std::string config_file = "./examples/AppCargaManual/etc/appcargamanual.yaml";
        console->info("Carregar arquivo: " + config_file);
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        YAML::Node canvas = screen["canvas"];

        // Controlador de video
        Chimera::CanvasGL* video =
            new Chimera::CanvasGL(screen["name"].as<std::string>(), canvas["w"].as<int>(), canvas["h"].as<int>());

        // Gerenciador do grapho de cena
        Chimera::SceneMng* sceneMng = new Chimera::SceneMng();

        ChimeraLoaders::ShadersLoader sl; // = new ChimeraLoaders::ShadersLoader();

        YAML::Node shaders = config["shaders"];
        // Chimera::ShadersManager *shader =  sceneMng->getShadersManager();
        console->info("Shaders identificados: %d", shaders.size());
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

        console->info("Loop de Game encerrado!!!!");
        delete pControle;
        delete game;
        delete sceneMng;

        delete pShadowMap;

        delete video;

    } catch (const Chimera::Exception& ex) {
        console->error("Falha grave: " + ex.getMessage());
        return -1;
    } catch (const std::exception& ex) { console->error("Falha grave: %s", ex.what()); } catch (const std::string& ex) {
        console->error("Falha grave: " + ex);
    } catch (...) { console->error("Falha Desconhecida"); }

    console->info("AppShader finalizado com sucesso");
    return 0;
}
