#include "Game.hpp"
#include "BSPTreeBuilder.hpp"
#include "chimera/core/Events.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/node/Transform.hpp"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

Game::Game(Chimera::CanvasGL* _pCanvas, Chimera::Shader* _pShader) : pCanvas(_pCanvas), pShader(_pShader) {
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

void Game::setDrawSplit(std::vector<Triangle>* _pListPolygon) {

    std::vector<glm::vec3> vVertice;
    std::vector<glm::vec3> vNormal;
    std::vector<glm::ivec3> vIndex;
    std::vector<glm::vec3> vColor;

    // T 0
    vVertice.push_back(glm::vec3(-250, -100, 0));
    vVertice.push_back(glm::vec3(-50, 100, 0));
    vVertice.push_back(glm::vec3(-50, -100, 0));

    // T 1
    vVertice.push_back(glm::vec3(50, -100, -250));
    vVertice.push_back(glm::vec3(50, 100, 100));
    vVertice.push_back(glm::vec3(50, -100, 100));

    // Face 0
    vIndex.push_back(glm::ivec3(0, 1, 2));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(1, 1, 1));

    // Face 1
    vIndex.push_back(glm::ivec3(3, 4, 5));
    vNormal.push_back(glm::vec3(-1, 0, 0));
    vColor.push_back(glm::vec3(1, 0, 0));

    for (int face = 0; face < 2; face++) {
        Triangle t = Triangle(vVertice[vIndex[face].x], vVertice[vIndex[face].y], vVertice[vIndex[face].z]);

        for (int i = 0; i < 3; i++) {
            t.vertex[i].color = vColor[face];
            t.vertex[i].normal = vNormal[face];
        }
        _pListPolygon->push_back(t);
    }
}

void Game::setDrawTest(std::vector<Triangle>* _pListPolygon) {

    std::vector<glm::vec3> vVertice;
    std::vector<glm::vec3> vNormal;
    std::vector<glm::ivec3> vIndex;
    std::vector<glm::vec3> vColor;

    // plano 0
    vVertice.push_back(glm::vec3(100, 100, 100));
    vVertice.push_back(glm::vec3(-100, 100, 100));
    vVertice.push_back(glm::vec3(-100, -100, 100));
    vVertice.push_back(glm::vec3(100, -100, 100));

    // plano 1
    vVertice.push_back(glm::vec3(50, 100, 0));
    vVertice.push_back(glm::vec3(250, 100, 0));
    vVertice.push_back(glm::vec3(50, -100, 0));
    vVertice.push_back(glm::vec3(250, -100, 0));

    // plano 2
    vVertice.push_back(glm::vec3(-250, 100, -100));
    vVertice.push_back(glm::vec3(-50, 100, -100));
    vVertice.push_back(glm::vec3(-250, -100, -100));
    vVertice.push_back(glm::vec3(-50, -100, -100));

    // plano 3
    vVertice.push_back(glm::vec3(100, 100, -200));
    vVertice.push_back(glm::vec3(-100, 100, -200));
    vVertice.push_back(glm::vec3(-100, -100, -200));
    vVertice.push_back(glm::vec3(100, -100, -200));

    // plano 4
    vVertice.push_back(glm::vec3(900, 100, -500));
    vVertice.push_back(glm::vec3(-900, 100, -500));
    vVertice.push_back(glm::vec3(-900, -100, -500));
    vVertice.push_back(glm::vec3(900, -100, -500));

    // Face 0
    vIndex.push_back(glm::ivec3(0, 1, 2));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(1, 1, 1));
    // Face 1
    vIndex.push_back(glm::ivec3(2, 3, 0));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(1, 1, 1));

    // Face 2
    vIndex.push_back(glm::ivec3(5, 4, 6));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(1, 0, 0));
    // Face 3
    vIndex.push_back(glm::ivec3(6, 7, 5));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(1, 0, 0));

    // Face 4
    vIndex.push_back(glm::ivec3(9, 8, 10));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(0, 0, 1));
    // Face 5
    vIndex.push_back(glm::ivec3(10, 11, 9));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(0, 0, 1));

    // Face 6
    vIndex.push_back(glm::ivec3(12, 13, 14));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(0, 1, 0));
    // Face 7
    vIndex.push_back(glm::ivec3(14, 15, 12));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(0, 1, 0));

    // Face 8
    vIndex.push_back(glm::ivec3(16, 17, 18));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(1, 1, 0));
    // Face 9
    vIndex.push_back(glm::ivec3(18, 19, 16));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(1, 1, 0));

    for (int face = 0; face < 10; face++) {
        Triangle t = Triangle(vVertice[vIndex[face].x], vVertice[vIndex[face].y], vVertice[vIndex[face].z]);

        for (int i = 0; i < 3; i++) {
            t.vertex[i].color = vColor[face];
            t.vertex[i].normal = vNormal[face];
        }
        _pListPolygon->push_back(t);
    }
}

