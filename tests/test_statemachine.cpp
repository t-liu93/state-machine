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
        // Logic for entering Idle state
    }
    void onExit(StateMachine &machine) override
    {
        // Logic for exiting Idle state
    }
};

class RunningState : public State
{
public:
    RunningState() : State("Running", StateId::Running) {}
    void onEnter(StateMachine &machine) override
    {
        // Logic for entering Running state
    }
    void onExit(StateMachine &machine) override
    {
        // Logic for exiting Running state
    }
};

class StoppedState : public State
{
public:
    StoppedState() : State("Stopped", StateId::Stopped) {}
    void onEnter(StateMachine &machine) override
    {
        // Logic for entering Stopped state
    }
    void onExit(StateMachine &machine) override
    {
        // Logic for exiting Stopped state
    }
};

class StateMachineTest : public ::testing::Test
{
protected:
    StateMachine sm;
    IdleState idleState;
    RunningState runningState;
    StoppedState stoppedState;
};

TEST_F(StateMachineTest, InitialStateIsNull)
{
    EXPECT_EQ(sm.getCurrentState(), nullptr);
}