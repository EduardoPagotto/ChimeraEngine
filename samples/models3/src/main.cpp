#include "chimera/core/Engine.hpp"
#include "chimera/core/collada/colladaLoad.hpp"
#include "chimera/core/visible/Material.hpp"
#include "chimera/core/visible/Transform.hpp"
#include "chimera/render/2d/Tile.hpp"
#include "chimera/render/collada/ColladaRender.hpp"
#include <cstdio>
#include <iostream>
#include <map>

#ifdef OVR_SET_TO_USE
#include "chimera/render/CanvasOVR.hpp"
#else
#include "chimera/core/device/CanvasGL.hpp"
#endif
#include "Game.hpp"

int main(int argn, char** argv) {
    using namespace Chimera;
    try {
        // SDL_LogSetPriority(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_DEBUG);
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("AppShader Iniciado");

        Scene scene;

        ColladaDom dom = loadFileCollada("./assets/models/nivel1.xml");
        colladaRegistryLoad(dom, scene.getRegistry());
        colladaRenderLoad(dom, scene.getRegistry());

        Engine engine(scene.getCanvas());

        // TODO: TESTAR no ARQUIVO!!!!!
        EmitterFont* ef = new EmitterFont(glm::vec3(0.0f, 0.0f, 10.0f), 1.5f);
        { // Cria emissor de particula
            Entity re = scene.getRegistry().createEntity("Renderable Particle System");
            TransComponent& tc = re.addComponent<TransComponent>();
            tc.trans = new Transform();
            tc.trans->setPosition(glm::vec3(-5.0, 5.0, 4.0));

            MaterialComponent& material = re.addComponent<MaterialComponent>();
            material.material->addTexture(SHADE_TEXTURE_DIFFUSE,
                                          TextureManager::loadFromFile("Particle2", "./assets/textures/Particle2.png", TexParam()));
            material.material->init();

            Shader& shader = re.addComponent<Shader>();
            std::unordered_map<GLenum, std::string> shadeData;
            shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/ParticleEmitter.frag";
            shadeData[GL_VERTEX_SHADER] = "./assets/shaders/ParticleEmitter.vert";
            ShaderManager::load("ParticleEmitter", shadeData, shader);

            ParticleContainer& pc = re.addComponent<ParticleContainer>();
            pc.life = 4.0f;
            pc.max = 2000;

            ef->pushParticleContainer(&pc);
        }

        scene.pushEmitters(ef);
        scene.setShadowPass(new ShadowPass(2048, 2048, glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 1.0f, 150.0f)));

        Game* game = new Game(&scene, &engine);

        engine.pushState(game);
        engine.pushState(&scene);

        Collada::destroy(); // clean loader

        engine.run();

        SDL_Log("Loop de Game encerrado!!!!");
        delete game;

        SDL_Log("AppShader finalizado com sucesso");
        return 0;

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
