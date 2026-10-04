#include "chimera_base/StateStack.hpp"
#include <algorithm>

namespace ce {

    void StateStack::push_state(std::shared_ptr<IStateMachine> state) {
        state_insert_ = states_.emplace(state_insert_, state);
        state->on_attach();
    }

    void StateStack::push_overlay(std::shared_ptr<IStateMachine> overlay) {
        states_.emplace_back(overlay);
        overlay->on_attach();
    }

    void StateStack::pop_state(std::shared_ptr<IStateMachine> state) {
        if (auto it = std::find(states_.begin(), states_.end(), state); it != states_.end()) {
            states_.erase(it);
            state_insert_--;
        }
        state->on_deatach();
    }

    void StateStack::pop_overlay(std::shared_ptr<IStateMachine> overlay) {
        if (auto it = std::find(states_.begin(), states_.end(), overlay); it != states_.end()) {
            states_.erase(it);
        }

        overlay->on_deatach();
    }

    std::shared_ptr<IStateMachine> StateStack::get_state(const std::string& name) {
        for (std::shared_ptr<IStateMachine> state : states_) {
            if (state->get_name() == name) {
                return state;
            }
        }

        return nullptr;
    }
} // namespace ce
