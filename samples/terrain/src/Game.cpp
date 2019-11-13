#include "Game.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/render/LoadObj.hpp"

//#include "chimera/render/AABB.hpp"

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

#define SHADOW_MAP_SIZE 4096
#define gl_max_texture_size 4096 // FIXME: ????

Game::Game(Chimera::CanvasGL* _pCanvas, Chimera::Shader* _pShader) : pCanvas(_pCanvas), pShader(_pShader) {
    isPaused = false;

    LightAngle = 22.5f;

    Wireframe = false;
    DisplayShadowMap = false;
    RenderAABB = false;
    VisualizeRenderingOrder = false;
    SortVisibleGeometryNodes = true;
    RenderSlow = false;

    Depth = -1;
}

Game::~Game() {

    terra.Destroy();

    glDeleteTextures(1, &ShadowMap);
    glDeleteTextures(1, &RotationTexture);

    if (GLEW_EXT_framebuffer_object) {
        glDeleteFramebuffersEXT(1, &FBO);
    }
}

void Game::joystickCapture(Chimera::JoystickManager& joy) {}

void Game::joystickStatus(Chimera::JoystickManager& joy) {}

void Game::keyCapture(SDL_Keycode tecla) {

    unsigned short Keys = 0x00;

    // if (GetKeyState('W') & 0x80)
    //     Keys |= 0x01;
    // if (GetKeyState('S') & 0x80)
    //     Keys |= 0x02;
    // if (GetKeyState('A') & 0x80)
    //     Keys |= 0x04;
    // if (GetKeyState('D') & 0x80)
    //     Keys |= 0x08;
    // // if(GetKeyState('R') & 0x80) Keys |= 0x10;
    // // if(GetKeyState('F') & 0x80) Keys |= 0x20;

    // if (GetKeyState(VK_SHIFT) & 0x80)
    //     Keys |= 0x40;
    // if (GetKeyState(VK_CONTROL) & 0x80)
    //     Keys |= 0x80;

    // if (Keys & 0x3F) {
    //     glm::vec3 Movement = camera.OnKeys(Keys, FrameTime * 0.5f);

    //     CheckCameraTerrainPosition(Movement);

    //     camera.Move(Movement);
    // }

    switch (tecla) {
        case SDLK_ESCAPE:
            SDL_Event l_eventQuit;
            l_eventQuit.type = SDL_QUIT;
            if (SDL_PushEvent(&l_eventQuit) == -1) {
                throw Chimera::Exception(std::string(SDL_GetError()));
            }
            break;

        case SDLK_w:
            Keys |= 0x01;
        case SDLK_s:
            Keys |= 0x02;
        case SDLK_a:
            Keys |= 0x04;
        case SDLK_d:
            Keys |= 0x08;
            // case SDLK_r:
            //     Keys |= 0x10;
            // case SDLK_f:
            //     Keys |= 0x20;
            // if (GetKeyState(VK_SHIFT) & 0x80)
            //     Keys |= 0x40;
            // if (GetKeyState(VK_CONTROL) & 0x80)
            //     Keys |= 0x80;
            if (Keys & 0x3F) {
                glm::vec3 Movement = camera.OnKeys(Keys, 1.0f); // FrameTime * 0.5f);
                CheckCameraTerrainPosition(Movement);
                camera.Move(Movement);
            }
        case SDLK_F1:
            Wireframe = !Wireframe;
            break;
        case SDLK_F2:
            DisplayShadowMap = !DisplayShadowMap;
            break;
        case SDLK_F3:
            RenderAABB = !RenderAABB;
            break;
        case SDLK_F4:
            VisualizeRenderingOrder = !VisualizeRenderingOrder;
            break;
        case SDLK_F5:
            SortVisibleGeometryNodes = !SortVisibleGeometryNodes;
            break;
        case SDLK_F6:
            RenderSlow = !RenderSlow;
            break;
        case SDLK_F7:
            terra.SaveBinary((char*)"terrain-saved.bin");
            break;
        case SDLK_1:
            if (terra.LoadBinary((char*)"terrain1.bin")) {
                glm::vec3 Movement;
                CheckCameraTerrainPosition(Movement);
                camera.Move(Movement);
                RenderShadowMap();
            }
            break;
        case SDLK_2:
            if (terra.LoadTexture2D((char*)"terrain2.jpg", 32.0f, -16.0f)) {
                glm::vec3 Movement;
                CheckCameraTerrainPosition(Movement);
                camera.Move(Movement);
                RenderShadowMap();
            }
            break;
        case SDLK_3:
            if (terra.LoadTexture2D((char*)"terrain3.jpg", 128.0f, -64.0f)) {
                glm::vec3 Movement;
                CheckCameraTerrainPosition(Movement);
                camera.Move(Movement);
                RenderShadowMap();
            }
            break;
        case SDLK_4:
            if (terra.LoadTexture2D((char*)"terrain4.jpg", 128.0f, -64.0f)) {
                glm::vec3 Movement;
                CheckCameraTerrainPosition(Movement);
                camera.Move(Movement);
                RenderShadowMap();
            }
            break;
        case SDLK_ASTERISK:
            Depth++;
            break;
        case SDLK_SLASH:
            if (Depth > -1)
                Depth--;
            break;
        case SDLK_PLUS:
            LightAngle += 3.75f;
            RenderShadowMap();
            break;
        case SDLK_MINUS:
            LightAngle -= 3.75f;
            RenderShadowMap();
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
        if (botaoIndex == 1) {
            camera.OnMouseMove(LastX - mm.xrel, LastY - mm.yrel);
            LastX = mm.xrel;
            LastY = mm.yrel;
            // trackBall.tracking(mm.xrel, mm.yrel);
        } else if (botaoIndex == 2) {
        }
        camera.OnMouseWheel(mm.yrel);
        // trackBall.offSet(mm.yrel);
    }
}

