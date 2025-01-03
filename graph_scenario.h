#ifndef GRAPH_SCENARIO_H
#define GRAPH_SCENARIO_H

#define MAX_NEIGHBOURS 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "game_data_structures.h"
#include "load_game_data.h"

ScenarioNode* createScenarioNode(Scenario* scenario);
void addScenarioNode(ScenarioGraph* graph, ScenarioNode* scenario);
void connectScenarioNodes(ScenarioGraph* graph);
void freeScenarioGraph(ScenarioGraph* graph);

#endif // GRAPH_SCENARIO_H
