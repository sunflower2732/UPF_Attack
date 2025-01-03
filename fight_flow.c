#include "battle_system.h"

//Function to compute the damage to be inflicted in a character
int calculateDamage(int attackerAtp, int defenderDef) {
    int damage = attackerAtp - defenderDef;
    if (damage < 0) {   //ensure non-negative
        damage = 0;
    }
    return damage;
}

// Function to check whether all enemies are defeated
bool enemies_defeated(int num_enemies, Character *scenario_enemies[])
{
    int defeated = 0;
    for (int i = 0; i < num_enemies; i++)
    {
        if (scenario_enemies[i]->hp == 0)
        {
            defeated++;
        }
    }
    if (defeated == num_enemies)
    {
        return true;
    }
    return false;
}

// Fucntion to display the player's available skills
void skills_menu(Character *player, SkillDictionary* dictionary){
    printf("\n");
    printf("\033[1;4;34mSkill's Menu\033[1;0;34m \u2193\u2193\033[0m\n"); //use of Unicode, ANSI escape GPT provided)
    printf("\n");
    for (int i = 0; i < player->num_skills; i++) {   
        if(strcmp(player->skills[i].name, "Time Strike") == 0) {
            if(player->timeStrikeUsed == 0) {
                printf("\033[37;44m%d. Time Strike \033[0m\n", i);             //use of Unicode, ANSI escape GPT provided
                printf("Temporary modifier. Your statistics will be improved for 1 turn. \n");
                printf("\n");
            } 
        } else {
            printf("\033[37;44m%d. %s: \033[0m atp: +%d, def: +%d, (+%d \033[31m\u2665\033[0m)\n", i, player->skills[i].name, player->skills[i].modifier.atp, player->skills[i].modifier.def, player->skills[i].modifier.hp); //use of ANSI escape, Unicode GPT provided
        
        for (int j=0; j < MAX_PLAYER_SKILLS; j++){
            if (strcmp(dictionary->skills[j].skill_name, player->skills[i].name)==0){
                printf("Usage count: %d\n", dictionary->skills[j].usage_count);
            }
        }
        
        if(player->skills[i].type == true){
            printf("Temporary modifier. Your statistics will be improved for %d turns. \n", player->skills[i].duration);
        }
        else
        {
            printf("Direct attack skill. \n");
        }
        printf("\n");
        }        
    }
}

// Function to deal with the addition of a player's skill
void skill_handling(Character *player, int skill, SkillDictionary* dictionary)
{
    // add the player statistic improvements of the skill
    player->atp = player->atp + player->skills[skill].modifier.atp;
    player->def += player->skills[skill].modifier.def;
    player->hp += player->skills[skill].modifier.hp;

    //Update the skill dictionary
    push(&player->moveHistory, &player->skills[skill], dictionary);
    updateSkillUsage(dictionary, player->skills[skill].name);  

    if (player->skills[skill].type == true)
    {
        player->skills[skill].active = true; // the temporary modifier is active
        // move the tm to the last position (not occupied by another active tm)
        Skill swap = player->skills[player->num_skills - 1];
        player->skills[player->num_skills - 1] = player->skills[skill];
        player->skills[skill] = swap;
    }
    // reduce the duration of all active tm since they have exhausted another turn
    for (int i = 0; i < MAX_SKILLS; i++)
    {
        if (player->skills[i].type == true && player->skills[i].active == true && strcmp(player->skills[i].name, player->skills[skill].name) != 0)
        {
            player->skills[i].duration--;
            if (player->skills[i].duration == 0)
            {
                player->atp -= player->skills[i].modifier.atp;
                player->skills[i].active = false;
                if (player->atp < 0)
                {
                    player->atp = 0; // ensure non-zero stats
                }
                player->def -= player->skills[i].modifier.def;
                if (player->def < 0)
                {
                    player->def = 0;
                }
                player->hp -= player->skills[i].modifier.hp;
                if (player->hp < 0)
                {
                    player->hp = 0;
                }
            }
        }
    }
    if (player->skills[player->num_skills - 1].active == true)
    {                         // if we have just dealt with a tm
        player->num_skills--; // this way, in the next round it won't be shown in the skill's menu (since it has been exhausted).
    }
}