void Game::start() {

    pCanvas->initGL();

    terra.LoadBinary((char*)"./samples/terrain/data/terrain1.bin");

    // Shader.UniformLocations = new GLuint[2];
    UniformLocations = new GLuint[2];
    pShader->link(); //??

    // Shader.UniformLocations[0] = glGetUniformLocation(Shader, "ShadowMatrix");
    UniformLocations[0] = pShader->getUniformLocation("ShadowMatrix");
    // Shader.UniformLocations[1] = glGetUniformLocation(Shader, "LightDirection");
    UniformLocations[1] = pShader->getUniformLocation("LightDirection");

    pShader->link(); // glUseProgram(Shader);??

    pShader->setGlUniform1i((char*)"ShadowMap", 0);
    pShader->setGlUniform1i((char*)"RotationTexture", 1);
    pShader->setGlUniform1f((char*)"Scale", 1.0f / 64.0f);
    pShader->setGlUniform1f((char*)"Radius", 1.0f / 1024.0f);
    pShader->unlink(); // glUseProgram(0);

    ShadowMapSize = SHADOW_MAP_SIZE > gl_max_texture_size ? gl_max_texture_size : SHADOW_MAP_SIZE;

    glGenTextures(1, &ShadowMap);
    glBindTexture(GL_TEXTURE_2D, ShadowMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, ShadowMapSize, ShadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    srand(SDL_GetTicks());

    glm::vec4* RotationTextureData = new glm::vec4[4096];

    float RandomAngle = 3.14f * 2.0f * (float)rand() / (float)RAND_MAX;

    for (int i = 0; i < 4096; i++) {
        RotationTextureData[i].x = cos(RandomAngle);
        RotationTextureData[i].y = sin(RandomAngle);
        RotationTextureData[i].z = -RotationTextureData[i].y;
        RotationTextureData[i].w = RotationTextureData[i].x;

        RotationTextureData[i] *= 0.5f;
        RotationTextureData[i] += 0.5f;

        RandomAngle += 3.14f * 2.0f * (float)rand() / (float)RAND_MAX;
    }

    glGenTextures(1, &RotationTexture);
    glBindTexture(GL_TEXTURE_2D, RotationTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 64, 64, 0, GL_RGBA, GL_FLOAT, RotationTextureData);
    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] RotationTextureData;

    glGenFramebuffersEXT(1, &FBO);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
    glDrawBuffers(0, NULL);
    glReadBuffer(GL_NONE);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, ShadowMap, 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    RenderShadowMap();

    float Height = terra.GetHeight(0.0f, 0.0f);

    camera.SetPerspective(45.0f, (float)pCanvas->getWidth() / (float)pCanvas->getHeight(), 0.125f, 1024.0f);
    camera.Look(glm::vec3(0.0f, Height + 1.75f, 0.0f), glm::vec3(0.0f, Height + 1.75f, -1.0f));
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

    Chimera::ViewPoint vp; // CADE?????

    pCanvas->calcPerspectiveProjectionView(0, &vp, camera.ViewMatrix, camera.ProjectionMatrix);

    pCanvas->afterStart();

    if (!RenderSlow) {
        terra.CheckVisibility(camera.frustum, SortVisibleGeometryNodes);
    }

    if (Wireframe) {
        glColor3f(0.0f, 0.0f, 0.0f);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        if (RenderSlow) {
            terra.RenderSlow();
        } else {
            terra.Render();
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glColor3f(1.0f, 1.0f, 1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ShadowMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, RotationTexture);

    pShader->link();

    if (RenderSlow) {
        terra.RenderSlow();
    } else {
        terra.Render(VisualizeRenderingOrder);
    }

    glUseProgram(0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (!RenderSlow && RenderAABB) {
        glColor3f(0.0f, 1.0f, 0.0f);

        terra.RenderAABB(Depth);
    }

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    if (DisplayShadowMap) {
        glViewport(16, 16, 256, 256);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glColor3f(1.0f, 1.0f, 1.0f);

        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, ShadowMap);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(-1.0f, 1.0f);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);

        glDisable(GL_TEXTURE_2D);
    }
}

void Game::CheckCameraTerrainPosition(glm::vec3& Movement) {
    glm::vec3 CameraPosition = camera.reference + Movement, Min = terra.GetMin(), Max = terra.GetMax();

    if (CameraPosition.x < Min.x)
        Movement += glm::vec3(Min.x - CameraPosition.x, 0.0f, 0.0f);
    if (CameraPosition.x > Max.x)
        Movement += glm::vec3(Max.x - CameraPosition.x, 0.0f, 0.0f);
    if (CameraPosition.z < Min.z)
        Movement += glm::vec3(0.0f, 0.0f, Min.z - CameraPosition.z);
    if (CameraPosition.z > Max.z)
        Movement += glm::vec3(0.0f, 0.0f, Max.z - CameraPosition.z);

    CameraPosition = camera.reference + Movement;

    float Height = terra.GetHeight(CameraPosition.x, CameraPosition.z);

    Movement += glm::vec3(0.0f, Height + 1.75f - camera.reference.y, 0.0f);
}

void Game::RenderShadowMap() {
    glm::vec3 LightPosition = glm::vec3(0.0f, 20.0f, 0.0f);
    // FIXME: ????
    // glm::rotate(glm::vec3((float)terra.GetSize(), 0.0f, 0.0f), -LightAngle,glm::vec3(0.0f, 1.0f, -1.0f));

    glm::vec3 LightDirection = normalize(LightPosition);

    LightViewMatrix = glm::lookAt(LightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 Min, Max;

    terra.GetMinMax(LightViewMatrix, Min, Max);

    LightProjectionMatrix = glm::ortho(Min.x, Max.x, Min.y, Max.y, -Max.z, -Min.z);

    glm::mat4 BiasMatrix =
        glm::mat4(0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f);

    ShadowMatrix = BiasMatrix * LightProjectionMatrix * LightViewMatrix;

    pShader->link();

    glUniformMatrix4fv(UniformLocations[0], 1, GL_FALSE, glm::value_ptr(ShadowMatrix));

    glUniform3fv(UniformLocations[1], 1, glm::value_ptr(LightDirection));
    pShader->unlink();

    glViewport(0, 0, ShadowMapSize, ShadowMapSize);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);

    glClear(GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(LightProjectionMatrix));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(LightViewMatrix));

    glEnable(GL_DEPTH_TEST);

    terra.RenderSlowToShadowMap();

    glDisable(GL_DEPTH_TEST);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
