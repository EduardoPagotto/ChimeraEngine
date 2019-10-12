#include "Game.hpp"
#include "BSPTreeBuilder.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/node/Transform.hpp"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

#include "chimera/core/LoadObj.hpp"

Game::Game(Chimera::CanvasGL* _pCanvas, Chimera::Shader* _pShader) : pCanvas(_pCanvas), pShader(_pShader) {
    isPaused = false;
    debug_init = 0;

    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);

    pTex = new Chimera::TexImg("./models/grid2.png");
}

Game::~Game() {}

void Game::joystickCapture(Chimera::JoystickManager& joy) {}

void Game::joystickStatus(Chimera::JoystickManager& joy) {}

void Game::keyCapture(SDL_Keycode tecla) {

    switch (tecla) {
        case SDLK_ESCAPE:
            SDL_Event l_eventQuit;
            l_eventQuit.type = SDL_QUIT;
            if (SDL_PushEvent(&l_eventQuit) == -1) {
                throw Chimera::Exception(std::string(SDL_GetError()));
            }
            break;
        case SDLK_1:
            debug_init = 1;

            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "iniciado log");
            break;
        case SDLK_F10:
            Chimera::eventsSend(Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN, nullptr, nullptr);
            break;
        default:
            break;
    }
}

void Game::mouseButtonUpCapture(SDL_MouseButtonEvent mb) {
    botaoIndex = mb.button;
    estadoBotao = mb.state;
}

void Game::mouseButtonDownCapture(SDL_MouseButtonEvent mb) {
    botaoIndex = mb.button;
    estadoBotao = mb.state;
}

void Game::mouseMotionCapture(SDL_MouseMotionEvent mm) {

    if (estadoBotao == SDL_PRESSED) {
        if (botaoIndex == 1)
            trackBall.tracking(mm.xrel, mm.yrel);
        else if (botaoIndex == 2)
            trackBall.offSet(mm.yrel);
    }
}

void Game::loadModelObj(const char* _file, std::vector<Chimera::Triangle>* _pListPolygon) {

    Chimera::MeshData m;
    loadObj(_file, &m);

    for (short indice = 0; indice < m.vertexIndex.size(); indice += 3) {

        Chimera::Triangle t = Chimera::Triangle(glm::vec3(0.0, 0.0, 0.0),  // A zerados carga em loop
                                                glm::vec3(0.0, 0.0, 0.0),  // B zerados carga em loop
                                                glm::vec3(0.0, 0.0, 0.0)); // C zerados carga em loop

        for (short tri = 0; tri < 3; tri++) {

            t.vertex[tri].position = m.vertexList[m.vertexIndex[indice + tri]];

            if (m.normalList.size() > 0)
                t.vertex[tri].normal = m.normalList[m.normalIndex[indice + tri]];

            if (m.colorList.size() > 0)
                t.vertex[tri].color = m.colorList[m.vertexIndex[indice + tri]];

            if (m.textureList.size() > 0)
                t.vertex[tri].texture = m.textureList[m.textureIndex[indice + tri]];
        }

        _pListPolygon->push_back(t);
        // debugTriangle(&t);
    }
}

void Game::start() {

    Chimera::ViewPoint* pVp = new Chimera::ViewPoint();
    pVp->position = glm::vec3(0.0, 0.0, 600.0);
    pVp->front = glm::vec3(0.0, 0.0, 0.0);
    pVp->up = glm::vec3(0.0, 1.0, 0.0);
    trackBall.init(pVp);
    trackBall.setMax(1000.0);

    pCanvas->initGL();

    // pCanvas->afterStart();
    glEnable(GL_COLOR_MATERIAL);

    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    // glDisable(GL_LIGHTING);
    // glCullFace(GL_BACK);

    pTex->init();

    std::vector<Chimera::Triangle> listPolygons;
    loadModelObj((const char*)"./samples/bsptree/models/square1.obj", &listPolygons);
    // loadModelObj((const char*)"./samples/bsptree/models/split1.obj", &listPolygons);
    // loadModelObj((const char*)"./samples/bsptree/models/teste1.obj", &listPolygons);

    std::reverse(listPolygons.begin(), listPolygons.end());

    BSPTreeBuilder builder(&listPolygons);
    pBspTree = new BSPTree(builder.getNodeRoot());

    vertexBuffer.create(5000);
}

void Game::stop() {}

void Game::newFPS(const unsigned int& fps) {}

void Game::userEvent(const SDL_Event& _event) {
    Chimera::KindOp op = (Chimera::KindOp)_event.user.code;
    if (op == Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN) {
        pCanvas->toggleFullScreen();
    }
}

void Game::windowEvent(const SDL_WindowEvent& _event) {
    switch (_event.event) {
        case SDL_WINDOWEVENT_ENTER:
            isPaused = false;
            break;
        case SDL_WINDOWEVENT_LEAVE:
            isPaused = true;
            break;
        case SDL_WINDOWEVENT_RESIZED:
            pCanvas->reshape(_event.data1, _event.data2);
            break;
    }
}

bool Game::paused() { return isPaused; }

void Game::render() {
    pCanvas->before();

    Chimera::ViewPoint* vp = trackBall.getViewPoint();

    std::vector<Chimera::Triangle> listPolygons;
    pBspTree->draw(&vp->position, &listPolygons);

    std::vector<int> listaDebug;
    std::vector<Chimera::VertexDataFull> vVertice;
    for (int face = 0; face < listPolygons.size(); face++) {
        listaDebug.push_back(listPolygons[face].getSerial());
        for (int i = 0; i < 3; i++) {
            Chimera::VertexDataFull vd = listPolygons[face].vertex[i];
            vVertice.push_back(vd);
        }
    }

    pShader->link();

    // Calcula view e projection baseado em vp
    pCanvas->calcPerspectiveProjectionView(0, vp, view, projection);

    pShader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

    // aplica a textura
    pTex->apply(0, "material.tDiffuse", pShader);

    // if (debug_init == 1) {
    //     debug_init = 0;
    //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Vertex size: %d", (int)vVertice.size());
    //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", vp->position.x, vp->position.y,
    //                  vp->position.z);
    //     for (int i = 0; i < listaDebug.size(); i++)
    //         SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Face: %d", listaDebug[i]);
    // }

    vertexBuffer.render(vVertice);

    pCanvas->after();
    pCanvas->swapWindow();
}
