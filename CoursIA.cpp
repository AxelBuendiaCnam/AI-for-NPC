// CoursIA.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <cstdarg>  // used for variables args in FSM_STATE constructor
#include "dt.h"
#include "fsm.h"
#include "bt.h"

float FleeBhv(void* data) { std::cout << "Flee bahevior is active!" << std::endl; return 1.f;  };
float GuardBhv(void* data) { std::cout << "Guard bahevior is active!" << std::endl; return 1.f; };
float AttackBhv(void* data) { std::cout << "Attack bahevior is active!" << std::endl; return 1.f;  };

BT_NODE::State BT_FleeBhv(void* data) { std::cout << "Flee bahevior is active!" << std::endl; return BT_NODE::State::SUCCESS; };
BT_NODE::State BT_GuardBhv(void* data) { std::cout << "Guard bahevior is active!" << std::endl; return BT_NODE::State::SUCCESS; };
BT_NODE::State BT_AttackBhv(void* data) { std::cout << "Attack bahevior is active!" << std::endl; return BT_NODE::State::SUCCESS; };

struct ENVIRONMENT {
  float EnemyVisible;
  float EnemyStronger;
};

float NoEnemy(void* data) { ENVIRONMENT* env = ((ENVIRONMENT*)data); return (1.f - env->EnemyVisible); };
float StrongerEnemy(void* data) { ENVIRONMENT* env = ((ENVIRONMENT*)data); return env->EnemyVisible * env->EnemyStronger; };
float WeakerEnemy(void* data) { ENVIRONMENT* env = ((ENVIRONMENT*)data); return env->EnemyVisible * (1.f - env->EnemyStronger); };

BT_NODE::State  EnemyNearby(void* data) { ENVIRONMENT* env = ((ENVIRONMENT*)data); if (env->EnemyVisible > 0.f) return BT_NODE::State::SUCCESS; return BT_NODE::State::FAILURE; }
BT_NODE::State  EnemyWeaker(void* data) { ENVIRONMENT* env = ((ENVIRONMENT*)data); if (env->EnemyStronger <= 0.f) return BT_NODE::State::SUCCESS; return BT_NODE::State::FAILURE; }

int main() {
  ENVIRONMENT     Env;
  void* NPCAgent = 0;
  // DECISION TREE TEST
  DT_NODE DT_AttackLeaf(0, 0, AttackBhv);
  DT_NODE DT_FleeLeaf(0, 0, FleeBhv);
  DT_NODE DT_GuardLeaf(0, 0, GuardBhv);
  DT_NODE DT_EnemyStronger(&DT_FleeLeaf, &DT_AttackLeaf, StrongerEnemy);
  DT_NODE DT_EnemyVisibleRoot(&DT_GuardLeaf, &DT_EnemyStronger, NoEnemy);
  std::cout << "Decision Tree demo!" << std::endl;
  // FINITE STATE MACHINE TEST
  FSM_STATE       FSM_Flee(FleeBhv), FSM_Guard(GuardBhv), FSM_Attack(AttackBhv);
  FSM_TRANSITION  FSM_FleeGuard(NoEnemy, &FSM_Guard), FSM_FleeAttack(WeakerEnemy, &FSM_Attack);
  FSM_Flee.InitTransitions(2, &FSM_FleeGuard, &FSM_FleeAttack);
  FSM_TRANSITION  FSM_GuardAttack(WeakerEnemy, &FSM_Attack), FSM_GuardFlee(WeakerEnemy, &FSM_Flee);
  FSM_Guard.InitTransitions(2, &FSM_GuardAttack, &FSM_GuardFlee);
  FSM_TRANSITION  FSM_AttackGuard(NoEnemy, &FSM_Guard), FSM_AttackFlee(StrongerEnemy, &FSM_Flee);
  FSM_Attack.InitTransitions(2, &FSM_AttackGuard, &FSM_AttackFlee);
  FSM             FSM_NPCGuard(3, &FSM_Flee, &FSM_Guard, &FSM_Attack);
  std::cout << "Finite State Machine demo!" << std::endl;
  // BEHAVIOUR TREE
  BT_SELECTOR     BT_NPCGuard(3);
  BT_SEQUENCER    BT_FIGHT(&BT_NPCGuard, 3);
  BT_LEAF         BT_EnemyNearby(&BT_FIGHT, EnemyNearby);
  BT_LEAF         BT_EnemyWeaker(&BT_FIGHT, EnemyWeaker);
  BT_LEAF         BT_FightEnemy(&BT_FIGHT, BT_AttackBhv);
  BT_SEQUENCER    BT_FLEE(&BT_NPCGuard, 2);
  BT_FLEE.AddChild(&BT_EnemyNearby);  // Use the same node
  BT_LEAF         BT_FleeEnemy(&BT_FLEE, BT_FleeBhv);
  BT_LEAF         BT_GUARD(&BT_NPCGuard, BT_GuardBhv);
  std::cout << "Behaviour Tree demo!" << std::endl;
  // Start thinking
  //--- Cycle 1 ---
  Env.EnemyVisible = 0.f;
  Env.EnemyStronger = 0.f;
  DT_EnemyVisibleRoot.Evaluate(&Env);      // NPC should Guard
  FSM_NPCGuard.Evaluate(&Env);
  FSM_NPCGuard.Behave(NPCAgent);  // NPC should Guard
  BT_NPCGuard.Evaluate(&Env);     // NPC should Guard
  //--- Cycle 2 ---
  Env.EnemyVisible = 0.7f;
  Env.EnemyStronger = 0.f;
  DT_EnemyVisibleRoot.Evaluate(&Env);      // NPC should Attack
  FSM_NPCGuard.Evaluate(&Env);
  FSM_NPCGuard.Behave(NPCAgent);  // NPC should Attack
  BT_NPCGuard.Evaluate(&Env);     // NPC should Attack
}
