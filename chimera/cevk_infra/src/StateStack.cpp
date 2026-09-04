#include "cevk_infra/StateStack.hpp"
#include <algorithm>

namespace ce {

    void StateStack::pushState(std::shared_ptr<IStateMachine> state) {
        stateInsert = states.emplace(stateInsert, state);
        state->onAttach();
    }

    void StateStack::pushOverlay(std::shared_ptr<IStateMachine> overlay) {
        states.emplace_back(overlay);
        overlay->onAttach();
    }

    void StateStack::popState(std::shared_ptr<IStateMachine> state) {
        if (auto it = std::find(states.begin(), states.end(), state); it != states.end()) {
            states.erase(it);
            stateInsert--;
        }
        state->onDeatach();
    }

    void StateStack::popOverlay(std::shared_ptr<IStateMachine> overlay) {
        if (auto it = std::find(states.begin(), states.end(), overlay); it != states.end()) {
            states.erase(it);
        }

        overlay->onDeatach();
    }

    std::shared_ptr<IStateMachine> StateStack::getState(const std::string& name) {
        for (std::shared_ptr<IStateMachine> state : states) {
            if (state->getName() == name) {
                return state;
            }
        }

        return nullptr;
    }
} // namespace ce
