#pragma once
#include "IStateMachine.hpp"
#include <vector>

namespace Chimera {

class StateStack {
  public:
    StateStack();
    ~StateStack();
    inline void clear() { states.clear(); }

    void pushState(IStateMachine* state);
    void pushOverlay(IStateMachine* overlay);
    void popState(IStateMachine* state);
    void popOverlay(IStateMachine* overlay);

    std::vector<IStateMachine*>::iterator begin() { return states.begin(); }
    std::vector<IStateMachine*>::iterator end() { return states.end(); }

  private:
    std::vector<IStateMachine*> states;
    std::vector<IStateMachine*>::iterator stateInsert;
};
} // namespace Chimera