#pragma once

//========================
// FINITE STATE MACHINE //
//========================
class FSM_STATE;  // pre declaration to use in FSM_TRANSITION

//--------------------
class FSM_TRANSITION {
    //---
public:
    typedef float (*TRANSITION_CBK)(void* data);
    //---
private:
    TRANSITION_CBK  TransitionCbk;
    FSM_STATE* OutputSate;
    FSM_TRANSITION() {}
    //---
public:
    FSM_TRANSITION(TRANSITION_CBK cbk, FSM_STATE* outputState) {
        TransitionCbk = cbk;
        OutputSate = outputState;
    }
    //---
    ~FSM_TRANSITION() {}
    //---
    float Evaluate(void* data) {
        if (TransitionCbk)
            return TransitionCbk(data);
        return 0.f;
    }
    //---
    FSM_STATE* GetOutputState() { return OutputSate; }
};
//--------------------
class FSM;    // pre declaration to use in FSM_STATE
class FSM_STATE {
    //---
public:
    typedef float (*BEHAVIOR_CBK)(void* data);
    //---
private:
    BEHAVIOR_CBK      BehaviorCbk;
    FSM* SubFSM;
    FSM_TRANSITION** Transitions;
    size_t            TransitionsCount;
    //---
    FSM_STATE() { SubFSM = 0; }
    //---
public:
    ~FSM_STATE() {
        if (Transitions)
            delete[] Transitions;
    }
    //---
    FSM_STATE(BEHAVIOR_CBK cbk) { BehaviorCbk = cbk; TransitionsCount = 0; SubFSM = 0; }
    //---
    FSM_STATE(FSM* subFSM) { BehaviorCbk = 0; TransitionsCount = 0; SubFSM = subFSM; }
    //---
    void InitTransitions(size_t count, ...);
    //---
    FSM_STATE* Evaluate(void* data);
    //---
    void  Behave(void* data);
    //---
    void  Reset();
};
//--------------------
class FSM {
    //---
    FSM_STATE** States;
    FSM_STATE* CurrentState;
    size_t      StatesCount;
    //---
    FSM() {};
    //---
public:
    ~FSM() {
        if (States)
            delete[] States;
    };
    //---
    FSM(size_t count, ...);
    //---
    FSM_STATE* Evaluate(void* data);
    //---
    void  Reset() { if (StatesCount) CurrentState = States[0]; else CurrentState = 0; }
    //---
    FSM_STATE* Behave(void* data) { if (CurrentState) CurrentState->Behave(data); return CurrentState; }
};