#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/TextureManager.hpp"
#include "chimera/core/io/MouseDevice.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/render/LoadHeightMap.hpp"
#include <glm/gtc/type_ptr.hpp>

Game::Game(Chimera::Canvas* canvas) : Application(canvas) {
    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);

    pShader = shaderLibrary.load("./assets/shaders/MeshFullShadow.glsl");

    pMaterial = new Chimera::Material();
    pMaterial->setDefaultEffect();
    pMaterial->setShine(50.0f);

    Chimera::TextureManager::loadFromFile("grid2", "./assets/textures/grid2.png", Chimera::TextureParameters());
    pMaterial->addTexture(SHADE_TEXTURE_DIFFUSE, Chimera::TextureManager::getLast());

    Chimera::ViewPoint* pVp = new Chimera::ViewPoint();
    pVp->position = glm::vec3(0.0, 0.0, 600.0);
    pVp->front = glm::vec3(0.0, 0.0, 0.0);
    pVp->up = glm::vec3(0.0, 1.0, 0.0);
    trackBall.init(pVp);
    trackBall.setMax(1000.0);

    // Light
    pLight = new Chimera::Light();
    pLight->setDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    pLight->setAmbient(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
    pLight->setPosition(glm::vec3(0, 150, 0));

    pHeightMap = nullptr;
}

Game::~Game() { delete pShader; }

void Game::onStart() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color

    // whitou the z-buffer
    // glEnable(GL_COLOR_MATERIAL);
    // glEnable(GL_NORMALIZE);
    // glShadeModel(GL_SMOOTH);

    // enable z-buffer here
    canvas->afterStart();

    // pTex->init();
    pMaterial->init();

    Chimera::MeshData m;

    Chimera::LoadHeightMap loader(32, 32);
    loader.getMesh("./assets/heightmaps/terrain3.jpg", m, glm::vec3(1000.0, 200.0, 1000.0));
    // loader.getMesh("./assets/heightmaps/heightmap_16x16.png", m, glm::vec3(100.0, 30.0, 100.0));
    // loader.getMesh("./assets/heightmaps/heightmap_4x4.png", m, glm::vec3(1000.0, 10.0, 1000.0));
    loader.split(m.vertexIndex);

    std::vector<Chimera::VertexData> vertexDataIn;
    vertexDataFromMesh(&m, vertexDataIn);

    pHeightMap = new Chimera::RenderableIndex(loader.vNodes, vertexDataIn);
}

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;

    switch (event.type) {
        case SDL_USEREVENT: {
            switch (event.user.code) {
                case Chimera::EVENT_TOGGLE_FULL_SCREEN:
                    canvas->toggleFullScreen();
                    break;
            }

        } break;
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    Chimera::utilSendEvent(Chimera::EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_1:
                    render3d.logToggle();
                    break;
                case SDLK_F10:
                    Chimera::utilSendEvent(Chimera::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
                    break;
                default:
                    break;
            }
        } break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION: {
            if (Chimera::MouseDevice::getButtonState(1) == SDL_PRESSED) {
                if (event.type == SDL_MOUSEMOTION) {
                    trackBall.tracking(event.motion.xrel, event.motion.yrel);
                }
            } else if (Chimera::MouseDevice::getButtonState(3) == SDL_PRESSED) {

                if (event.type == SDL_MOUSEMOTION) {
                    trackBall.offSet(event.motion.yrel);
                }
            }
        } break;
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_ENTER:
                    Chimera::utilSendEvent(Chimera::EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    Chimera::utilSendEvent(Chimera::EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    canvas->reshape(event.window.data1, event.window.data2);
                    break;
            }
        } break;
    }
    return true;
}

void Game::onUpdate() {
    canvas->before();

    Chimera::ViewPoint* vp = trackBall.getViewPoint();
    if (render3d.getLog() == true) {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", vp->position.x, vp->position.y, vp->position.z);
    }

    pShader->enable();

    // Calcula view e projection baseado em vp
    canvas->calcPerspectiveProjectionView(0, vp, view, projection);

    glm::mat4 projectionMatrixInverse = glm::inverse(projection);
    glm::mat4 viewMatrixInverse = glm::inverse(view);
    glm::mat4 viewProjectionMatrixInverse = viewMatrixInverse * projectionMatrixInverse;
    frustum.set(viewProjectionMatrixInverse);

    pLight->setUniform(pShader);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0)); //_pMesh->getTransform()->getModelMatrix(pTransform->getPosition());
    if (pShader == nullptr)
        return;

    int shadows = 0;
    pShader->setUniform1i("shadows", shadows);

    pShader->setUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

    // aplica material ao shader
    pMaterial->setUniform(pShader);

    // NEW
    render3d.begin(&frustum);
    pHeightMap->submit(&render3d); // render3d.submit(pHeightMap);
    render3d.end();

    render3d.flush();

    canvas->after();
    canvas->swapWindow();
}
