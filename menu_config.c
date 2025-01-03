#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu_config.h"

void start_new_game(Character *player, Scenario *scenarios, ScenarioGraph *graph, int *fights_scenario, int index_subscenarios[MAX_OPTIONS], Skill *playerSkills){
    if(player->degree[1] == '\0'){
        printf("Looks like you haven't configured your character yet.\n");
        configure_character(player);
        start_new_game(player, scenarios, graph, fights_scenario, index_subscenarios, playerSkills);
    } else {
        ScenarioNode* current_node = graph->nodes[0]; 
        SkillDictionary* dictionary = (SkillDictionary*)malloc(sizeof(SkillDictionary));
        initializeSkillDictionary(dictionary, playerSkills); 
        int choice_sub = scenario_initialise(player, current_node, fights_scenario, index_subscenarios, dictionary, playerSkills); // Start with the "Library" scenario
        subscenario_initialise(graph, scenarios, current_node, choice_sub, player, fights_scenario, index_subscenarios, dictionary, playerSkills); 
    }
}

void configure_character(Character *player){
    printf("Configurating character...\n"); //use of ANSI escape GPT provided
    printf("\n");
    printf("\033[30;47mEnter your character name:\033[0m ");
    scanf("%s", player->name);
    printf(" \n");
    printf("\U0001F4D6 \033[1;4;32mAvailable degrees\033[0m \U0001F4D6\n");
    printf(" \n");
    printf("\033[32m1.\033[0m Audiovisual Communications\n");
    printf("\033[32m2.\033[0m Biomedical Engineering\n");
    printf("\033[32m3.\033[0m Computer Science\n");
    printf(" \n");
    printf("\033[30;47mEnter your degree number:\033[0m ");

    int degree_choice;
    scanf("%d", &degree_choice);
    switch (degree_choice) {
        case 1:
            strcpy(player->degree, "Audiovisual Communications");
            player->hp = 100;
            player->atp = 15;
            player->def = 10;
            break;
        case 2:
            strcpy(player->degree, "Biomedical Engineering");
            player->hp = 120;
            player->atp = 15;
            player->def = 15;
            break;
        case 3:
            strcpy(player->degree, "Computer Science");
            player->hp = 80;
            player->atp = 25;
            player->def = 5;
            break;

        default:
            printf("Invalid choice. Defaulting to Computer Science.\n");
            strcpy(player->degree, "Computer Science");
            player->hp = 80;
            player->atp = 25;
            player->def = 5;
            break;
    }
    printf("\n");
    printf("\033[33m**************************************\033[0m\n");      //use of ANSI escape GPT provided
    printf("\033[1;37;43m Character configurated successfully! \033[0m\n"); //use of ANSI escape GPT provided
    printf("\033[33m**************************************\033[0m\n");      //use of ANSI escape GPT provided
    printf("Name: %s\n", player->name);
    printf("Degree: %s\n", player->degree);
    printf("------------------------------------\n");
    printf("Health Points %d \033[31m\u2665\033[0m\n", player->hp);         //use of ANSI escape and Unicode GPT provided
    printf("Attack Points: %d\n", player->atp);
    printf("Defense Points: %d\n", player->def);
    printf("------------------------------------\n");
}

///  main menu
void menu(Scenario *scenarios, ScenarioGraph *graph, int *fights_scenario, int index_subscenarios[MAX_OPTIONS], Skill *playerSkills){
    int choice;
    Character player;
    do{
        //print game's name + use of figlet with GPT assistance
        printf("\033[1;37;43m"); // Set bold white text on yellow background GPT provided
        printf("  ___ _   _ _____ _____ ____ _____  _  _____ ___ ___  _   _      ____  _   _ ____  _____  \n");
        printf(" |_ _| \\ | |  ___| ____/ ___|_   _|/ \\|_   _|_ _/ _ \\| \\ | |    / __ \\| | | |  _ \\|  ___| \n");
        printf("  | ||  \\| | |_  |  _| \\___ \\ | | / _ \\ | |  | | | | |  \\| |   / / _` | | | | |_) | |_    \n");
        printf("  | || |\\  |  _| | |___ ___) || |/ ___ \\| |  | | |_| | |\\  |  | | (_| | |_| |  __/|  _|   \n");
        printf(" |___|_| \\_|_|   |_____|____/ |_/_/   \\_\\_| |___\\___/|_| \\_|   \\ \\__,_|\\___/|_|   |_|     \n");
        printf("                                                                \\____/                    \n");
        printf("                                                                                          \n");
        printf("\033[0m\n"); // Reset to default colors GPT provided

        printf("\n\033[1;30;33m \u2605 \u2605 \u2605 \u2605 \u2605 MAIN MENU \u2605 \u2605 \u2605 \u2605 \n");   //use of Unicode, ANSI escape GPT provided
        printf("\033[0m\n");                                 //use of ANSI escape GPT provided
        printf(" \033[33m1.\033[0m Start New Game\n");
        printf(" \033[33m2.\033[0m Configure Character\n");
        printf(" \033[33m3.\033[0m Exit\n");
        printf("\n");
        printf(" \033[1;30;33m\u2605 \u2605 \u2605 \u2605 \u2605 \u2605 \u2605 \u2605 \u2605 \u2605 \u2605 \u2605 \u2605 \u2605 \n"); //use of Unicode
        printf("\n");
        printf("\033[30;47mEnter your choice:\033[0m ");     //use of ANSI escape GPT provided
        scanf("%d", &choice);

        switch(choice){
            case 1:
            {    
                start_new_game(&player, scenarios, graph, fights_scenario, index_subscenarios, playerSkills);
                break;
            }
            case 2:
            {
                configure_character(&player);                           //Pass the address of the object
            }
            break;

            case 3:
                printf("\033[31mExiting session...\n");                 //use of ANSI escape GPT provided
                exit(0);

            default:
                printf("\033[37;41mInvalid choice.\033[0m Please try again.\n"); //use of ANSI escape GPT provided
        }

    } while (choice != 3);
}


