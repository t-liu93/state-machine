#pragma once
#include <string>
#include <array>
#include <cstdint>

constexpr uint32_t transitionTableMultipler = 4; // transition table has <transitionTablemultipler> time size of <MAX_NUMBER_OF_STATES>
enum class StateId;
enum class EventId;

class StateMachineBase;
class State
{
    friend class StateMachineBase;

public:
    explicit State(const std::string &name, StateId id) : name(name), id(id) {};
    virtual ~State() = default;
    virtual void onEnter(StateMachineBase &machine) = 0;
    virtual void onExit(StateMachineBase &machine) = 0;
    virtual void onUpdate(StateMachineBase &machine) = 0;
    StateId getId() const { return id; }
    const std::string &getName() const { return name; }

private:
    std::string name;
    StateId id;
};

class StateMachineBase
{
public:
    virtual ~StateMachineBase() = default;
    virtual void postEvent(EventId event) = 0;
    virtual State *getCurrentState() const = 0;
};

template <uint32_t MAX_NUMBER_OF_STATES, uint32_t MAX_NUMBER_OF_TRANSITIONS = (MAX_NUMBER_OF_STATES * transitionTableMultipler)>
class StateMachine : public StateMachineBase
{
private:
    struct Entry
    {
        StateId id;
        State *ptr;
    };
    struct TransitionEntry
    {
        StateId from;
        EventId event;
        StateId to;
        bool inUse;
    };

public:
    StateMachine() : currentState(nullptr) {}
    StateMachine(const StateMachine &) = delete;
    ~StateMachine() = default;

    void postEvent(EventId event) override
    {
        handleEvent(event);
    }

    State *getCurrentState() const override
    {
        return currentState;
    }

    bool addStateRaw(State *state)
    {
        if (!state)
            return false;
        if (getStateById(state->getId()) != nullptr)
        {
            return false;
        }
        if (stateCount < MAX_NUMBER_OF_STATES)
        {
            states[stateCount++] = {state->getId(), state};
            return true;
        }
        return false;
    }

    void setInitialState(StateId id)
    {
        currentState = getStateById(id);
        if (currentState)
        {
            currentState->onEnter(*this);
        }
    }

    bool addTransition(StateId from, EventId event, StateId to)
    {
        if (transitionCount >= MAX_NUMBER_OF_TRANSITIONS)
            return false;
        for (uint16_t i = 0; i < transitionCount; ++i)
        {
            if (transitions[i].inUse && transitions[i].from == from && transitions[i].event == event)
                return false;
        }
        transitions[transitionCount++] = TransitionEntry{from, event, to, true};
        return true;
    }

    bool removeTransition(StateId from, EventId event)
    {
        for (uint16_t i = 0; i < transitionCount; ++i)
        {
            if (transitions[i].inUse && transitions[i].from == from && transitions[i].event == event)
            {
                transitions[i].inUse = false;
                return true;
            }
        }
        return false;
    }

    const TransitionEntry *findTransition(StateId from, EventId event) const
    {
        for (uint16_t i = 0; i < transitionCount; ++i)
        {
            if (transitions[i].inUse && transitions[i].from == from && transitions[i].event == event)
                return &transitions[i];
        }
        return nullptr;
    }

    bool handleEvent(EventId event)
    {
        if (!currentState)
            return false;
        const auto t = findTransition(currentState->getId(), event);
        if (!t)
            return false;
        auto dst = getStateById(t->to);
        if (!dst)
            return false;
        currentState->onExit(*this);
        currentState = dst;
        currentState->onEnter(*this);
        return true;
    }

    void update()
    {
        if (currentState)
            currentState->onUpdate(*this);
    }

    State *getStateById(StateId id) const
    {
        for (uint16_t i = 0; i < stateCount; ++i)
        {
            if (states[i].id == id)
            {
                return states[i].ptr;
            }
        }
        return nullptr;
    }

private:
    State *currentState;
    std::array<Entry, MAX_NUMBER_OF_STATES> states{};
    uint16_t stateCount = 0;

    std::array<TransitionEntry, MAX_NUMBER_OF_TRANSITIONS> transitions{};
    uint16_t transitionCount = 0;
};
