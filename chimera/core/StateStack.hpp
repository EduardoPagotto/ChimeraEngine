#pragma once
#include "IStateMachine.hpp"
#include <algorithm>
#include <vector>

namespace ce {

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
        if (auto it = std::find(states.begin(), states.end(), state); it != states.end()) {
            states.erase(it);
            stateInsert--;
        }
        state->onDeatach();
    }

    inline void popOverlay(IStateMachine* overlay) {
        if (auto it = std::find(states.begin(), states.end(), overlay); it != states.end())
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
} // namespace ce