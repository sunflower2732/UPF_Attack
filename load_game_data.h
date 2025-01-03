#ifndef LOAD_GAME_DATA_H
#define LOAD_GAME_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_data_structures.h"
#include "graph_scenario.h"

void load_game_data(const char* filename, Scenario* scenarios, ScenarioGraph* graph, Skill* playerSkills);

#endif // LOAD_GAME_DATA_H