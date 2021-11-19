#include "Game.hpp"
#include "chimera/core/JoystickManager.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/loader/PhysicsScene.hpp"
#include "chimera/loader/VisualScene.hpp"
#include "chimera/render/2d/BatchRender2D.hpp"
#include "chimera/render/3d/RenderableParticles.hpp"
#include "chimera/render/CameraOrthographic.hpp"
#include "chimera/render/FontManager.hpp"
#include "chimera/render/TextureManager.hpp"
#include "chimera/render/partition/LoadObj.hpp"
#include "chimera/render/scene/CameraController.hpp"
#include "chimera/render/scene/Components.hpp"

Game::Game(Chimera::Engine* engine) : engine(engine) {
    using namespace Chimera;
    pCorpoRigido = nullptr;
    // pEmissor = nullptr;
    crt.yaw = 0.0f;
    crt.pitch = 0.0f;
    crt.roll = 0.0f;
    crt.throttle = 0.0;
    crt.hat = 0;

    // ShaderManager::load("./assets/shaders/MeshFullShadow.glsl", shader[0]);
    // ShaderManager::load("./assets/shaders/ParticleEmitter.glsl", shader[1]);
    // ShaderManager::load("./assets/shaders/HUD.glsl", shader[2]);
    // ShaderManager::load("./assets/shaders/ShadowMappingDepth.glsl", shader[3]);

    // // create and add particle to scene
    // Chimera::NodeGroup* gParticle = new Chimera::NodeGroup(pRoot, "ParticleGroup");
    // gParticle->setShader(&shader[1]);
    // Chimera::NodeParticleEmitter* pParticleEmitter = new Chimera::NodeParticleEmitter(gParticle, "testeZ1", 10000);
    // pParticleEmitter->setTransform(new Chimera::Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 5.0, 4.0))));
    // pParticleEmitter->loadTexDiffuse("TexParticleEmmiter", std::string("./assets/textures/Particle2.png"));

    {
        // FPS
        Shader shader;
        ShaderManager::load("./assets/shaders/Text2D.glsl", shader);
        tile = new Tile(new Chimera::BatchRender2D(), shader, new Chimera::CameraOrthographic(512.0, -1.0f, 1.0f));

        FontManager::add(new Chimera::FontAtlas("FreeSans_22", "./assets/fonts/FreeSans.ttf", 22));
        FontManager::get()->setScale(glm::vec2(1.0, 1.0)); // em TileLayer ortho values!!!
        lFPS = new Label("None", -8, 0, glm::vec4(1.0, 1.0, 1.0, 1.0));
        tile->add(lFPS);

        tile->getCamera()->setViewportSize(engine->getCanvas()->getWidth(), engine->getCanvas()->getHeight());
        engine->pushState(tile);
    }

    {
        NodeGroup* pRoot = new NodeGroup(nullptr, "root_real");
        NodeGroup* group1 = new NodeGroup(pRoot, "none");
        VisualScene libV("./assets/models/piso2.xml", group1, &activeScene);
        libV.target();

        PhysicsScene libP("./assets/models/piso2.xml", nullptr, &activeScene);
        libP.target();

        // injeta controlador de camera
        auto view1 = activeScene.getRegistry().view<CameraComponent>();
        for (auto entity : view1) {
            Entity e = Entity{entity, &activeScene};
            e.addComponent<NativeScriptComponent>().bind<CameraController>("CameraController");
        }

        // A cada mesh
        auto view = activeScene.getRegistry().view<MeshData>();
        for (auto entity : view) {
            // Ajusta metodo de entidades
            Entity e = Entity{entity, &activeScene};

            // Adiciona o shader
            Shader& shader = e.addComponent<Shader>();
            ShaderManager::load("./assets/shaders/MeshFullShadow.glsl", shader);
        }

        // // Localiza objeto como o primario //EfeitoZoltan-mesh
        // TODO: implementar melhor
        auto solidView = activeScene.getRegistry().view<Solid>();
        for (auto ent : solidView) {
            Entity entity = {ent, &activeScene};
            auto& tc = entity.getComponent<TagComponent>();
            if (tc.tag == "EfeitoZoltan-mesh") {
                Solid& solid = entity.getComponent<Solid>();
                pCorpoRigido = &solid;
                break;
            }
        }
    }

    EmitterFont* ef = new EmitterFont();

    {
        Entity re = activeScene.createEntity("Renderable Particle System");
        Transform& tc = re.getComponent<Transform>();
        tc.setPosition(glm::vec3(0.0f, 10.0f, 2.0f));

        Material& material = re.addComponent<Material>();
        TextureManager::loadFromFile("Particle2", "./assets/textures/Particle2.png", TexParam());
        material.addTexture(SHADE_TEXTURE_DIFFUSE, TextureManager::getLast());
        material.init();

        Shader& shader = re.addComponent<Shader>();
        ShaderManager::load("./assets/shaders/ParticleEmitter.glsl", shader);

        ParticleContainer& pc = re.addComponent<ParticleContainer>();
        pc.life = 4.0f;
        pc.max = 2000;

        ef->pushParticleContainer(&pc);
    }

    activeScene.pushEmitters(ef);

    activeScene.onViewportResize(engine->getCanvas()->getWidth(), engine->getCanvas()->getHeight());
    engine->pushState(&activeScene);

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Constructor Game");
}

