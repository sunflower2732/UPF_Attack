#ifndef BATTLE_SYSTEM_H
#define BATTLE_SYSTEM_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "game_data_structures.h"
#include "menu_config.h"
#include "time_strike.h"

// QUEUE TURN SYSTEM FUNCTIONS
Queue* createQueue();
bool isQueueEmpty(Queue* queue);
void enqueue(Queue* queue, Character* character);
Character* dequeue(Queue* queue);
void initializeBattleQueue(Queue* queue, Character* player, Character* enemies[MAX_ENEMIES], int num_enemies);

// FIGHT FLOW FUNCTIONS

bool enemies_defeated(int num_enemies, Character* scenario_enemies[]);
void skills_menu(Character *player, SkillDictionary* dictionary);
void skill_handling(Character *player, int skill, SkillDictionary* dictionary);
int calculateDamage(int attackerAtp, int defenderDef);
void processPlayerTurn(Character* player, Character* scenario_enemies[], int num_enemies, SkillDictionary* dictionary, Skill* playerSkills);
void processEnemyTurn(Character* enemy, Character* player);
bool fight_flow(Queue* queue, Character* player, Character* scenario_enemies[], int num_enemies, SkillDictionary* dictionary, Skill* playerSkills);
bool is_index(int index, int index_scenario[MAX_OPTIONS], int *fight_scenarios);
void fight_flow_management(bool result, Character *player, Scenario *scenarios, ScenarioGraph *graph, ScenarioNode *current_node, int *fights_scenarios, int index_scenarios[MAX_OPTIONS], SkillDictionary* dictionary, Skill* playerSkills);

#endif //BATTLE_SYSTEM_H
