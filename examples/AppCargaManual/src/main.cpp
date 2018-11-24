#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.hpp"
#endif

#include <iostream>

#include <map>

#include <VideoDevice.h>

#ifdef OVR_SET_TO_USE
#include "OvrDevice.hpp"
#endif

#include <Game.h>

#include <PhysicsScene.h>
#include <ShadersLoader.h>
#include <VisualScene.h>

#include <ExceptionChimera.h>
#include <FlowControl.h>
#include <HUD.h>
#include <ParticleEmitter.h>
#include <Singleton.h>
#include <Transform.h>

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif

    std::map<std::string, Chimera::Shader*> mapa;

    auto console = spdlog::stdout_color_st("chimera");
    console->info("AppShader Iniciado");

    spdlog::set_level(spdlog::level::debug);

    for (int i = 0; i < argn; i++) {
        console->info("Parametros {0}: {1}", i, argv[i]);
    }

    try {
        std::string config_file = "./examples/AppCargaManual/etc/appcargamanual.yaml";
        console->info("Carregar arquivo:{0}", config_file);
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        YAML::Node canvas = screen["canvas"];

        // Controlador de video
        Chimera::Video* video =
            new Chimera::VideoDevice(canvas["w"].as<int>(), canvas["h"].as<int>(),
                                     screen["name"].as<std::string>());

        // Gerenciador do grapho de cena
        Chimera::SceneMng* sceneMng = new Chimera::SceneMng();

        ChimeraLoaders::ShadersLoader sl; // = new ChimeraLoaders::ShadersLoader();

        YAML::Node shaders = config["shaders"];
        // Chimera::ShadersManager *shader =  sceneMng->getShadersManager();
        console->info("Shaders identificados: {0}", shaders.size());
        for (std::size_t i = 0; i < shaders.size(); i++) {
            YAML::Node shader_item = shaders[i];
            Chimera::Shader* pShader =
                sl.loadShader(shader_item["name"].as<std::string>(),
                              shader_item["vertex"].as<std::string>(),
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

        // Vincula o shader de mesh com sombra a scena carregada
        group1->setShader(mapa["mesh-default"]); // group1->setShaderName("mesh-default");
        group1->setShadowMap(new Chimera::ShadowMapVisitor("shadow1", 2048, 2048,
                                                           mapa["simpleDepthShader"]));

        // Novo Grupos com shader de Emissor particula GLSL de particula e o vincula a
        // cena e ao shader
        Chimera::Group* gParticle = new Chimera::Group(sceneMng, "ParticleGroup");
        gParticle->setShader(
            mapa["particle-default"]); // gParticle->setShaderName("particle-default");

        Chimera::Transform* posParticle =
            new Chimera::Transform(gParticle, "posicaoParticle");
        posParticle->setPosition(glm::vec3(-5.0, 5.0, 4.0));
        Chimera::ParticleEmitter* pParticleEmitter =
            new Chimera::ParticleEmitter(posParticle, "testeZ1", 10000);
        pParticleEmitter->loadTexDiffuse("TexParticleEmmiter",
                                         std::string("./models/Particle2.png"));

        // Novo Grupos com shader de um HUD ao Grapho da cena
        Chimera::Group* gHud = new Chimera::Group((Chimera::Node*)sceneMng, "HUD-Group");
        gHud->setShader(mapa["hud-default"]);
        Chimera::HUD* pHUD = new Chimera::HUD(gHud, "HUD-Default");
        Chimera::Font* pFont =
            new Chimera::Font(font, 18); // TODO: carregar size da fonte
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
        delete video;

    } catch (const Chimera::ExceptionBase& ex) {
        console->error("Falha grave:{0}", ex.getMessage());
        return -1;
    } catch (const std::exception& ex) {
        console->error("Falha grave:{0}", ex.what());
    } catch (const std::string& ex) {
        console->error("Falha grave:{0}", ex);
    } catch (...) { console->error("Falha Desconhecida"); }

    console->info("AppShader finalizado com sucesso");
    return 0;
}
