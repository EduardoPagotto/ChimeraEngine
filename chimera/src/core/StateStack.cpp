#include "chimera/core/StateStack.hpp"
#include <algorithm>

namespace Chimera {

StateStack::StateStack() { stateInsert = states.begin(); }

StateStack::~StateStack() {
    for (IStateMachine* state : states) {
        delete state;
    }
    states.clear();
}

void StateStack::pushState(IStateMachine* state) { stateInsert = states.emplace(stateInsert, state); }

void StateStack::pushOverlay(IStateMachine* overlay) { states.emplace_back(overlay); }

void StateStack::popState(IStateMachine* state) {

    auto it = std::find(states.begin(), states.end(), state);
    if (it != states.end()) {

        states.erase(it);
        stateInsert--;
    }
}

void StateStack::popOverlay(IStateMachine* overlay) {

    auto it = std::find(states.begin(), states.end(), overlay);
    if (it != states.end())
        states.erase(it);
}

} // namespace Chimera