#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/Singleton.hpp"
#include "chimera/core/io/JoystickManager.hpp"
#include "chimera/core/io/MouseDevice.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/core/windows/CanvasGL.hpp"
#include "chimera/loader/VisualScene.hpp"
#include "chimera/node/NodeGroup.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/node/VisitParser.hpp"
#include "chimera/physic_loader/PhysicsScene.hpp"

Game::Game(Chimera::Canvas* canvas) : Application(canvas) {
    pCorpoRigido = nullptr;
    pEmissor = nullptr;
    pOrbitalCam = nullptr;

    textoFPS = "fps: 0";
    sPosicaoObj = "pos:(,,)";

    crt.yaw = 0.0f;
    crt.pitch = 0.0f;
    crt.roll = 0.0f;
    crt.throttle = 0.0;
    crt.hat = 0;

    shaderLibrary.load("./assets/shaders/MeshFullShadow.glsl");
    shaderLibrary.load("./assets/shaders/ParticleEmitter.glsl");
    shaderLibrary.load("./assets/shaders/HUD.glsl");
    shaderLibrary.load("./assets/shaders/ShadowMappingDepth.glsl");
    std::string model = "./assets/models/piso2.xml";

    // Cria grupo shader como filho de scene
    Chimera::NodeGroup* pRoot = new Chimera::NodeGroup(nullptr, "root_real");
    Chimera::NodeGroup* group1 = new Chimera::NodeGroup(pRoot, "none");

    ChimeraLoaders::VisualScene libV(model, group1);
    libV.target();

    // Cria mundo fisico e o vincula a scena
    Chimera::PhysicsControl* pPC = new Chimera::PhysicsControl(); // ddddddddddddd onde sesta ver no main anterir
    ChimeraLoaders::PhysicsScene libP(model, pPC);
    libP.target();

    // Vincula o shader de calculo de sobra e ShadowMap com textura de resultado
    group1->setShader(shaderLibrary.get("MeshFullShadow"));
    group1->setNodeVisitor(new Chimera::VisitorShadowMap(&this->renderV.render3D, shaderLibrary.get("ShadowMappingDepth"), 2048, 2048));

    // create and add particle to scene
    Chimera::NodeGroup* gParticle = new Chimera::NodeGroup(pRoot, "ParticleGroup");
    gParticle->setShader(shaderLibrary.get("ParticleEmitter"));
    Chimera::NodeParticleEmitter* pParticleEmitter = new Chimera::NodeParticleEmitter(gParticle, "testeZ1", 10000);
    pParticleEmitter->setTransform(new Chimera::Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 5.0, 4.0))));

    pParticleEmitter->loadTexDiffuse("TexParticleEmmiter", std::string("./assets/textures/Particle2.png"));

    // Create and add hud data text
    Chimera::NodeGroup* gHud = new Chimera::NodeGroup((Chimera::Node*)pRoot, "HUD-Group");
    gHud->setShader(shaderLibrary.get("HUD"));
    Chimera::NodeHUD* pHUD = new Chimera::NodeHUD(gHud, "HUD-Default");

    Chimera::Font* pFont = new Chimera::Font("./assets/fonts/FreeSans.ttf", 18);
    pHUD->addFont(pFont);

    root = pRoot;
    physicWorld = pPC;

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Constructor Game");
}

