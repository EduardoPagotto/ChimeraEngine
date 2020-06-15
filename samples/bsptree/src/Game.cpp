#include "Game.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/render/LoadObj.hpp"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

Game::Game(Chimera::CanvasGL* _pCanvas, Chimera::Shader* _pShader) : pCanvas(_pCanvas), pShader(_pShader) {
    isPaused = false;
    debugParser = false;

    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);

    pTex = new Chimera::TextureImg(SHADE_TEXTURE_DIFFUSE, "./data/images/grid2.png");
}

Game::~Game() { bspTree.destroy(); }

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
            debugParser = true;
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
        else if (botaoIndex == 3)
            trackBall.offSet(mm.yrel);
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

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    pTex->init();

    // unsigned char map[] = {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // marca
    //                        0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0,  // marca
    //                        0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,  // marca
    //                        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1,  // marca
    //                        0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,  // marca
    //                        0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,  // marca
    //                        0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1,  // marca
    //                        0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
    //                        1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
    //                        0, 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
    //                        0, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1,  // marca
    //                        0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 1,  // marca
    //                        0, 1, 0, 1, 1, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,  // marca
    //                        1, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1,  // marca
    //                        1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
    //                        1, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
    //                        1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
    //                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // marca

    Chimera::MeshData m;
    std::vector<Chimera::Triangle> listPolygons;

    // initPolygons(map, &listPolygons);

    Chimera::LoaderObj loader;
    // loader.getMesh("./data/models/tela01.obj", m); // quadrado simples pequeno
    loader.getMesh("./data/models/map02.obj", m); // Sala com 5 espacos em forma de X
    // loader.getMesh("./data/models/parede_simples.obj", m); // FIXME Falha para EPSILON 1e-2
    // loader.getMesh("./data/models/square2.obj", m); // 2 quadrado teste de split lateral
    // loader.getMesh("./data/models/square1.obj", m); // 2 quadrado teste de split centro
    // loader.getMesh("./data/models/split1.obj", m); // 2 triangulos sem textura
    // loader.getMesh("./data/models/cubo_textura_simples.obj", m);
    // m.changeSize(30.0, true);

    // std::vector<unsigned int> indexTriangles;
    // m.toTriangle(listPolygons, indexTriangles);
    // indexTriangles.clear(); // is sequential, not used here
    // std::reverse(listPolygons.begin(), listPolygons.end());

    // Cria lista multiplo Index
    std::vector<Chimera::VertexData> vVertexSequencial;
    m.toVertexData(vVertexSequencial);

    // // Cria BSP usando Vertex indexado
    // std::vector<Chimera::VertexData> vVertexIndexed;
    // std::vector<unsigned int> vIndex;
    // vertexDataIndexCompile(vVertexSequencial, vVertexIndexed, vIndex);
    // bspTree.createIndexed(vVertexIndexed, vIndex);
    // // -------

    // Cria o BSP Sequencial sem indice
    bspTree.createSequencial(vVertexSequencial);
    // -------

    renderDynamic.create(15000000);
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

    if (debugParser == true) {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", vp->position.x, vp->position.y, vp->position.z);
    }

    // constroi vertex dinamico baseado no viewpoint
    std::vector<Chimera::VertexData> vVertice;
    bspTree.render(&vp->position, &vVertice, debugParser);

    // debugParser = false;

    pShader->link();

    // Calcula view e projection baseado em vp
    pCanvas->calcPerspectiveProjectionView(0, vp, view, projection);

    pShader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

    // aplica a textura
    pTex->apply(pShader);

    renderDynamic.render(vVertice);

    pCanvas->after();
    pCanvas->swapWindow();
}

// void Game::addPolygon(Chimera::VertexData* pVertexs, int NOV, std::vector<Triangle*>& _vTriangle) {

//     // TODO: Testar!!!!!!
//     int loop;
//     int v0, v1, v2;
//     int NumberOfIndices = (NOV - 2) * 3;

