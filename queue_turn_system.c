#include <string.h>
#include "battle_system.h"

//function turn queue initialise 
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    queue->size = 0;
    return queue;
}

//function is turn queue empty
bool isQueueEmpty(Queue* queue) {
    return (queue->front == NULL);
}

// function to enqueue a turn
void enqueue(Queue* queue, Character* character) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->character = character;
    newNode->next = NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    queue->size++;
}

// function to dequeue a turn
Character* dequeue(Queue* queue) {
    if (isQueueEmpty(queue)) {
        return NULL;
    }
    QueueNode* temp = queue->front;
    Character* character = temp->character;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return character;
}

//initialise the battle queue with alternating turns
void initializeBattleQueue(Queue* queue, Character* player, Character* enemies[MAX_ENEMIES], int num_enemies) {
    srand(time(NULL));
    int playerTurn = rand() % 2;  // Randomly choose who starts first (0 for player, 1 for enemy)
    for (int i = 0; i < MAX_TURNS; i++) {
        if ((i % 2 == playerTurn) || (num_enemies == 0)) { // podríem treure segona condició?
            enqueue(queue, player);
        } 
        else {
            enqueue(queue, enemies[0]);
        }
    }
}
