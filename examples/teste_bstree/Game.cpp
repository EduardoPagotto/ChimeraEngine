#include "Game.hpp"
#include "BSPTreeBuilder.hpp"
#include "chimera/core/Events.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/node/Transform.hpp"
#include <glm/gtc/type_ptr.hpp>

Game::Game(Chimera::CanvasGL* _pCanvas) : pCanvas(_pCanvas) {
    isPaused = false;
    debug_init = 0;
    log = Chimera::Logger::get();
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
            log->debug("iniciado log");
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

void Game::setCube(ListPolygon* _pPolygonList) {
    Polygon p[10];
    glm::vec3 v[10];
    glm::vec3 n[10];
    glm::vec3 c[10];
    v[0] = glm::vec3(-100, -100, 100);
    v[1] = glm::vec3(-100, 100, 100);
    v[2] = glm::vec3(100, 100, 100);
    v[3] = glm::vec3(100, -100, 100);
    v[4] = glm::vec3(-100, -100, -100);
    v[5] = glm::vec3(-100, 100, -100);
    v[6] = glm::vec3(100, 100, -100);
    v[7] = glm::vec3(100, -100, -100);

    // Right Wall
    p[0].setVertices(v[2], v[3], v[7]);
    n[0] = glm::vec3(1, 0, 0);
    c[0] = glm::vec3(1, 1, 1);

    p[1].setVertices(v[2], v[7], v[6]);
    n[1] = glm::vec3(1, 0, 0);
    c[1] = glm::vec3(1, 1, 1);

    // Back Wall
    p[2].setVertices(v[4], v[5], v[6]);
    n[2] = glm::vec3(0, 0, -1);
    c[2] = glm::vec3(1, 1, 1);

    p[3].setVertices(v[4], v[6], v[7]);
    n[3] = glm::vec3(0, 0, -1);
    c[3] = glm::vec3(1, 1, 1);

    // Left Wall
    p[4].setVertices(v[5], v[4], v[0]);
    n[4] = glm::vec3(-1, 0, 0);
    c[4] = glm::vec3(1, 1, 1);

    p[5].setVertices(v[5], v[0], v[1]);
    n[5] = glm::vec3(-1, 0, 0);
    c[5] = glm::vec3(1, 1, 1);

    // Floor
    p[6].setVertices(v[3], v[7], v[4]);
    n[6] = glm::vec3(0, -1, 0);
    c[6] = glm::vec3(1, 1, 1);

    p[7].setVertices(v[3], v[4], v[0]);
    n[7] = glm::vec3(0, -1, 0);
    c[7] = glm::vec3(1, 1, 1);

    // Ceiling
    p[8].setVertices(v[2], v[6], v[5]);
    n[8] = glm::vec3(0, 1, 0);
    c[8] = glm::vec3(1, 1, 1);

    p[9].setVertices(v[2], v[5], v[1]);
    n[9] = glm::vec3(0, 1, 0);
    c[9] = glm::vec3(1, 1, 1);

    for (int i = 0; i < 10; i++) {
        p[i].setNormais(n[i], n[i], n[i]);
        p[i].setColor(c[i]);
        p[i].setId(polygon_id++);
        _pPolygonList->addToList(&p[i]);
    }
}

void Game::setOctahedran(ListPolygon* _pPolygonList) {
    glm::vec3 p[6];
    Polygon t[8];

    p[0] = glm::vec3(0, 50, 0);
    p[1] = glm::vec3(0, 0, -50);
    p[2] = glm::vec3(-50, 0, 0);
    p[3] = glm::vec3(50, 0, 0);
    p[4] = glm::vec3(0, 0, 50);
    p[5] = glm::vec3(0, -50, 0);

    t[0].setVertices(p[0], p[1], p[2]);
    t[1].setVertices(p[0], p[1], p[3]);
    t[2].setVertices(p[0], p[4], p[3]);
    t[3].setVertices(p[0], p[4], p[2]);
    t[4].setVertices(p[5], p[4], p[3]);
    t[5].setVertices(p[5], p[3], p[1]);
    t[6].setVertices(p[5], p[1], p[2]);
    t[7].setVertices(p[5], p[2], p[4]);

    for (int i = 0; i < 8; i++) {
        t[i].setId(polygon_id++);
        t[i].setColor(glm::vec3(1, 0, 0));
        t[i].computeFaceNormalsFromVertices();
        _pPolygonList->addToList(&t[i]);
    }
}

void Game::setDrawTest(ListPolygon* _pPolygonList) {

    Polygon p[14];
    glm::vec3 v[20];
    glm::vec3 n[20];
    glm::vec3 c[10];
    glm::ivec3 index[10];

    v[0] = glm::vec3(100, 100, 100);
    v[1] = glm::vec3(-100, 100, 100);
    v[2] = glm::vec3(-100, -100, 100);
    v[3] = glm::vec3(100, -100, 100);

    v[4] = glm::vec3(50, 100, 0);
    v[5] = glm::vec3(250, 100, 0);
    v[6] = glm::vec3(50, -100, 0);
    v[7] = glm::vec3(250, -100, 0);

    v[8] = glm::vec3(-250, 100, -100);
    v[9] = glm::vec3(-50, 100, -100);
    v[10] = glm::vec3(-250, -100, -100);
    v[11] = glm::vec3(-50, -100, -100);

    v[12] = glm::vec3(100, 100, -200);
    v[13] = glm::vec3(-100, 100, -200);
    v[14] = glm::vec3(-100, -100, -200);
    v[15] = glm::vec3(100, -100, -200);

    v[16] = glm::vec3(900, 100, -500);
    v[17] = glm::vec3(-900, 100, -500);
    v[18] = glm::vec3(-900, -100, -500);
    v[19] = glm::vec3(900, -100, -500);

    n[0] = glm::vec3(0, 0, 1);
    n[1] = glm::vec3(0, 0, 1);
    n[2] = glm::vec3(0, 0, 1);
    n[3] = glm::vec3(0, 0, 1);

    n[4] = glm::vec3(0, 0, 1);
    n[5] = glm::vec3(0, 0, 1);
    n[6] = glm::vec3(0, 0, 1);
    n[7] = glm::vec3(0, 0, 1);

    n[8] = glm::vec3(0, 0, 1);
    n[9] = glm::vec3(0, 0, 1);
    n[10] = glm::vec3(0, 0, 1);
    n[11] = glm::vec3(0, 0, 1);

    n[12] = glm::vec3(0, 0, 1);
    n[13] = glm::vec3(0, 0, 1);
    n[14] = glm::vec3(0, 0, 1);
    n[15] = glm::vec3(0, 0, 1);

    n[16] = glm::vec3(0, 0, 1);
    n[17] = glm::vec3(0, 0, 1);
    n[18] = glm::vec3(0, 0, 1);
    n[19] = glm::vec3(0, 0, 1);

    index[0] = glm::ivec3(0, 1, 2);
    index[1] = glm::ivec3(2, 3, 0);

    index[2] = glm::ivec3(5, 4, 6);
    index[3] = glm::ivec3(6, 7, 5);

    index[4] = glm::ivec3(9, 8, 10);
    index[5] = glm::ivec3(10, 11, 9);

    index[6] = glm::ivec3(12, 13, 14);
    index[7] = glm::ivec3(14, 15, 12);

    index[8] = glm::ivec3(16, 17, 18);
    index[9] = glm::ivec3(18, 19, 16);

    c[0] = glm::vec3(1, 1, 1);
    c[1] = glm::vec3(1, 1, 1);
    c[2] = glm::vec3(1, 0, 0);
    c[3] = glm::vec3(1, 0, 0);
    c[4] = glm::vec3(0, 0, 1);
    c[5] = glm::vec3(0, 0, 1);
    c[6] = glm::vec3(0, 1, 0);
    c[7] = glm::vec3(0, 1, 0);
    c[8] = glm::vec3(1, 1, 0);
    c[9] = glm::vec3(1, 1, 0);

    for (int face = 0; face < 10; face++) {
        p[face].setId(face);
        p[face].setVertices(v[index[face].x], v[index[face].y], v[index[face].z]);
        p[face].setColor(c[face]);
        p[face].setNormais(n[index[face].x], n[index[face].y], n[index[face].z]);
        _pPolygonList->addToList(&p[face]);
    }
}

// void Game::setDrawTest(ListPolygon* _pPolygonList) {

//     Polygon p[6];
//     glm::vec3 v[7];
//     glm::vec3 n[7];
//     glm::vec3 c[6];
//     glm::ivec3 index[6];

//     v[0] = glm::vec3(100, 100, 100);
//     v[1] = glm::vec3(-100, 100, 100);
//     v[2] = glm::vec3(-100, -100, 100);
//     v[3] = glm::vec3(100, -100, 100);
//     v[4] = glm::vec3(100, -100, -100);
//     v[5] = glm::vec3(-100, -100, -100);
//     v[6] = glm::vec3(-100, 100, -100);

//     n[0] = glm::vec3(0, 0, -1);
//     n[1] = glm::vec3(0, 0, -1);
//     n[2] = glm::vec3(0, 0, -1);

//     n[3] = glm::vec3(0, 0, -1);
//     n[4] = glm::vec3(0, 0, -1);
//     n[5] = glm::vec3(0, 0, -1);

//     n[6] = glm::vec3(0, 0, -1);

//     index[0] = glm::ivec3(0, 1, 2);
//     index[1] = glm::ivec3(2, 3, 0);
//     index[2] = glm::ivec3(3, 2, 5);
//     index[3] = glm::ivec3(5, 4, 3);
//     index[4] = glm::ivec3(1, 2, 5);
//     index[5] = glm::ivec3(5, 6, 1);

//     c[0] = glm::vec3(1, 1, 1);
//     c[1] = glm::vec3(1, 1, 1);
//     c[2] = glm::vec3(1, 0, 0);
//     c[3] = glm::vec3(1, 0, 0);
//     c[4] = glm::vec3(0, 0, 1);
//     c[5] = glm::vec3(0, 0, 1);

//     for (int face = 0; face < 6; face++) {
//         p[face].setId(face);
//         p[face].setVertices(v[index[face].x], v[index[face].y], v[index[face].z]);
//         p[face].setColor(c[face]);
//         p[face].setNormais(n[index[face].x], n[index[face].y], n[index[face].z]);
//         _pPolygonList->addToList(&p[face]);
//     }
// }

void Game::start() {

    Chimera::ViewPoint* pVp = new Chimera::ViewPoint();
    pVp->position = glm::vec3(0.0, 0.0, 600.0);
    pVp->front = glm::vec3(0.0, 0.0, 0.0);
    pVp->up = glm::vec3(0.0, 1.0, 0.0);
    trackBall.init(pVp);
    trackBall.setMax(1000.0);

    lightPosition = glm::vec4(0.0, 100.0, 0.0, 1.0);

    polygon_id = 1;

    ListPolygon* pPolygonList = new ListPolygon();
    // setCube(pPolygonList);
    // setOctahedran(pPolygonList);
    setDrawTest(pPolygonList);

    BSPTreeBuilder builder(pPolygonList);
    pBspTree = new BSPTree(builder.getNodeRoot()); // buildBSPTree(pPolygonList);

    delete pPolygonList;
    pPolygonList = nullptr;

    pCanvas->initGL();

    // pCanvas->afterStart();
    glEnable(GL_COLOR_MATERIAL);

    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    // glDisable(GL_LIGHTING);
    // glCullFace(GL_BACK);
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

    glViewport(0, 0, pCanvas->getWidth(), pCanvas->getHeight());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(vp->fov, (GLfloat)(float)pCanvas->getWidth() / (float)pCanvas->getHeight(), vp->near, vp->far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // glm::vec3 dir = vp->position + vp->front;
    gluLookAt(vp->position.x, vp->position.y, vp->position.z, vp->front.x, vp->front.y, vp->front.z, vp->up.x, vp->up.y,
              vp->up.z);
    // gluLookAt(vp->position.x, vp->position.y, vp->position.z, dir.x, dir.y, dir.z, vp->up.x, vp->up.y, vp->up.z);
    // gluLookAt(vp->position.x, vp->position.y, vp->position.z, vp->direction.x, vp->direction.y, vp->direction.z,
    //          vp->rotation.x, vp->rotation.y, vp->rotation.z);

    ListPolygon* finalpl = new ListPolygon();
    pBspTree->draw(&vp->position, finalpl);

    if (debug_init == 1)
        log->debug("eye: %0.2f; %0.3f; %0.3f", vp->position.x, vp->position.y, vp->position.z);

    finalpl->begin();

    Polygon* fi = nullptr;
    while ((fi = finalpl->next()) != NULL) {

        if (debug_init == 1)
            log->debug("Poligono: " + std::to_string(fi->getId()));

        glm::vec3 cc = fi->getColor();
        glColor3f(cc.x, cc.y, cc.z);
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 3; i++) {
            glNormal3f(fi->getNormais()[i].x, fi->getNormais()[i].y, fi->getNormais()[i].z);
            glVertex3f(fi->getVertices()[i].x, fi->getVertices()[i].y, fi->getVertices()[i].z);
        }
        glEnd();
    }

    debug_init = 0;

    delete finalpl;
    finalpl = nullptr;

    // GLfloat ambientColor[] = {0.4, 0.4, 0.4, 1};
    // glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    // GLfloat diffusedLightColor[] = {0.75, 0.75, 0.75, 1};
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusedLightColor);

    // GLfloat specularLightColor[] = {0.0, 1, 1, 1};
    // glLightfv(GL_LIGHT0, GL_SPECULAR, specularLightColor);

    // glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(lightPosition));

    pCanvas->after();
}
