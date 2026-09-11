#pragma once
#include "chimera_base/IStateMachine.hpp"
#include <memory>
#include <vector>

namespace ce {

    /// @brief Stack os state machines
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20270731
    class StateStack {
      public:
        StateStack() { stateInsert = states.begin(); }
        virtual ~StateStack() { states.clear(); }
        void clear() { states.clear(); }

        void pushState(std::shared_ptr<IStateMachine> state);
        void pushOverlay(std::shared_ptr<IStateMachine> overlay);
        void popState(std::shared_ptr<IStateMachine> state);
        void popOverlay(std::shared_ptr<IStateMachine> overlay);

        std::shared_ptr<IStateMachine> getState(const std::string& name);

        std::vector<std::shared_ptr<IStateMachine>>::iterator begin() { return states.begin(); }
        std::vector<std::shared_ptr<IStateMachine>>::iterator end() { return states.end(); }

      private:
        std::vector<std::shared_ptr<IStateMachine>> states;
        std::vector<std::shared_ptr<IStateMachine>>::iterator stateInsert;
    };
} // namespace ce
