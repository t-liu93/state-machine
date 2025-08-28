#include <gtest/gtest.h>
#include "statemachine.hpp"

enum class StateId
{
    Idle,
    Running,
    Stopped,
    A,
    B,
    C,
};

enum class EventId
{
    Start,
    Stop,
    Pause,
    E1,
    E2
};

class IdleState : public State
{
public:
    IdleState() : State("Idle", StateId::Idle) {}
    void onEnter(StateMachineBase &machine) override
    {
        entered = true;
    }
    void onExit(StateMachineBase &machine) override
    {
        exited = true;
    }
    void onUpdate(StateMachineBase &machine) override
    {
        updateCounter++;
    }
    bool entered = false;
    bool exited = false;
    uint32_t updateCounter = 0;
};

class RunningState : public State
{
public:
    RunningState() : State("Running", StateId::Running) {}
    void onEnter(StateMachineBase &machine) override
    {
        entered = true;
    }
    void onExit(StateMachineBase &machine) override
    {
        exited = true;
    }
    void onUpdate(StateMachineBase &machine) override
    {
        updateCounter++;
    }
    bool entered = false;
    bool exited = false;
    uint32_t updateCounter = 0;
};

struct LogState : State
{
    std::vector<std::string> *log;
    LogState(const std::string &n, StateId id, std::vector<std::string> *l) : State(n, id), log(l) {}
    void onEnter(StateMachineBase &m) override { log->push_back(getName() + ":enter"); }
    void onExit(StateMachineBase &m) override { log->push_back(getName() + ":exit"); }
    void onUpdate(StateMachineBase &m) override { log->push_back(getName() + ":update"); }
};

class StateMachineTest : public ::testing::Test
{
};

TEST_F(StateMachineTest, InitialStateIsNull)
{
    StateMachine<5> sm;
    EXPECT_EQ(sm.getCurrentState(), nullptr);
}

TEST_F(StateMachineTest, AddState)
{
    StateMachine<5> sm;
    IdleState idle;
    EXPECT_TRUE(sm.addStateRaw(&idle));
    EXPECT_FALSE(sm.addStateRaw(&idle)); // Duplicate state
}

TEST_F(StateMachineTest, AddStateNullptr)
{
    StateMachine<5> sm;
    EXPECT_FALSE(sm.addStateRaw(nullptr));
}

TEST_F(StateMachineTest, AddStateOverload)
{
    StateMachine<1> sm;
    IdleState idle;
    RunningState running;
    EXPECT_TRUE(sm.addStateRaw(&idle));
    EXPECT_FALSE(sm.addStateRaw(&running)); // Exceeds max states
}

TEST_F(StateMachineTest, SetInitialState)
{
    StateMachine<5> sm;
    IdleState idle;
    sm.addStateRaw(&idle);
    sm.setInitialState(StateId::Idle);
    EXPECT_EQ(sm.getCurrentState()->getId(), StateId::Idle);
    EXPECT_TRUE(idle.entered);
}

TEST_F(StateMachineTest, GetStateById)
{
    StateMachine<5> sm;
    IdleState idle;
    sm.addStateRaw(&idle);
    EXPECT_EQ(sm.getStateById(StateId::Idle), &idle);
    EXPECT_EQ(sm.getStateById(StateId::Running), nullptr);
}

TEST_F(StateMachineTest, Update)
{
    StateMachine<5> sm;
    IdleState idle;
    sm.addStateRaw(&idle);
    sm.setInitialState(StateId::Idle);
    EXPECT_EQ(idle.updateCounter, 0);
    sm.update();
    EXPECT_EQ(idle.updateCounter, 1);
    sm.update();
    EXPECT_EQ(idle.updateCounter, 2);
}

TEST_F(StateMachineTest, EventTransitionSuccess)
{
    StateMachine<5> sm;
    IdleState idle;
    RunningState running;
    sm.addStateRaw(&idle);
    sm.addStateRaw(&running);
    sm.setInitialState(StateId::Idle);
    EXPECT_TRUE(sm.addTransition(StateId::Idle, EventId::Start, StateId::Running));
    EXPECT_TRUE(idle.entered);
    sm.handleEvent(EventId::Start);
    EXPECT_TRUE(running.entered);
    EXPECT_TRUE(idle.exited);
    EXPECT_EQ(sm.getCurrentState()->getId(), StateId::Running);
    EXPECT_EQ(running.updateCounter, 0);
    sm.update();
    EXPECT_EQ(running.updateCounter, 1);
}

TEST(StateMachineCorner, GetStateByIdOnlyChecksRegistered)
{
    constexpr uint32_t N = 3;
    StateMachine<N> sm;
    std::vector<std::string> log;
    LogState s1("s1", StateId::A, &log);
    EXPECT_TRUE(sm.addStateRaw(&s1));
    // get existing
    EXPECT_NE(sm.getStateById(StateId::A), nullptr);
    // get non-existing must be nullptr
    EXPECT_EQ(sm.getStateById(StateId::B), nullptr);
}

TEST(StateMachineCorner, TransitionOverflowAndMissingDst)
{
    constexpr uint32_t N = 2;
    constexpr uint32_t M = 2; // small transition table
    StateMachine<N, M> sm;
    std::vector<std::string> log;
    LogState s1("s1", StateId::A, &log), s2("s2", StateId::B, &log);
    sm.addStateRaw(&s1);
    sm.addStateRaw(&s2);
    EXPECT_TRUE(sm.addTransition(StateId::A, EventId::E1, StateId::B));
    EXPECT_TRUE(sm.addTransition(StateId::B, EventId::E2, StateId::A));
    // overflow
    EXPECT_FALSE(sm.addTransition(StateId::A, EventId::E2, StateId::B));
    // remove and verify missing dst handling
    sm.removeTransition(StateId::A, EventId::E1);
    // recreate but point to unregistered id
    EXPECT_FALSE(sm.addTransition(StateId::A, EventId::E1, StateId::C));
    sm.setInitialState(StateId::A);
    EXPECT_FALSE(sm.handleEvent(EventId::E1)); // should fail because dst missing
}