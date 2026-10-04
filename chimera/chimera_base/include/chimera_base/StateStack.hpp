#pragma once
#include "IStateMachine.hpp"
#include <memory>
#include <vector>

namespace ce {

    /// @brief Stack os state machines
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20261004
    class StateStack {
      public:
        StateStack() { state_insert_ = states_.begin(); }
        virtual ~StateStack() { states_.clear(); }
        void clear() { states_.clear(); }

        void push_state(std::shared_ptr<IStateMachine> state);
        void push_overlay(std::shared_ptr<IStateMachine> overlay);
        void pop_state(std::shared_ptr<IStateMachine> state);
        void pop_overlay(std::shared_ptr<IStateMachine> overlay);

        std::shared_ptr<IStateMachine> get_state(const std::string& name);

        std::vector<std::shared_ptr<IStateMachine>>::iterator begin() { return states_.begin(); }
        std::vector<std::shared_ptr<IStateMachine>>::iterator end() { return states_.end(); }

      private:
        std::vector<std::shared_ptr<IStateMachine>> states_;
        std::vector<std::shared_ptr<IStateMachine>>::iterator state_insert_;
    };
} // namespace ce
