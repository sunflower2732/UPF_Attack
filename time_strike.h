#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game_data_structures.h"

void push(StackNode** top, Skill* skill, SkillDictionary* dictionary);
Skill pop(StackNode** top, SkillDictionary* dictionary);
int getNthFromTop(StackNode* top, int n, Character *player, Skill* playerSkills);
int timeStrike(Character* player, SkillDictionary* dictionary, Skill* playerSkills);
void skill_handling_ts(Character* player, int chosenSkill, Skill* playerSkills);

//dictionary functions
void initializeSkillDictionary(SkillDictionary* dictionary, Skill* playerSkills);
void updateSkillUsage(SkillDictionary* dictionary, char skill_name[]);
