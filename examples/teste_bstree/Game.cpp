#include "Game.hpp"
#include "chimera/core/Events.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/node/Transform.hpp"

Game::Game(Chimera::CanvasGL* _pCanvas) : pCanvas(_pCanvas) { isPaused = false; }

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
        case SDLK_F10:
            Chimera::eventsSend(Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN, nullptr, nullptr);
            break;
        default:
            break;
    }
}

void Game::mouseButtonUpCapture(SDL_MouseButtonEvent mb) {}

void Game::mouseButtonDownCapture(SDL_MouseButtonEvent mb) {}

void Game::mouseMotionCapture(SDL_MouseMotionEvent mm) {}

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
    c[6] = glm::vec3(0, 0, 0);

    p[7].setVertices(v[3], v[4], v[0]);
    n[7] = glm::vec3(0, -1, 0);
    c[7] = glm::vec3(0, 0, 0);

    // Ceiling
    p[8].setVertices(v[2], v[6], v[5]);
    n[8] = glm::vec3(0, 1, 0);
    c[8] = glm::vec3(0, 0, 0);

    p[9].setVertices(v[2], v[5], v[1]);
    n[9] = glm::vec3(0, 1, 0);
    c[9] = glm::vec3(0, 0, 0);

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

void Game::start() {

    lightPosition[0] = 0;
    lightPosition[1] = 99.9;
    lightPosition[2] = 0;
    lightPosition[3] = 1;

    polygon_id = 0;

    eyeX = 0;
    eyeY = 0;
    centerX = 0;
    centerY = 0;

    ListPolygon* pPolygonList = new ListPolygon();
    setCube(pPolygonList);
    setOctahedran(pPolygonList);
    pBspTree = buildBSPTree(pPolygonList);

    delete pPolygonList;
    pPolygonList = nullptr;

    pCanvas->initGL();

    // pCanvas->afterStart();
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    GLfloat ambientColor[] = {0.4, 0.4, 0.4, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    GLfloat diffusedLightColor[] = {0.75, 0.75, 0.75, 1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusedLightColor);
    GLfloat specularLightColor[] = {0.0, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
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
        case SDLK_r:
            eyeX -= 10;
            centerX -= 10;
            break;
        case SDLK_l:
            eyeX += 10;
            centerX += 10;
            break;
        case SDLK_u:
            eyeY -= 10;
            centerY -= 10;
            break;
        case SDLK_d:
            eyeY += 10;
            centerY += 10;
            break;
    }
}

bool Game::paused() { return isPaused; }

void Game::render() {
    pCanvas->before();

    // glClearColor(1, 1, 0.0, 1.0);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // orthographicMatrixMul();
    GLfloat left = -300, right = 300, top = 300, bottom = -300, near = 300, far = -300;
    GLfloat tx = -(right + left) / (right - left);
    GLfloat ty = -(top + bottom) / (top - bottom);
    GLfloat tz = -(far + near) / (far - near);
    GLfloat orthographicsMatrix[16] = {
        2 / (right - left), 0, 0, 0, 0, 2 / (top - bottom), 0, 0, 0, 0, -2 / (far - near), 0, tx, ty, tz, 1};
    glMultMatrixf(orthographicsMatrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // if (moveeye)
    //    gluLookAt(eyeX, eyeY, 75, centerX, centerY, 0, 0, 1, 0);
    // else
    gluLookAt(eyeX, eyeY, 75, 0, 0, 0, 0, 1, 0);

    // setLightingParameters();
    GLfloat ambientColor[] = {0.4, 0.4, 0.4, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    GLfloat diffusedLightColor[] = {0.75, 0.75, 0.75, 1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusedLightColor);
    GLfloat specularLightColor[] = {0.0, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glm::vec3 eye(eyeX, eyeY, 75);

    ListPolygon* finalpl = new ListPolygon();

    drawBSPTree(pBspTree->root, &eye, finalpl);

    finalpl->begin();

    Polygon* fi = nullptr;
    while ((fi = finalpl->next()) != NULL) {
        glm::vec3 cc = fi->getColor();
        glColor3f(cc.x, cc.y, cc.z);
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 3; i++) {
            glNormal3f(fi->getNormais()[i].x, fi->getNormais()[i].y, fi->getNormais()[i].z);
            glVertex3f(fi->getVertices()[i].x, fi->getVertices()[i].y, fi->getVertices()[i].z);
        }
        glEnd();
    }

    delete finalpl;
    finalpl = nullptr;

    // glutSwapBuffers();

    pCanvas->after();
}
