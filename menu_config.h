#ifndef MENU_CONFIG_H
#define MENU_CONFIG_H

#include "game_data_structures.h"
#include "cJSON.h"                  //include JSON file
#include "load_game_data.h"         //include JSON function
#include "battle_system.h"      //include turn queue system
#include "time_strike.h"

int scenario_initialise(Character* player, ScenarioNode* current_node, int *fights_scenario, int index_subscenarios[MAX_OPTIONS], SkillDictionary* dictionary, Skill *playerSkills);
void subscenario_initialise(ScenarioGraph* graph, Scenario *scenarios, ScenarioNode* current_node, int choice_index, Character* player, int *fights_scenario, int index_subscenarios[MAX_OPTIONS], SkillDictionary* dictionary, Skill* playerSkills);
void menu(Scenario *scenarios, ScenarioGraph *graph, int *fights_scenario, int index_subscenarios[MAX_OPTIONS], Skill *playerSkills);
void configure_character(Character *player);
void start_new_game(Character *player, Scenario *scenarios, ScenarioGraph *graph, int *fights_scenario, int index_subscenarios[MAX_OPTIONS], Skill *playerSkills);
void choose_skills(Character* player, Skill* playerSkills, SkillDictionary* dictionary);

#endif //MENU_CONFIG_H
