#include "Game.hpp"
#include "TileLayer.hpp"
#include "chimera_base/ICanva.hpp"
#include "chimera_base/InputManager.hpp"
#include "chimera_core/gl/AssetManager.hpp"
#include "chimera_render/2d/Label.hpp"
#include "chimera_render/2d/Sprite.hpp"
// #include "chimera_base/Transform.hpp"
// #include "chimera_base/event.hpp"
// #include "chimera_core/gl/AssetManager.hpp"
// #include "chimera_core/gl/FontMng.hpp"
// #include "chimera_core/gl/ShaderMng.hpp"
// #include "chimera_core/gl/TextureLoader.hpp"
// #include "chimera_render/2d/Group.hpp"
// #include <time.h>

Game::Game(std::shared_ptr<entt::registry> registry, ce::Engine* engine) : registry(registry), engine(engine) {

    using namespace ce;
    srand(time(nullptr));
    // Group* group = new Group(glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 5.0f, 0.0f)));
    // group->add(new Sprite(0.0f, 0.0f, 6.0f, 3.0f, glm::vec4(1, 1, 1, 1)));
    // Group* button = new Group(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f)));
    // button->add(new Sprite(0.0f, 0.0f, 5.0f, 2.0f, glm::vec4(1, 0, 1, 1)));
    // button->add(new Sprite(0.5f, 0.5f, 3.0f, 1.0f, glm::vec4(0.2f, 0.3f, 0.8f, 1)));
    // group->add(button);
    // layer->add(group);

    this->inputManager = registry->ctx().get<std::shared_ptr<ce::InputManager>>();
    this->canvas = std::dynamic_pointer_cast<ce::CanvasGL>(registry->ctx().get<std::shared_ptr<ce::ICanva>>());
    if (this->canvas == nullptr) {
        throw std::runtime_error("Canva not found in CTX");
    }

    auto asset = registry->ctx().get<std::shared_ptr<ce::AssetManager>>();
    TexParam tp;

    asset->loadTexture("t01", "./assets/textures/grid1.png", tp);
    asset->loadTexture("t02", "./assets/textures/grid2.png", tp);
    asset->loadTexture("t03", "./assets/textures/grid3.png", tp);

    std::unordered_map<GLenum, std::string> shadeData;
    shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/Basic2D.frag";
    shadeData[GL_VERTEX_SHADER] = "./assets/shaders/Basic2D.vert";

    shader = asset->loadShader("Basic2D", shadeData).handle();
}

Game::~Game() {}

void Game::onAttach() {

    // ApplicationGL::onAttach();

    using namespace ce; // 26:10 ->
                        // https://www.youtube.com/watch?v=wYVaIOUhz6s&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=96
                        // (video 96) video 103 finaliza o pick mouse colocar para rodar o scene como
                        // renderbuffer!!!!!!!!!

    layer = std::make_shared<TileLayer>(shader);

    layer->getCamera()->setViewportSize(canvas->getWidth(), canvas->getHeight());

    auto asset = registry->ctx().get<std::shared_ptr<ce::AssetManager>>();

    for (float y = -8.0F; y < 8.0F; y++) {

        for (float x = -14.0F; x < 14.0F; x++) {

            if (rand() % 4 == 0) {
                layer->add(new Sprite(x, y, 1.0F, 1.0F, glm::vec4(rand() % 1000 / 1000.0F, 0, 1, 1)));
            } else {
                layer->add(new Sprite(x, y, 1.0F, 1.0F, asset->getTextureFromIndex(rand() % 3).handle()));
            }
        }
    }

    auto font = asset->loadFont("FreeSans_22", "./assets/fonts/FreeSans.ttf", 22).handle();

    font->scale = glm::vec2(0.04, 0.04);

    lFPS = new Label("None", 0, 0, font, glm::vec4(1.0, 1.0, 1.0, 1.0));

    layer->add(lFPS);

    engine->getStack().pushState(layer);
}

void Game::onDeatach() {
    // ApplicationGL::onDeatach();
}

void Game::onRender() {
    // this->onRender(); // FIXME: ???????
    // ApplicationGL::onRender();
}

void Game::onEvent(const SDL_Event& event) {
    using namespace ce;

    if (event.type == CHIMERA_EVENT01) {
        if (static_cast<EventCE>(event.user.code) == EventCE::NEW_FPS) {
            uint32_t* pFps = (uint32_t*)event.user.data1;
            fps = *pFps;
            SDL_Log("FPS: %d", fps);
        }
    }
}

void Game::onUpdate(const double& ts) {

    lFPS->setText(std::string("FPS: ") + std::to_string(fps));

    if (this->inputManager->getKeyboard()->isKeyPressed(SDL_SCANCODE_ESCAPE)) {
        sendChimeraEvent(ce::EventCE::FLOW_STOP, nullptr, nullptr);
        return;
    }

    if (this->inputManager->getKeyboard()->isKeyPressed(SDL_SCANCODE_F1)) {
        sendChimeraEvent(ce::EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
        return;
    }
}
