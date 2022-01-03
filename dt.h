#pragma once

//=================
// DECISION TREE //
//=================

class DT_NODE {
    //---
public:
    typedef float (*EVALUATE_CBK)(void* data);
    //---
private:
    EVALUATE_CBK  EvaluateCbk;
    DT_NODE* TrueNode;
    DT_NODE* FalseNode;
    //---
    DT_NODE() {}
    //---
    float          Condition(void* data) {
        if (EvaluateCbk) // check if callback is not null
            return EvaluateCbk(data);
        else
            return 0.f; // by default returns false (0.f)
    };
    //---
public:
    ~DT_NODE() {}
    //---
    DT_NODE(DT_NODE* trueNode, DT_NODE* falseNode, EVALUATE_CBK cbk) {
        TrueNode = trueNode;
        FalseNode = falseNode;
        EvaluateCbk = cbk;
    }
    //---
    DT_NODE* Evaluate(void* data) {
        if (TrueNode && FalseNode) {
            if (Condition(data) > 0.5f)
                return TrueNode->Evaluate(data);  // returns the result of the true child node
            else
                return FalseNode->Evaluate(data); // returns the result of the false child node
        }
        if (EvaluateCbk)
            EvaluateCbk(data);  // if callback exists, call it (to perform the action)
        return this;  // return the current node, here we know that we are a leaf.
    }
    //---
};

