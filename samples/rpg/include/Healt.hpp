#pragma once
#include "common.hpp"

class HealtPoint {
  private:
    Range range;

  public:
    explicit HealtPoint(int32_t max) {
        range.name = "HP";
        range.value = range.value_max = max;
    }

    explicit HealtPoint(const HealtPoint& o) = delete;

    HealtPoint& operator=(const HealtPoint& o) = delete;

    virtual ~HealtPoint() noexcept = default;

    void level_up(int32_t value) noexcept { range.value_max = range.value = value; }

    void reset() noexcept { range.value = range.value_max; }

    const bool defesa(int32_t value) noexcept {

        range.value -= value;
        if (range.value <= 0) {
            range.value = 0;
            return false;
        }
        return true;
    }

    inline const std::string show() const noexcept { return range.show(); }
    inline const int32_t& get_value() const noexcept { return range.value; }
};
