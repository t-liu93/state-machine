#include "statemachine.hpp"

State::State(const std::string &name, StateId id)
    : name(name), id(id)
{
}

StateMachine::StateMachine() : currentState(nullptr) {}

void StateMachine::addState(std::unique_ptr<State> state)
{
    states[state->id] = std::move(state);
}

void StateMachine::setInitialState(StateId id)
{
    if (currentState == nullptr)
    {
        if (states.find(id) != states.end())
        {
            currentState = states[id].get();
            currentState->onEnter(*this);
        }
    }
}

void StateMachine::handleEvent(EventId event)
{
}

void StateMachine::update()
{
    // Update the current state
}

State *StateMachine::getCurrentState() const
{
    return currentState;
}
