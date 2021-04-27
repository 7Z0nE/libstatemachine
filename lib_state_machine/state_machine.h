#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <map>
#include <vector>
#include "state.h"
#include "event_data.h"
#include <stdexcept>
#include <memory>
#include <string>

template <typename E>
class State;

template <typename E>
using StateMap = std::map<E, std::shared_ptr<State<E>>>;

template <typename E>
using StateMapEntry = std::pair<E, std::shared_ptr<State<E>>>;

template <typename E>
using TransitionMap = std::map<std::pair<E, E>, bool>;

template <typename E>
using TransitionMapEntry = std::pair<std::pair<E,E>, bool>;

template <typename E>
class StateMachine {

private:
    StateMap<E> states;
    TransitionMap<E> allowed_transitions;
    E state;
    bool running = false;

public:
    StateMachine();
    void registerState(const std::shared_ptr<State<E>>& state);
    void registerTransition(E from, E to);
    bool transition(E to, const EventData& d);
    E getState();
    bool getRunning();
    std::vector<E> getAvailableTransitions();
    TransitionMap<E> getTransitionTable();
    void start(E initial_state);
    void stop();
};


/// IMPLEMENTATION ///

template <typename E>   
StateMachine<E>::StateMachine() {}

template <typename E>   
void StateMachine<E>::registerState(const std::shared_ptr<State<E>>& new_state) {
    E new_id = new_state->get_id();
    this->states.insert(StateMapEntry<E>(new_id, new_state));
    for(const auto& v: this->states) {
        E id = v.first;
        this->allowed_transitions.insert(TransitionMapEntry<E>(std::pair<E,E>(new_id, id), false));
        this->allowed_transitions.insert(TransitionMapEntry<E>(std::pair<E,E>(id, new_id), false));
    }
}


template <typename E>   
void StateMachine<E>::registerTransition(E from, E to) {
    typename StateMap<E>::iterator it_from = this->states.find(from);
    if (it_from == this->states.end())
        throw std::runtime_error("Transition cannot be registered because state given as from doesnt exist.");
    
    typename StateMap<E>::iterator it_to = this->states.find(to);
    if (it_to == this->states.end())
        throw std::runtime_error("Transition cannot be registered because state given as to doesnt exist.");
    
    typename TransitionMap<E>::iterator it = this->allowed_transitions.find(std::pair<E,E>(from, to));
    if (it == this->allowed_transitions.end())
        this->allowed_transitions.insert(TransitionMapEntry<E>(std::pair<E,E>(from, to), true));
    else
        it->second = true;
}


template <typename E>   
bool StateMachine<E>::transition(E to, const EventData& d) {
    typename StateMap<E>::iterator it_to = this->states.find(to);
    char err_msg[1000];
    if (it_to == this->states.end()) {
        throw std::runtime_error("State is not registered.");
    }
    typename TransitionMap<E>::iterator it = this->allowed_transitions.find(std::pair<E,E>(this->state, to));
    if (it == this->allowed_transitions.end()){
        throw std::runtime_error("Transition not registered and therefore not allowed.");
    }
    else if (!it->second) {
        std::sprintf(err_msg, "Transition from {} to {} is not allowed.", this->states.at(this->state)->get_name(), this->states.at(this->state)->get_name());
        throw std::runtime_error("Transition forbidden.");
    }
    if (! this->running) {
        throw std::runtime_error("Start SM before trying to transition.");
    }
    this->states.at(this->state)->exit();
    this->state = to;
    this->states.at(this->state)->enter(d);
    this->states.at(this->state)->act(*this);
    return 1;
}


template <typename E>   
E StateMachine<E>::getState() {
    return this->state;
}


template <typename E>   
std::vector<E> StateMachine<E>::getAvailableTransitions() {
    std::vector<E> available_transitions;
    for (const auto& s:this->states) {
        if (this->allowed_transitions.at(std::pair<E,E>(this->state, s.first))) {
            available_transitions.push_back(s.first);
        }
    }
    return available_transitions;
}


template <typename E>   
TransitionMap<E> StateMachine<E>::getTransitionTable() {
    return this->allowed_transitions;
}


template <typename E>   
void StateMachine<E>::start(E initial_state) {
    this->running = true;
    EmptyData e;
    this->state = initial_state;
    this->states.at(initial_state)->enter(e);
    this->states.at(initial_state)->act(*this);
}

template <typename E>
void StateMachine<E>::stop() {
    this->running = false;
}

template <typename E>
bool StateMachine<E>::getRunning() {
    return this->running;
}

#endif // STATE_MACHINE_H