int scenario_initialise(Character* player, ScenarioNode* current_node, int *fights_scenario, int index_subscenarios[MAX_OPTIONS], SkillDictionary* dictionary, Skill *playerSkills) {
    // we update the number of fights held, as well as the index of those fights (because we start a new scenario)
    *fights_scenario = 0;
    player->timeStrikeUsed = 0;
    for(int i=0; i<MAX_OPTIONS; i++){
        index_subscenarios[i] = -1;
    }
    if (current_node == NULL) {
        printf("Error: Current node is NULL.\n");
        return -1;
    }

    // Print the details of the current node's scenario
    printf("\033[1;37;41m %s \033[0m\n", current_node->scenario.name);          //print scenario name + use of GPT provided ANSI escape
    printf("\033[0;37m%s\n", current_node->scenario.description);   //print scenario description + use of GPT provided ANSI escape

    // Prompt the player to make a choice
    printf("\033[30;47mChoose an option:\033[0m\n");      //print list + use of GPT provided ANSI escape
    for (int i = 0; i < MAX_OPTIONS; i++) {
        if (strlen(current_node->scenario.decision_options[i].response) > 0) {
            printf("\033[0;33m%d: \033[0;37m%s\n", i + 1, current_node->scenario.decision_options[i].response); //print decision options + use of GPT provided ANSI escape
        }
    }

    int choice;
    scanf("%d", &choice);
    
    //Display available p_skills and prompt player to choose 4 for this scenario
    choose_skills(player, playerSkills, dictionary);
    

    return choice - 1; // Adjust for zero-based index
}

void subscenario_initialise(ScenarioGraph* graph, Scenario *scenarios, ScenarioNode* current_node, int choice_index, Character* player, int *fights_scenario, int index_subscenarios[MAX_OPTIONS], SkillDictionary* dictionary, Skill* playerSkills){
    index_subscenarios[*fights_scenario] = choice_index; // store the index of the subscenario where the player is fighting
    *fights_scenario+=1; //each time the player visits a subscenario, it holds a fight. We must update the variable.
    if (current_node == NULL) {
        printf("Error: Current node is NULL.\n");
        return;
    }

    if (choice_index < 0 || choice_index >= MAX_OPTIONS) {
        printf("Error: Invalid choice index.\n");
        return;
    }

    Option* chosen_option = &current_node->scenario.decision_options[choice_index];
    printf("%s\n", chosen_option->narrative_before);

    // Initialize TurnQueue
    Queue* queue = createQueue();

    // Initialize enemies array
    int num_enemies = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (strlen(chosen_option->enemies[i].name) > 0) {
            num_enemies++;
        } else {
            break;
        }
    }

    if (num_enemies == 0) {
        printf("Warning: No enemies found for this decision option.\n");
    } else {
        Character* scenario_enemies[MAX_ENEMIES]; // inicialitzar a una constant
        for (int i = 0; i < num_enemies; i++) {
            scenario_enemies[i] = malloc(sizeof(Character));
            if (!scenario_enemies[i]) {
                printf("Error: Memory allocation failed for scenario_enemies[%d].\n", i);
                // Free previously allocated memory
                for (int j = 0; j < i; j++) {
                    free(scenario_enemies[j]);
                }
                free(queue);
                return;
            }
            strcpy(scenario_enemies[i]->name, chosen_option->enemies[i].name);
            scenario_enemies[i]->hp = chosen_option->enemies[i].hp;
            scenario_enemies[i]->atp = chosen_option->enemies[i].atp;
            scenario_enemies[i]->def = chosen_option->enemies[i].def;
            strcpy(scenario_enemies[i]->degree, "enemy");
            scenario_enemies[i]->num_skills = 4;
            for (int j = 0; j < 4; j++) {
                scenario_enemies[i]->skills[j] = chosen_option->enemies[i].skills[j];
            }
            // Debugging information for each enemy
            printf("Initialized enemy %d: %s, HP: %d, ATP: %d, DEF: %d\n", i + 1, scenario_enemies[i]->name, scenario_enemies[i]->hp, scenario_enemies[i]->atp, scenario_enemies[i]->def);
        }
        initializeBattleQueue(queue, player, scenario_enemies, num_enemies);
        bool result = fight_flow(queue, player, scenario_enemies, num_enemies, dictionary, playerSkills);

        // Free allocated memory for scenario_enemies
        for (int i = 0; i < num_enemies; i++) {
            free(scenario_enemies[i]);
        }

        if(result == true && strcmp(current_node->scenario.name, "Classroom") == 0) {
            printf("\033[1;37;44m"); //Figlet and ANSI escape GPT provided
            printf("                                                          \n");
            printf("           _|      _|     _|_|      _|    _|              \n");
            printf("            _|    _|    _|    _|    _|    _|              \n");
            printf("             _|  _|     _|    _|    _|    _|              \n");
            printf("               _|       _|    _|    _|    _|              \n");
            printf("               _|         _|_|        _|_|                \n");
            printf("                                                          \n");
            printf("                                                          \n");
            printf("           _|            _|   |_ _|   | \\ | |            \n");
            printf("            _|          _|     | |    |  \\| |            \n");
            printf("            _|    _|    _|     | |    |     |            \n");
            printf("             _| _|  _| _|      | |    | |\\  |            \n");
            printf("              _|      _|      |_ _|   |_| \\_|            \n");
            printf("                                                          \n");
            printf("\033[0m\n"); //Figlet and ANSI escape GPT provided
            exit(0);
        }
                
        fight_flow_management(result, player, scenarios, graph, current_node, fights_scenario, index_subscenarios, dictionary, playerSkills);
    }
    free(queue);
}

