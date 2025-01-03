#ifndef GAME_DATA_STRUCTURES_H
#define GAME_DATA_STRUCTURES_H

#define MAX_SCENARIOS 7
#define MAX_OPTIONS 3
#define MAX_ENEMIES 11
#define MAX_PLAYER_SKILLS 9
#define MAX_SKILLS 4
#define MAX_TURNS 10

#include <stdbool.h>

//Struct for attributes
typedef struct {
    int hp;
    int atp;
    int  def;
} Attributes;

//Struct for skill
typedef struct{
    char name[20];
    char description[200];
    bool type;                  // if true, direct attack. if false, temporary modifier
    int duration;               // it is initialized as -1 if direct attack.
    Attributes modifier;        // it includes how many points it adds to atk, hp and def.
    bool active;                // if true, the temporary modifier has been used in combat (but is not exhausted)
} Skill;

// Struct for enemies
typedef struct {
    char name[50];
    char description[250];
    int hp;     // Health points
    int atp;    // Attack points
    int def;    // Defense points
    Skill skills[MAX_SKILLS]; //skills struct 
} Enemy;

// Struct for options available to player
typedef struct {
    char response[100];             // Response text for the player's choice
    char narrative_before[600];     // Narrative text before battling enemies
    Enemy enemies[MAX_ENEMIES];     // Array of enemies the player will face
    char narrative_after[600];      // Narrative text after battling enemies
} Option;

// Struct for scenarios
typedef struct {
    char name[50];                         // Name of scenario
    char description[1000];                  // Description of scenario
    Option decision_options[MAX_OPTIONS];   // Array of decision options for player
} Scenario;

typedef struct StackNode{ //represents a node in the stack
    Skill skill;
    struct StackNode* next;
} StackNode;

typedef struct Stack {
    StackNode* nodes;
    StackNode top;
} Stack;

//Dictionary structures for skill usage tracking (key)
typedef struct {
    char skill_name[50];
    int usage_count;
} SkillUsage;

typedef struct {
    SkillUsage skills[MAX_SKILLS];
    int size;
} SkillDictionary;

// Struct for character
typedef struct {
    char name[50];
    int hp;     // Health points
    int atp;    // Attack points
    int def;     // Defense points
    char degree[50]; // Enemy
    int num_skills;
    Skill skills[MAX_SKILLS];
    StackNode* moveHistory; //player w stack (linked list) for move history
    int timeStrikeUsed; //flag for the Time Strike move
} Character;

//node structure for queue
typedef struct QueueNode {
    Character* character;
    struct QueueNode* next;
} QueueNode;

//queue structure
typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

//node in scenario graph
typedef struct ScenarioNode {
    Scenario scenario;          //Information about scenario
    struct ScenarioNode** neighbours;  //Array of pointers to the adjacent scenarios
    int num_neigbours;
} ScenarioNode;

//scenario graph
typedef struct ScenarioGraph {
    ScenarioNode* nodes[MAX_SCENARIOS]; //Array of nodes representing scenarios
    int num_nodes; 
} ScenarioGraph;

#endif //GAME_DATA_STRUCTURES_H