Game::~Game() { SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Destructor Game"); }

void Game::updatePos() {

    using namespace Chimera;

    float propulsaoLRUD = 5.0f;
    float torque = 0.5f;

    if (crt.hat & (uint8_t)SDL_HAT_UP)
        pCorpoRigido->applyForce(glm::vec3(0.0, 0.0, propulsaoLRUD));

    if (crt.hat & (uint8_t)SDL_HAT_DOWN)
        pCorpoRigido->applyForce(glm::vec3(0.0, 0.0, -propulsaoLRUD));

    if (crt.hat & (uint8_t)SDL_HAT_RIGHTUP)
        pCorpoRigido->applyForce(glm::vec3(propulsaoLRUD, 0.0, 0.0));

    if (crt.hat & (uint8_t)SDL_HAT_LEFTUP)
        pCorpoRigido->applyForce(glm::vec3(-propulsaoLRUD, 0.0, 0.0));

    if ((crt.roll != 0.0) || (crt.pitch != 0.0) || (crt.yaw != 0.0) || (crt.throttle != 0.0)) {
        pCorpoRigido->applyForce(glm::vec3(0.0, crt.throttle, 0.0));
        pCorpoRigido->applyTorc(glm::vec3(-torque * crt.pitch, -torque * crt.roll, -torque * crt.yaw));
    }
}

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;

    switch (event.type) {
        case SDL_USEREVENT: {
            Chimera::Node* n1 = (Chimera::Node*)event.user.data1;
            Chimera::Node* n2 = (Chimera::Node*)event.user.data2;

            switch (event.user.code) {
                case Chimera::EVENT_COLLIDE_START: {
                    Chimera::Node* n1 = (Chimera::Node*)event.user.data1;
                    Chimera::Node* n2 = (Chimera::Node*)event.user.data2;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao start: %s -> %s", n1->getName().c_str(), n2->getName().c_str());
                } break;
                case Chimera::EVENT_COLLIDE_ON: {
                    Chimera::Node* n1 = (Chimera::Node*)event.user.data1;
                    Chimera::Node* n2 = (Chimera::Node*)event.user.data2;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao on: %s -> %s", n1->getName().c_str(), n2->getName().c_str());
                } break;
                case Chimera::EVENT_COLLIDE_OFF: {
                    Chimera::Node* n1 = (Chimera::Node*)event.user.data1;
                    Chimera::Node* n2 = (Chimera::Node*)event.user.data2;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao off: %s -> %s", n1->getName().c_str(), n2->getName().c_str());
                } break;
                case Chimera::EVENT_TOGGLE_FULL_SCREEN:
                    canvas->toggleFullScreen();
                    break;
                case Chimera::EVENT_NEW_FPS: {
                    uint32_t* fps = (uint32_t*)event.user.data1;
                    glm::vec3 val1 = pCorpoRigido->getPosition();
                    sPosicaoObj = "pos:(" + std::to_string(val1.x) + "," + std::to_string(val1.y) + "," + std::to_string(val1.z) + ")";
                    textoFPS = "fps: " + std::to_string(*fps) + std::string(" Periodo: ") + std::to_string(physicWorld->getLastPeriod());
                } break;
                default:
                    break;
            }

        } break;
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    utilSendEvent(EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_F1:
                    pHUD->setOn(!pHUD->isOn());
                    break;
                case SDLK_F10:
                    utilSendEvent(EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
                    break;
                case SDLK_UP:
                    pCorpoRigido->applyForce(glm::vec3(10.0, 0.0, 0.0));
                    break;
                case SDLK_DOWN:
                    pCorpoRigido->applyForce(glm::vec3(-10.0, 0.0, 0.0));
                    break;
                case SDLK_LEFT:
                    pCorpoRigido->applyForce(glm::vec3(0.0, 10.0, 0.0));
                    break;
                case SDLK_RIGHT:
                    pCorpoRigido->applyForce(glm::vec3(0.0, -10.0, 0.0));
                    break;
                case SDLK_a:
                    pCorpoRigido->applyTorc(glm::vec3(0.0, 0.0, 10.0));
                    break;
                case SDLK_s:
                    pCorpoRigido->applyTorc(glm::vec3(0.0, 0.0, -10.0));
                    break;
                default:
                    break;
            }
        } break;

        case SDL_JOYAXISMOTION:
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
        case SDL_JOYHATMOTION:
        case SDL_JOYBALLMOTION: {

            JoystickState* pJoy = JoystickManager::select(event.jball.which);

            // Captura joystick 0 se existir
            if (pJoy != nullptr) {

                pJoy->debug();

                float propulsaoPrincipal = 3.0f;

                crt.yaw = JoystickState::scale16(pJoy->getAxis((uint8_t)JOY_AXIX_COD::LEFT_X, 32));
                crt.pitch = JoystickState::scale16(pJoy->getAxis((uint8_t)JOY_AXIX_COD::LEFT_Y, 32));
                crt.roll = JoystickState::scale16(pJoy->getAxis((uint8_t)JOY_AXIX_COD::RIGHT_X, 32));

                // double throttle = 0;
                // crt.throttle = -propulsaoPrincipal * ((1 + JoystickState::scale16(pJoy->getAxis((uint8_t)JOY_AXIX_COD::LEFT_TRIGGER))) /
                // 2);

                if (pJoy->getButtonState((uint8_t)JOY_BUTTON_COD::X) == SDL_PRESSED) {

                    // glm::vec3 posicao = pEmissor->getPosSource();
                    // posicao.x = posicao.x - 0.1f;
                    // pEmissor->setPosSource( posicao );
                }

                if (pJoy->getButtonState((uint8_t)JOY_BUTTON_COD::B) == SDL_PRESSED) { // ( or SDL_RELEASED)

                    // glm::vec3 posicao = pEmissor->getPosSource();
                    // posicao.x = posicao.x + 0.1f;
                    // pEmissor->setPosSource(posicao);
                }

                crt.hat = pJoy->getHat(0);
            }

        } break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION: {
        } break;
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_ENTER:
                    utilSendEvent(EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    utilSendEvent(EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    canvas->reshape(event.window.data1, event.window.data2);
                    break;
            }
        } break;
    }
    return true;
}

void Game::onStart() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color

    root->initializeChilds();

    canvas->afterStart();

    // Localiza a camera
    pOrbitalCam = (Chimera::NodeCamera*)root->findChild("Camera-camera", true);

    // Localiza objeto como o primario //EfeitoZoltan-mesh
    Chimera::NodeMesh* pMesh = (Chimera::NodeMesh*)root->findChild("EfeitoZoltan-mesh", true);
    pCorpoRigido = (Chimera::Solid*)pMesh->getTransform();

    // Localiza o Emissor de particula
    pEmissor = (Chimera::NodeParticleEmitter*)root->findChild("testeZ1", true);

    renderV.pVideo = (Chimera::CanvasGL*)canvas;
    renderV.pTransform = (Chimera::Transform*)pCorpoRigido;

    // Localiza o HUD
    pHUD = (Chimera::NodeHUD*)root->findChild("HUD-Default", true);
    if (pHUD != nullptr) {
        pHUD->addText(0, glm::ivec2(350, 30), glm::vec4(0.0, 0.0, 1.0, 1.0), 1.0, &sPosicaoObj);
        pHUD->addText(0, glm::ivec2(10, 30), glm::vec4(1.0, 0.0, 0.0, 1.0), 1.0, &textoFPS);
    }
}

void Game::onUpdate() {

    physicWorld->stepSim();
    physicWorld->checkCollisions();
    this->updatePos();

    for (int eye = 0; eye < canvas->getTotEyes(); eye++) {

        canvas->before(eye);

        renderV.eye = eye;
        Chimera::visitParserTree(root, &renderV); // dfs(root, &rv);//DFS(root);

        canvas->after(eye);
    }

    canvas->swapWindow();
}