// Process the player's turn
void processPlayerTurn(Character* player, Character* scenario_enemies[], int num_enemies, SkillDictionary* dictionary, Skill* playerSkills) {
    int skill;
    int skill_timeStrike;
    
    printf("\n");
    printf("\033[1;30;41m \U0001F4E3 \U0001F4E3 \U0001F4E3 \U0001F4E3  Round's state  \U0001F4E3 \U0001F4E3 \U0001F4E3 \U0001F4E3 \033[0m\n"); //use of Unicode, ANSI escape GPT provided
    printf("\n");
    printf("Enemies: ");
    for(int i=0; i<num_enemies; i++){
        printf("%s  (%d \033[31m\u2665\033[0m) \t", scenario_enemies[i]->name, scenario_enemies[i]->hp); //use of ANSI escape, Unicode GPT provided    
    }
    printf("\n");
    printf("Player: %s (%d \033[31m\u2665\033[0m), atp: %d, def: %d \n", player->name, player->hp, player->atp, player->def); //use of ANSI escape, Unicode GPT provided
    // Display player's skill and make him choose a valid one

    do{
        skills_menu(player, dictionary);
        printf("Select a skill from the options above: ");
        scanf("%d", &skill);
        if(skill<0 || skill > player->num_skills-1){  
            printf("Please, introduce a valid skill index");
        }
    }while(skill<0 || skill > player->num_skills-1);
    
    // Change the stats of the player and combat
    if(strcmp(player->skills[skill].name, "Time Strike") == 0) {
        skill_timeStrike = timeStrike(player, dictionary, playerSkills);
    } else {
        skill_handling(player, skill, dictionary);
    }
    printf("Player: %s (%d \033[31m\u2665\033[0m), atp: %d, def: %d \n", player->name, player->hp, player->atp, player->def); //use of ANSI escape, Unicode GPT provided
    printf("Now, choose an enemy: ");
    for(int i=0; i<num_enemies; i++){
        printf("%d: %s (%d \033[31m\u2665\033[0m) \t", i, scenario_enemies[i]->name, scenario_enemies[i]->hp); //use of ANSI escape, Unicode GPT provided
    }

    int enemy;
    scanf("%d", &enemy);
    if(enemy<0 || enemy > num_enemies-1){
        while(enemy<0 || enemy > num_enemies-1) {
            printf("Introduce a valid index for an enemy: ");
            scanf("%d", &enemy);
        }
    }
    int damage = calculateDamage(player->atp, scenario_enemies[enemy]->def);
    scenario_enemies[enemy]->hp -= damage;
    if(scenario_enemies[enemy]->hp<0){
        scenario_enemies[enemy]->hp = 0;
    }
    if(strcmp(player->skills[skill].name, "Time Strike") == 0) {
        int hp_set_back = playerSkills[skill_timeStrike].modifier.hp *2;
        int atp_set_back = playerSkills[skill_timeStrike].modifier.atp *2;
        int def_set_back = playerSkills[skill_timeStrike].modifier.def *2;

        player->hp = player->hp - hp_set_back;
        player->atp = player->atp - atp_set_back;
        player->def = player->def - def_set_back;
    }
}

// Process the enemy's turn
void processEnemyTurn(Character* enemy, Character* player) {
    printf("%s's turn. HP: %d, ATP: %d, DEF: %d\n", enemy->name, enemy->hp, enemy->atp, enemy->def);
    srand(time(NULL));
    int sk_index = rand() % 3; // to get a random integer between 0 and 3.
    enemy->def = enemy->def + enemy->skills[sk_index].modifier.def;
    int enemy_temp_atp = enemy->atp + enemy->skills[sk_index].modifier.atp;
    int damage = calculateDamage(enemy_temp_atp, player->def);
    printf("Damage inflicted: %d \n", damage);
    player->hp-=damage;
    if(player->hp<0){
        player->hp = 0;
    }
    printf("%s attacked you.\n", enemy->name);
}

// Function to carry on the loop of the fight
bool fight_flow(Queue* round, Character* player, Character* scenario_enemies[], int num_enemies, SkillDictionary* dictionary, Skill* playerSkills) {
    while (!isQueueEmpty(round)) {
        Character* currentCharacter = dequeue(round);
            if (strcmp(currentCharacter->degree, "enemy") == 0) {
            printf("Processing enemy turn...\n");
            processEnemyTurn(currentCharacter, player);
            if (player->hp <= 0) {
                printf("Player is defeated. Battle lost!\n");
                return false;
            }
        } else {
            printf("Processing player turn...\n");
            processPlayerTurn(currentCharacter, scenario_enemies, num_enemies, dictionary, playerSkills);
            if(enemies_defeated(num_enemies, scenario_enemies)) {
                printf("\U0001F44F\U0001F44F You've defeated your enemy! \U0001F44F\U0001F44F \n"); //use of Unicode GPT provided
                return true;
            }
        }
        if (currentCharacter->hp > 0) {
            enqueue(round, currentCharacter);
        } 
    }
    printf("Maximum number of turns reached.\n");
    return false;
}

// Function to implement a linear search
bool is_index(int index, int index_scenario[MAX_OPTIONS], int *fight_scenarios){
    for(int i=0; i<*fight_scenarios; i++){
        if(index == index_scenario[i]){
            return true;
        }
    }
    return false;
}