//function to display all available p_skills + return 4 selected skills
void choose_skills(Character* player, Skill* playerSkills, SkillDictionary* dictionary){
    printf("\n");
    printf("\033[1;4;35mAvailable skills\033[0m \U0001F3CB\n");
    printf("\n");
    player->num_skills = 0;
    int num_chosen_skills = 0;  //initialise counter for chosen skills
    while (num_chosen_skills < 3) {
        //display all skills available
        for (int i = 0; i < MAX_PLAYER_SKILLS; i++){
            if(playerSkills[i].duration != 0) {
                printf("\033[1;37m%d. \033[0;37;45m %s \033[0m\n", i, playerSkills[i].name);                //print Skill name, use of ANSI escape GPT provided
                printf("Description: %s\n", playerSkills[i].description);                                   //print Skill description
                if (playerSkills[i].type) {                                                                 //determine Skill type
                    printf("\033[4;37mTemporary Modifier\033[0m \033[1;33m\u2620\033[0m\n");                //use of Unicode, ANSI escape GPT provided
                } else {
                    printf("\033[4;37mDirect Attack\033[0m \033[1;33m\u2694\033[0m\n");                     //use of Unicode, ANSI escape GPT provided
                }
                printf("\n");
            }   
        } 
        for (int i = 0; i < MAX_PLAYER_SKILLS; i++) {
            if(dictionary->skills[i].usage_count > 0) {
                printf("\033[1;37m%d. \033[0;37;45m Time Strike \033[0m\n", MAX_PLAYER_SKILLS);             //use of Unicode, ANSI escape GPT provided
                printf("Description: Use Time Strike to re-execute a previous move with double power\n");
                printf("\033[4;37mTemporary Modifier\033[0m \033[1;33m\u2620\033[0m\n");
                break;
            }
        }

        //prompt player to choose skill + parse info to chosen skills array
        for(int i = 0; i < MAX_SKILLS; i++) {
            printf("Choose a skill (insert number): ");
            int skill_index;
            scanf("%d", &skill_index);
            if(skill_index == MAX_PLAYER_SKILLS) {      //check if chosen skill is Time Strike
                if (player->timeStrikeUsed > 0) {           //if time strike has been used, prompt to choose else
                    printf("Time Strike can only be used once per battle. Choose another skill.\n");
                } else {                                //if time strike not used, add time strike to skills
                    strcpy(player->skills[i].name, "Time Strike");
                    strcpy(player->skills[i].description, "Use Time Strike to re-execute a previous move with double power");
                    player->num_skills++;
                    num_chosen_skills++;
                }
            } else {                                    //for any other skill that is not time strike, add to player skills
                strcpy(player->skills[i].name, playerSkills[skill_index].name);
                strcpy(player->skills[i].description, playerSkills[skill_index].description);
                player->skills[i].duration = playerSkills[skill_index].duration;
                player->skills[i].type = playerSkills[skill_index].type;
                player->skills[i].active = playerSkills[skill_index].active;
                player->skills[i].modifier.hp = playerSkills[skill_index].modifier.hp;
                player->skills[i].modifier.atp = playerSkills[skill_index].modifier.atp;
                player->skills[i].modifier.def = playerSkills[skill_index].modifier.def;
                player->num_skills++;
                num_chosen_skills++;
            }  
        } 
    } 
    //return with player's skills changed
    return; 
}
