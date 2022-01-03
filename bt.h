#pragma once

//==================
// BEHAVIOUR TREE //
//==================

class BT_NODE {
  //---
public:
  enum State {
    RUNNING,
    SUCCESS,
    FAILURE
  };
  //---
protected:
  BT_NODE** Children;
  size_t    ChildrenCount;
  size_t    ChildrenMaxCount;
  BT_NODE*  Parent;
  //---
  ~BT_NODE() {
    delete[] Children;
  }
  //---
  BT_NODE(size_t childrenMaxCount) : Parent(0), ChildrenMaxCount(childrenMaxCount), ChildrenCount(0) {
    InitChildrenArray(childrenMaxCount);
  }
  //---
  BT_NODE(BT_NODE* parent, size_t childrenMaxCount) : Parent(parent), ChildrenMaxCount(childrenMaxCount), ChildrenCount(0) {
    InitChildrenArray(childrenMaxCount);
    if (Parent)
      Parent->AddChild(this);
  }
  //---
private:
  BT_NODE() {}
  //---
  void InitChildrenArray(size_t childrenMaxCount) {
    Children = new BT_NODE * [ChildrenMaxCount];
  }
  //---
public:
  virtual State Evaluate(void* data) = 0;
  //---
  size_t AddChild(BT_NODE* child);
  //---
};

class BT_SEQUENCER : public BT_NODE {
public:
  BT_SEQUENCER(BT_NODE* parent, size_t childrenMaxCount) : BT_NODE(parent, childrenMaxCount) {};
  BT_SEQUENCER(size_t childrenMaxCount) : BT_NODE(childrenMaxCount) {};
  State Evaluate(void* data);
};

class BT_SELECTOR : public BT_NODE {
public:
  BT_SELECTOR(BT_NODE* parent, size_t childrenMaxCount) : BT_NODE(parent, childrenMaxCount) {};
  BT_SELECTOR(size_t childrenMaxCount) : BT_NODE(childrenMaxCount) {};
  State Evaluate(void* data);
};

class BT_LEAF : public BT_NODE {
public:
  typedef BT_NODE::State (*EVALUATE_CBK)(void* data);
protected:
  EVALUATE_CBK  EvaluateCBK;
public:
  BT_LEAF(BT_NODE* parent, EVALUATE_CBK evaluateCBK) : EvaluateCBK(evaluateCBK), BT_NODE(parent, 0) {}
  State Evaluate(void* data);
};