// Function to integrate the fights with the loop of the game
void fight_flow_management(bool result, Character *player, Scenario *scenarios, ScenarioGraph *graph, ScenarioNode *current_node, int *fights_scenarios, int index_scenarios[MAX_OPTIONS], SkillDictionary* dictionary, Skill* playerSkills){
    // This function sets the next step of the game's session after the player has fought.
    if (!result){
        printf("\n");
        printf("\033[1;37;41m"); //Figlet and ANSI escape GPT provided
        printf("                                                          \n");
        printf("           _|_|_|    _|_|    _|      _|  _|_|_|_|         \n");
        printf("         _|        _|    _|  _|_|  _|_|  _|               \n");
        printf("         _|  _|_|  _|_|_|_|  _|  _|  _|  _|_|_|           \n");
        printf("         _|    _|  _|    _|  _|      _|  _|               \n");
        printf("           _|_|_|  _|    _|  _|      _|  _|_|_|_|         \n");
        printf("                                                          \n");
        printf("                                                          \n");
        printf("           _|_|    _|      _|  _|_|_|_|  _|_|_|           \n");
        printf("         _|    _|  _|      _|  _|        _|    _|         \n");
        printf("         _|    _|  _|      _|  _|_|_|    _|_|_|           \n");
        printf("         _|    _|    _|  _|    _|        _|    _|         \n");
        printf("           _|_|        _|      _|_|_|_|  _|    _|         \n");
        printf("                                                          \n");
        printf("\033[0m\n"); //Figlet and ANSI escape GPT provided

        exit(0);
    }
    else{
        int choice;
        // Mandatory if fights < 2. Subscenario options: only those in which the player has not fought (still in progress)
        if (*fights_scenarios < 2)
        {
            printf("****************** You have to navigate to another subscenario *******************\n");
            choice = 1;
        }
        else if (*fights_scenarios == 3){
            printf("******************** You have to continue with the story *************************\n");
            if(current_node->neighbours[0]->num_neigbours == 2){ // If the next neighbour is Gutenberg
                // We force the player into choosing between the Cafeteria or Tallers
                printf("%s \n",current_node->neighbours[0]->scenario.description);
                for (int i = 0; i < MAX_NEIGHBOURS; i++) {
                    if (strlen(current_node->neighbours[0]->scenario.decision_options[i].response) > 0) {
                        printf("%d: %s\n", i + 1, current_node->neighbours[0]->scenario.decision_options[i].response);
                    }
                }
                scanf("%d", &choice);
                current_node = current_node->neighbours[0]->neighbours[choice-1]; // Subtract 1 because the index of the first option is 0, not 1.
            }
            else{
                current_node = current_node->neighbours[0];
            }
            choice = scenario_initialise(player, current_node, fights_scenarios, index_scenarios, dictionary, playerSkills);
            subscenario_initialise(graph, scenarios, current_node, choice, player, fights_scenarios, index_scenarios, dictionary, playerSkills);
        }
        else{
            choice = 2;
        }
        printf("\033[30;47mChoose an option:\033[0m\n");
        if(choice == 1 || choice == 2){
            for(int i=0; i<MAX_OPTIONS; i++){
                if(!is_index(i, index_scenarios, fights_scenarios)){ // If the player has not visited yet the scenario
                     printf("\033[0;33m%d: \033[0;37m%s\n", i+1, current_node->scenario.decision_options[i].response);
                }
            }
        }
        if(choice == 2){
            printf("\033[33m4. \033[0mContinue with the story"); //use of ANSI escape GPT provided
            printf("\n");
        }
        scanf("%d", &choice);
        if (choice < 4){ // If a subsecenario has already been chosen, we force the player into choosing again.
            while(is_index(choice, index_scenarios, fights_scenarios)||choice==4){
                printf("Please, choose a non visited subscenario");
                scanf("%d", &choice);
            }
            subscenario_initialise(graph, scenarios, current_node, choice-1, player, fights_scenarios, index_scenarios, dictionary, playerSkills);
        }
        else{ // If continue with the story is chosen
            if(current_node->neighbours[0]->num_neigbours == 2){ // If the next neighbour is Gutenberg
                printf("%s \n",current_node->neighbours[0]->scenario.description);
                // We prompt the player into choosing between the Cafeteria or Tallers
                for (int i = 0; i < MAX_NEIGHBOURS; i++) {
                    if (strlen(current_node->neighbours[0]->scenario.decision_options[i].response) > 0) {
                        printf("\033[0;33m%d: \033[0;37m%s\n", i + 1, current_node->neighbours[0]->scenario.decision_options[i].response);
                    }
                }
                scanf("%d", &choice);
                current_node = current_node->neighbours[0]->neighbours[choice-1];
            }
            else{
                current_node = current_node->neighbours[0];
            }
            choice = scenario_initialise(player, current_node, fights_scenarios, index_scenarios, dictionary, playerSkills);
            subscenario_initialise(graph, scenarios, current_node, choice, player, fights_scenarios, index_scenarios, dictionary, playerSkills);
           
        }
    }
}
