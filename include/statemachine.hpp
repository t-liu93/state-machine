#pragma once
#include <string>
#include <map>
#include <memory>

enum class StateId;
enum class EventId;

using TransitionKey = std::pair<StateId, EventId>;
using TransitionTable = std::map<TransitionKey, StateId>;

class StateMachine;

class State
{
    friend class StateMachine;

public:
    explicit State(const std::string &name, StateId id);
    virtual ~State() = default;
    virtual void onEnter(StateMachine &machine) = 0;
    virtual void onExit(StateMachine &machine) = 0;
    StateId getId() const { return id; }
    const std::string &getName() const { return name; }

private:
    std::string name;
    StateId id;
};

class StateMachine
{
public:
    StateMachine();
    ~StateMachine() = default;

    void addState(std::unique_ptr<State> state);
    void setInitialState(StateId id);
    void handleEvent(EventId event);
    void update();

    State *getCurrentState() const;

private:
    State *currentState;
    std::map<StateId, std::unique_ptr<State>> states;
};
