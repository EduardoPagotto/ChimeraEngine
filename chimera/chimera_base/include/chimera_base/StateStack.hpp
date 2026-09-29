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
        StateStack() { state_insert_ = states_.begin(); }
        virtual ~StateStack() { states_.clear(); }
        void clear() { states_.clear(); }

        void pushState(std::shared_ptr<IStateMachine> state);
        void pushOverlay(std::shared_ptr<IStateMachine> overlay);
        void popState(std::shared_ptr<IStateMachine> state);
        void popOverlay(std::shared_ptr<IStateMachine> overlay);

        std::shared_ptr<IStateMachine> getState(const std::string& name);

        std::vector<std::shared_ptr<IStateMachine>>::iterator begin() { return states_.begin(); }
        std::vector<std::shared_ptr<IStateMachine>>::iterator end() { return states_.end(); }

      private:
        std::vector<std::shared_ptr<IStateMachine>> states_;
        std::vector<std::shared_ptr<IStateMachine>>::iterator state_insert_;
    };
} // namespace ce
