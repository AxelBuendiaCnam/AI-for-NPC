#include <cstdarg>  // used for variables args in FSM_STATE constructor
#include "fsm.h"

void FSM_STATE::InitTransitions(size_t count, ...) {
    if (TransitionsCount) // already initialized
        return;
    TransitionsCount = count;
    va_list transitionsList;          // create the variable arguments list
    va_start(transitionsList, count); // init the variable arguments list
    Transitions = new FSM_TRANSITION * [TransitionsCount];  // initialize the array of transitions
    for (size_t iTransition = 0; iTransition < TransitionsCount; ++iTransition) {
        Transitions[iTransition] = va_arg(transitionsList, FSM_TRANSITION*);  // set each transition
    }
    va_end(transitionsList);  // close the variable arguments list
}
//---
FSM_STATE* FSM_STATE::Evaluate(void* data) {
    float           bestTransitionScore = 0.f;
    FSM_TRANSITION* bestTransition = 0;
    for (size_t iTransition = 0; iTransition < TransitionsCount; ++iTransition) {
        FSM_TRANSITION* transition = Transitions[iTransition];
        float score = transition->Evaluate(data);
        if (score > bestTransitionScore) {
            bestTransitionScore = score;
            bestTransition = transition;
        }
    }
    if (bestTransition)
        return bestTransition->GetOutputState();
    return this;  // Default state is self
}
//---
void  FSM_STATE::Behave(void* data) {
    if (BehaviorCbk)
        BehaviorCbk(data);
    else if (SubFSM)
        SubFSM->Evaluate(data);
}
//---
void  FSM_STATE::Reset() {
  if (SubFSM) SubFSM->Reset();
}
//--------------------
FSM::FSM(size_t count, ...) {
    StatesCount = count;
    CurrentState = 0;
    va_list statesList;          // create the variable arguments list
    va_start(statesList, count); // init the variable arguments list
    States = new FSM_STATE * [StatesCount];  // initialize the array of transitions
    for (size_t iState = 0; iState < StatesCount; ++iState) {
        States[iState] = va_arg(statesList, FSM_STATE*);  // set each transition
    }
    va_end(statesList);  // close the variable arguments list
};
//---
FSM_STATE* FSM::Evaluate(void* data) {
    if (!CurrentState) {
        if (!StatesCount)
            return 0;
        CurrentState = States[0];
    }
    FSM_STATE*  newCurrentState = CurrentState->Evaluate(data);
    if (CurrentState != newCurrentState)
        CurrentState->Reset();
    CurrentState = newCurrentState;
    return CurrentState;
}