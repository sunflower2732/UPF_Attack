#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph_scenario.h"

ScenarioNode* createScenarioNode(Scenario* scenario) {
    ScenarioNode* node = (ScenarioNode*)malloc(sizeof(ScenarioNode));
    if (node) {
        node->scenario = *scenario;
        node->neighbours = (ScenarioNode**)malloc(sizeof(ScenarioNode) * MAX_NEIGHBOURS);
    }
    return node;
}

void addScenarioNode(ScenarioGraph* graph, ScenarioNode* scenario) {
    if (graph->num_nodes >= MAX_SCENARIOS) {
        printf("Error: Maximum number of scenarios reached.\n");
        return;
    }
    graph->nodes[graph->num_nodes++] = scenario; // Add the node to the link of nodes and update number of nodes afterwards.
}

void connectScenarioNodes(ScenarioGraph* graph) {  
    //graph_nodes = [L, G, T1, T2, C1, C2, CL]

    //Connect the nodes 
    graph->nodes[0]->neighbours[0] = graph->nodes[1]; // Connect Library to Gutenberg square scenario
    graph->nodes[0]->num_neigbours = 1;

    graph->nodes[1]->neighbours[0] = graph->nodes[2]; // Connect Gutenberg with Tallers (T1)
    graph->nodes[1]->neighbours[1] = graph->nodes[5]; // Connect Gutenberg with Cafeteria (C2)
    graph->nodes[1]->num_neigbours = 2;

    graph->nodes[2]->neighbours[0] = graph->nodes[4]; // Connect Tallers (T1) to Cafeteria (C1)
    graph->nodes[2]->num_neigbours = 1;

    graph->nodes[4]->neighbours[0] = graph->nodes[6]; // Connect Cafeteria (C1) to Classroom
    graph->nodes[4]->num_neigbours = 1;


    graph->nodes[6]->neighbours = NULL; // Classroom has no neighbours
    graph->nodes[6]->num_neigbours = 0;

    graph->nodes[5]->neighbours[0] = graph->nodes[3]; // Connect Cafeteria (C2) with Tallers (T2)
    graph->nodes[5]->num_neigbours = 1;

    graph->nodes[3]->neighbours[0] = graph->nodes[6]; // Connect Tallers T2 with Classroom
    graph->nodes[3]->num_neigbours = 1;
}

void freeScenarioGraph(ScenarioGraph* graph) {
    for (int i = 0; i < graph->num_nodes; i++) {
        free(graph->nodes[i]);
    }
}
