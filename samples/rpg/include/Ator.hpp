#pragma once
#include "Experience.hpp"
#include "Healt.hpp"
#include <iostream>
#include <memory>

class Ator {
  private:
    std::string name;
    int32_t level{1}, xp_base{0}, incremento{0}, ataque{0};

    std::shared_ptr<HealtPoint> hp;
    std::shared_ptr<ExpiriencePoint> xp;

  public:
    Ator(const std::string& name, const int32_t& level, const int32_t& xp, const int32_t& inc) noexcept
        : name(name), level(level), xp_base(xp), incremento(inc) {

        int32_t base_hp = this->xp_base + (this->incremento * (this->level - 1));
        int32_t base_bat = base_hp * this->level;
        this->ataque = base_bat;

        this->hp = std::make_shared<HealtPoint>(base_hp);
        this->xp = std::make_shared<ExpiriencePoint>(base_bat * 3);
    }

    inline const std::string get_name() const noexcept { return name; }
    // inline const int32_t getLevel() const noexcept { return this->level; }
    std::shared_ptr<HealtPoint> get_HP() { return hp; }
    std::shared_ptr<ExpiriencePoint> get_XP() { return xp; }

    const std::string show() const noexcept {
        return std::format("{} Level: {} {} {} Dano: {}", name, level, xp->show(), hp->show(), ataque);
    }

    void level_up() noexcept {
        this->level++;
        const int32_t base_hp = this->xp_base + (this->incremento * (this->level - 1));
        const int32_t base_bat = base_hp * this->level;

        this->xp->level_up(base_hp);
        this->hp->level_up(base_bat * 3);
        this->ataque = base_bat;
    }

    bool atacar(std::shared_ptr<Ator> oponente) {

        if (this->hp->get_value() <= 0) {
            std::cout << "-----morto-----" << std::endl;
            return false;
        }

        if (oponente->get_HP()->defesa(this->ataque))
            return true;

        std::cout << std::format(">>>>> {} was killed!!\n", oponente->get_name());

        this->hp->reset();
        this->xp->kill_enemy(oponente->get_XP()->get_value() * 3);

        if (this->xp->finished()) {
            std::cout << std::format(">>>>>>> {} -> LEVEL UP !!", this->name) << std::endl;
            this->level_up();
        }
        return false;
    }
};
