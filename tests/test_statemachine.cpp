#include <gtest/gtest.h>
#include "statemachine.hpp"

enum class StateId
{
    Idle,
    Running,
    Stopped
};

enum class EventId
{
    Start,
    Stop,
    Pause
};

class IdleState : public State
{
public:
    IdleState() : State("Idle", StateId::Idle) {}
    void onEnter(StateMachine &machine) override
    {
        entered = true;
    }
    void onExit(StateMachine &machine) override
    {
        // Logic for exiting Idle state
    }
    bool entered = false;
};

class RunningState : public State
{
public:
    RunningState() : State("Running", StateId::Running) {}
    void onEnter(StateMachine &machine) override
    {
        entered = true;
        // Logic for entering Running state
    }
    void onExit(StateMachine &machine) override
    {
        // Logic for exiting Running state
    }
    bool entered = false;
};

class StoppedState : public State
{
public:
    StoppedState() : State("Stopped", StateId::Stopped) {}
    void onEnter(StateMachine &machine) override
    {
    }
    void onExit(StateMachine &machine) override
    {
        // Logic for exiting Stopped state
    }
};

class StateMachineTest : public ::testing::Test
{
};

TEST_F(StateMachineTest, InitialStateIsNull)
{
    StateMachine sm;
    EXPECT_EQ(sm.getCurrentState(), nullptr);
}

TEST_F(StateMachineTest, AddAndSetInitialState)
{
    StateMachine sm;
    auto idle = std::make_unique<IdleState>();
    auto originPtr = idle.get();
    sm.addState(std::move(idle));
    EXPECT_EQ(sm.getCurrentState(), nullptr);
    EXPECT_EQ(originPtr->entered, false);
    sm.setInitialState(StateId::Idle);
    EXPECT_EQ(sm.getCurrentState()->getId(), StateId::Idle);
    EXPECT_EQ(originPtr->entered, true);
}
