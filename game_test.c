#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"                  //include JSON file
#include "game_data_structures.h"   //include data structures
#include "load_game_data.h"         //include JSON function
#include "menu_config.h"            //include menu functions
#include "battle_system.h"
#include "graph_scenario.h"

int main() {
    //initialise scenarios, graph, and skills and load them from JSON
    Scenario scenarios[MAX_SCENARIOS];
    ScenarioGraph graph;
    graph.num_nodes = 0;
    Skill playerSkills[MAX_PLAYER_SKILLS];
    int numPlayerSkills = 0;
    load_game_data("game_config.json", scenarios, &graph, playerSkills);
    
    //keep track of the subscenarios of the main scenario the player has visited
    int *fights_scenario = malloc(sizeof(int)); 
    int index_subscenario[MAX_OPTIONS];

    //start game menu
    menu(scenarios, &graph, fights_scenario, index_subscenario, playerSkills);
    return 0;
}
