#include "load_game_data.h"
#include "game_data_structures.h"
#include "graph_scenario.h"

void load_game_data(const char* filename, Scenario* scenarios, ScenarioGraph* graph, Skill* playerSkills) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s.\n", filename);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_data = (char*)malloc(file_size + 1);
    if (!json_data) {
        printf("Memory allocation error for JSON data.\n");
        exit(1);
    }
    fread(json_data, 1, file_size, file);
    fclose(file);
    json_data[file_size] = '\0';

    cJSON *root = cJSON_Parse(json_data);
    if (!root) {
        printf("Error parsing JSON data.\n");
        free(json_data);
        exit(1);
    }

    cJSON *scenarios_json = cJSON_GetObjectItem(root, "scenarios");
    if (!scenarios_json) {
        printf("Scenarios data not found in JSON.\n");
        cJSON_Delete(root);
        free(json_data);
        exit(1);
    }

    for (int i = 0; i < cJSON_GetArraySize(scenarios_json); i++) {
        cJSON *scenario_item = cJSON_GetArrayItem(scenarios_json, i);
        cJSON *name_item = cJSON_GetObjectItem(scenario_item, "name");
        cJSON *description_item = cJSON_GetObjectItem(scenario_item, "description");
        if (!name_item || !description_item) {
            printf("Error: Missing 'name' or 'description' field in scenario object.\n");
            continue;
        }

        Scenario newScenario;
        strncpy(newScenario.name, name_item->valuestring, sizeof(newScenario.name) - 1);
        newScenario.name[sizeof(newScenario.name) - 1] = '\0';
        strncpy(newScenario.description, description_item->valuestring, sizeof(newScenario.description) - 1);
        newScenario.description[sizeof(newScenario.description) - 1] = '\0';

        // Initialize decision options
        cJSON *decision_options_json = cJSON_GetObjectItem(scenario_item, "decision_options");
        if (!decision_options_json) {
            printf("Error: Missing 'decision_options' field in scenario object.\n");
            continue;
        }
        cJSON *decision_option;
        int num_options = cJSON_GetArraySize(decision_options_json);
        for (int j = 0; j < num_options && j < MAX_OPTIONS; j++) {
            decision_option = cJSON_GetArrayItem(decision_options_json, j);
            cJSON *response_item = cJSON_GetObjectItem(decision_option, "response");
            cJSON *narrative_before_item = cJSON_GetObjectItem(decision_option, "narrative_before");
            if (!response_item || !narrative_before_item) {
                printf("Error: Missing 'response' or 'narrative_before' field in decision option.\n");
                continue;
            }
            strncpy(newScenario.decision_options[j].response, response_item->valuestring, sizeof(newScenario.decision_options[j].response) - 1);
            newScenario.decision_options[j].response[sizeof(newScenario.decision_options[j].response) - 1] = '\0';
            strncpy(newScenario.decision_options[j].narrative_before, narrative_before_item->valuestring, sizeof(newScenario.decision_options[j].narrative_before) - 1);
            newScenario.decision_options[j].narrative_before[sizeof(newScenario.decision_options[j].narrative_before) - 1] = '\0';

            // Initialize enemies
            cJSON *enemies_json = cJSON_GetObjectItem(decision_option, "enemy");
            if (enemies_json) {
                int num_enemies = cJSON_GetArraySize(enemies_json);
                for (int k = 0; k < num_enemies && k < MAX_ENEMIES; k++) {
                    cJSON *enemy_item = cJSON_GetArrayItem(enemies_json, k);
                    cJSON *enemy_name_item = cJSON_GetObjectItem(enemy_item, "name");
                    cJSON *enemy_description_item = cJSON_GetObjectItem(enemy_item, "description");
                    cJSON *enemy_hp_item = cJSON_GetObjectItem(enemy_item, "hp");
                    cJSON *enemy_atp_item = cJSON_GetObjectItem(enemy_item, "atp");
                    cJSON *enemy_def_item = cJSON_GetObjectItem(enemy_item, "def");
                    if (!enemy_name_item || !enemy_description_item || !enemy_hp_item || !enemy_atp_item || !enemy_def_item) {
                        printf("Error: Missing fields in enemy object.\n");
                        continue;
                    }
                    strncpy(newScenario.decision_options[j].enemies[k].name, enemy_name_item->valuestring, sizeof(newScenario.decision_options[j].enemies[k].name) - 1);
                    newScenario.decision_options[j].enemies[k].name[sizeof(newScenario.decision_options[j].enemies[k].name) - 1] = '\0';
                    strncpy(newScenario.decision_options[j].enemies[k].description, enemy_description_item->valuestring, sizeof(newScenario.decision_options[j].enemies[k].description) - 1);
                    newScenario.decision_options[j].enemies[k].description[sizeof(newScenario.decision_options[j].enemies[k].description) - 1] = '\0';
                    newScenario.decision_options[j].enemies[k].hp = enemy_hp_item->valueint;
                    newScenario.decision_options[j].enemies[k].atp = enemy_atp_item->valueint;
                    newScenario.decision_options[j].enemies[k].def = enemy_def_item->valueint;
                    // Initialize enemy skills
                    cJSON *enemy_skills = cJSON_GetObjectItem(enemy_item, "skills");
                    if (enemy_skills) {
                        int num_skills = cJSON_GetArraySize(enemy_skills);
                        for (int l = 0; l < num_skills && l < MAX_SKILLS; l++) {
                            cJSON *skill_item = cJSON_GetArrayItem(enemy_skills, l);
                            cJSON *skill_name_item = cJSON_GetObjectItem(skill_item, "name");
                            cJSON *skill_description_item = cJSON_GetObjectItem(skill_item, "description");
                            cJSON *skill_type_item = cJSON_GetObjectItem(skill_item, "type");
                            cJSON *skill_duration_item = cJSON_GetObjectItem(skill_item, "duration");
                            cJSON *modifier_item = cJSON_GetObjectItem(skill_item, "modifier"); 
                            if (!skill_name_item) {
                                printf("Error: Missing name fields in enemy skill object.\n");
                                continue;
                            }
                            if (!skill_description_item) {
                                printf("Error: Missing description fields in enemy skill object.\n");
                                continue;
                            }
                            if (!skill_type_item) {
                                printf("Error: Missing type fields in enemy skill object.\n");
                                continue;
                            }
                            if (!skill_duration_item) {
                                printf("Error: Missing duration fields in enemy skill object.\n");
                                continue;
                            }
                            if (!modifier_item) {
                                printf("Error: Missing modifier fields in enemy skill object.\n");
                                continue;
                            }
                            strncpy(newScenario.decision_options[j].enemies[k].skills[l].name, skill_name_item->valuestring, sizeof(newScenario.decision_options[j].enemies[k].skills[l].name) - 1);
                            newScenario.decision_options[j].enemies[k].skills[l].name[sizeof(newScenario.decision_options[j].enemies[k].skills[l].name) - 1] = '\0';
                            strncpy(newScenario.decision_options[j].enemies[k].skills[l].description, skill_description_item->valuestring, sizeof(newScenario.decision_options[j].enemies[k].skills[l].description) - 1);
                            newScenario.decision_options[j].enemies[k].skills[l].description[sizeof(newScenario.decision_options[j].enemies[k].skills[l].description) - 1] = '\0';
                            newScenario.decision_options[j].enemies[k].skills[l].type = skill_type_item->valueint;
                            newScenario.decision_options[j].enemies[k].skills[l].duration = skill_duration_item->valueint;
                            cJSON *modifier_hp_item = cJSON_GetObjectItem(modifier_item, "hp");
                            cJSON *modifier_atp_item = cJSON_GetObjectItem(modifier_item, "atp");
                            cJSON *modifier_def_item = cJSON_GetObjectItem(modifier_item, "def");
                            if (!modifier_hp_item || !modifier_atp_item || !modifier_def_item) {
                                printf("Error: Missing fields in enemy modifier object.\n");
                                continue;
                            }
                            newScenario.decision_options[j].enemies[k].skills[l].modifier.hp = modifier_hp_item->valueint;
                            newScenario.decision_options[j].enemies[k].skills[l].modifier.atp = modifier_atp_item->valueint;
                            newScenario.decision_options[j].enemies[k].skills[l].modifier.def = modifier_def_item->valueint;
                            newScenario.decision_options[j].enemies[k].skills[l].active = false;
                        }
                    }
                }
            }
        }

        ScenarioNode* scenarioNode = createScenarioNode(&newScenario);
        addScenarioNode(graph, scenarioNode);
        if(strcmp(newScenario.name,"Tallers") == 0 ||strcmp(newScenario.name,"Café") == 0  ){
            ScenarioNode* scenarioNode2 = createScenarioNode(&newScenario);
            addScenarioNode(graph, scenarioNode2);
        }
        
    }
    int num_skills = 0;
    // Load player skills
    cJSON *player_skills_json = cJSON_GetObjectItem(root, "player_skills");
    if (player_skills_json) {
        int total_skills = cJSON_GetArraySize(player_skills_json);
        for (int m = 0; m < total_skills && num_skills < MAX_PLAYER_SKILLS; m++) {
            cJSON *skill_item = cJSON_GetArrayItem(player_skills_json, m);
            cJSON *skill_name_item = cJSON_GetObjectItem(skill_item, "name");
            cJSON *skill_description_item = cJSON_GetObjectItem(skill_item, "description");
            cJSON *skill_type_item = cJSON_GetObjectItem(skill_item, "type");
            cJSON *skill_duration_item = cJSON_GetObjectItem(skill_item, "duration");
            cJSON *modifier_item = cJSON_GetObjectItem(skill_item, "modifier");
            if (!skill_name_item || !skill_description_item || !skill_type_item || !skill_duration_item || !modifier_item) {
                printf("Error: Missing fields in player skill object.\n");
                continue;
            }
            strncpy(playerSkills[m].name, skill_name_item->valuestring, sizeof(playerSkills[m].name) - 1);
            playerSkills[m].name[sizeof(playerSkills[m].name) - 1] = '\0';
            strncpy(playerSkills[m].description, skill_description_item->valuestring, sizeof(playerSkills[m].description) - 1);
            playerSkills[m].description[sizeof(playerSkills[m].description) - 1] = '\0';
            playerSkills[m].type = skill_type_item->valueint;
            playerSkills[m].duration = skill_duration_item->valueint;
            cJSON *modifier_hp_item = cJSON_GetObjectItem(modifier_item, "hp");
            cJSON *modifier_atp_item = cJSON_GetObjectItem(modifier_item, "atp");
            cJSON *modifier_def_item = cJSON_GetObjectItem(modifier_item, "def"); //|| !modifier_atp_item || !modifier_def_item
            if (!modifier_hp_item ) {
                printf("Error: Missing fields in player hp modifier object.\n");
                continue;
            }
            if (!modifier_atp_item ) {
                printf("Error: Missing fields in player atp modifier object.\n");
                continue;
            }
            if (!modifier_def_item ) {
                printf("Error: Missing fields in player def modifier object.\n");
                continue;
            }
            playerSkills[m].modifier.hp = modifier_hp_item->valueint;
            playerSkills[m].modifier.atp = modifier_atp_item->valueint;
            playerSkills[m].modifier.def = modifier_def_item->valueint;
            playerSkills[m].active = false;
            num_skills++;
        }
    } else {
        printf("Player skills data not found in JSON.\n");
    }
    connectScenarioNodes(graph);

    // Clean up cJSON and JSON data
    cJSON_Delete(root);
    free(json_data);
}
