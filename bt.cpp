#include <cassert>
#include "bt.h"

size_t BT_NODE::AddChild(BT_NODE* child) {
	assert(ChildrenCount < ChildrenMaxCount);
	Children[ChildrenCount++] = child;
	return ChildrenCount;
}

BT_NODE::State	BT_SEQUENCER::Evaluate(void* data) {
	for (size_t childIdx = 0; childIdx < ChildrenCount; ++childIdx) {
		BT_NODE::State	childState = Children[childIdx]->Evaluate(data);
		if (childState == BT_NODE::State::FAILURE) {
			return BT_NODE::State::FAILURE;
		}
		if (childState == BT_NODE::State::RUNNING) {
			return BT_NODE::State::FAILURE;
		}
	}
	return BT_NODE::State::SUCCESS;
}

BT_NODE::State	BT_SELECTOR::Evaluate(void* data) {
	for (size_t childIdx = 0; childIdx < ChildrenCount; ++childIdx) {
		BT_NODE::State	childState = Children[childIdx]->Evaluate(data);
		if (childState == BT_NODE::State::SUCCESS) {
			return BT_NODE::State::SUCCESS;
		}
		if (childState == BT_NODE::State::RUNNING) {
			return BT_NODE::State::FAILURE;
		}
	}
	return BT_NODE::State::FAILURE;
}

BT_NODE::State	BT_LEAF::Evaluate(void* data) {
	assert(EvaluateCBK);
	return EvaluateCBK(data);
}
