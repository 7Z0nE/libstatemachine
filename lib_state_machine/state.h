#ifndef STATE_H
#define STATE_H

#include "state_machine.h"
#include "event_data.h"
#include <string>


template <typename E>
class StateMachine;

template <class E>
class State {
private:
    E id;
    std::string name;
public:
    State (E id, std::string name);
    virtual ~State() {};
    virtual void enter(const EventData& data) = 0;
    virtual void exit() = 0;
    virtual void act(StateMachine<E>& state_machine) = 0;
    E get_id();
    std::string get_name();
};

template <typename E>
class LambdaState : public State<E> {
private:
    void (*const act_func) (StateMachine<E>& state_machine);
public:
    LambdaState(E id, void (*const act_func) (StateMachine<E>& state_machine), std::string name);
    virtual void enter(const EventData& data);
    virtual void act(StateMachine<E>& state_machine);
    virtual void exit();
};


/// IMPLEMENTATION ///

template <typename E>
State<E>::State(E id, std::string name) : id{id}, name{name} {}

template <typename E>
E State<E>::get_id() {
    return this->id;
}

template <typename E>
std::string State<E>::get_name() {
    return this->name;
}

template <typename E>
LambdaState<E>::LambdaState(
    E id,
    void (*const act_func) (StateMachine<E>& state_machine),
    std::string name)
    : State<E>(id, name), act_func{act_func} {}
    
template <typename E>
void LambdaState<E>::enter(const EventData& data) {}

template <typename E>
void LambdaState<E>::act(StateMachine<E>& state_machine) {
    this->act_func(state_machine);
}

template <typename E>
void LambdaState<E>::exit() {}


#endif // STATE_H
