#include "chimera_base/StateStack.hpp"
#include <algorithm>

namespace ce {

    void StateStack::pushState(std::shared_ptr<IStateMachine> state) {
        state_insert_ = states_.emplace(state_insert_, state);
        state->on_attach();
    }

    void StateStack::pushOverlay(std::shared_ptr<IStateMachine> overlay) {
        states_.emplace_back(overlay);
        overlay->on_attach();
    }

    void StateStack::popState(std::shared_ptr<IStateMachine> state) {
        if (auto it = std::find(states_.begin(), states_.end(), state); it != states_.end()) {
            states_.erase(it);
            state_insert_--;
        }
        state->on_deatach();
    }

    void StateStack::popOverlay(std::shared_ptr<IStateMachine> overlay) {
        if (auto it = std::find(states_.begin(), states_.end(), overlay); it != states_.end()) {
            states_.erase(it);
        }

        overlay->on_deatach();
    }

    std::shared_ptr<IStateMachine> StateStack::getState(const std::string& name) {
        for (std::shared_ptr<IStateMachine> state : states_) {
            if (state->get_name() == name) {
                return state;
            }
        }

        return nullptr;
    }
} // namespace ce