//     std::vector<unsigned int> vIndex;

//     for (loop = 0; loop < NumberOfIndices / 3; loop++) {
//         if (loop == 0) {
//             v0 = 0;
//             v1 = 1;
//             v2 = 2;
//         } else {
//             v1 = v2;
//             v2++;
//         }

//         // Calcula Normal
//         glm::vec3 u = pVertexs[1].position - pVertexs[0].position;
//         glm::vec3 v = pVertexs[2].position - pVertexs[0].position;
//         glm::vec3 normal = glm::normalize(glm::cross(u, v));

//         pVertexs[0].normal = normal;
//         pVertexs[1].normal = normal;
//         pVertexs[2].normal = normal;

//         vVertex.push_back(pVertexs[0]);
//         vVertex.push_back(pVertexs[1]);
//         vVertex.push_back(pVertexs[2]);

//         _vTriangle.push_back(new Triangle(v0, v1, v2, normal));
//     }
// }

// void Game::initPolygons(unsigned char* map) {

//     // TODO: Testar!!!!!!
//     std::vector<Triangle*> vTriangle;

//     Chimera::VertexData vl[4][4];
//     int direction[4] = {0};
//     for (int y = 0; y < 40; y++) {
//         for (int x = 0; x < 20; x++) {
//             int offset = (y * 20) + x;
//             // check what the digit is in the current map location
//             if (map[offset] != 0) {
//                 if (map[offset] == 2) { // North East Wall
//                     vl[0][0] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
//                     vl[0][1] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
//                     vl[0][2] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
//                     vl[0][3] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
//                     direction[0] = 1;
//                 }
//                 if (map[offset] == 3) { // North West Wall
//                     vl[0][0] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
//                     vl[0][1] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
//                     vl[0][2] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
//                     vl[0][3] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
//                     direction[0] = 1;
//                 }

//                 if (map[offset] == 1) { // Its a Standared wall
//                     if (x > 0) {
//                         if (map[offset - 1] == 0) { // if theres nothing to the left add a left facingwall
//                             vl[0][0] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
//                             vl[0][1] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
//                             vl[0][2] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
//                             vl[0][3] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
//                             direction[0] = 1;
//                         }
//                     }
//                     if (x < 19) {
//                         if (map[offset + 1] == 0) { // if there is nothing to the right add a right facing wall
//                             vl[1][0] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
//                             vl[1][1] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
//                             vl[1][2] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
//                             vl[1][3] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
//                             direction[1] = 1;
//                         }
//                     }
//                     if (y > 0) {
//                         if (map[offset - 20] == 0) { // if there is nothing south add a south facing wall
//                             vl[2][0] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
//                             vl[2][1] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
//                             vl[2][2] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
//                             vl[2][3] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) + 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
//                             direction[2] = 1;
//                         }
//                     }
//                     if (y < 39) {
//                         if (map[offset + 20] == 0) { // if there is nothing to the north add a north facing wall
//                             vl[3][0] = {glm::vec3(x - 10.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 0)};
//                             vl[3][1] = {glm::vec3(x - 9.5f, 3.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 0)};
//                             vl[3][2] = {glm::vec3(x - 9.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(1, 1)};
//                             vl[3][3] = {glm::vec3(x - 10.5f, 0.0f, (20.0f - y) - 0.5f), glm::vec3(0.0), glm::vec2(0, 1)};
//                             direction[3] = 1;
//                         }
//                     }
//                 } // end for if offset==1

//                 // build the polygons
//                 for (int a = 0; a < 4; a++) {
//                     if (direction[a] != 0)
//                         addPolygon(&vl[a][0], 4, vTriangle);
//                 }
//             } // end for if offset!=0
//         }
//     }
//     root = bsptreeBuild(vTriangle);
//     // BSPTreeRootNode = new NODE;
//     // BuildBspTree(BSPTreeRootNode, PolygonList);
// }