void Game::setSquare1(std::vector<Triangle>* _pListPolygon) {

    std::vector<glm::vec3> vVertice;
    std::vector<glm::vec3> vNormal;
    std::vector<glm::ivec3> vIndex;
    std::vector<glm::vec3> vColor;
    std::vector<glm::vec2> vTex;

    // Quadrado
    vVertice.push_back(glm::vec3(100, 100, 0));
    vVertice.push_back(glm::vec3(-100, 100, 0));
    vVertice.push_back(glm::vec3(-100, -100, 0));
    vVertice.push_back(glm::vec3(100, -100, 0));

    // Texturas
    vTex.push_back(glm::vec2(1, 1));
    vTex.push_back(glm::vec2(1, 0));
    vTex.push_back(glm::vec2(0, 0));
    vTex.push_back(glm::vec2(0, 1));

    // Face 0
    vIndex.push_back(glm::ivec3(0, 1, 2));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(1, 1, 1));

    // Face 1
    vIndex.push_back(glm::ivec3(2, 3, 0));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(1, 0, 0));

    for (int face = 0; face < 2; face++) {
        Triangle t = Triangle(vVertice[vIndex[face].x], vVertice[vIndex[face].y], vVertice[vIndex[face].z]);

        t.vertex[0].texture = vTex[vIndex[face].x];
        t.vertex[1].texture = vTex[vIndex[face].y];
        t.vertex[2].texture = vTex[vIndex[face].z];

        for (int i = 0; i < 3; i++) {

            t.vertex[i].color = vColor[face];
            t.vertex[i].normal = vNormal[face];
        }
        _pListPolygon->push_back(t);
    }
}

void Game::buildBuffer(int max) {

    // TODO: continua com doc: http://www.songho.ca/opengl/gl_vbo.html#create
    // https://www.khronos.org/opengl/wiki/Vertex_Rendering

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, max, nullptr, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // texture coord attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // limpa dados
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(2);
}

void Game::start() {

    Chimera::ViewPoint* pVp = new Chimera::ViewPoint();
    pVp->position = glm::vec3(0.0, 0.0, 600.0);
    pVp->front = glm::vec3(0.0, 0.0, 0.0);
    pVp->up = glm::vec3(0.0, 1.0, 0.0);
    trackBall.init(pVp);
    trackBall.setMax(1000.0);

    std::vector<Triangle> listPolygons;
    setSquare1(&listPolygons);
    std::reverse(listPolygons.begin(), listPolygons.end());

    BSPTreeBuilder builder(&listPolygons);
    pBspTree = new BSPTree(builder.getNodeRoot());

    buildBuffer(1000);

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

    std::vector<Triangle> listPolygons; // = new listPolygons();
    pBspTree->draw(&vp->position, &listPolygons);

    std::vector<VertexData> vVertice;
    for (int face = 0; face < listPolygons.size(); face++) {
        for (int i = 0; i < 3; i++) {
            VertexData vd = listPolygons[face].vertex[i];
            vVertice.push_back(vd);
        }
    }

    // TODO: link o shader aqui!!!
    // TODO: Colocar viewmatrix no shade

    glBindVertexArray(VAO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vVertice.size() * sizeof(VertexData), &vVertice[0]);

    if (debug_init == 1)
        log->debug("eye: %0.2f; %0.3f; %0.3f", vp->position.x, vp->position.y, vp->position.z);

    glDrawArrays(GL_TRIANGLES, 0, vVertice.size() * 3); //?? https://www.youtube.com/watch?v=S_xUgzFMIso

    glBindVertexArray(0);

    // for (auto it = listPolygons.begin(); it != listPolygons.end(); it++) {

    //     Triangle* fi = &(*it);

    //     if (debug_init == 1)
    //         log->debug("Poligono: " + std::to_string(fi->getSerial()));

    //     glBegin(GL_TRIANGLES);
    //     for (int i = 0; i < 3; i++) {
    //         glm::vec3 cc = fi->vertex[i].color;
    //         glColor3f(cc.x, cc.y, cc.z);
    //         glNormal3f(fi->vertex[i].normal.x, fi->vertex[i].normal.y, fi->vertex[i].normal.z);
    //         glVertex3f(fi->vertex[i].position.x, fi->vertex[i].position.y, fi->vertex[i].position.z);
    //     }
    //     glEnd();
    // }
    debug_init = 0;

    // GLfloat ambientColor[] = {0.4, 0.4, 0.4, 1};
    // glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    // GLfloat diffusedLightColor[] = {0.75, 0.75, 0.75, 1};
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusedLightColor);

    // GLfloat specularLightColor[] = {0.0, 1, 1, 1};
    // glLightfv(GL_LIGHT0, GL_SPECULAR, specularLightColor);

    pCanvas->after();
}
