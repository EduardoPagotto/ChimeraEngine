#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/render/LoadObj.hpp"
#include <glm/gtc/type_ptr.hpp>

Game::Game(Chimera::CanvasGL* _pCanvas, Chimera::Shader* _pShader) : pCanvas(_pCanvas), pShader(_pShader) {
    debug_init = 0;
    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);
}

Game::~Game() {}

void Game::joystickEvent(Chimera::IO::JoystickState* pJoy, SDL_Event* pEventSDL) {}

void Game::keboardEvent(SDL_Keycode tecla) {

    switch (tecla) {
        case SDLK_ESCAPE:
            Chimera::IO::utilSendEvent(Chimera::IO::EVENT_FLOW_STOP, nullptr, nullptr);
            break;
        case SDLK_1:
            debug_init = 1;
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "iniciado log");
            break;
        case SDLK_F10:
            Chimera::IO::utilSendEvent(Chimera::IO::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
            break;
        default:
            break;
    }
}

void Game::mouseEvent(Chimera::IO::MouseDevice* pMouse, SDL_Event* pEventSDL) {

    if (pMouse->getButtonState(1) == SDL_PRESSED) {
        if (pEventSDL->type == SDL_MOUSEMOTION) {
            trackBall.tracking(pEventSDL->motion.xrel, pEventSDL->motion.yrel);
        }
    } else if (pMouse->getButtonState(3) == SDL_PRESSED) {
        if (pEventSDL->type == SDL_MOUSEMOTION) {
            trackBall.offSet(pEventSDL->motion.yrel);
        }
    }
}

void Game::start() {

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
        material.addTexture(new Chimera::TextureImg(SHADE_TEXTURE_DIFFUSE, "./data/images/grid2.png"));
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

void Game::userEvent(const SDL_Event& _event) {
    switch (_event.user.code) {
        case Chimera::IO::EVENT_TOGGLE_FULL_SCREEN:
            pCanvas->toggleFullScreen();
            break;
        case Chimera::IO::EVENT_FLOW_START:
            this->start();
        default:
            break;
    }
}

void Game::windowEvent(const SDL_WindowEvent& _event) {
    switch (_event.event) {
        case SDL_WINDOWEVENT_ENTER:
            Chimera::IO::utilSendEvent(Chimera::IO::EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
            break;
        case SDL_WINDOWEVENT_LEAVE:
            Chimera::IO::utilSendEvent(Chimera::IO::EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
            break;
        case SDL_WINDOWEVENT_RESIZED:
            pCanvas->reshape(_event.data1, _event.data2);
            break;
        default:
            break;
    }
}

void Game::update() {
    pCanvas->before();

    Chimera::ViewPoint* vp = trackBall.getViewPoint();

    pShader->enable();

    // Calcula view e projection baseado em vp
    pCanvas->calcPerspectiveProjectionView(0, vp, view, projection);

    pShader->setUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

    material.apply(pShader);
    // aplica a textura
    // pTex->apply(0, "material.tDiffuse", pShader);

    render3D.submit(rederable);
    render3D.flush();

    pCanvas->after();
    pCanvas->swapWindow();
}
