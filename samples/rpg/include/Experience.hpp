#pragma once
#include "common.hpp"

class ExpiriencePoint {
  private:
    Range range;

  public:
    explicit ExpiriencePoint(int32_t max) {
        range.name = "XP";
        range.value = 1;
        range.value_max = max;
    }

    explicit ExpiriencePoint(const ExpiriencePoint& o) = delete;

    ExpiriencePoint& operator=(const ExpiriencePoint& o) = delete;

    virtual ~ExpiriencePoint() noexcept = default;

    void level_up(int32_t value) noexcept {
        range.value = 1;
        range.value_max = value;
    }

    inline const bool finished() const noexcept { return (range.value >= range.value_max); }

    void kill_enemy(int32_t value) noexcept { range.value += value; }

    inline const std::string show() const noexcept { return range.show(); }
    inline const int32_t& get_value() const noexcept { return range.value; }
};
