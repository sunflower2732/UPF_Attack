#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game_data_structures.h"
#include "time_strike.h"
#include "battle_system.h"

void push(StackNode** top, Skill* skill, SkillDictionary* dictionary) {
    /* Adds a skill to the top of the stack and updates the skill dictionary */
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    newNode->skill = *skill;
    newNode->next = *top;
    *top = newNode;

}

Skill pop(StackNode** top, SkillDictionary* dictionary) {
    /* Removes and returns a skill from the top of the stack */
    if (*top == NULL) {
        Skill emptySkill = {"", "", false, -1, {0, 0, 0}, false};
        return emptySkill;
    }
    StackNode* temp = *top;
    Skill poppedSkill = temp->skill;
    *top = (*top)->next;
    free(temp);

    return poppedSkill;
}

int timeStrike(Character* player, SkillDictionary* dictionary, Skill* playerSkills) {
    /* Checks if time strike has already been used. If not, it selects a random move from the stack, doubles its power, and executes it. */
    if (player->timeStrikeUsed > 0) {
        printf("Time Strike can only be used once per battle.\n");
        return -1;
    }

    StackNode* current = player->moveHistory;
    int moveCount = 0;
    while (current != NULL) {
        moveCount++;
        current = current->next;
    }

    if (moveCount == 0) {
        printf("No moves in history to strike from.\n");
        return -1;
    }

    srand(time(NULL));
    int k = rand() % moveCount;

    int chosenSkill = getNthFromTop(player->moveHistory, k, player, playerSkills);
    printf("Time Strike re-executes move %s with double power!\n", playerSkills[chosenSkill].name);
    
    player->timeStrikeUsed = 1;
    skill_handling_ts(player, chosenSkill, playerSkills);

    return chosenSkill;
}

int getNthFromTop(StackNode* top, int n, Character *player, Skill* playerSkills) {
    /* Retrieves the n-th skill index from the top of the stack without removing it */
    StackNode* current = malloc(sizeof(StackNode));
    current = top;      //select the top of the stack as current node

    for (int i = 0; i < n; i++) {   //iterate from 0 to random number generated in time strike
        if (current != NULL) {      //check if 
            current = current->next;
        } else {
            return -1; //failure -- index not found
        }
    }

    // Find the index of the skill in the player's skill array
    int index = 0;
    bool found = false;
    while (found == false && index < MAX_PLAYER_SKILLS) {
        if (strcmp(current->skill.name, playerSkills[index].name) == 0) {
            found = true;
        } else {
            index++;
        }
    }
    
    if (index > MAX_SKILLS) {
        return -1; //failure -- index not found
    }

    return index;
}

void initializeSkillDictionary(SkillDictionary* dictionary, Skill* playerSkills) {
    /*Skill Dictionary initialization function*/
    dictionary->size = MAX_PLAYER_SKILLS;
    for (int i = 0; i < MAX_PLAYER_SKILLS; i++) {
        strcpy(dictionary->skills[i].skill_name, playerSkills[i].name); //set name to use as key
        dictionary->skills[i].usage_count = 0; //set initial usage count to zero
    }
}

void updateSkillUsage(SkillDictionary* dictionary, char skill_name[]) {
    /*updating skill dictionary when used*/
    for (int i = 0; i < dictionary->size; i++) {
        if (strcmp(dictionary->skills[i].skill_name, skill_name) == 0) {
            dictionary->skills[i].usage_count++;
            return;
        }
    }
}

void skill_handling_ts(Character* player, int chosenSkill, Skill* playerSkills) {
    int modified_hp = playerSkills[chosenSkill].modifier.hp * 2;
    int modified_atp = playerSkills[chosenSkill].modifier.atp * 2;
    int modified_def = playerSkills[chosenSkill].modifier.def * 2;
    
    player->atp = player->atp + modified_atp;
    player->def = player->def + modified_def;
    player->hp = player->hp + modified_hp;
    
    printf("skill hp: %d\n", modified_hp);
    printf("skill atp: %d\n", modified_atp);
    printf("skill def: %d\n", modified_def);
}
