#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/TextureManager.hpp"
#include "chimera/core/io/MouseDevice.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/render/LoadObj.hpp"
#include <glm/gtc/type_ptr.hpp>

Game::Game(Chimera::Core::CanvasGL* _pCanvas, Chimera::Shader* _pShader) : pCanvas(_pCanvas), pShader(_pShader) {
    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);
}

Game::~Game() {}

void Game::onStart() {

    Chimera::ViewPoint* pVp = new Chimera::ViewPoint();
    pVp->position = glm::vec3(0.0, 0.0, 600.0);
    pVp->front = glm::vec3(0.0, 0.0, 0.0);
    pVp->up = glm::vec3(0.0, 1.0, 0.0);
    trackBall.init(pVp);
    trackBall.setMax(1000.0);

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color
    // Habilita o depth buffer/culling face
    pCanvas->afterStart();
    glEnable(GL_COLOR_MATERIAL);

    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    // glDisable(GL_LIGHTING);
    // glCullFace(GL_BACK);

    Chimera::MeshData m;
    Chimera::LoaderObj loader;
    // loader.getMesh("./data/models/tela01.obj", m);
    // loader.getMesh("./data/models/square2.obj", m);
    // loader.getMesh("./data/models/parede_simples.obj", m);
    // loader.getMesh("./data/models/cubo_textura_simples.obj", m);
    loader.getMesh("./data/models/map02.obj", m);
    // loader.getMesh("./data/models/salaSplit3.obj", m); // Sala L com Split

    if (loader.hasMaterial() == true)
        loader.getMaterial(material);
    else {
        material.setDefaultEffect();
        Chimera::TextureManager::loadFromFile("tex_mapa", "./data/images/grid2.png", Chimera::TextureParameters());
        material.addTexture(SHADE_TEXTURE_DIFFUSE, Chimera::TextureManager::getLast());
    }

    material.init();

    // m.changeSize(100.0, material.hasTexture());
    // m.textureFix();

    std::vector<Chimera::VertexData> renderData;
    vertexDataFromMesh(&m, renderData);

    std::vector<uint32_t> index;
    std::vector<Chimera::VertexData> vertexDataOut;
    vertexDataIndexCompile(renderData, vertexDataOut, index);
    this->rederable = new Chimera::RenderableStatic(&vertexDataOut[0], vertexDataOut.size(), &index[0], index.size());
}

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;
    using namespace Chimera::Core;
    switch (event.type) {
        case SDL_USEREVENT: {
            switch (event.user.code) {
                case Chimera::Core::EVENT_TOGGLE_FULL_SCREEN:
                    pCanvas->toggleFullScreen();
                    break;
            }

        } break;
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_1:
                    render3D.logToggle();
                    break;
                case SDLK_F10:
                    Chimera::Core::utilSendEvent(Chimera::Core::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
                    break;
                default:
                    break;
            }
        } break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION: {
            if (Chimera::Core::MouseDevice::getButtonState(1) == SDL_PRESSED) {
                if (event.type == SDL_MOUSEMOTION) {
                    trackBall.tracking(event.motion.xrel, event.motion.yrel);
                }
            } else if (Chimera::Core::MouseDevice::getButtonState(3) == SDL_PRESSED) {
                if (event.type == SDL_MOUSEMOTION) {
                    trackBall.offSet(event.motion.yrel);
                }
            }
        } break;
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_ENTER:
                    Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    pCanvas->reshape(event.window.data1, event.window.data2);
                    break;
            }
        } break;
    }
    return true;
}

void Game::onUpdate() {
    pCanvas->before();

    Chimera::ViewPoint* vp = trackBall.getViewPoint();

    pShader->enable();

    // Calcula view e projection baseado em vp
    pCanvas->calcPerspectiveProjectionView(0, vp, view, projection);

    glm::mat4 projectionMatrixInverse = glm::inverse(projection);
    glm::mat4 viewMatrixInverse = glm::inverse(view);
    glm::mat4 viewProjectionMatrixInverse = viewMatrixInverse * projectionMatrixInverse;
    frustum.set(viewProjectionMatrixInverse);

    pShader->setUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

    material.setUniform(pShader);
    // aplica a textura
    // pTex->apply(0, "material.tDiffuse", pShader);

    render3D.begin(&frustum);
    rederable->submit(&render3D);
    render3D.end();

    render3D.flush();

    pCanvas->after();
    pCanvas->swapWindow();
}
