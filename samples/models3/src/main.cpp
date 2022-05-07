#include "chimera/core/Engine.hpp"
#include "chimera/core/collada/ColladaCanvas.hpp"
#include "chimera/core/collada/colladaLoad.hpp"
#include "chimera/core/visible/CameraOrthographic.hpp"
#include "chimera/core/visible/Components.hpp"
#include "chimera/core/visible/FontManager.hpp"
#include "chimera/core/visible/Transform.hpp"
#include "chimera/render/2d/BatchRender2D.hpp"
#include "chimera/render/2d/Tile.hpp"
#include "chimera/render/scene/Components.hpp"
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

        Chimera::Label* lFPS = nullptr;
        Scene scene;

        ColladaDom dom = loadFileCollada("./assets/models/nivel1.xml");
        CanvasGL* pCanvas = colladaCanvasGL(dom, scene.getRegistry());

        Engine engine(pCanvas);

        colladaRegistryLoad(dom, scene.getRegistry());

        colladaRenderLoad(dom, scene.getRegistry());

        { // FPS
            ComponentTile& tc = scene.getRegistry().findComponent<ComponentTile>("TileText");
            lFPS = new Label("None", -8, 0, glm::vec4(1.0, 1.0, 1.0, 1.0));
            tc.tile->add(lFPS);
            tc.tile->getCamera()->setViewportSize(pCanvas->getWidth(), pCanvas->getHeight());
            engine.pushState(tc.tile);
        }

        // TODO: TESTAR no ARQUIVO!!!!!
        EmitterFont* ef = new EmitterFont();
        { // Cria emissor de particula
            Entity re = scene.getRegistry().createEntity("Renderable Particle System");
            ComponentTrans& tc = re.addComponent<ComponentTrans>();
            tc.trans = new Transform();
            tc.trans->setPosition(glm::vec3(-5.0, 5.0, 4.0));

            ComponentMaterial& material = re.addComponent<ComponentMaterial>();
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
        scene.onViewportResize(pCanvas->getWidth(), pCanvas->getHeight()); // so depois

        Game* game = new Game(&scene);
        game->lFPS = lFPS;

        engine.pushState(game);
        engine.pushState(&scene);

        Collada::destroy(); // clean loader

        engine.run();

        SDL_Log("Loop de Game encerrado!!!!");
        delete game;

        delete pCanvas;

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
