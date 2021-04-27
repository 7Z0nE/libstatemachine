//
// Created by fred on 4/26/21.
//
#include "test_state_machine.h"
#include "state_machine.h"
#include <iostream>

enum STATES { A, B };

void stateABehaviour(StateMachine<STATES>& sm) {
    std::cout << "State A" << std::flush << std::endl;
}

void stateBBehaviour(StateMachine<STATES>& sm) {
    std::cout << "State B" << std::flush << std::endl;
}

StateMachineTest::StateMachineTest() {};

StateMachineTest::~StateMachineTest() {};

void StateMachineTest::SetUp() {

};

void StateMachineTest::TearDown() {};

TEST(StateMachineTest, ConstructStateMachine) {
    StateMachine<STATES> sm();
}

TEST(StateMachineTest, CreateLambdaState) {
    LambdaState<STATES> s(A, &stateABehaviour, "A");
}

TEST(StateMachineTest, RegisterState) {
    StateMachine<STATES> sm;
    std::shared_ptr<State<STATES>> s(new LambdaState<STATES>(A, &stateABehaviour, "A"));
    sm.registerState(s);
}

TEST(StateMachineTest, Start) {
    StateMachine<STATES> sm;
    std::shared_ptr<State<STATES>> s(new LambdaState<STATES>(A, &stateABehaviour, "A"));
    sm.registerState(s);
    sm.start(A);
    EXPECT_TRUE(sm.getRunning());
    EXPECT_EQ(sm.getState(), A);
}

TEST(StateMachineTest, Transition) {
    StateMachine<STATES> sm;
    std::shared_ptr<State<STATES>> s_a(new LambdaState<STATES>(A, &stateABehaviour, "A"));
    std::shared_ptr<State<STATES>> s_b(new LambdaState<STATES>(B, &stateBBehaviour, "B"));
    sm.registerState(s_a);
    sm.registerState(s_b);
    sm.registerTransition(A, B);
    sm.start(A);
    EmptyData e;
    EXPECT_NO_THROW(sm.transition(B, e));
    EXPECT_EQ(sm.getState(), B);
    EXPECT_THROW(sm.transition(STATES::A, e), std::runtime_error);
}