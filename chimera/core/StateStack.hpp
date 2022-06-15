#pragma once
#include "IStateMachine.hpp"
#include <vector>
#include <algorithm>

namespace Chimera {

class StateStack {
  public:
    StateStack() { stateInsert = states.begin(); }
    virtual ~StateStack() { states.clear(); }
    inline void clear() { states.clear(); }

    inline void pushState(IStateMachine* state) {
        stateInsert = states.emplace(stateInsert, state);
        state->onAttach();
    }

    inline void pushOverlay(IStateMachine* overlay) {
        states.emplace_back(overlay);
        overlay->onAttach();
    }

    inline void popState(IStateMachine* state) {
        auto it = std::find(states.begin(), states.end(), state);
        if (it != states.end()) {
            states.erase(it);
            stateInsert--;
        }
        state->onDeatach();
    }

    inline void popOverlay(IStateMachine* overlay) {
        auto it = std::find(states.begin(), states.end(), overlay);
        if (it != states.end())
            states.erase(it);

        overlay->onDeatach();
    }

    inline IStateMachine* getState(const std::string& name) const {
        for (IStateMachine* state : states) {
            if (state->getName() == name)
                return state;
        }
        return nullptr;
    }

    inline std::vector<IStateMachine*>::iterator begin() { return states.begin(); }
    inline std::vector<IStateMachine*>::iterator end() { return states.end(); }

  private:
    std::vector<IStateMachine*> states;
    std::vector<IStateMachine*>::iterator stateInsert;
};
} // namespace Chimera