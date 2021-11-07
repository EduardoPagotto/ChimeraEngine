#pragma once
#include "IStateMachine.hpp"
#include <vector>

namespace Chimera {

class StateStack {
  public:
    StateStack();
    ~StateStack();
    inline void clear() { layers.clear(); }

    void pushLayer(IStateMachine* layer);
    void pushOverlay(IStateMachine* overlay);
    void popLayer(IStateMachine* layer);
    void popOverlay(IStateMachine* overlay);

    std::vector<IStateMachine*>::iterator begin() { return layers.begin(); }
    std::vector<IStateMachine*>::iterator end() { return layers.end(); }

  private:
    std::vector<IStateMachine*> layers;
    std::vector<IStateMachine*>::iterator layerInsert;
};
} // namespace Chimera