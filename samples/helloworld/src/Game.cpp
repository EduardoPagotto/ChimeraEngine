#include "Game.hpp"
#include "TileLayer.hpp"
#include "chimera_base/ICanva.hpp"
#include "chimera_base/InputManager.hpp"
#include "chimera_core/gl/AssetManager.hpp"
#include "chimera_render/2d/Label.hpp"
#include "chimera_render/2d/Sprite.hpp"

Game::Game(std::shared_ptr<entt::registry> registry, ce::Engine* engine) : registry_(registry), engine_(engine) {

    using namespace ce;
    srand(time(nullptr));
    // Group* group = new Group(glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 5.0f, 0.0f)));
    // group->add(new Sprite(0.0f, 0.0f, 6.0f, 3.0f, glm::vec4(1, 1, 1, 1)));
    // Group* button = new Group(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f)));
    // button->add(new Sprite(0.0f, 0.0f, 5.0f, 2.0f, glm::vec4(1, 0, 1, 1)));
    // button->add(new Sprite(0.5f, 0.5f, 3.0f, 1.0f, glm::vec4(0.2f, 0.3f, 0.8f, 1)));
    // group->add(button);
    // layer->add(group);

    this->input_manager_ = registry->ctx().get<std::shared_ptr<ce::InputManager>>();
    this->canvas_ = std::dynamic_pointer_cast<ce::CanvasGL>(registry->ctx().get<std::shared_ptr<ce::ICanva>>());
    if (this->canvas_ == nullptr) {
        throw std::runtime_error("Canva not found in CTX");
    }

    auto asset = registry->ctx().get<std::shared_ptr<ce::AssetManager>>();
    TexParam tp;

    asset->load_texture("t01", "./assets/textures/grid1.png", tp);
    asset->load_texture("t02", "./assets/textures/grid2.png", tp);
    asset->load_texture("t03", "./assets/textures/grid3.png", tp);

    std::unordered_map<GLenum, std::string> shade_data;
    shade_data[GL_FRAGMENT_SHADER] = "./assets/shaders/Basic2D.frag";
    shade_data[GL_VERTEX_SHADER] = "./assets/shaders/Basic2D.vert";

    shader_ = asset->load_shader("Basic2D", shade_data).handle();
}

Game::~Game() {}

void Game::on_attach() {

    // ApplicationGL::onAttach();

    using namespace ce; // 26:10 ->
                        // https://www.youtube.com/watch?v=wYVaIOUhz6s&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=96
                        // (video 96) video 103 finaliza o pick mouse colocar para rodar o scene como
                        // renderbuffer!!!!!!!!!

    layer_ = std::make_shared<TileLayer>(shader_);

    layer_->get_camera()->set_viewport_size(canvas_->width(), canvas_->height());

    auto asset = registry_->ctx().get<std::shared_ptr<ce::AssetManager>>();

    for (float y = -8.0F; y < 8.0F; y++) {

        for (float x = -14.0F; x < 14.0F; x++) {

            if (rand() % 4 == 0) {
                layer_->add(new Sprite(x, y, 1.0F, 1.0F, glm::vec4(rand() % 1000 / 1000.0F, 0, 1, 1)));
            } else {
                layer_->add(new Sprite(x, y, 1.0F, 1.0F, asset->get_texture_from_index(rand() % 3).handle()));
            }
        }
    }

    auto font = asset->load_font("FreeSans_22", "./assets/fonts/FreeSans.ttf", 22).handle();

    font->scale = glm::vec2(0.04, 0.04);

    l_fps_ = new Label("None", 0, 0, font, glm::vec4(1.0, 1.0, 1.0, 1.0));

    layer_->add(l_fps_);

    engine_->stack().push_state(layer_);
}

void Game::on_deatach() {
    // ApplicationGL::onDeatach();
}

void Game::on_render() {
    // this->onRender(); // FIXME: ???????
    // ApplicationGL::onRender();
}

void Game::on_event(const SDL_Event& event) {
    using namespace ce;

    if (event.type == chimera_even_t01) {
        if (static_cast<EventCE>(event.user.code) == EventCE::NEW_FPS) {
            uint32_t* p_fps = (uint32_t*)event.user.data1;
            fps_ = *p_fps;
            SDL_Log("FPS: %d", fps_);
        }
    }
}

void Game::on_update(const double& ts) {

    l_fps_->set_text(std::string("FPS: ") + std::to_string(fps_));

    if (this->input_manager_->get_keyboard()->is_key_pressed(SDL_SCANCODE_ESCAPE)) {
        send_chimera_event(ce::EventCE::FLOW_STOP, nullptr, nullptr);
        return;
    }

    if (this->input_manager_->get_keyboard()->is_key_pressed(SDL_SCANCODE_F1)) {
        send_chimera_event(ce::EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
        return;
    }
}
