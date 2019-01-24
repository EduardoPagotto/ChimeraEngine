#include "Game.hpp"
#include "BSPTreeBuilder.hpp"
#include "chimera/core/Events.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/node/Transform.hpp"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

#define BUFFER_OFFSET(i) ((void*)(i))

Game::Game(Chimera::CanvasGL* _pCanvas, Chimera::Shader* _pShader) : pCanvas(_pCanvas), pShader(_pShader) {
    isPaused = false;
    debug_init = 0;

    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);

    pTex = new Chimera::TexImg("./models/grid2.png");

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

    // Face Triangulo Z
    // vVertice.push_back(glm::vec3(0, 400, 0));
    // vVertice.push_back(glm::vec3(-200, 0, 0));
    // vVertice.push_back(glm::vec3(200, 0, 0));
    // vTex.push_back(glm::vec2(0.5, 0));
    // vTex.push_back(glm::vec2(0, 1));
    // vTex.push_back(glm::vec2(1, 1));

    // // Quadrado 1
    vVertice.push_back(glm::vec3(-100, -100, 0));
    vVertice.push_back(glm::vec3(100, -100, 0));
    vVertice.push_back(glm::vec3(100, 100, 0));
    vVertice.push_back(glm::vec3(-100, 100, 0));

    // // Quadrado 2
    vVertice.push_back(glm::vec3(200, -100, -100));
    vVertice.push_back(glm::vec3(200, -100, 100));
    vVertice.push_back(glm::vec3(200, 100, 100));
    vVertice.push_back(glm::vec3(200, 100, -100));

    // Texturas quad 1
    vTex.push_back(glm::vec2(0, 1));
    vTex.push_back(glm::vec2(1, 1));
    vTex.push_back(glm::vec2(1, 0));
    vTex.push_back(glm::vec2(0, 0));

    // Texturas quad 2
    vTex.push_back(glm::vec2(0, 1));
    vTex.push_back(glm::vec2(1, 1));
    vTex.push_back(glm::vec2(1, 0));
    vTex.push_back(glm::vec2(0, 0));

    // Face 0
    vIndex.push_back(glm::ivec3(0, 1, 2));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(1, 1, 1));

    // Face 1
    vIndex.push_back(glm::ivec3(2, 3, 0));
    vNormal.push_back(glm::vec3(0, 0, 1));
    vColor.push_back(glm::vec3(1, 0, 0));

    // Face 2
    vIndex.push_back(glm::ivec3(4, 5, 6));
    vNormal.push_back(glm::vec3(-1, 0, 0));
    vColor.push_back(glm::vec3(1, 1, 1));

    // // Face 3
    // vIndex.push_back(glm::ivec3(6, 7, 4));
    // vNormal.push_back(glm::vec3(-1, 0, 0));
    // vColor.push_back(glm::vec3(1, 0, 0));

    for (int face = 0; face < vIndex.size(); face++) {
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

    // refs: ver Particle.cpp in node sub-projetc
    // http://www.songho.ca/opengl/gl_vbo.html#create
    // https://www.khronos.org/opengl/wiki/Vertex_Rendering

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, max, nullptr, GL_STREAM_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(12));
    glEnableVertexAttribArray(1);

    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(24));
    glEnableVertexAttribArray(2);

    // texture coord attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(36));
    glEnableVertexAttribArray(3);

    // limpa dados
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
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

    std::vector<Triangle> listPolygons;
    setSquare1(&listPolygons);
    // setDrawSplit(&listPolygons);
    // setDrawTest(&listPolygons);

    std::reverse(listPolygons.begin(), listPolygons.end());

    BSPTreeBuilder builder(&listPolygons);
    pBspTree = new BSPTree(builder.getNodeRoot());

    buildBuffer(5000);
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

    std::vector<Triangle> listPolygons;
    pBspTree->draw(&vp->position, &listPolygons);

    std::vector<int> listaDebug;
    std::vector<VertexData> vVertice;
    for (int face = 0; face < listPolygons.size(); face++) {
        listaDebug.push_back(listPolygons[face].getSerial());
        for (int i = 0; i < 3; i++) {
            VertexData vd = listPolygons[face].vertex[i];
            vVertice.push_back(vd);
        }
    }

    pShader->link();

    projection = pCanvas->getPerspectiveProjectionMatrix(vp->fov, vp->near, vp->far, 0);
    view = glm::lookAt(vp->position, vp->front, vp->up); // View Matrix

    // pShader->setGlUniform3fv("viewPos", 1, glm::value_ptr(vp->position));
    pShader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

    // aplica a textura
    pTex->apply(0, "material.tDiffuse", pShader);

    if (debug_init == 1) {
        debug_init = 0;
        log->debug("Vertex size: %d", vVertice.size());
        log->debug("Eye: %0.2f; %0.3f; %0.3f", vp->position.x, vp->position.y, vp->position.z);
        for (int i = 0; i < listaDebug.size(); i++)
            log->debug("Face: %d", listaDebug[i]);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    int tot = vVertice.size() * sizeof(VertexData);
    // glBufferData(GL_ARRAY_BUFFER, 5000, nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tot, &vVertice[0]);

    glDrawArrays(GL_TRIANGLES, 0, vVertice.size()); //?? https://www.youtube.com/watch?v=S_xUgzFMIso

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    pCanvas->after();
}
