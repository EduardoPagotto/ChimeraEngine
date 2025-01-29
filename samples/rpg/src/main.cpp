#include "Ator.hpp"
#include "Game.hpp"
#include "core/Engine.hpp"
#include "core/Registry.hpp"
#include "core/ServiceLocator.hpp"
#include "coreGL/CanvasGL.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

// bool war(std::shared_ptr<Ator> heroi, std::shared_ptr<Ator> oponente, int turno) {
//     std::cout << std::format(">>>>>[ {0} ]>>>>>", turno) << std::endl;
//     std::cout << heroi->show() << std::endl;
//     std::cout << oponente->show() << std::endl;
//     bool r1 = heroi->atacar(oponente);
//     bool r2 = oponente->atacar(heroi);
//     std::cout << heroi->show() << std::endl;
//     std::cout << oponente->show() << std::endl;
//     std::cout << std::format("<<<<<[ {0} ]<<<<<", turno) << std::endl;
//     return r1 & r2;
// }

// ce::ServiceLocator g_service_locator;

int main(int argn, char** argv) {

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("RPG Iniciado");

    // Entity entity = registry->createEntity("chimera_engine", "chimera_engine");
    // ce::Entity e = ce::g_registry.createEntity("mechanical_engine", "mechanical_engine");
    ce::g_registry.createEntity("mechanical_engine", "mechanical_engine");
    ce::g_service_locator.registerService(std::make_shared<ce::CanvasGL>("RPG", 800, 600));

    Game game;

    ce::Engine engine;
    engine.getStack().pushState(&game);
    engine.run();

    // dynamic_cast<ce::ICanva>( expression )

    // // ref: https://github.com/nlohmann/json
    // using namespace nlohmann;

    // std::ifstream db("./assets/dados.json");
    // json data = json::parse(db);

    // std::vector<std::shared_ptr<Ator>> monstros;

    // auto atores = data["atores"];

    // std::shared_ptr<Ator> heroi;

    // int count{0};
    // for (auto item : atores) {

    //     const std::string name = item["name"].get<std::string>();
    //     const int32_t level = item["level"].get<int32_t>();
    //     const int32_t base_xp = item["base_xp"].get<int32_t>();
    //     const int32_t incremento = item["incremento"].get<int32_t>();

    //     if (count == 0) {
    //         heroi = std::make_shared<Ator>(name, level, base_xp, incremento);
    //         std::cout << heroi->show() << std::endl;
    //     } else {
    //         monstros.push_back(std::make_shared<Ator>(name, level, base_xp, incremento));
    //         std::cout << monstros.back()->show() << std::endl;
    //     }

    //     count++;
    //     std::cout.flush();
    // }

    // for (auto& m : monstros) {
    //     int turno{1};
    //     while (war(heroi, m, turno))
    //         turno++;

    //     if (m->get_HP()->get_value() > 0)
    //         break;
    // }

    return 0;
}