Game::~Game() { SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Destructor Game"); }

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;

    switch (event.type) {
        case SDL_USEREVENT: {
            // Chimera::Node* n1 = (Chimera::Node*)event.user.data1;
            // Chimera::Node* n2 = (Chimera::Node*)event.user.data2;

            switch (event.user.code) {
                // case Chimera::EVENT_COLLIDE_START: {
                //     Chimera::Node* n1 = (Chimera::Node*)event.user.data1;
                //     Chimera::Node* n2 = (Chimera::Node*)event.user.data2;
                //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao start: %s -> %s", n1->getName().c_str(), n2->getName().c_str());
                // } break;
                // case Chimera::EVENT_COLLIDE_ON: {
                //     Chimera::Node* n1 = (Chimera::Node*)event.user.data1;
                //     Chimera::Node* n2 = (Chimera::Node*)event.user.data2;
                //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao on: %s -> %s", n1->getName().c_str(), n2->getName().c_str());
                // } break;
                // case Chimera::EVENT_COLLIDE_OFF: {
                //     Chimera::Node* n1 = (Chimera::Node*)event.user.data1;
                //     Chimera::Node* n2 = (Chimera::Node*)event.user.data2;
                //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Colisao off: %s -> %s", n1->getName().c_str(), n2->getName().c_str());
                // } break;
                case Chimera::EVENT_TOGGLE_FULL_SCREEN:
                    engine->getCanvas()->toggleFullScreen();
                    break;
                case Chimera::EVENT_NEW_FPS: {
                    uint32_t* pFps = (uint32_t*)event.user.data1;
                    fps = *pFps;
                    // glm::vec3 val1 = pCorpoRigido->getPosition();
                    // sPosicaoObj = "pos:(" + std::to_string(val1.x) + "," + std::to_string(val1.y) + "," + std::to_string(val1.z) + ")";
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
                    // case SDLK_F1:
                    //     pHUD->setOn(!pHUD->isOn());
                    // break;
                case SDLK_1:
                    activeScene.getRender()->logToggle();
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

                double throttle = 0;
                crt.throttle = -propulsaoPrincipal * ((1 + JoystickState::scale16(pJoy->getAxis((uint8_t)JOY_AXIX_COD::LEFT_TRIGGER))) / 2);

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
                    engine->getCanvas()->reshape(event.window.data1, event.window.data2);
                    break;
            }
        } break;
    }
    return true;
}

void Game::onAttach() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // // Localiza o Emissor de particula
    // pEmissor = (Chimera::NodeParticleEmitter*)root->findChild("testeZ1", true);
    // renderV.pVideo = (Chimera::CanvasGL*)engine->getCanvas();
    // renderV.pTransform = (Chimera::Transform*)pCorpoRigido;
}

void Game::onDeatach() {}

void Game::onUpdate(const double& ts) {
    activeScene.onUpdate(ts); // atualiza camera e script de camera

    using namespace Chimera;

    float propulsaoLRUD = 5.0f;
    float torque = 0.5f;

    if (pCorpoRigido) {
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

    lFPS->setText(std::string("FPS: ") + std::to_string(fps));
}

void Game::onRender() {}